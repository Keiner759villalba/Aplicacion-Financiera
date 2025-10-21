#include "compra.h"
#include "transaccion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Compatibilidad entre nombres de macro */
#ifndef MAX_TRANSACCIONES
#ifdef MAXCOMPRAS
#define MAX_TRANSACCIONES MAXCOMPRAS
#else
#define MAX_TRANSACCIONES 20
#endif
#endif

/* Contador de compras realizadas en esta ejecución (no persistente) */
static int compras_realizadas = 0;

/* ------------------- FUNCIONES PÚBLICAS ------------------- */

void realizarCompra(Transaccion *t) {
    char pan[20];
    char franquicia[32];
    char expiry[6]; /* "MM/YY" + '\0' */
    char cvv[5];
    double monto;

    if (!t) return;

    /* Verificar límite de compras */
    if (compras_realizadas >= MAX_TRANSACCIONES) {
        printf("Se alcanzó el número máximo de compras (%d). Operación cancelada.\n", MAX_TRANSACCIONES);
        return;
    }

    limpiarConsola();
    printf("=== Realizar Compra ===\n");

    printf("Ingrese el PAN (Primary Account Number): ");
    if (scanf("%19s", pan) != 1) {
        printf("Entrada inválida. Operación cancelada.\n");
        return;
    }
    if (!validarPAN(pan)) {
        printf("PAN inválido. Operación cancelada.\n");
        return;
    }

    const char *fr = validarFranquicia(pan);
    strncpy(franquicia, fr, sizeof(franquicia) - 1);
    franquicia[sizeof(franquicia) - 1] = '\0';
    printf("Franquicia: %s\n", franquicia);

    /* solicitar y validar fecha de expiración en formato MM/YY */
    printf("Ingrese la fecha de expiración (MM/YY): ");
    if (scanf("%5s", expiry) != 1) {
        printf("Entrada inválida. Operación cancelada.\n");
        return;
    }
    if (!validarExpiracion(expiry)) {
        printf("Fecha de expiración inválida o tarjeta expirada. Operación cancelada.\n");
        return;
    }

    /* solicitar y validar CVV antes del monto */
    printf("Ingrese CVV: ");
    if (scanf("%4s", cvv) != 1) {
        printf("Entrada inválida. Operación cancelada.\n");
        return;
    }
    int cvvlen = strlen(cvv);
    if (!(cvvlen == 3 || cvvlen == 4)) {
        printf("CVV inválido (debe tener 3 o 4 dígitos). Operación cancelada.\n");
        return;
    }
    for (int i = 0; i < cvvlen; ++i) {
        if (cvv[i] < '0' || cvv[i] > '9') {
            printf("CVV inválido (solo dígitos). Operación cancelada.\n");
            return;
        }
    }

    printf("Ingrese el monto de la compra: ");
    if (scanf("%lf", &monto) != 1 || monto <= 0.0) {
        printf("Monto inválido. Operación cancelada.\n");
        return;
    }

    /* Asignar valores a la transacción */
    t->referencia = generarReferencia();
    t->monto = monto;
    strncpy(t->pan, pan, sizeof(t->pan) - 1);
    t->pan[sizeof(t->pan) - 1] = '\0';
    /* Copiar el CVV validado a la transacción para que pueda ser verificado posteriormente */
    strncpy(t->cvv, cvv, sizeof(t->cvv) - 1);
    t->cvv[sizeof(t->cvv) - 1] = '\0';

    time_t now = time(NULL);
    struct tm *lt = localtime(&now);
    int day = lt ? lt->tm_mday : 0;
    int mon = lt ? (lt->tm_mon + 1) : 0;
    int yr  = lt ? ((lt->tm_year + 1900) % 100) : 0;
    if (day < 0) day = 0; else if (day > 99) day %= 100;
    if (mon  < 0) mon  = 0; else if (mon  > 99) mon  %= 100;
    if (yr   < 0) yr   = 0; else if (yr   > 99) yr   %= 100;

    /* Formatear fecha en buffer temporal y copiar de forma segura */
    char tmpfecha[16];
    snprintf(tmpfecha, sizeof tmpfecha, "%02d/%02d/%02d", day, mon, yr);
    strncpy(t->fecha, tmpfecha, sizeof(t->fecha) - 1);
    t->fecha[sizeof(t->fecha) - 1] = '\0';

    t->tipo = TIPO_COMPRA;
    t->anulada = 0;

    /* Guardar transacción en archivo */
    guardarTransaccion(t);

    compras_realizadas++;

    printf("\n✅ Compra realizada con éxito!\n");
    printf("Referencia: %d | Monto: %.2f | Franquicia: %s | Fecha: %s\n",
           t->referencia, t->monto, franquicia, t->fecha);
}

/* ------------------- FUNCIONES AUXILIARES ------------------- */

/* Algoritmo de Luhn para validar el PAN */
int validarLump(const char *pan) {
    if (!pan) return 0;
    int len = strlen(pan);
    int sum = 0;
    int alt = 0;
    for (int i = len - 1; i >= 0; i--) {
        if (pan[i] < '0' || pan[i] > '9') return 0;
        int digit = pan[i] - '0';
        if (alt) {
            digit *= 2;
            if (digit > 9) digit -= 9;
        }
        sum += digit;
        alt = !alt;
    }
    return (sum % 10 == 0);
}

/* Valida formato y contenido del PAN */
int validarPAN(const char *pan) {
    if (!pan) return 0;
    int len = strlen(pan);
    if (len < 13 || len > 19) {
        return 0; /* PAN debe tener entre 13 y 19 dígitos */
    }
    for (int i = 0; i < len; i++) {
        if (pan[i] < '0' || pan[i] > '9') {
            return 0; /* PAN debe contener solo dígitos */
        }
    }
    return validarLump(pan);
}

/* Valida MM/YY y que no esté vencida */
int validarExpiracion(const char *mmYY) {
    if (!mmYY || strlen(mmYY) != 5) return 0;
    if (mmYY[2] != '/') return 0;
    for (int i = 0; i < 5; ++i) {
        if (i == 2) continue;
        if (mmYY[i] < '0' || mmYY[i] > '9') return 0;
    }
    int month = (mmYY[0]-'0')*10 + (mmYY[1]-'0');
    int year = (mmYY[3]-'0')*10 + (mmYY[4]-'0');
    if (month < 1 || month > 12) return 0;

    time_t now = time(NULL);
    struct tm *lt = localtime(&now);
    int curMonth = lt ? (lt->tm_mon + 1) : 0;
    int curYear = lt ? ((lt->tm_year + 1900) % 100) : 0;

    if (year > curYear) return 1;
    if (year == curYear && month >= curMonth) return 1;
    return 0;
}

/* Detecta la franquicia por el prefijo del PAN */
const char* validarFranquicia(const char *pan) {
    if (!pan) return "Desconocida";
    if (strncmp(pan, "4", 1) == 0)
        return "Visa";
    else if (strncmp(pan, "5", 1) == 0)
        return "MasterCard";
    else if (strncmp(pan, "3", 1) == 0 && (pan[1] == '4' || pan[1] == '7'))
        return "American Express";
    else if (strncmp(pan, "6", 1) == 0)
        return "Discover";
    else
        return "Desconocida";
}

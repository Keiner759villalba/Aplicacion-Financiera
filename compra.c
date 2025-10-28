#include "compra.h"
#include "transaccion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TRANSACCIONES 20

// Declaraciones internas
static int leerCadena(const char *mensaje, char *dest, size_t tam);
static void limpiarBuffer(void);

// Contar compras efectivas guardadas en el archivo
int contarComprasEfectivas() {
    FILE *f = fopen("transacciones.dat", "rb");
    if (!f) return 0;

    Transaccion t;
    int count = 0;

    while (fread(&t, sizeof(Transaccion), 1, f)) {
        if (t.tipo == TIPO_COMPRA && t.anulada == 0)
            count++;
    }

    fclose(f);
    return count;
}

int realizarCompra(Transaccion *t) {
    char pan[20];
    char franquicia[32];
    char expiry[6];
    char cvv[5];
    double monto;

    if (!t) return 1;

    int compras_realizadas = contarComprasEfectivas();
    if (compras_realizadas >= MAX_TRANSACCIONES) {
        printf("Se alcanzo el numero maximo de compras (%d).\n", MAX_TRANSACCIONES);
        return 1;
    }

    printf("=== Realizar Compra ===\n");

    if (!leerCadena("Ingrese el PAN (Primary Account Number): ", pan, sizeof(pan)))
        return 1;
    if (!validarPAN(pan)) {
        printf("PAN invalido.\n");
        return 1;
    }

    const char *fr = validarFranquicia(pan);
    strncpy(franquicia, fr, sizeof(franquicia) - 1);
    franquicia[sizeof(franquicia) - 1] = '\0';
    printf("Franquicia detectada: %s\n", franquicia);

    if (!leerCadena("Ingrese la fecha de expiracion (MM/YY): ", expiry, sizeof(expiry)))
        return 1;
    if (!validarExpiracion(expiry)) {
        printf("Fecha de expiracion invalida o tarjeta vencida.\n");
        return 1;
    }
    limpiarBuffer();

    if (!leerCadena("Ingrese el CVV: ", cvv, sizeof(cvv)))
        return 1;
    int cvvlen = strlen(cvv);
    if (!(cvvlen == 3 || cvvlen == 4)) {
        printf("CVV invalido (3 o 4 digitos).\n");
        return 1;
    }

    printf("Ingrese el monto de la compra: ");
    if (scanf("%lf", &monto) != 1 || monto <= 0.0) {
        printf("Monto invalido.\n");
        limpiarBuffer();
        return 1;
    }
    limpiarBuffer();

    // Guardar datos en la transaccion
    t->referencia = generarReferencia();
    t->monto = monto;
    strncpy(t->pan, pan, sizeof(t->pan) - 1);
    t->pan[sizeof(t->pan) - 1] = '\0';
    strncpy(t->cvv, cvv, sizeof(t->cvv) - 1);
    t->cvv[sizeof(t->cvv) - 1] = '\0';

    time_t now = time(NULL);
    struct tm *lt = localtime(&now);
    snprintf(t->fecha, sizeof(t->fecha), "%02d/%02d/%02d",
             lt->tm_mday, lt->tm_mon + 1, (lt->tm_year + 1900) % 100);
    t->tipo = TIPO_COMPRA;
    t->anulada = 0;

    guardarTransaccion(t);

    printf("\nCompra realizada con exito.\n");
    printf("Referencia: %d | Monto: %.2f | Franquicia: %s | Fecha: %s\n",
           t->referencia, t->monto, franquicia, t->fecha);

    return 0;
}

// Lectura de cadenas sin dejar saltos de linea
static int leerCadena(const char *mensaje, char *dest, size_t tam) {
    printf("%s", mensaje);
    fflush(stdout);

    if (fgets(dest, (int)tam, stdin) == NULL) {
        printf("Error al leer la entrada.\n");
        return 0;
    }

    size_t len = strlen(dest);
    if (len > 0 && dest[len - 1] == '\n') dest[len - 1] = '\0';
    if (strlen(dest) == 0) {
        printf("No se permite campo vacio.\n");
        return 0;
    }
    return 1;
}

static void limpiarBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Algoritmo de Luhn
int validarPAN(const char *pan) {
    int len = (int)strlen(pan);
    if (len < 13 || len > 19) return 0;

    int suma = 0;
    int doble = 0;
    for (int i = len - 1; i >= 0; i--) {
        int digito = pan[i] - '0';
        if (digito < 0 || digito > 9) return 0;

        if (doble) {
            digito *= 2;
            if (digito > 9) digito -= 9;
        }
        suma += digito;
        doble = !doble;
    }
    return (suma % 10) == 0;
}

// Validacion de fecha de expiracion
int validarExpiracion(const char *mmYY) {
    if (strlen(mmYY) != 5 || mmYY[2] != '/') return 0;

    int mes = (mmYY[0] - '0') * 10 + (mmYY[1] - '0');
    int anio = (mmYY[3] - '0') * 10 + (mmYY[4] - '0');

    if (mes < 1 || mes > 12) return 0;

    time_t now = time(NULL);
    struct tm *lt = localtime(&now);
    int anioActual = (lt->tm_year + 1900) % 100;
    int mesActual = lt->tm_mon + 1;

    if (anio < anioActual || (anio == anioActual && mes < mesActual)) {
        return 0; // Tarjeta vencida
    }

    return 1;
}

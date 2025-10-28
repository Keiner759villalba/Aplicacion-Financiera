#include "anulacion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "transaccion.h"



// Limpia el buffer de entrada de stdin
void limpiarBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

// Elimina el salto de linea final de una cadena
void quitarSaltoDeLinea(char *str) {
    if (!str) return;
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
}
int leerCadenaAnulacion(const char *mensaje, char *dest, size_t tam) {
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

// Lee una linea de forma segura y controla el buffer
void leerLineaSegura(char *buffer, size_t tamano) {
    if (fgets(buffer, (int)tamano, stdin)) {
        quitarSaltoDeLinea(buffer);

        if (strlen(buffer) == 0)
            buffer[0] = '\0';

        // Si no hay salto, limpiar el resto del buffer
        if (!strchr(buffer, '\n'))
            limpiarBufferEntrada();
    } else {
        buffer[0] = '\0';
    }
}

// Lectura segura para campos cortos (como referencia o CVV)
int leerEntrada(const char *mensaje, char *dest, size_t tam) {
    printf("%s", mensaje);
    fflush(stdout);
    leerLineaSegura(dest, tam);

    if (strlen(dest) == 0) {
        printf("No se permite campo vacio.\n");
        return 0;
    }
    return 1;
}

// Funcion generica para leer entrada en anulacion
int leerEntradaAnulacion(const char *mensaje, void *destino, const char *formato, size_t tam_max) {
    char entrada[64];
    printf("%s", mensaje);
    
    if (fgets(entrada, sizeof(entrada), stdin) == NULL) {
        return 0;
    }

    // Remover salto de línea si existe
    size_t len = strlen(entrada);
    if (len > 0 && entrada[len-1] == '\n') {
        entrada[len-1] = '\0';
        len--;
    }

    // Validar longitud para campos con tamaño máximo
    if (tam_max > 0 && len > tam_max) {
        return 0;
    }

    // Convertir según el formato requerido
    if (sscanf(entrada, formato, destino) != 1) {
        return 0;
    }

    return 1;
}

// Funcion principal de anulacion
int anularTransaccion()
{
    int refTransaccion;
    char panConfirmacion[5];
    char cvvConfirmacion[5];
    char entrada[32];  // moved declaration to start of function

    FILE *f = fopen("transacciones.dat", "rb+");
    if (!f)
    {
        printf("No se encontro el archivo de transacciones.\n");
        return -1;  // return error code
    }

    if (!leerEntradaAnulacion("Ingrese la referencia de la transaccion a anular: ", 
                             &refTransaccion, "%d", 0)) {
        printf("Referencia invalida. Anulacion cancelada.\n");
        fclose(f);
        return -1;
    }

    if (refTransaccion <= 0) {
        printf("Referencia invalida. Anulacion cancelada.\n");
        fclose(f);
        return -1;
    }

    Transaccion t;
    int encontrada = 0;

    // Buscar la transaccion
    while (fread(&t, sizeof(Transaccion), 1, f)) {
        if (t.referencia == refTransaccion) {
            encontrada = 1;

            if (t.anulada == 1) {
                printf("La transaccion ya esta anulada.\n");
                fclose(f);
                return -1;
            }

            // Confirmar PAN
            printf("Ingrese los ultimos 4 digitos del PAN para confirmar: ");
            if (fgets(entrada, sizeof(entrada), stdin) == NULL || sscanf(entrada, "%4s", panConfirmacion) != 1)
            {
                printf("PAN invalido. Anulacion cancelada.\n");
                fclose(f);
                return -1;
            }

            if (strlen(panConfirmacion) != 4) {
                printf("Debe ingresar exactamente 4 digitos.\n");
                fclose(f);
                return -1;
            }

            if (strncmp(t.pan + strlen(t.pan) - 4, panConfirmacion, 4) != 0) {
                printf("PAN no coincide. Anulacion cancelada.\n");
                fclose(f);
                return -1;
            }

            // Confirmar CVV
            printf("Ingrese el CVV para confirmar: ");
            if (fgets(entrada, sizeof(entrada), stdin) == NULL || sscanf(entrada, "%4s", cvvConfirmacion) != 1)
            {
                printf("CVV invalido. Anulacion cancelada.\n");
                fclose(f);
                return -1;
            }

            // Remove duplicated CVV check
            if (strcmp(t.cvv, cvvConfirmacion) != 0)
            {
                printf("CVV no coincide. Anulacion cancelada.\n");
                fclose(f);
                return -1;
            }

            // Marcar como anulada
            t.anulada = 1;
            t.tipo = TIPO_ANULACION;

            fseek(f, -(long)sizeof(Transaccion), SEEK_CUR);
            fwrite(&t, sizeof(Transaccion), 1, f);
            fflush(f);

            printf("Transaccion con referencia %d anulada exitosamente.\n", refTransaccion);
            fclose(f);
            return 0; // success
        }
    }

    // Add return values
    if (!encontrada) {
        printf("No se encontro la transaccion con referencia %d.\n", refTransaccion);
        fclose(f);
        return -1;
    }

    fclose(f);
    return 0; // success
}


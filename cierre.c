#include "cierre.h"
#include "transaccion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* muestra resumen + opcion para vaciar el archivo */
int leerEntero(const char *mensaje, int *dest);

int mostrarCierre(void)
{
    FILE *f = fopen("transacciones.dat", "rb");
    if (!f) {
        printf("No se encontro el archivo de transacciones.\n");
        return 1;
    }

    Transaccion t;
    int totalTransacciones = 0;
    int comprasEfectivas = 0;
    int anulaciones = 0;
    int cierres = 0;
    int reimpresiones = 0;
    int reportes = 0;

    double montoCompras = 0.0;
    double montoAnulaciones = 0.0;

    while (fread(&t, sizeof(Transaccion), 1, f) == 1) {
        totalTransacciones++;

        /* contar por tipo (si existe registro de tipo) */
        switch (t.tipo) {
            case TIPO_COMPRA:
                if (t.anulada == 0) {
                    comprasEfectivas++;
                    montoCompras += t.monto;
                } else {
                    /* si la compra esta marcada como anulada la contabilizamos como anulacion */
                    anulaciones++;
                    montoAnulaciones += t.monto;
                }
                break;
            case TIPO_ANULACION:
                anulaciones++;
                /* si el registro de anulacion lleva monto, sumarlo tambien */
                montoAnulaciones += t.monto;
                break;
            
            default:
                /* si el tipo no esta definido, no contamos aqui */
                break;
        }
    }

    fclose(f);

    double montoNeto = montoCompras - montoAnulaciones;

    limpiarConsola();
    printf("=== CIERRE DE TRANSACCIONES ===\n\n");
    printf("Total de registros (registros en archivo): %d\n", totalTransacciones);
    printf("Compras efectivas: %d\n", comprasEfectivas);
    printf("Anulaciones: %d\n", anulaciones);
    
    printf("\nMonto total de compras (no anuladas):     %.2f\n", montoCompras);
    printf("Monto total de anulaciones (registradas): %.2f\n", montoAnulaciones);
    printf("Monto neto procesado:                      %.2f\n", montoNeto);
    printf("\n==============================\n");

    int confirmar = 0;
    if (!leerEntero("Confirma el cierre y vacia el archivo de transacciones? (1=Si / 0=No): ", &confirmar)) {
        printf("Entrada invalida. Cierre cancelado.\n");
        return 1;
    }
    if (confirmar != 0 && confirmar != 1) {
        printf("Entrada invalida. Cierre cancelado.\n");
        return 1;
    }
    if (confirmar == 1) {
        FILE *fw = fopen("transacciones.dat", "wb");
        if (!fw) {
            printf("No se pudo abrir el archivo para limpiar.\n");
            return 1;
        }
        fclose(fw);
        printf("Archivo de transacciones vaciado correctamente.\n");
    } else {
        printf("Cierre cancelado por el usuario.\n");
    }

    return 0;
}

/* helper: lee entero desde stdin con fgets y validacion */
int leerEntero(const char *mensaje, int *dest)
{
    char buffer[32];
    char *endptr;
    printf("%s", mensaje);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') buffer[len-1] = '\0';
    if (buffer[0] == '\0') return 0;
    long val = strtol(buffer, &endptr, 10);
    if (*endptr != '\0') return 0;
    *dest = (int)val;
    return 1;
}

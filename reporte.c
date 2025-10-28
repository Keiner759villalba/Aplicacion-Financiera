#include <stdio.h>
#include <stdlib.h>
#include "transaccion.h"

int mostrarReporteTotales(void) {
    FILE *archivo = fopen("transacciones.dat", "rb");
    if (!archivo) {
        printf("Error: no se pudo abrir el archivo de transacciones.\n");
        return 1;
    }

    Transaccion t;
    int totalTransacciones = 0;
    int comprasEfectivas = 0;
    int anulaciones = 0;
    float montoCompras = 0;
    float montoAnulaciones = 0;

    while (fread(&t, sizeof(Transaccion), 1, archivo)) {
        totalTransacciones++;

        if (t.anulada == 0) { // activa = compra efectiva
            comprasEfectivas++;
            montoCompras += t.monto;
        } else if (t.anulada == 1) { // anulada
            anulaciones++;
            montoAnulaciones += t.monto;
        }
    }
    

    fclose(archivo);

    float montoNeto = montoCompras - montoAnulaciones;

    printf("\n=== REPORTE DE TOTALES ===\n");
    printf("Total de transacciones: %d\n", totalTransacciones);
    printf("Compras efectivas: %d\n", comprasEfectivas);
    printf("Anulaciones: %d\n", anulaciones);
    printf("-----------------------------\n");
    printf("Monto total de compras:     $%.2f\n", montoCompras);
    printf("Monto total de anulaciones: $%.2f\n", montoAnulaciones);
    printf("-----------------------------\n");
    printf("Monto neto procesado:       $%.2f\n", montoNeto);
    printf("=============================\n\n");
    printf("Presione Enter para volver al menú...");
    getchar();
    return 0;
}

#include "cierre.h"
#include "transaccion.h"
#include <stdio.h>
#include <stdlib.h>

void cantidadTransaccionesCierre()
{
    int cantidad = obtenerCantidadTransacciones();
    printf("Cantidad total de transacciones realizadas: %d\n", cantidad);
}
void montoTotalCierre()
{
    FILE *f = fopen("transacciones.dat", "rb");
    if (!f)
    {
        printf("No se encontró el archivo de transacciones.\n");
        return;
    }

    Transaccion t;
    double total = 0.0;

    while (fread(&t, sizeof(Transaccion), 1, f))
    {
        if (t.anulada == 0)
        { // Solo sumar transacciones no anuladas
            total += t.monto;
        }
    }
    fclose(f);
    printf("Monto total de transacciones realizadas: %.2f\n", total);
}

void tiposDeTransaccionesCierre()
{
    FILE *f = fopen("transacciones.dat", "rb");
    if (!f)
    {
        printf("No se encontró el archivo de transacciones.\n");
        return;
    }

    Transaccion t;
    int compras = 0, anulaciones = 0, cierres = 0, reimpresiones = 0, reportes = 0;

    while (fread(&t, sizeof(Transaccion), 1, f))
    {
        if (t.anulada == 0)
        { // Solo contar transacciones no anuladas
            switch (t.tipo)
            {
            case TIPO_COMPRA:
                compras++;
                break;
            case TIPO_ANULACION:
                anulaciones++;
                break;
            case TIPO_CIERRE:
                cierres++;
                break;
            case TIPO_REIMPRESION:
                reimpresiones++;
                break;
            case TIPO_REPORTE:
                reportes++;
                break;
            }
        }
    }
    fclose(f);
    printf("Tipos de transacciones realizadas:\n");
    printf("Compras: %d\n", compras);
    printf("Anulaciones: %d\n", anulaciones);
    printf("Cierres: %d\n", cierres);
    printf("Reimpresiones: %d\n", reimpresiones);
    printf("Reportes: %d\n", reportes);
}

void mostrarCierre()
{
    limpiarConsola();
    printf("=== CIERRE DE TRANSACCIONES ===\n\n");
    cantidadTransaccionesCierre();
    tiposDeTransaccionesCierre();
    montoTotalCierre();

    printf("\n==============================\n");
    printf("Esta seguro que desea realizar el cierre? (1=Sí / 0=No): ");
    int confirmar;
    if (scanf("%d", &confirmar) != 1 || (confirmar != 0 && confirmar != 1))
    {
        printf("Entrada inválida. Cierre cancelado.\n");
        return;
    }
    else if (confirmar == 1)
    {
        FILE *f = fopen("transacciones.dat", "wb");
        if (!f)
        {
            printf("No se pudo abrir el archivo para limpiar.\n");
            return;
        }
        fclose(f);
        printf("✅ Archivo de transacciones vaciado correctamente.\n");
    }
    else
    {
        printf("\nCierre cancelado por el usuario.\n");
    }
}
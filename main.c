#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compra.h"
#include "transaccion.h"
#include "anulacion.h"
#include "reimpresion.h"
#include "reporte.h"
#include "cierre.h"

void limpiarConsola(void);
void pausar(void);
int leerOpcionMenu(void);
int preguntarContinuar(const char *mensaje);

int main() {
    int opcion;
    int continuar = 1;
    Transaccion t;

    // Reset reference persistence at startup (truncate or create empty file)
    // This ensures references start from 1 each run (or until cierre resets differently).
    {
        FILE *fref = fopen("last_ref.dat", "wb");
        if (fref) fclose(fref);
    }

    do {
        limpiarConsola();

        printf("=============================================\n");
        printf("         SISTEMA FINANCIERO CONSOLA\n");
        printf("=============================================\n");
        printf("|   1 |  Realizar compra                    |\n");
        printf("|   2 |  Anular transaccion                 |\n");
        printf("|   3 |  Cierre de transacciones            |\n");
        printf("|   4 |  Reimpresion de transacciones       |\n");
        printf("|   5 |  Reporte de totales                 |\n");
        printf("|   6 |  Salir                              |\n");
        printf("=============================================\n");

        opcion = leerOpcionMenu();
        limpiarConsola();

        switch (opcion) {
            case 1:
                // MODO COMPRA: ejecutar compra, mostrar datos, pausar y luego limpiar pantalla
                printf(">>> MODO COMPRA <<<\n\n");
                if (realizarCompra(&t) != 0) {
                    printf("\nError en la compra.\n");
                    pausar();           // permitir ver el error
                } else {
                    pausar();           // mostrar los datos de la compra antes de limpiar
                }
                limpiarConsola();
                break;

            case 2:
                // Anulacion: mostrar resultado y volver al menu (no cerrar la aplicacion)
                if (anularTransaccion() != 0) {
                    printf("\nError en la anulacion.\n");
                } else {
                    printf("\nAnulacion realizada correctamente.\n");
                }
                pausar();
                break;

            case 3: {
                int rc = mostrarCierre();
                if (rc == 0) {
                    printf("\nCierre realizado correctamente.\n");
                    pausar();
                } else if (rc == 2) {
                    /* User cancelled: volver al menu */
                    pausar();
                } else {
                    printf("\nError en el cierre.\n");
                    pausar();
                }
                break;
            }

            case 4:
                procesarReimpresion();
                pausar();
                break;

            case 5:
                mostrarReporteTotales();
                pausar();
                break;

            case 6:
                continuar = 0;
                break;

            default:
                printf("Opcion invalida.\n");
                pausar();
        }

    } while (continuar);

    return 0;
}

void limpiarConsola(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pausar(void) {
    printf("\nPresione Enter para continuar...");
    while (getchar() != '\n');
}

int leerOpcionMenu(void) {
    char entrada[10];
    int opcion = 0;
    printf("Seleccione una opcion: ");
    if (fgets(entrada, sizeof(entrada), stdin) != NULL)
        sscanf(entrada, "%d", &opcion);
    return opcion;
}

int preguntarContinuar(const char *mensaje) {
    char entrada[10];
    int respuesta = 0;
    printf("%s ", mensaje);
    if (fgets(entrada, sizeof(entrada), stdin) != NULL)
        sscanf(entrada, "%d", &respuesta);
    return respuesta;
}

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
                if (realizarCompra(&t) != 0) {
                    printf("\nError en la compra.\n");

                    // Limpia cualquier \n pendiente antes de preguntar
                    while (getchar() != '\n' && !feof(stdin));

                    if (!preguntarContinuar("Desea volver al menu principal? (1=Si / 0=Salir): "))
                        continuar = 0;
                } else {
                    printf("\nCompra realizada correctamente.\n");

                    while (getchar() != '\n' && !feof(stdin));

                    if (!preguntarContinuar("Desea realizar otra transaccion? (1=Si / 0=No): "))
                        continuar = 0;
                }
                break;

            case 2:
                if (anularTransaccion() != 0) {
                    printf("\nError en la anulacion.\n");

                    while (getchar() != '\n' && !feof(stdin));

                    if (!preguntarContinuar("Desea volver al menu principal? (1=Si / 0=Salir): "))
                        continuar = 0;
                } else {
                    printf("\nAnulacion realizada correctamente.\n");

                    while (getchar() != '\n' && !feof(stdin));

                    if (!preguntarContinuar("Desea realizar otra transaccion? (1=Si / 0=No): "))
                        continuar = 0;
                }
                break;

            case 3:
                if (mostrarCierre() != 0) {
                    printf("\nError en el cierre.\n");

                    while (getchar() != '\n' && !feof(stdin));

                    if (!preguntarContinuar("Desea volver al menu principal? (1=Si / 0=Salir): "))
                        continuar = 0;
                } else {
                    printf("\nCierre realizado correctamente.\n");

                    while (getchar() != '\n' && !feof(stdin));

                    if (!preguntarContinuar("Desea realizar otra transaccion? (1=Si / 0=No): "))
                        continuar = 0;
                }
                break;

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

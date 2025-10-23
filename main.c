#include <stdio.h>
#include <stdlib.h>
#include "compra.h"
#include "transaccion.h"
#include "anulacion.h"
#include "reimpresion.h"
#include "reporte.h"
#include "cierre.h"

// Prototipo para la pausa entre pantallas
void pausar(void);

// --- FUNCION PRINCIPAL ---
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
        printf("Seleccione una opcion: ");

        if (scanf("%d", &opcion) != 1) {
            printf("\nEntrada invalida. Intente de nuevo.\n");
            while (getchar() != '\n');
            continue;
        }

        limpiarConsola();

        switch (opcion) {
            case 1:
                printf(">>> MODO COMPRA <<<\n\n");
                realizarCompra(&t);
                printf("\nDesea realizar otra compra? (1=Si / 0=No): ");
                if (scanf("%d", &continuar) != 1) {
                    printf("\nEntrada invalida, regresando al menu...\n");
                    while (getchar() != '\n');
                    continuar = 1;
                }
                break;

            case 2:
                printf(">>> MODO ANULACION <<<\n\n");
                anularTransaccion();
                pausar();
                break;

            case 3:
                printf(">>> CIERRE DE TRANSACCIONES <<<\n\n");
                mostrarCierre();
                pausar();
                break;

            case 4:
                printf(">>> REIMPRESION DE TRANSACCIONES <<<\n\n");
                procesarReimpresion();  // definida en reimpresion.c
                pausar();
                break;

            case 5:
                printf(">>> REPORTE DE TOTALES <<<\n\n");
                mostrarReporteTotales();
                pausar();
                break;

            case 6:
                printf("\nCerrando aplicacion...\n");
                continuar = 0;
                break;

            default:
                printf("\nOpcion invalida. Intente nuevamente.\n");
                pausar();
                break;
        }
    } while (continuar);

    return 0;
}

// --- FUNCIONES AUXILIARES ---
void pausar(void) {
    printf("\nPresione Enter para continuar...");
    getchar(); getchar(); // dos veces por el salto de linea pendiente
}

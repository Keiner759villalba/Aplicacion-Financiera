#include "cierre.h"
#include <stdio.h>
#include <stdlib.h>
#include "compra.h"
#include "transaccion.h"
#include "anulacion.h"
#include "reimpresion.h"

int main() {
    int opcion;
    Transaccion t;
    int continuar = 1;

    do {
        limpiarConsola();
        printf("=== MENÚ PRINCIPAL ===\n");
        printf("1. Realizar compra\n");
        printf("2. Anular transacción\n");
        printf("3. Cierre de transacciones\n");
        printf("4. Reimpresión de transacciones\n");
        printf("5. Salir\n");
        printf("======================\n");
        printf("Seleccione una opcion: ");

        if (scanf("%d", &opcion) != 1) {
            printf("Entrada inválida. Intente de nuevo.\n");
            while (getchar() != '\n'); // limpiar el buffer
            continue;
        }

        switch (opcion) {
            case 1:
                realizarCompra(&t);
                printf("\n¿Desea realizar otra compra? (1=Sí / 0=No): ");
                if (scanf("%d", &continuar) != 1) {
                    printf("Entrada inválida, regresando al menú...\n");
                    while (getchar() != '\n');
                    continuar = 1;
                }
                break;

            case 2:
                limpiarConsola();
                anularTransaccion();
                printf("\nPresione Enter para volver al menú...");
                getchar(); getchar(); // doble getchar para pausar
                break;

            case 3:
                limpiarConsola();
                mostrarCierre();
                printf("\nPresione Enter para volver al menú...");
                getchar(); getchar(); // doble getchar para pausar

                break;
            case 4:
                limpiarConsola();
                {
                    FILE *ftrans = fopen("transacciones.dat", "rb");
                    if (!ftrans) {
                        printf("No hay transacciones registradas para reimpresión.\n");
                    } else {
                        if (fseek(ftrans, 0, SEEK_END) != 0) {
                            printf("No se pudo acceder al archivo de transacciones.\n");
                        } else {
                            long size = ftell(ftrans);
                            if (size < (long)sizeof(Transaccion)) {
                                printf("No hay transacciones válidas para reimpresión.\n");
                            } else {
                                /* Archivo existe y tiene al menos una transacción -> llamar módulo */
                                ordenarTransaccionesAtrasAdelante();
                            }
                        }
                        fclose(ftrans);
                    }

                    /* Consumir cualquier entrada pendiente y pausar */
                    int c;
                    while ((c = getchar()) != '\n' && c != EOF) { }
                    printf("\nPresione Enter para volver al menú...");
                    getchar();
                }
                break;
            case 5: 
                printf("Saliendo del programa...\n");
                continuar = 0;
                break;

            default:
                printf("Opción inválida. Intente de nuevo.\n");
                break;
        }
    } while (continuar);

    return 0;
}

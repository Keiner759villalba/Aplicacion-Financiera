#include "anulacion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "transaccion.h"

void limpiarBufferEntrada(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

void quitarSaltoDeLinea(char *str)
{
    if (!str)
        return;
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
}


void anularTransaccion()
{
    int refTransaccion;
    char panConfirmacion[5];
    char cvvConfirmacion[5];

    FILE *f = fopen("transacciones.dat", "rb+");
    if (!f)
    {
        printf("No se encontró el archivo de transacciones.\n");
        return;
    }

    printf("Ingrese la referencia de la transacción a anular: ");
    if (scanf("%d", &refTransaccion) != 1)
    {
        printf("Entrada inválida. Anulación cancelada.\n");
        fclose(f);
        return;
    }

    Transaccion t;
    int encontrada = 0;

    while (fread(&t, sizeof(Transaccion), 1, f))
    {
        if (t.referencia == refTransaccion)
        {
            encontrada = 1;

            if (t.anulada == 1)
            {
                printf("La transacción ya está anulada.\n");
                fclose(f);
                return;
            }

            printf("Ingrese los últimos 4 dígitos del PAN para confirmar: ");
            scanf("%4s", panConfirmacion);
            if (strncmp(t.pan + strlen(t.pan) - 4, panConfirmacion, 4) != 0)
            {
                printf("PAN no coincide. Anulación cancelada.\n");
                fclose(f);
                return;
            }

            printf("Ingrese el CVV para confirmar: ");
            limpiarBufferEntrada(); // limpia entrada previa
            if (!fgets(cvvConfirmacion, sizeof(cvvConfirmacion), stdin))
            {
                printf("Error leyendo CVV.\n");
                fclose(f);
                return;
            }
            quitarSaltoDeLinea(cvvConfirmacion);

            if (strcmp(t.cvv, cvvConfirmacion) != 0)
            {
                printf("CVV no coincide. Anulación cancelada.\n");
                fclose(f);
                return;
            }

            if (strcmp(t.cvv, cvvConfirmacion) != 0)
            {
                printf("CVV no coincide. Anulación cancelada.\n");
                fclose(f);
                return;
            }

            t.anulada = 1;
            t.tipo = TIPO_ANULACION;

            fseek(f, -(long)sizeof(Transaccion), SEEK_CUR);
            fwrite(&t, sizeof(Transaccion), 1, f);
            fflush(f);

            printf("Transacción con referencia %d anulada exitosamente.\n", refTransaccion);
            fclose(f);
            return;
        }
    }

    if (!encontrada)
    {
        printf("No se encontró la transacción con referencia %d.\n", refTransaccion);
    }

    fclose(f);
}

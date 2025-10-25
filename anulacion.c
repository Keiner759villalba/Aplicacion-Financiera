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

void leerLineaSegura(char *buffer, size_t tamano)
{
    if (fgets(buffer, (int)tamano, stdin))
    {
        quitarSaltoDeLinea(buffer);
        // si el usuario no escribio nada, forzar cadena vacia
        if (strlen(buffer) == 0)
            buffer[0] = '\0';

        // limpiar resto de entrada si el usuario escribio mas alla del limite
        if (!strchr(buffer, '\n'))
            limpiarBufferEntrada();
    }
    else
    {
        buffer[0] = '\0';
    }
}

int anularTransaccion()
{
    int refTransaccion;
    char panConfirmacion[5];
    char cvvConfirmacion[5];

    FILE *f = fopen("transacciones.dat", "rb+");
    if (!f)
    {
        printf("No se encontro el archivo de transacciones.\n");
        return 1;
    }

    printf("Ingrese la referencia de la transaccion a anular: ");
    if (scanf("%d", &refTransaccion) != 1)
    {
        printf("Entrada invalida. Anulacion cancelada.\n");
        limpiarBufferEntrada();
        fclose(f);
        return 1;
    }

    limpiarBufferEntrada(); // limpia salto despues del numero

    Transaccion t;
    int encontrada = 0;

    while (fread(&t, sizeof(Transaccion), 1, f))
    {
        if (t.referencia == refTransaccion)
        {
            encontrada = 1;

            if (t.anulada == 1)
            {
                printf("La transaccion ya esta anulada.\n");
                fclose(f);
                return 1;
            }

            printf("Ingrese los ultimos 4 digitos del PAN para confirmar: ");
            leerLineaSegura(panConfirmacion, sizeof(panConfirmacion));

            if (strlen(panConfirmacion) != 4)
            {
                printf("Debe ingresar exactamente 4 digitos.\n");
                fclose(f);
                return 1;
            }

            if (strncmp(t.pan + strlen(t.pan) - 4, panConfirmacion, 4) != 0)
            {
                printf("PAN no coincide. Anulacion cancelada.\n");
                fclose(f);
                return 1;
            }

            printf("Ingrese el CVV para confirmar: ");
            leerLineaSegura(cvvConfirmacion, sizeof(cvvConfirmacion));

            if (strlen(cvvConfirmacion) != 3)
            {
                printf("Debe ingresar exactamente 3 digitos.\n");
                fclose(f);
                return 1;
            }

            if (strcmp(t.cvv, cvvConfirmacion) != 0)
            {
                printf("CVV no coincide. Anulacion cancelada.\n");
                fclose(f);
                return 1;
            }

            t.anulada = 1;
            t.tipo = TIPO_ANULACION;

            fseek(f, -(long)sizeof(Transaccion), SEEK_CUR);
            fwrite(&t, sizeof(Transaccion), 1, f);
            fflush(f);

            printf("Transaccion con referencia %d anulada exitosamente.\n", refTransaccion);
            fclose(f);
            return 0;
        }
    }

    if (!encontrada)
    {
        printf("No se encontro la transaccion con referencia %d.\n", refTransaccion);
    }

    fclose(f);
    return 0;
}

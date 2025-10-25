#include "transaccion.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int transacciones_guardadas = 0;

int generarReferencia() {
    int last = 0;
    FILE *f = fopen(REF_FILE, "r");
    if (f) {
        if (fscanf(f, "%d", &last) != 1) {
            last = 0;
        }
        fclose(f);
    }
    int next = last + 1;

    /* reescribir el archivo con la nueva referencia */
    f = fopen(REF_FILE, "w");
    if (!f) {
        /* fallo al escribir: informar por stderr y devolver algo razonable */
        fprintf(stderr, "Error abriendo %s para escribir: %s\n", REF_FILE, strerror(errno));
        return next; /* devolver el siguiente aunque no se haya persistido */
    }
    fprintf(f, "%d\n", next);
    fflush(f);
    fclose(f);

    return next;
}

void guardarTransaccion(const Transaccion *t) {
    FILE *f = fopen("transacciones.dat", "ab");
    if (!f) {
        perror("Error al abrir el archivo de transacciones");
        return;
    }
    fwrite(t, sizeof(Transaccion), 1, f);
    transacciones_guardadas++;
    fclose(f);
}

void listarTransacciones() {
    FILE *f = fopen("transacciones.dat", "rb");
    if (!f) {
        printf("No hay transacciones registradas.\n");
        return;
    }

    Transaccion t;
    while (fread(&t, sizeof(Transaccion), 1, f)) {
        printf("Referencia: %d | Monto: %.2f | Tipo: %d | Estado: %s\n",
               t.referencia, t.monto, t.tipo,
               t.anulada ? "Anulada" : "Activa");
    }

    fclose(f);
}

int obtenerCantidadTransacciones() {
    return transacciones_guardadas;
}



/* Detecta la franquicia por el prefijo del PAN */
const char* validarFranquicia(const char *pan) {
    if (!pan) return "Desconocida";
    if (strncmp(pan, "4", 1) == 0)
        return "Visa";
    else if (strncmp(pan, "5", 1) == 0)
        return "MasterCard";
    else if (strncmp(pan, "3", 1) == 0 && (pan[1] == '4' || pan[1] == '7'))
        return "American Express";
    else if (strncmp(pan, "6", 1) == 0)
        return "Discover";
    else
        return "Desconocida";
}

#include "transaccion.h"
#include <stdio.h>
#include <stdlib.h>

int generarReferencia() {
    static int consecutivo = 1;
    return consecutivo++;
}

void guardarTransaccion(const Transaccion *t) {
    FILE *f = fopen("transacciones.dat", "ab");
    if (!f) {
        perror("Error al abrir el archivo de transacciones");
        return;
    }
    fwrite(t, sizeof(Transaccion), 1, f);
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

void limpiarConsola() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

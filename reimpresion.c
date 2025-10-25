#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "transaccion.h"
#include "compra.h"
#include "reimpresion.h"
typedef struct NodoTransaccion {
    Transaccion dato;
    struct NodoTransaccion *sig;
} NodoTransaccion;
int procesarReimpresion() {
    FILE *ftrans = fopen("transacciones.dat", "rb");
    if (!ftrans) {
        printf("No hay transacciones registradas.\n");
        return 1;
    }

    // Mover puntero al final para verificar tamaño del archivo
    if (fseek(ftrans, 0, SEEK_END) != 0) {
        printf("Error al acceder al archivo de transacciones.\n");
        fclose(ftrans);
        return 1;
    }

    long size = ftell(ftrans);
    fclose(ftrans);

    // Verifica si hay al menos una transaccion valida
    if (size < (long)sizeof(Transaccion)) {
        printf("No existen transacciones validas para reimpresion.\n");
        return 1;
    }

    printf("=== REIMPRESION DE TRANSACCIONES ===\n\n");
    ordenarTransaccionesAtrasAdelante();
    return 0;
}


const char* tipoTransaccionToString(TipoTransaccion tipo) {
    switch (tipo) {
        case TIPO_COMPRA:      return "Compra";
        case TIPO_ANULACION:   return "Anulada";
        case TIPO_CIERRE:      return "Cierre";
        case TIPO_REIMPRESION: return "Reimpresión";
        case TIPO_REPORTE:     return "Reporte";
        default:               return "Desconocido";
    }
}

void ordenarTransaccionesAtrasAdelante() {
    FILE *file = fopen(ARCHIVO_TRANSACCIONES, "rb");
    if (!file) {
        printf("No se pudo abrir el archivo de transacciones para ordenar.\n");
        return;
    }

    NodoTransaccion *inicio = NULL;
    Transaccion t;

    // Leer cada transacción del archivo
    while (fread(&t, sizeof(Transaccion), 1, file)) {
        NodoTransaccion *nuevo = (NodoTransaccion *)malloc(sizeof(NodoTransaccion));
        if (!nuevo) {
            printf("Error de memoria al ordenar transacciones.\n");
            fclose(file);
            return;
        }

        nuevo->dato = t;
        nuevo->sig = inicio; // Insertar al inicio (orden inverso)
        inicio = nuevo;
    }
    fclose(file);

    // Mostrar transacciones en orden descendente
    NodoTransaccion *aux = inicio;
    printf("\n===== REIMPRESIÓN DE TRANSACCIONES =====\n");
    
    while (aux) {
        printf("Ref: %d | PAN: %s | Franquicia: %s | Monto: %.2f | Tipo: %s\n",
               aux->dato.referencia, aux->dato.pan,
               validarFranquicia(aux->dato.pan),  // Correcto: validarFranquicia es el nombre de la función
              
               aux->dato.monto,  tipoTransaccionToString(aux->dato.tipo));
        aux = aux->sig;
    }

    printf("=========================================\n");

    // Liberar memoria
    while (inicio) {
        NodoTransaccion *temp = inicio;
        inicio = inicio->sig;
        free(temp);
    }
}

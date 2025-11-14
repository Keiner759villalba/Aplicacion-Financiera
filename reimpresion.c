#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>  // Para _getch() en Windows
#include "transaccion.h"
#include "compra.h"
#include "reimpresion.h"

// Estructura de nodo doble
typedef struct NodoDoble {
    Transaccion dato;
    struct NodoDoble *ant;
    struct NodoDoble *sig;
} NodoDoble;

// === Función auxiliar: enmascara el PAN ===
void enmascararPAN(const char *pan, char *panMask) {
    int len = strlen(pan);
    if (len <= 8) {
        strcpy(panMask, pan);
        return;
    }

    // Copia primeros 4
    strncpy(panMask, pan, 4);
    panMask[4] = '\0';

    // Agrega asteriscos
    for (int i = 4; i < len - 4; i++) {
        strcat(panMask, "*");
    }

    // Agrega últimos 4
    strcat(panMask, pan + len - 4);
}

// === Función para convertir tipo a texto ===
const char* tipoTransaccionToString(TipoTransaccion tipo) {
    switch (tipo) {
        case TIPO_COMPRA:      return "Compra";
        case TIPO_ANULACION:   return "Anulacion";
        case TIPO_CIERRE:      return "Cierre";
        case TIPO_REIMPRESION: return "Reimpresion";
        case TIPO_REPORTE:     return "Reporte";
        default:               return "Desconocido";
    }
}

// === Cargar lista doble desde archivo ===
NodoDoble* cargarTransacciones() {
    FILE *file = fopen(ARCHIVO_TRANSACCIONES, "rb");
    if (!file) {
        printf("No se pudo abrir el archivo de transacciones.\n");
        return NULL;
    }

    NodoDoble *inicio = NULL, *actual = NULL;
    Transaccion t;

    while (fread(&t, sizeof(Transaccion), 1, file)) {
        NodoDoble *nuevo = (NodoDoble*)malloc(sizeof(NodoDoble));
        nuevo->dato = t;
        nuevo->sig = NULL;
        nuevo->ant = actual;

        if (actual)
            actual->sig = nuevo;
        else
            inicio = nuevo;

        actual = nuevo;
    }

    fclose(file);

    // Ir al final para recorrer de atrás hacia adelante
    while (actual && actual->sig)
        actual = actual->sig;

    return actual; // Último nodo (más reciente)
}

// === Mostrar una transacción en formato bonito ===
void mostrarTransaccion(const Transaccion *t) {
    char panMask[32];
    enmascararPAN(t->pan, panMask);

    printf("\n=========================================\n");
    printf("Referencia : %-6d\n", t->referencia);
    printf("PAN         : %-19s\n", panMask);
    printf("Franquicia  : %-10s\n", validarFranquicia(t->pan));
    printf("Monto       : $%-10.2f\n", t->monto);
    printf("Tipo        : %-12s\n", tipoTransaccionToString(t->tipo));
    printf("=========================================\n");
}

// Declare the console clear function implemented in main.c
extern void limpiarConsola(void);

// === Reimpresión interactiva ===
int procesarReimpresion() {
    NodoDoble *actual = cargarTransacciones();
    if (!actual) {
        printf("No hay transacciones para reimprimir.\n");
        return 1;
    }

    printf("=== REIMPRESIÓN DE TRANSACCIONES ===\n");
    printf("Use 'a' (izquierda) y 'd' (derecha) para navegar. 'q' para salir.\n\n");

    char tecla;
    do {
        limpiarConsola(); // use centralized cleaner from main.c
        mostrarTransaccion(&actual->dato);

        // Mostrar hints de navegación
        printf("\n");
        if (actual->ant) printf("< ");
        printf("[q para salir]");
        if (actual->sig) printf(" >");
        printf("\n");

        tecla = _getch();
        if (tecla == 'a' && actual->ant)
            actual = actual->ant;
        else if (tecla == 'd' && actual->sig)
            actual = actual->sig;

    } while (tecla != 'q');

    // Liberar memoria
    while (actual && actual->ant)
        actual = actual->ant;

    while (actual) {
        NodoDoble *tmp = actual;
        actual = actual->sig;
        free(tmp);
    }

    printf("\nSaliendo de reimpresion...\n");
    return 0;
}

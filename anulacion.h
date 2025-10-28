#ifndef ANULACION_H
#define ANULACION_H

#include <stdio.h>
#include <stdlib.h>
#include "transaccion.h"

/**
 * Busca una transacción por su número de referencia,
 * valida los últimos 4 dígitos del PAN y el CVV,
 * y la marca como anulada sin eliminarla del archivo.
 */
int anularTransaccion(void);  // explicit void, returns int for error handling

void limpiarBufferEntrada(void);
void quitarSaltoDeLinea(char *str);
int leerCadenaAnulacion(const char *mensaje, char *dest, size_t tam);
int leerEntrada(const char *mensaje, char *buffer, size_t tamano);

#endif

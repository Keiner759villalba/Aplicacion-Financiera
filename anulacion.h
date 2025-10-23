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
void anularTransaccion();

void limpiarBufferEntrada(void);
void quitarSaltoDeLinea(char *str);

#endif

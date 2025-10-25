#ifndef REIMPRESION_H
#define REIMPRESION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compra.h"
#include "transaccion.h"


#define ARCHIVO_TRANSACCIONES "transacciones.dat"

int procesarReimpresion(void);
void ordenarTransaccionesAtrasAdelante(void);
const char* tipoTransaccionToString(TipoTransaccion tipo);


#endif 

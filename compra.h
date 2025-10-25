#ifndef COMPRA_H
#define COMPRA_H

#include "transaccion.h"

int realizarCompra(Transaccion *t);
int validarPAN(const char *pan);
int validarExpiracion(const char *mmYY);
const char* validarFranquicia(const char *pan);

#endif

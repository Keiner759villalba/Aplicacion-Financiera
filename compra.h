#ifndef COMPRA_H
#define COMPRA_H

#include "transaccion.h"

/* Máximo de compras permitidas (persistente en memoria por ejecución).
   Define también MAXCOMPRAS para compatibilidad con código previo. */
#ifndef MAXCOMPRAS
#define MAXCOMPRAS 20
#endif

/* Firma usada en el programa: main.c llama a realizarCompra(&t) */
void realizarCompra(Transaccion *t);

/* Validadores y utilidades expuestas por compra.c */
int validarPAN(const char *pan);
int validarLump(const char *pan);
int validarExpiracion(const char *mmYY);
const char* validarFranquicia(const char *pan);

#endif /* COMPRA_H */

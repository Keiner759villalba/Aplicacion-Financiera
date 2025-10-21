#ifndef CIERRE_H
#define CIERRE_H

#include "transaccion.h"

/* 
 * Muestra la cantidad total de transacciones realizadas.
 */
void cantidadTransaccionesCierre(void);

/* 
 * Muestra los tipos de transacciones realizadas (compra, anulación, etc.).
 */
void tiposDeTransaccionesCierre(void);

/* 
 * Muestra el monto total de las transacciones no anuladas.
 */
void montoTotalCierre(void);

/* 
 * Función principal del módulo de cierre: muestra el resumen,
 * pide confirmación y, si se acepta, elimina todas las transacciones.
 */
void mostrarCierre(void);

#endif /* CIERRE_H */

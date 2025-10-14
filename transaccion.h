#ifndef TRANSACCION_H
#define TRANSACCION_H

typedef enum {
    TIPO_COMPRA,
    TIPO_ANULACION,
    TIPO_CIERRE,
    TIPO_REIMPRESION,
    TIPO_REPORTE
} TipoTransaccion;

typedef struct {
    int referencia;
    TipoTransaccion tipo;
    double monto;
    char pan[20];
    char fecha[10];
    int anulada; // 0 = no, 1 = sí
} Transaccion;

int generarReferencia();
void guardarTransaccion(const Transaccion *t);
void listarTransacciones();
void limpiarConsola();

#endif

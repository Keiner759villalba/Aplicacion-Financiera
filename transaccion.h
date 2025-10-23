#ifndef TRANSACCION_H
#define TRANSACCION_H

/* Archivo donde se guarda la última referencia usada (persistente entre ejecuciones) */
#ifndef REF_FILE
#define REF_FILE "last_ref.dat"
#endif

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
    char cvv[5];
    char fecha[10];
    int anulada; /* 0 = no, 1 = sí */
    struct Transaccion *sig;
} Transaccion;

int generarReferencia(void);
void guardarTransaccion(const Transaccion *t);
void listarTransacciones(void);
int obtenerCantidadTransacciones(void);
void limpiarConsola(void);
const char* validarFranquicia(const char *pan);

#endif /* TRANSACCION_H */

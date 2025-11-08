# Detect OS and set commands accordingly
ifeq ($(OS),Windows_NT)
    RM = del /Q
    EXE = app.exe
    CLEAN_CMD = $(RM) *.o $(EXE)
else
    RM = rm -f
    EXE = app
    CLEAN_CMD = $(RM) *.o $(EXE)
endif

CC = gcc
CFLAGS = -Wall -Wextra
SRCS = main.c compra.c transaccion.c anulacion.c cierre.c reimpresion.c reporte.c
OBJS = $(SRCS:.c=.o)

# Target principal
$(EXE): $(OBJS)
	$(CC) $(OBJS) -o $(EXE)

# Regla para compilar .c a .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpiar archivos compilados
clean:
	$(CLEAN_CMD)

# Ejecutar el programa
run: $(EXE)
	./$(EXE)

# Instalar (copiar templates si no existen)
install:
	@if [ ! -f transacciones.dat ]; then \
		cp templates/transacciones.dat.template transacciones.dat; \
	fi
	@if [ ! -f last_ref.dat ]; then \
		cp templates/last_ref.dat.template last_ref.dat; \
	fi

.PHONY: clean run install

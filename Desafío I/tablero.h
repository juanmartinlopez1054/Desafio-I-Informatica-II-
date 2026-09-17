#ifndef tablero_h
#define tablero_h

#include "configuracion.h"

int calcular_bytes(int filas, int columnas);
int calcular_bits(int filas, int columnas);

void inicializar_tablero(unsigned char*& tablero, int filas, int columnas, int& capacidad_bytes);
void destruir_tablero(unsigned char*& tablero, int& capacidad_bytes);

unsigned char obtener_ficha(const unsigned char* tablero, int posicion);
void establecer_ficha(unsigned char* tablero, int posicion, unsigned char valor);

int obtener_posicion(int fila, int columna, int columnas);

void llenar_tablero_aleatorio(unsigned char* tablero, int filas, int columnas);

void copiar_tablero_logico(const unsigned char* origen, int filas_origen, int columnas_origen, unsigned char* destino, int filas_destino, int columnas_destino);

void reconstruir_tablero(unsigned char*& tablero, int filas_antiguas, int columnas_antiguas, int filas_nuevas, int columnas_nuevas,int& capacidad_bytes);

#endif

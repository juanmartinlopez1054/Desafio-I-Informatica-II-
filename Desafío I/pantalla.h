#ifndef pantalla_h
#define pantalla_h

void mostrar_juego(const unsigned char* tablero, int filas, int columnas, int eliminaciones_usuario, int fichas_eliminadas_total, int combinaciones_totales, int cascadas_movimiento, int puntuacion);
void mostrar_binario(const unsigned char* tablero, int filas, int columnas);
void mostrar_fichas(const unsigned char* tablero, int filas, int columnas);

#endif

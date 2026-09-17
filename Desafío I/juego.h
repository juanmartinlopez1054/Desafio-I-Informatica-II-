#ifndef juego_h
#define juego_h

void eliminar_ficha_individual(unsigned char* tablero, int filas, int columnas, int fila, int columna);

int marcar_combinaciones(const unsigned char* tablero, int filas, int columnas, unsigned char* marcadas);

int eliminar_marcadas(unsigned char* tablero, int filas, int columnas, const unsigned char* marcadas);

int aplicar_gravedad_y_rellenar(unsigned char* tablero, int filas, int columnas);

int resolver_cascadas(unsigned char* tablero, int filas, int columnas, int& fichas_eliminadas_total, int& combinaciones_totales);

bool agregar_fila(unsigned char*& tablero, int& filas, int columnas, int fila_insertar, int& capacidad_bytes);

bool eliminar_fila(unsigned char*& tablero, int& filas, int columnas, int fila_a_eliminar, int& capacidad_bytes);

bool agregar_columna(unsigned char*& tablero, int filas, int& columnas, int columna_insertar, int& capacidad_bytes);

bool eliminar_columna(unsigned char*& tablero, int filas, int& columnas, int columna_a_eliminar, int& capacidad_bytes);

#endif

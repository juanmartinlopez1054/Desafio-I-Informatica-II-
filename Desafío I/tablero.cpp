using namespace std;

int calcular_bits(int filas, int columnas){
    return filas*columnas*3;
}

int calcular_bytes(int filas, int columnas){
    int bits = calcular_bits(filas, columnas);
    return (bits+7)/8;
}

int obtener_posicion(int fila, int columna, int columnas){
    return fila*columnas + columna;
}
using namespace std;
#include <cstdlib>

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

void establecer_ficha(unsigned char* tablero, int posicion, unsigned char valor){
    valor = valor & 7;

    int posicion_bit = posicion*3;
    int posicion_byte = posicion_bit/8;
    int desplazamiento = posicion_bit%8;

    if (desplazamiento <= 5){
        unsigned char mascara = 7 << desplazamiento;
        unsigned char mascara_inversa = ~mascara;
        unsigned char valor_desplazado = valor << desplazamiento;
        unsigned char byte_actual = tablero[posicion_byte];
        unsigned char byte_limpio = byte_actual & mascara_inversa;
        unsigned char byte_nuevo = byte_limpio | valor_desplazado;
        tablero[posicion_byte] = byte_nuevo;
        return;
    }
    unsigned int byte_inferior = tablero[posicion_byte];
    unsigned int byte_superior = tablero[posicion_byte + 1];
    unsigned int dos_bytes = byte_inferior | (byte_superior << 8);
    unsigned int mascara = 7 << desplazamiento;
    unsigned int mascara_inversa = ~mascara;
    unsigned int valor_desplazado = valor << desplazamiento;

    dos_bytes = (dos_bytes & mascara_inversa) | valor_desplazado;

    tablero[posicion_byte] = dos_bytes & 255;
    tablero[posicion_byte+1] = (dos_bytes >> 8) & 255;
}

void inicializar_tablero(unsigned char*& tablero, int filas, int columnas, int& capacidad_bytes){
    capacidad_bytes = calcular_bytes(filas, columnas);
    tablero = new unsigned char[capacidad_bytes];

    for (int indice = 0; indice < capacidad_bytes; ++indice){
        tablero[indice] = 0;
    }
    llenar_tablero_aleatorio(tablero, filas, columnas);
}

void llenar_tablero_aleatorio(unsigned char* tablero, int filas, int columnas){
    int posiciones_totales = filas*columnas;

    for (int posicion = 0; posicion < posiciones_totales; ++posicion){
        unsigned char valor = (unsigned char)((rand()%6)+1);
        establecer_ficha(tablero, posicion, valor);
    }
}

void destruir_tablero(unsigned char*& tablero, int& capacidad_bytes){
    delete[] tablero;
    tablero = nullptr;
    capacidad_bytes = 0;
}
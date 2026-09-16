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

unsigned char obtener_ficha(const unsigned char* tablero, int posicion){
    int posicion_bit = posicion* 3;
    int posicion_byte = posicion_bit / 8;
    int desplazamiento = posicion_bit % 8;

    if (desplazamiento <= 5){
        unsigned char byte_actual = tablero[posicion_byte];
        unsigned char mascara = 7 << desplazamiento;
        unsigned char bits_aislados = byte_actual & mascara;
        unsigned char ficha = bits_aislados >> desplazamiento;
        return ficha;
    }
    unsigned int byte_inferior = tablero[posicion_byte];
    unsigned int byte_superior = tablero[posicion_byte + 1];
    unsigned int dos_bytes = byte_inferior | (byte_superior << 8);
    unsigned int dos_bytes_desplazados = dos_bytes >> desplazamiento;
    unsigned char ficha = dos_bytes_desplazados & 7;
    return ficha;
}

void copiar_tablero_logico(const unsigned char* origen, int filas_origen, int columnas_origen, unsigned char* destino, int filas_destino, int columnas_destino){
    int filas_a_copiar;
    if (filas_origen < filas_destino){
        filas_a_copiar = filas_origen;
    }
    else{
        filas_a_copiar = filas_destino;
    }
    int columnas_a_copiar;
    if (columnas_origen < columnas_destino){
        columnas_a_copiar = columnas_origen;
    }
    else{
        columnas_a_copiar = columnas_destino;
    }
    for (int fila = 0; fila < filas_a_copiar; fila+=1){
        for (int columna = 0; columna < columnas_a_copiar; ++columna){
            int posicion_origen = obtener_posicion(fila, columna, columnas_origen);
            int posicion_destino = obtener_posicion(fila, columna, columnas_destino);

            establecer_ficha(destino, posicion_destino, obtener_ficha(origen, posicion_origen));
        }
    }
}

void reconstruir_tablero(unsigned char*& tablero, int filas_antiguas, int columnas_antiguas, int filas_nuevas, int columnas_nuevas, int& capacidad_bytes){
    int bytes_requeridos = calcular_bytes(filas_nuevas, columnas_nuevas);
    bool debe_redimensionar = false;

    if (bytes_requeridos > capacidad_bytes){
        debe_redimensionar = true;
    }
    else if (bytes_requeridos*100 < capacidad_bytes*65){
        debe_redimensionar = true;
    }
    if (debe_redimensionar){
        unsigned char* nuevo_tablero = new unsigned char[bytes_requeridos];

        for (int indice = 0; indice < bytes_requeridos; ++indice){
            nuevo_tablero[indice] = 0;
        }
        copiar_tablero_logico(tablero, filas_antiguas, columnas_antiguas, nuevo_tablero, filas_nuevas, columnas_nuevas);

        delete[] tablero;
        tablero = nuevo_tablero;
        capacidad_bytes = bytes_requeridos;
    }
    else{
        unsigned char* temporal = new unsigned char[bytes_requeridos];

        for (int indice = 0; indice < bytes_requeridos; ++indice){
            temporal[indice] = 0;
        }

        copiar_tablero_logico(tablero, filas_antiguas, columnas_antiguas, temporal, filas_nuevas, columnas_nuevas);

        for (int indice = 0; indice < bytes_requeridos; ++indice){
            tablero[indice] = temporal[indice];
        }
        delete[] temporal;
    }
}
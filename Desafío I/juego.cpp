#include "configuracion.h"
#include <cstdlib>
#include <iostream>
#include "tablero.h"

using namespace std;

void eliminar_ficha_individual(unsigned char* tablero, int filas, int columnas, int fila, int columna){
    if (fila < 0 || fila >= filas || columna < 0 || columna >= columnas){
        return;
    }
    int posicion = obtener_posicion(fila, columna, columnas);
    if (obtener_ficha(tablero, posicion) != vacio){
        establecer_ficha(tablero, posicion, vacio);
    }
}

int marcar_combinaciones(const unsigned char* tablero, int filas, int columnas, unsigned char* marcadas){
    int combinaciones = 0;
    int posiciones_totales = filas * columnas;
    for (int indice = 0; indice < posiciones_totales; indice+=1){
        marcadas[indice] = 0;
    }
    for (int fila = 0; fila < filas; fila+=1){
        int inicio = 0;
        while (inicio < columnas){
            unsigned char valor = obtener_ficha(tablero, obtener_posicion(fila, inicio, columnas));
            if (valor == vacio){
                inicio+=1;
                continue;
            }
            int fin = inicio + 1;

            while (fin < columnas && obtener_ficha(tablero, obtener_posicion(fila, fin, columnas)) == valor){
                fin+=1;
            }
            if (fin - inicio >= 3){
                combinaciones+=1;

                for (int columna = inicio; columna < fin; columna+=1){
                    marcadas[obtener_posicion(fila, columna, columnas)] = 1;
                }
            }

            inicio = fin;
        }
    }
    for (int columna = 0; columna < columnas; columna+=1){
        int inicio = 0;

        while (inicio < filas){
            unsigned char valor =
                obtener_ficha(tablero, obtener_posicion(inicio, columna, columnas));

            if (valor == vacio){
                inicio+=1;
                continue;
            }
            int fin = inicio + 1;

            while (fin < filas && obtener_ficha(tablero, obtener_posicion(fin, columna, columnas)) == valor){
                fin+=1;
            }
            if (fin - inicio >= 3){
                combinaciones+=1;

                for (int fila = inicio; fila < fin; fila+=1){
                    marcadas[obtener_posicion(fila, columna, columnas)] = 1;
                }
            }

            inicio = fin;
        }
    }
    return combinaciones;
}

int eliminar_marcadas(unsigned char* tablero, int filas, int columnas, const unsigned char* marcadas){
    int eliminadas = 0;
    int posiciones_totales = filas*columnas;

    for (int posicion = 0; posicion < posiciones_totales; posicion+=1){
        if (marcadas[posicion] != 0 && obtener_ficha(tablero, posicion) != vacio){
            establecer_ficha(tablero, posicion, vacio);
            eliminadas+=1;
        }
    }
    return eliminadas;
}

int aplicar_gravedad_y_rellenar(unsigned char* tablero, int filas, int columnas){
    int movido = 0;

    for (int columna = 0; columna < columnas; columna += 1){
        int fila_escritura = filas - 1;

        for (int fila_leer = filas - 1; fila_leer >= 0; fila_leer-=1){
            int posicion_lectura = obtener_posicion(fila_leer, columna, columnas);
            unsigned char valor = obtener_ficha(tablero, posicion_lectura);

            if (valor != vacio){
                int posicion_escritura = obtener_posicion(fila_escritura, columna, columnas);

                if (posicion_escritura != posicion_lectura){
                    establecer_ficha(tablero, posicion_escritura, valor);
                    establecer_ficha(tablero, posicion_lectura, vacio);
                    movido = 1;
                }

                fila_escritura-=1;
            }
        }
        for (int fila = fila_escritura; fila >= 0; fila-=1){
            int posicion = obtener_posicion(fila, columna, columnas);

            unsigned char valor = (unsigned char)((rand()%6)+1);

            establecer_ficha(tablero, posicion, valor);
        }
    }

    return movido;
}

int resolver_cascadas(unsigned char* tablero, int filas, int columnas, int& fichas_eliminadas_total, int& combinaciones_totales){
    int cascadas_totales = 0;
    int posiciones_totales = filas*columnas;
    unsigned char* marcadas = new unsigned char[posiciones_totales];

    while (true){
        int combinaciones = marcar_combinaciones(tablero, filas, columnas, marcadas);

        if (combinaciones == 0){
            break;
        }
        int eliminadas_esta_ola = eliminar_marcadas(tablero, filas, columnas, marcadas);

        fichas_eliminadas_total += eliminadas_esta_ola;
        combinaciones_totales += combinaciones;

        cascadas_totales += 1;

        aplicar_gravedad_y_rellenar(tablero, filas, columnas);
    }
    delete[] marcadas;
    marcadas = nullptr;

    return cascadas_totales;
}

void copiar_cambio_fila(const unsigned char* tablero_antiguo, int columnas_antiguas, unsigned char* nuevo_tablero, int filas_nuevas, int columnas_nuevas, int fila_a_eliminar){
    for (int fila_nueva = 0; fila_nueva < filas_nuevas; fila_nueva+=1){
        int fila_antigua;

        if (fila_nueva < fila_a_eliminar){
            fila_antigua = fila_nueva;
        }
        else{
            fila_antigua = fila_nueva + 1;
        }
        for (int columna = 0; columna < columnas_antiguas; columna+=1){
            unsigned char ficha = obtener_ficha(tablero_antiguo, obtener_posicion(fila_antigua, columna, columnas_antiguas));
            int pos_nueva = obtener_posicion(fila_nueva, columna, columnas_nuevas);

            establecer_ficha(nuevo_tablero, pos_nueva, ficha);
        }
    }
}

void copiar_cambio_columna(const unsigned char* tablero_antiguo, int filas_antiguas, int columnas_antiguas, unsigned char* nuevo_tablero, int columnas_nuevas, int columna_eliminar){
    for (int fila = 0; fila < filas_antiguas; fila+=1){
        for (int columna_nueva = 0; columna_nueva < columnas_nuevas; columna_nueva+=1){
            int columna_antigua;

            if (columna_nueva < columna_eliminar){
                columna_antigua = columna_nueva;
            }
            else{
                columna_antigua = columna_nueva + 1;
            }
            unsigned char ficha = obtener_ficha(tablero_antiguo, obtener_posicion(fila, columna_antigua, columnas_antiguas));
            int pos_nueva = obtener_posicion(fila, columna_nueva, columnas_nuevas);
            establecer_ficha(nuevo_tablero, pos_nueva, ficha);
        }
    }
}
void confirmar_tablero_temporal(unsigned char*& tablero, unsigned char* temporal, int bytes_requeridos, int& capacidad_bytes, bool redimensionar_fisicamente){
    if (redimensionar_fisicamente){
        delete[] tablero;
        tablero = temporal;
        capacidad_bytes = bytes_requeridos;
        return;
    }
    for (int indice = 0; indice < bytes_requeridos; indice+=1){
        tablero[indice] = temporal[indice];
    }
    delete[] temporal;
}

bool agregar_fila(unsigned char*& tablero, int& filas, int columnas, int fila_insertar, int& capacidad_bytes)
{
    if (fila_insertar < 0 || fila_insertar > filas)
    {
        return false;
    }
    int columnas_antiguas = columnas;
    int filas_nuevas = filas + 1;
    int columnas_nuevas = columnas;
    int bytes_requeridos = calcular_bytes(filas_nuevas, columnas_nuevas);

    unsigned char* nuevo_tablero = new unsigned char[bytes_requeridos];

    for (int indice = 0; indice < bytes_requeridos; indice+=1){
        nuevo_tablero[indice] = 0;
    }
    for (int fila_nueva = 0; fila_nueva < filas_nuevas; fila_nueva+=1){
        if (fila_nueva == fila_insertar){
            for (int columna = 0; columna < columnas_nuevas; columna+=1){
                unsigned char ficha_aleatoria = (unsigned char)((rand() % 6) + 1);
                int pos_nueva = obtener_posicion(fila_nueva, columna, columnas_nuevas);

                establecer_ficha(nuevo_tablero, pos_nueva, ficha_aleatoria);
            }
        }
        else{
            int fila_antigua;

            if (fila_nueva < fila_insertar){
                fila_antigua = fila_nueva;
            }
            else{
                fila_antigua = fila_nueva - 1;
            }
            for (int columna = 0; columna < columnas_nuevas; columna+=1){
                unsigned char ficha = obtener_ficha(tablero, obtener_posicion(fila_antigua, columna, columnas_antiguas));
                int pos_nueva = obtener_posicion(fila_nueva, columna, columnas_nuevas);

                establecer_ficha(nuevo_tablero, pos_nueva, ficha);
            }
        }
    }
    bool redimensionar_fisicamente = bytes_requeridos > capacidad_bytes;
    confirmar_tablero_temporal(tablero, nuevo_tablero, bytes_requeridos, capacidad_bytes, redimensionar_fisicamente);
    filas = filas_nuevas;
    
    return true;
}

bool eliminar_fila(unsigned char*& tablero, int& filas, int columnas, int fila_a_eliminar, int& capacidad_bytes){
    if (filas <= 1 || fila_a_eliminar < 0 || fila_a_eliminar >= filas){
        return false;
    }

    int columnas_antiguas = columnas;
    int filas_nuevas = filas - 1;
    int columnas_nuevas = columnas;

    int bytes_requeridos = calcular_bytes(filas_nuevas, columnas_nuevas);

    unsigned char* nuevo_tablero = new unsigned char[bytes_requeridos];

    for (int indice = 0; indice < bytes_requeridos; indice+=1)
    {
        nuevo_tablero[indice] = 0;
    }

    copiar_cambio_fila(tablero, columnas_antiguas, nuevo_tablero, filas_nuevas, columnas_nuevas, fila_a_eliminar);
    bool redimensionar_fisicamente = bytes_requeridos * 100 < capacidad_bytes * 65;

    confirmar_tablero_temporal(tablero, nuevo_tablero, bytes_requeridos, capacidad_bytes, redimensionar_fisicamente);

    filas = filas_nuevas;

    return true;
}

bool agregar_columna(unsigned char*& tablero, int filas, int& columnas, int columna_insertar, int& capacidad_bytes){
    if (columna_insertar < 0 || columna_insertar > columnas){
        return false;
    }

    int columnas_antiguas = columnas;
    int filas_nuevas = filas;
    int columnas_nuevas = columnas + 1;

    int bytes_requeridos = calcular_bytes(filas_nuevas, columnas_nuevas);

    unsigned char* nuevo_tablero = new unsigned char[bytes_requeridos];

    for (int indice = 0; indice < bytes_requeridos; indice+=1){
        nuevo_tablero[indice] = 0;
    }

    for (int fila = 0; fila < filas_nuevas; fila+=1){
        for (int columna_nueva = 0; columna_nueva < columnas_nuevas; columna_nueva+=1){
            if (columna_nueva == columna_insertar){
                unsigned char ficha_aleatoria = (unsigned char)((rand()%6) + 1);
                int pos_nueva = obtener_posicion(fila, columna_nueva, columnas_nuevas);

                establecer_ficha(nuevo_tablero, pos_nueva, ficha_aleatoria);
            }
            else{
                int columna_antigua;

                if (columna_nueva < columna_insertar){
                    columna_antigua = columna_nueva;
                }
                else{
                    columna_antigua = columna_nueva - 1;
                }
                unsigned char ficha = obtener_ficha(tablero, obtener_posicion(fila, columna_antigua, columnas_antiguas));
                int pos_nueva = obtener_posicion(fila, columna_nueva, columnas_nuevas);

                establecer_ficha(nuevo_tablero, pos_nueva, ficha);
            }
        }
    }
    bool redimensionar_fisicamente = bytes_requeridos > capacidad_bytes;
    confirmar_tablero_temporal(tablero, nuevo_tablero, bytes_requeridos, capacidad_bytes, redimensionar_fisicamente);
    columnas = columnas_nuevas;

    return true;
}
//MIA
bool eliminar_columna(unsigned char*& tablero, int filas, int& columnas, int columna_a_eliminar, int& capacidad_bytes){
    if (columnas <= 1 || columna_a_eliminar < 0 || columna_a_eliminar >= columnas){
        return false;
    }

    int filas_antiguas = filas;
    int columnas_antiguas = columnas;
    int filas_nuevas = filas;
    int columnas_nuevas = columnas-1;

    int bytes_requeridos = calcular_bytes(filas_nuevas, columnas_nuevas);

    unsigned char* nuevo_tablero = new unsigned char[bytes_requeridos];

    for (int indice = 0; indice < bytes_requeridos; indice+=1){
        nuevo_tablero[indice] = 0;
        }

    copiar_cambio_columna(tablero, filas_antiguas, columnas_antiguas, nuevo_tablero, columnas_nuevas, columna_a_eliminar);

    bool redimensionar_fisicamente = bytes_requeridos*100 < capacidad_bytes*65;

    confirmar_tablero_temporal(tablero, nuevo_tablero, bytes_requeridos, capacidad_bytes, redimensionar_fisicamente);

    columnas = columnas_nuevas;

    return true;
}

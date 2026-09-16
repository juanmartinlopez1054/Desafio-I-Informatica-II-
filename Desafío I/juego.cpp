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
                inicio;
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
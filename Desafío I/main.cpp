#include <cstdlib>
#include <ctime>
#include <iostream>
#include "tablero.h"
#include "juego.h"
#include "pantalla.h"
#include "menu.h"
using namespace std;

bool leer_coordenada(const char* texto, int& valor){
    cout << texto;
    char entrada[50]; 
    cin >> entrada; 
    
    valor = 0;
    int indice = 0;

    while (entrada[indice] != '\0'){
        if (entrada[indice] >= '0' && entrada[indice] <= '9'){
            valor = (valor * 10) + (entrada[indice] - '0');
        }
        else {
            return false;
        }
        indice += 1;
    }
    return true; 
}

void jugar_eliminacion_individual(unsigned char* tablero, int filas, int columnas, int fila, int columna, int& fichas_eliminadas_total, int& combinaciones_totales, int& cascadas_movimiento, int& eliminaciones_usuario, int& puntuacion){
    if (fila < 0 || fila >= filas || columna < 0 || columna >= columnas){
        cout << "Posicion fuera del tablero.\n";
        cascadas_movimiento = 0;
        return;
    }

    int posicion = obtener_posicion(fila, columna, columnas);

    if (obtener_ficha(tablero, posicion) == vacio){
        cout << "La posicion esta vacia.\n";
        cascadas_movimiento = 0;
        return;
    }

    eliminar_ficha_individual(tablero, filas, columnas, fila, columna);
    eliminaciones_usuario = eliminaciones_usuario + 1;
    fichas_eliminadas_total = fichas_eliminadas_total + 1;
    
    aplicar_gravedad_y_rellenar(tablero, filas, columnas);
    cascadas_movimiento = resolver_cascadas(tablero, filas, columnas, fichas_eliminadas_total, combinaciones_totales);

    puntuacion += cascadas_movimiento;
}

void procesar_cambio_estructura(unsigned char* tablero, int filas, int columnas, int& fichas_eliminadas_total, int& combinaciones_totales, int& cascadas_movimiento, int& puntuacion){
    aplicar_gravedad_y_rellenar(tablero, filas, columnas);

    cascadas_movimiento = resolver_cascadas(tablero, filas, columnas, fichas_eliminadas_total, combinaciones_totales);

    puntuacion += cascadas_movimiento;
}

int main(int cantidad_argumentos, char* argumentos[]){
    srand(time(0));

    unsigned char* tablero = nullptr;

    int filas = 0;
    int columnas = 0;
    int capacidad_bytes = 0;

    int eliminaciones_usuario = 0;
    int fichas_eliminadas_total = 0;
    int combinaciones_totales = 0;
    int cascadas_movimiento = 0;
    int puntuacion = 0;

    cout << "SWEET CRUSH\n";
    cout << "Ingrese las dimensiones iniciales.\n";

    while (filas <= 0){
        if (!leer_coordenada("Filas: ", filas) || filas <= 0){
            cout << "Ingrese un numero entero mayor que cero.\n";
            filas = 0;
        }
    }

    while (columnas <= 0){
        if (!leer_coordenada("Columnas: ", columnas) || columnas <= 0){
            cout << "Ingrese un numero entero mayor que cero.\n";
            columnas = 0;
        }
    }

    inicializar_tablero(tablero, filas, columnas, capacidad_bytes);

    mostrar_juego(tablero, filas, columnas, eliminaciones_usuario, fichas_eliminadas_total, combinaciones_totales, cascadas_movimiento, puntuacion);

    bool ejecutando = true;

    while (ejecutando){
        int opcion = mostrar_menu();

        cascadas_movimiento = 0;

        switch (opcion){
        case 1:{
            int fila;
            int columna;

            if (!leer_coordenada("Fila: ", fila) || !leer_coordenada("Columna: ", columna)){
                cout << "Entrada invalida.\n";
                break;
            }

            jugar_eliminacion_individual(tablero, filas, columnas, fila, columna, fichas_eliminadas_total, combinaciones_totales, cascadas_movimiento, eliminaciones_usuario, puntuacion);
            break;
        }

        case 2:{
            int posicion;

            if (!leer_coordenada("Fila donde se insertara la nueva fila: ", posicion)){
                cout << "Entrada invalida.\n";
                break;
            }
            if (agregar_fila(tablero, filas, columnas, posicion, capacidad_bytes)){
                procesar_cambio_estructura(tablero, filas, columnas, fichas_eliminadas_total, combinaciones_totales, cascadas_movimiento, puntuacion);
            }
            else{
                cout << "Posicion de fila invalida.\n";
            }

            break;
        }

        case 3:{
            int posicion;

            if (!leer_coordenada("Fila que desea eliminar: ", posicion)){
                cout << "Entrada invalida.\n";
                break;
            }

            if (eliminar_fila(tablero, filas, columnas, posicion, capacidad_bytes)){
                procesar_cambio_estructura(tablero, filas, columnas, fichas_eliminadas_total, combinaciones_totales, cascadas_movimiento, puntuacion);
            }
            else{
                cout << "No se puede eliminar esa fila.\n";
            }

            break;
        }

        case 4:{
            int posicion;

            if (!leer_coordenada("Columna donde se insertara la nueva columna: ",posicion)){
                cout << "Entrada invalida.\n";
                break;
            }

            if (agregar_columna(tablero, filas, columnas, posicion, capacidad_bytes)){
                procesar_cambio_estructura(tablero, filas, columnas, fichas_eliminadas_total, combinaciones_totales, cascadas_movimiento, puntuacion);
            }
            else{
                cout << "Posicion de columna invalida.\n";
            }

            break;
        }

        case 5:{
            int posicion;

            if (!leer_coordenada("Columna que desea eliminar: ", posicion)){
                cout << "Entrada invalida.\n";
                break;
            }

            if (eliminar_columna(tablero, filas, columnas, posicion, capacidad_bytes)){
                procesar_cambio_estructura(tablero, filas, columnas, fichas_eliminadas_total, combinaciones_totales, cascadas_movimiento, puntuacion);
            }
            else{
                cout << "No se puede eliminar esa columna.\n";
            }

            break;
        }

        case 6:
            ejecutando = false;
            break;

        default:
            cout << "Opcion invalida.\n";
            break;
        }

        if (ejecutando){
            mostrar_juego(tablero, filas, columnas, eliminaciones_usuario, fichas_eliminadas_total, combinaciones_totales, cascadas_movimiento, puntuacion);
        }
    }

    destruir_tablero(tablero, capacidad_bytes);

    cout << "\nMemoria liberada. Fin del juego.";

    return 0;
}

#include "pantalla.h"
#include "tablero.h"
#include "configuracion.h"
#include <iostream>
using namespace std;

char caracter_ficha(unsigned char valor){
    switch (valor){
    case ficha_a: return '@';
    case ficha_b: return '#';
    case ficha_c: return '$';
    case ficha_d: return 'X';
    case ficha_e: return '8';
    case ficha_f: return 'A';
    case vacio:   return '.';
    default:      return '?';
    }
}

void imprimir_tres_bits(unsigned char valor){
    int bit1 = (valor >> 2) & 1;
    int bit2 = (valor >> 1) & 1;
    int bit3 = valor & 1;

    cout << bit1 << bit2 << bit3;
}

void mostrar_binario(const unsigned char* tablero, int filas, int columnas){
    int posiciones_totales = filas * columnas;

    cout << "\nTABLERO EN BITS\n";
    
    cout << "     ";
    for (int columna = 0; columna < columnas; ++columna){
        if (columna < 10) {
            cout << " " << columna << "  "; 
        } else {
            cout << " " << columna << " ";  
        }
    }
    cout << "\n";
    for (int fila = 0; fila < filas; fila+=1){
        
        if (fila < 10) {
            cout << " " << fila << "   "; 
        } else {
            cout << fila << "   ";        
        }

        for (int columna = 0; columna < columnas; columna+=1)
        {
            int posicion = obtener_posicion(fila, columna, columnas);
            imprimir_tres_bits(obtener_ficha(tablero, posicion));
            cout << " ";
        }
        cout << "\n";
    }

    int bytes_totales = calcular_bytes(filas, columnas);
    cout << "\n";
    cout << "Bits validos: " << posiciones_totales*3;
    cout << " | Bytes utilizados: " << bytes_totales << "\n";
}

void mostrar_fichas(const unsigned char* tablero, int filas, int columnas){
    cout << "\nTABLERO DE FICHAS\n";

    cout << "     ";
    for (int columna = 0; columna < columnas; ++columna){
        if (columna < 10) {
            cout << columna << "  ";
        } else {
            cout << columna << " ";
        }
    }
    cout << "\n";

    for (int fila = 0; fila < filas; fila+=1){
        if (fila < 10) cout << " " << fila << "   ";
        else cout << fila << "   ";

        for (int columna = 0; columna < columnas; columna+=1){
            unsigned char valor = obtener_ficha(tablero, obtener_posicion(fila, columna, columnas));

            cout << caracter_ficha(valor) << "  ";
        }
        cout << "\n";
    }

    cout << "\nFichas: @ # $ X 8 A \n";
}

void mostrar_juego(const unsigned char* tablero, int filas, int columnas, int eliminaciones_usuario, int fichas_eliminadas_total, int combinaciones_totales, int cascadas_movimiento, int puntuacion){
    cout << "SWEET CRUSH\n";

    cout << "Dimensiones actuales: " << filas << " x " << columnas << "\n";

    mostrar_binario(tablero, filas, columnas);
    mostrar_fichas(tablero, filas, columnas);

    cout << "\nESTADISTICAS\n";
    cout << "Eliminaciones del usuario : " << eliminaciones_usuario << "\n";
    cout << "Fichas eliminadas total   : " << fichas_eliminadas_total << "\n";
    cout << "Combinaciones detectadas  : " << combinaciones_totales << "\n";
    cout << "Cascadas del movimiento   : " << cascadas_movimiento << "\n";
    cout << "Puntuacion                : " << puntuacion << "\n";
}

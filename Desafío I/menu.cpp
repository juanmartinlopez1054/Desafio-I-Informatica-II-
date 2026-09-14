#include <iostream>
using namespace std;

int mostrar_menu(){
    char entrada;

    cout << "MENU\n";
    cout << "1. Eliminar una ficha\n";
    cout << "2. Agregar una fila\n";
    cout << "3. Eliminar una fila\n";
    cout << "4. Agregar una columna\n";
    cout << "5. Eliminar una columna\n";
    cout << "6. Salir\n";
    cout << "Seleccione una opcion: ";

    cin >> entrada;

    if (entrada >= '1' && entrada <= '6'){
        return entrada - '0';
    }

    cout << "Opcion invalida. Intente de nuevo.\n";
    return 0;
}
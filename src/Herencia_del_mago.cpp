#include <iostream>
#include "tree.h"

using namespace std;

void clearInputBuffer() {
    cin.clear();
    // Limpiar buffer sin usar <limits>
    const int BUFFER_SIZE = 10000;
    cin.ignore(BUFFER_SIZE, '\n');
}

void displayMenu() {
    cout << "\n=== MENU PRINCIPAL ===" << endl;
    cout << "1. Cargar datos desde CSV" << endl;
    cout << "2. Mostrar línea de sucesión" << endl;
    cout << "3. Actualizar dueño del hechizo" << endl;
    cout << "4. Editar datos de mago" << endl;
    cout << "5. Mostrar herencia mágica" << endl;
    cout << "6. Guardar cambios en CSV" << endl;
    cout << "7. Establecer dueño manual" << endl;
    cout << "8. Salir" << endl;
    cout << "Seleccione opción: ";
}

int main() {
    Tree<Wizard> arbol;
    bool cargado = false;
    char opcion;

    do {
        displayMenu();
        cin >> opcion;
        clearInputBuffer();

        switch (opcion) {
            case '1': 
                arbol.buildFromCSV();
                cargado = true;
                break;
                
            case '2': 
                if (!cargado) {
                    cout << "Error: Cargue datos primero (opción 1)" << endl;
                } else {
                    arbol.mostrarLineaSucesionVivos();
                }
                break;
                
            case '3': 
                if (!cargado) {
                    cout << "Error: Cargue datos primero (opción 1)" << endl;
                } else {
                    arbol.actualizarOwner();
                }
                break;
                
            case '4': {
                if (!cargado) {
                    cout << "Error: Cargue datos primero (opción 1)" << endl;
                    break;
                }
                
                int id;
                Wizard nuevos;
                
                cout << "ID del mago a editar: ";
                if (!(cin >> id)) {
                    cout << "Error: ID inválido" << endl;
                    clearInputBuffer();
                    break;
                }
                clearInputBuffer();
                
                cout << "Nuevo nombre: ";
                getline(cin, nuevos.first_name);
                
                cout << "Nuevo apellido: ";
                getline(cin, nuevos.last_name);
                
                while (true) {
                    cout << "Género (M/F): ";
                    cin >> nuevos.gender;
                    if (nuevos.gender == 'M' || nuevos.gender == 'F') break;
                    cout << "Error: Ingrese M o F" << endl;
                    clearInputBuffer();
                }
                
                cout << "Edad: ";
                while (!(cin >> nuevos.age)) {
                    cout << "Error: Edad inválida: ";
                    clearInputBuffer();
                }
                
                cout << "ID del padre: ";
                while (!(cin >> nuevos.id_father)) {
                    cout << "Error: ID inválido: ";
                    clearInputBuffer();
                }
                
                cout << "Muerto? (1=Sí, 0=No): ";
                while (!(cin >> nuevos.is_dead)) {
                    cout << "Error: Ingrese 1 o 0: ";
                    clearInputBuffer();
                }
                
                clearInputBuffer();
                cout << "Tipo de magia: ";
                getline(cin, nuevos.type_magic);
                
                arbol.editarNodo(id, nuevos);
                break;
            }
                
            case '5': {
                if (!cargado) {
                    cout << "Error: Cargue datos primero (opción 1)" << endl;
                    break;
                }
                
                int id;
                cout << "ID del mago: ";
                if (!(cin >> id)) {
                    cout << "Error: ID inválido" << endl;
                    clearInputBuffer();
                    break;
                }
                clearInputBuffer();
                
                arbol.mostrarHechizos(id);
                break;
            }
                
            case '6':
                if (!cargado) {
                    cout << "Error: Cargue datos primero (opción 1)" << endl;
                } else {
                    arbol.guardarCSV();
                }
                break;
                
            case '7': {
                if (!cargado) {
                    cout << "Error: Cargue datos primero (opción 1)" << endl;
                } else {
                    int id;
                    cout << "ID del nuevo dueño: ";
                    if (!(cin >> id)) {
                        cout << "Error: ID inválido" << endl;
                        clearInputBuffer();
                        break;
                    }
                    clearInputBuffer();
                    
                    arbol.setOwner(id);
                }
                break;
            }
        
            case '8':
                cout << "Saliendo del programa..." << endl;
                break;
                
            default:
                cout << "Error: Opción inválida" << endl;
        }
    } while (opcion != '8');

    return 0;
}
#include <iostream>
#include "tree.h"

using namespace std;

int main() {
    Tree<Wizard> arbol;
    bool cargado = false;

    char opcion;
    do {
        cout << "========MENU========\n\n";
        cout << "1. Cargar Datos de la Escuela (desde CSV)\n";
        cout << "2. Mostrar Linea de Sucesion Actual\n";
        cout << "3. Gestionar Propietario del Hechizo\n";
        cout << "4. Editar Datos de un Mago\n";
        cout << "5. Mostrar Hechizos de un Mago\n";
        cout << "6. Terminar Programa\n";
        cout << "Ingrese su opcion: ";
        cin >> opcion;
        cout << "\n";

        switch (opcion) {
            case '1': 
                arbol.buildFromCSV("binary_tree.csv");
                cargado = true;
                cout << "Datos cargados correctamente\n";
                break;
                
            case '2': 
                if (!cargado) {
                    cout << "Primero cargue los datos\n";
                    break;
                }
                arbol.mostrarLineaSucesionVivos();
                break;
                
            case '3': 
                if (!cargado) {
                    cout << "Primero cargue los datos\n";
                    break;
                }
                arbol.actualizarOwner();
                break;
                
            case '4': {
                if (!cargado) {
                    cout << "Primero cargue los datos\n";
                    break;
                }
                int id;
                Wizard nuevos;
                
                cout << "ID del mago a editar: ";
                cin >> id;
                cout << "Nuevo nombre: ";
                cin >> nuevos.first_name;
                cout << "Nuevo apellido: ";
                cin >> nuevos.last_name;
                cout << "Nuevo genero (M/F): ";
                cin >> nuevos.gender;
                cout << "Nueva edad: ";
                cin >> nuevos.age;
                cout << "Esta muerto? (1:Si, 0:No): ";
                cin >> nuevos.is_dead;
                cout << "Tipo de magia: ";
                cin >> nuevos.type_magic;
                cout << "Es dueño? (1:Si, 0:No): ";
                cin >> nuevos.is_owner;
                
                arbol.editarNodo(id, nuevos);
                cout << "Datos actualizados\n";
                break;
            }
                
            case '5': {
                if (!cargado) {
                    cout << "Primero cargue los datos\n";
                    break;
                }
                int id;
                cout << "ID del mago: ";
                cin >> id;
                arbol.mostrarHechizos(id);
                break;
            }
                
            case '6': 
                cout << "Saliendo...\n";
                break;
                
            default:
                cout << "Opcion invalida\n";
        }
    } while (opcion != '6');

    return 0;
}
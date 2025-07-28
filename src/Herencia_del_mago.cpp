#include <iostream>
#include "tree.h"

using namespace std;

int main() {
    Tree<Wizard> arbol;
    bool cargado = false;

    char opcion;
    do {
        cout << "\n======== MENU PRINCIPAL ========\n";
        cout << "1. Cargar Datos desde CSV\n";
        cout << "2. Mostrar Linea de Sucesion\n";
        cout << "3. Actualizar Propietario del Hechizo\n";
        cout << "4. Editar Datos de Mago\n";
        cout << "5. Mostrar Herencia Magica\n";
        cout << "6. Guardar Cambios en CSV\n";
        cout << "7. Establecer Owner del Hechizo\n";
        cout << "8. Salir\n";
        cout << "Seleccione opcion: ";
        cin >> opcion;
        cin.ignore(); // Limpiar buffer

        switch (opcion) {
            case '1': 
                arbol.buildFromCSV();
                cargado = true;
                cout << "Datos cargados correctamente\n";
                break;
                
            case '2': 
                if (!cargado) cout << "Primero cargue los datos (opcion 1)\n";
                else arbol.mostrarLineaSucesionVivos();
                break;
                
            case '3': 
                if (!cargado) cout << "Primero cargue los datos (opcion 1)\n";
                else {
                    arbol.actualizarOwner();
                    cout << "Propietario actualizado\n";
                }
                break;
                
            case '4': {
                if (!cargado) {
                    cout << "Primero cargue los datos (opcion 1)\n";
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
                cout << "Genero (M/F): ";
                cin >> nuevos.gender;
                cout << "Edad: ";
                cin >> nuevos.age;
                cout << "Muerto? (1=Si, 0=No): ";
                cin >> nuevos.is_dead;
                cout << "Tipo de magia: ";
                cin >> nuevos.type_magic;
                
                arbol.editarNodo(id, nuevos);
                break;
            }
                
            case '5': {
                if (!cargado) {
                    cout << "Primero cargue los datos (opcion 1)\n";
                    break;
                }
                int id;
                cout << "ID del mago: ";
                cin >> id;
                arbol.mostrarHechizos(id);
                break;
            }
                
            case '6':
                if (!cargado) cout << "Primero cargue los datos (opcion 1)\n";
                else {
                    arbol.guardarCSV();
                    cout << "Datos guardados en CSV correctamente\n";
                }
                break;
                
            case '7': {
                if (!cargado) {
                    cout << "Primero cargue los datos (opcion 1)\n";
                } else {
                    int id;
                    cout << "ID del mago a establecer como dueño: ";
                    cin >> id;
                    arbol.setOwner(id);
                }
                break;
            }
        
            case '8':
                cout << "Saliendo del programa...\n";
                break;
                
            default:
                cout << "Opcion no valida\n";
        }
    } while (opcion != '8');

    return 0;
}
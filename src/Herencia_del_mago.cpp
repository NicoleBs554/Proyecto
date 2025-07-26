#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "src/tree.h"

using namespace std;

void menu (){
    char opcion;

    do{
        cout << "========MENU========\n\n";
        cout << "1.  Cargar Datos de la Escuela (desde CSV)\n";
        cout << "2.  Mostrar Línea de Sucesión Actual\n";
        cout << "3.  Gestionar Propietario del Hechizo\n";
        cout << "4.  Editar Datos de un Mago\n";
        cout << "5.  Mostrar Hechizos de un Mago\n";
        cout << "6.  Terminar Programa\n";
        cout << "Ingrese su opcion: ";
        cin >> opcion;
        cout << "\n";

        switch (opcion){
            case '1':
            break;

            case '2':
            break;

            case '3':
            break;

            case '4':
            break;

            case '5':
            break;

            case '6':
            break;
        }

    }while(opcion != '6');
    
}

int main(){
    menu();
    return 0;
}

/*
1. Cargar Datos de la Escuela (desde CSV)
Propósito: Esta es la primera acción que el usuario debe realizar al iniciar el programa. 
Permite cargar el archivo .csv con la información de los magos y construir el árbol binario en memoria.

Funcionamiento:
    - Preguntar al usuario la ruta del archivo .csv.
    - Leer el archivo línea por línea.
    - Validar los datos (por ejemplo, gender solo 'H' o 'M', is_dead/is_owner solo '0' o '1', etc.).
    - Crear los nodos del árbol y enlazarlos usando id e id_father.
    - Informar al usuario si la carga fue exitosa o si hubo errores.

2. Mostrar Línea de Sucesión Actual
Propósito: Cumple directamente con el requisito 2 del proyecto. 
Permite al usuario ver quiénes son los magos vivos en la cadena de mando del hechizo hasta el actual propietario.

Funcionamiento:
    - Identificar al mago actual con is_owner = 1.
    - Recorrer el árbol desde el dueño hacia sus ancestros, o viceversa, recolectando solo
    a los magos vivos que forman parte de esa línea de sucesión directa.
    - Presentar esta lista de forma clara (ej. "Fundador -> Maestro -> Heredero Actual").

3. Gestionar Propietario del Hechizo
Propósito: Esta opción es clave para la Parte 3 del proyecto. 
Aquí se activan las lógicas complejas de asignación del hechizo en caso de muerte o envejecimiento.

Funcionamiento: Puedes implementarlo de dos maneras:

    - Opción A (Automático): Al seleccionar esta opción, el programa verifica si el dueño 
    actual murió o si tiene más de 70 años. Si alguna de estas condiciones se cumple, aplica automáticamente 
    las reglas de la Parte 3 para encontrar al nuevo dueño y actualiza el is_owner en el árbol. Luego, informa
    al usuario quién es el nuevo propietario. Esta es la forma más fiel al "hechizo" automático.

Opción B (Submenú con Triggers): Podría llevar a un submenú:

"Simular Muerte del Propietario Actual" (ejecuta las reglas de muerte)

"Simular Envejecimiento del Propietario Actual" (ejecuta la regla de >70 años)

"Ver Propietario Actual" (solo muestra quién es, sin cambios)

Recomendación: La Opción A es más directa y cumple con la idea de un hechizo "automático". Si el dueño ya murió o ya tiene más de 70, al seleccionar esta opción se reasigna. Si no se cumplen las condiciones, simplemente se informa que el dueño actual sigue siendo el mismo.

4. Editar Datos de un Mago
Propósito: Cumple con el requisito 4 del proyecto. Permite modificar la información de cualquier mago
 existente en el árbol.

Funcionamiento:
    - Pedir al usuario el ID del mago cuyos datos desea editar.
    - Buscar el nodo correspondiente en el árbol.
    - Presentar los datos actuales del mago (nombre, apellido, género, edad, is_dead, type_magic, is_owner).
    - Permitir al usuario modificar cualquiera de esos campos excepto el id y el id_father.
    - Validar las nuevas entradas (ej. que gender siga siendo 'H' o 'M').
    - Confirmar la actualización de los datos.

5. Mostrar Hechizos de un Mago
Propósito: Cumple con el requisito 5 del proyecto. Aunque el CSV solo tiene type_magic, la frase 
"lista de hechizos de un mago registrado" sugiere algo más. Dado que solo tienes type_magic, podrías 
interpretarlo como:

Mostrar el type_magic del mago seleccionado. Y, si se interpreta como "herencia de hechizos", podría ser
mostrar los type_magic de sus ancestros directos hasta el fundador, indicando cómo esa magia pudo 
haber influido en su tipo de magia actual. Esto le da más profundidad.

Funcionamiento:
    - Pedir al usuario el ID del mago.
    - Buscar el nodo.
    - Mostrar su type_magic.

Opcional (pero recomendado para darle más sentido): Mostrar el type_magic de sus maestros y antepasados en la línea directa, si los tiene.

6. Terminar Programa
Propósito: Permite al usuario salir de la aplicación de forma controlada.

Funcionamiento: Simplemente finaliza el bucle principal del programa. Podrías añadir una pregunta de confirmación ("¿Está seguro que desea salir? (S/N)").
*/
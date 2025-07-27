#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include "node.h" 
using namespace std;

enum ramas { IZQ, DER };

bool esVivoElementalOUnique(const PersonData& pd) {
    return !pd.is_dead && (pd.type_magic == "elemental" || pd.type_magic == "unique");
}
bool esVivoMixed(const PersonData& pd) {
    return !pd.is_dead && pd.type_magic == "mixed";
}
bool esHombreVivo(const PersonData& pd) {
    return !pd.is_dead && pd.gender == 'H';
}

template<class T>
class Tree {
private:
    Node<T>* root;

    Node<T>* insertar(Node<T>* nodo, T valor);
    Node<T>* eliminar(Node<T>* nodo, T valor);
    void postOrden(Node<T>* nodo);
    void inOrden(Node<T>* nodo);
    void preOrden(Node<T>* nodo);
    int altura(Node<T>* nodo);
    int factorEquilibrio(Node<T>* nodo);

    // Cambiado: sucesionVivos sin vector
    void sucesionVivos(Node<T>* nodo, Node<T>** vivos, int& n_vivos, int max_vivos);
    Node<T>* buscarOwner(Node<T>* nodo);
    void reasignarOwner(Node<T>* nodo);
    void reasignarOwnerPorReglas(Node<T>* nodo);

public:
    Tree() { root = nullptr; }
    bool isEmpty();
    void insert(T valor);
    void eliminate(T valor);
    void print(int tipo = 0); // 0: inOrden, 1: preOrden, 2: postOrden
    void postOrden();
    void inOrden();
    void preOrden();
    int altura();
    int factorEquilibrio();

    // buildFromCSV usa nombre fijo
    void buildFromCSV(const string& filename = "binary_tree.csv");
    void mostrarSucesionVivos();
    void actualizarOwner();
    void editarNodo(int id, const T& nuevosDatos);
    void editarNodoRec(Node<T>* n, int id, const T& nuevosDatos);
    Node<T>* buscarPorId(Node<T>* nodo, int id);
    void mostrarLineaSucesionActual();
    Node<T>* buscarDescendiente(Node<T>* nodo, bool (*condicion)(const T&));
    void buscarMujerJoven(Node<T>* n, Node<T>*& mejor, int& min_edad);
    void buscarMujerJovenViva(Node<T>* n, Node<T>*& mejor, int& min_edad);
};

// Implementación de métodos fuera de la clase

template<class T>
bool Tree<T>::isEmpty() {
    return root == nullptr;
}

template<class T>
void Tree<T>::insert(T valor) {
    root = insertar(root, valor);
}

template<class T>
Node<T>* Tree<T>::insertar(Node<T>* nodo, T valor) {
    if (nodo == nullptr) {
        return new Node<T>(valor);
    }
    if (valor < nodo->getData()) {
        nodo->setLeft(insertar(nodo->getLeft(), valor));
    } else if (valor > nodo->getData()) {
        nodo->setRight(insertar(nodo->getRight(), valor));
    }
    // Si es igual, no se inserta (no se permiten duplicados)
    return nodo;
}

template<class T>
void Tree<T>::eliminate(T valor) {
    root = eliminar(root, valor);
}

template<class T>
Node<T>* Tree<T>::eliminar(Node<T>* nodo, T valor) {
    if (nodo == nullptr) return nullptr;
    if (valor < nodo->getData()) {
        nodo->setLeft(eliminar(nodo->getLeft(), valor));
    } else if (valor > nodo->getData()) {
        nodo->setRight(eliminar(nodo->getRight(), valor));
    } else {
        // Nodo encontrado
        if (nodo->getLeft() == nullptr && nodo->getRight() == nullptr) {
            delete nodo;
            return nullptr;
        } else if (nodo->getLeft() == nullptr) {
            Node<T>* temp = nodo->getRight();
            delete nodo;
            return temp;
        } else if (nodo->getRight() == nullptr) {
            Node<T>* temp = nodo->getLeft();
            delete nodo;
            return temp;
        } else {
            // Nodo con dos hijos: buscar el mayor de la izquierda
            Node<T>* temp = nodo->getLeft();
            while (temp->getRight() != nullptr)
                temp = temp->getRight();
            nodo->setData(temp->getData());
            // Usar setLeft para sincronizar correctamente
            nodo->setLeft(eliminar(nodo->getLeft(), temp->getData()));
        }
    }
    return nodo;
}

template<class T>
void Tree<T>::postOrden(Node<T>* nodo) {
    if (nodo != nullptr) {
        postOrden(nodo->getLeft());
        postOrden(nodo->getRight());
        nodo->print();
    }
}

template<class T>
void Tree<T>::inOrden(Node<T>* nodo) {
    if (nodo != nullptr) {
        inOrden(nodo->getLeft());
        nodo->print();
        inOrden(nodo->getRight());
    }
}

template<class T>
void Tree<T>::preOrden(Node<T>* nodo) {
    if (nodo != nullptr) {
        nodo->print();
        preOrden(nodo->getLeft());
        preOrden(nodo->getRight());
    }
}

template<class T>
void Tree<T>::print(int tipo) {
    if (tipo == 0) inOrden(root);
    else if (tipo == 1) preOrden(root);
    else if (tipo == 2) postOrden(root);
    cout << endl;
}

template<class T>
void Tree<T>::postOrden() { postOrden(root); }

template<class T>
void Tree<T>::inOrden() { inOrden(root); }

template<class T>
void Tree<T>::preOrden() { preOrden(root); }

template<class T>
int Tree<T>::altura(Node<T>* nodo) {
    if (nodo == nullptr) return 0;
    int izq = altura(nodo->getLeft());
    int der = altura(nodo->getRight());
    return 1 + (izq > der ? izq : der);
}

template<class T>
int Tree<T>::altura() {
    return altura(root);
}

template<class T>
int Tree<T>::factorEquilibrio(Node<T>* nodo) {
    if (nodo == nullptr) return 0;
    return altura(nodo->getRight()) - altura(nodo->getLeft());
}

template<class T>
int Tree<T>::factorEquilibrio() {
    return factorEquilibrio(root);
}

template<class T>
void Tree<T>::buildFromCSV(const string& filename) {
    const int MAX_NODOS = 1000;
    T datos[MAX_NODOS];
    Node<T>* nodos[MAX_NODOS] = {nullptr};
    int usados = 0;

    ifstream file("bin/binary_tree.csv");
    string line;
    getline(file, line); // Saltar cabecera

    while (getline(file, line) && usados < MAX_NODOS) {
        stringstream ss(line);
        string campo;
        T p;
        getline(ss, campo, ','); p.id = stoi(campo);
        getline(ss, p.first_name, ',');
        getline(ss, p.last_name, ',');
        getline(ss, campo, ','); p.gender = campo[0];
        getline(ss, campo, ','); p.age = stoi(campo);
        getline(ss, campo, ','); p.id_father = stoi(campo);
        getline(ss, campo, ','); p.is_dead = (campo == "1");
        getline(ss, p.type_magic, ',');
        getline(ss, campo, ','); p.is_owner = (campo == "1");
        datos[usados] = p;
        usados++;
    }

    for (int i = 0; i < usados; ++i) {
        nodos[i] = new Node<T>(datos[i]);
    }

    for (int i = 0; i < usados; ++i) {
        int padre = datos[i].id_father;
        if (padre == 0) continue;
        for (int j = 0; j < usados; ++j) {
            if (datos[j].id == padre) {
                if (nodos[j]->getLeft() == nullptr)
                    nodos[j]->setLeft(nodos[i]);
                else if (nodos[j]->getRight() == nullptr)
                    nodos[j]->setRight(nodos[i]);
                break;
            }
        }
    }

    for (int i = 0; i < usados; ++i) {
        if (datos[i].id_father == 0) {
            root = nodos[i];
            break;
        }
    }
}

template<class T>
void Tree<T>::mostrarSucesionVivos() {
    const int MAX_VIVOS = 1000;
    Node<T>* vivos[MAX_VIVOS];
    int n_vivos = 0;
    sucesionVivos(root, vivos, n_vivos, MAX_VIVOS);
    cout << "Lista de sucesión (vivos):" << endl;
    for (int i = 0; i < n_vivos; ++i) vivos[i]->print();
}

template<class T>
void Tree<T>::sucesionVivos(Node<T>* nodo, Node<T>** vivos, int& n_vivos, int max_vivos) {
    if (!nodo || n_vivos >= max_vivos) return;
    sucesionVivos(nodo->getLeft(), vivos, n_vivos, max_vivos);
    if (!nodo->getData().is_dead && n_vivos < max_vivos) vivos[n_vivos++] = nodo;
    sucesionVivos(nodo->getRight(), vivos, n_vivos, max_vivos);
}

template<class T>
Node<T>* Tree<T>::buscarOwner(Node<T>* nodo) {
    if (!nodo) return nullptr;
    if (nodo->getData().is_owner) return nodo;
    Node<T>* izq = buscarOwner(nodo->getLeft());
    if (izq) return izq;
    return buscarOwner(nodo->getRight());
}

template<class T>
void Tree<T>::buscarMujerJoven(Node<T>* n, Node<T>*& mejor, int& min_edad) {
    if (!n) return;
    const T& d = n->getData();
    if (d.gender == 'F' && !d.is_dead && d.age < min_edad) {
        mejor = n;
        min_edad = d.age;
    }
    buscarMujerJoven(n->getLeft(), mejor, min_edad);
    buscarMujerJoven(n->getRight(), mejor, min_edad);
}

template<class T>
void Tree<T>::buscarMujerJovenViva(Node<T>* n, Node<T>*& mejor, int& min_edad) {
    if (!n) return;
    const T& pd = n->getData();
    if (pd.gender == 'F' && !pd.is_dead && pd.age < min_edad) {
        mejor = n;
        min_edad = pd.age;
    }
    buscarMujerJovenViva(n->getLeft(), mejor, min_edad);
    buscarMujerJovenViva(n->getRight(), mejor, min_edad);
}

template<class T>
void Tree<T>::reasignarOwnerPorReglas(Node<T>* nodo) {
    Node<T>* hijo = nodo->getLeft();
    while (hijo) {
        auto& d = hijo->getData();
        if (!d.is_dead && (d.type_magic == "elemental" || d.type_magic == "unique")) {
            d.is_owner = true;
            cout << "Nuevo dueño: ";
            hijo->print();
            return;
        }
        hijo = hijo->getRight();
    }
    hijo = nodo->getLeft();
    while (hijo) {
        auto& d = hijo->getData();
        if (!d.is_dead && d.type_magic == "mixed") {
            d.is_owner = true;
            cout << "Nuevo dueño: ";
            hijo->print();
            return;
        }
        hijo = hijo->getRight();
    }
    hijo = nodo->getLeft();
    while (hijo) {
        auto& d = hijo->getData();
        if (!d.is_dead && d.gender == 'M') {
            d.is_owner = true;
            cout << "Nuevo dueño: ";
            hijo->print();
            return;
        }
        hijo = hijo->getRight();
    }
    Node<T>* mejor = nullptr;
    int min_edad = 200;
    // No hay vector vivos, así que recorremos todo el árbol
       
    buscarMujerJoven(root, mejor, min_edad);
    if (mejor) {
        mejor->getData().is_owner = true;
        cout << "Nuevo dueño: ";
        mejor->print();
        return;
    }
    if (mejor) {
        mejor->getData().is_owner = true;
        cout << "Nuevo dueño: ";
        mejor->print();
    }
}

template<class T>
void Tree<T>::editarNodo(int id, const T& nuevosDatos) {
    editarNodoRec(root, id, nuevosDatos);
}

template<class T>
void Tree<T>::editarNodoRec(Node<T>* n, int id, const T& nuevosDatos) {
    if (!n) return;
    if (n->getData().id == id) {
        T& d = n->getData();
        d.first_name = nuevosDatos.first_name;
        d.last_name = nuevosDatos.last_name;
        d.gender = nuevosDatos.gender;
        d.age = nuevosDatos.age;
        d.is_dead = nuevosDatos.is_dead;
        d.type_magic = nuevosDatos.type_magic;
        d.is_owner = nuevosDatos.is_owner;
        return;
    }
    editarNodoRec(n->getLeft(), id, nuevosDatos);
    editarNodoRec(n->getRight(), id, nuevosDatos);
}

template<class T>
Node<T>* Tree<T>::buscarPorId(Node<T>* nodo, int id) {
    if (!nodo) return nullptr;
    if (nodo->getData().id == id) return nodo;
    Node<T>* izq = buscarPorId(nodo->getLeft(), id);
    if (izq) return izq;
    return buscarPorId(nodo->getRight(), id);
}

template<class T>
void Tree<T>::mostrarLineaSucesionActual() {
    // 1. Buscar el propietario
    Node<T>* owner = buscarOwner(root);
    if (!owner) {
        cout << "No hay propietario actual.\n";
        return;
    }
    // 2. Recorrer hacia arriba usando id_father
    const int MAX_LINEA = 100;
    Node<T>* linea[MAX_LINEA];
    int n = 0;
    Node<T>* actual = owner;
    while (actual && n < MAX_LINEA) {
        if (!actual->getData().is_dead)
            linea[n++] = actual;
        int id_padre = actual->getData().id_father;
        if (id_padre == 0) break;
        actual = buscarPorId(root, id_padre);
    }
    // 3. Imprimir desde el fundador hasta el dueño actual
    cout << "Linea de sucesion viva:\n";
    for (int i = n - 1; i >= 0; --i) {
        linea[i]->print();
        if (i > 0) cout << " -> ";
    }
    cout << endl;
}

template<class T>
void Tree<T>::actualizarOwner() {
    Node<T>* owner = buscarOwner(root);
    if (!owner) {
        cout << "No hay propietario actual.\n";
        return;
    }
    // auto& d = owner->getData();
    T& d = owner->getData();
    // Condición adicional: Si el owner tiene más de 70 años
    if (d.age > 70) {
        // Buscar entre hijos directos
        Node<T>* hijo_mismo_tipo = nullptr;
        Node<T>* hijo_mas_viejo = nullptr;
        int max_edad = -1;
        // auto hijo = owner->getLeft();
        Node<T>* hijo = owner->getLeft();
        if (hijo && !hijo->getData().is_dead) {
            if (hijo->getData().type_magic == d.type_magic) {
                hijo_mismo_tipo = hijo;
            }
            if (hijo->getData().age > max_edad) {
                hijo_mas_viejo = hijo;
                max_edad = hijo->getData().age;
            }
        }
        hijo = owner->getRight();
        if (hijo && !hijo->getData().is_dead) {
            if (!hijo_mismo_tipo && hijo->getData().type_magic == d.type_magic) {
                hijo_mismo_tipo = hijo;
            }
            if (hijo->getData().age > max_edad) {
                hijo_mas_viejo = hijo;
                max_edad = hijo->getData().age;
            }
        }
        d.is_owner = false;
        if (hijo_mismo_tipo) {
            hijo_mismo_tipo->getData().is_owner = true;
            cout << "Nuevo dueño por edad >70 (mismo tipo de magia): "; hijo_mismo_tipo->print();
        } else if (hijo_mas_viejo) {
            hijo_mas_viejo->getData().is_owner = true;
            cout << "Nuevo dueño por edad >70 (más viejo): "; hijo_mas_viejo->print();
        } else {
            cout << "No hay discípulos vivos para transferir el hechizo.\n";
        }
        return;
    }
    // Si el owner está muerto, aplicar reglas de sucesión
    if (d.is_dead) {
        d.is_owner = false;
        Node<T>* nuevo = nullptr;
        // Escenario 1: Tiene discípulos
        if (owner->getLeft() || owner->getRight()) {
            nuevo = buscarDescendiente(owner, esVivoElementalOUnique);
            if (!nuevo) nuevo = buscarDescendiente(owner, esVivoMixed);
            if (!nuevo) nuevo = buscarDescendiente(owner, esHombreVivo);
            if (nuevo) {
                nuevo->getData().is_owner = true;
                cout << "Nuevo dueño por reglas de discípulos: "; nuevo->print();
                return;
            }
        }
        // Escenario 2: No tiene discípulos
        // Buscar maestro (padre)
        Node<T>* maestro = buscarPorId(root, d.id_father);
        if (maestro) {
            // Buscar compañeros (hermanos)
            Node<T>* hermano = maestro->getLeft();
            if (hermano && hermano != owner) {
                // Prioridad 1: Compañero vivo con magia igual
                if (!hermano->getData().is_dead && hermano->getData().type_magic == d.type_magic) {
                    hermano->getData().is_owner = true;
                    cout << "Nuevo dueño: compañero vivo con magia igual: "; hermano->print();
                    return;
                }
                // Prioridad 2: Discípulo vivo del compañero
                Node<T>* sub = buscarDescendiente(hermano, esVivoElementalOUnique);
                if (!sub) sub = buscarDescendiente(hermano, esVivoMixed);
                if (!sub) sub = buscarDescendiente(hermano, esHombreVivo);
                if (sub) {
                    sub->getData().is_owner = true;
                    cout << "Nuevo dueño: discípulo del compañero: "; sub->print();
                    return;
                }
            }
            hermano = maestro->getRight();
            if (hermano && hermano != owner) {
                // Prioridad 1: Compañero vivo con magia igual
                if (!hermano->getData().is_dead && hermano->getData().type_magic == d.type_magic) {
                    hermano->getData().is_owner = true;
                    cout << "Nuevo dueño: compañero vivo con magia igual: "; hermano->print();
                    return;
                }
                // Prioridad 2: Discípulo vivo del compañero
                Node<T>* sub = buscarDescendiente(hermano, esVivoElementalOUnique);
                if (!sub) sub = buscarDescendiente(hermano, esVivoMixed);
                if (!sub) sub = buscarDescendiente(hermano, esHombreVivo);
                if (sub) {
                    sub->getData().is_owner = true;
                    cout << "Nuevo dueño: discípulo del compañero: "; sub->print();
                    return;
                }
            }
            // Prioridad 3: Compañero del maestro
            Node<T>* abuelo = buscarPorId(root, maestro->getData().id_father);
            if (abuelo) {
               Node<T>* tio = abuelo->getLeft();
                    if (tio && tio != maestro) {
                        if (!tio->getData().is_dead) {
                            tio->getData().is_owner = true;
                            cout << "Nuevo dueño: compañero del maestro: "; tio->print();
                            return;
                        }
                    }
                    tio = abuelo->getRight();
                    if (tio && tio != maestro) {
                        if (!tio->getData().is_dead) {
                            tio->getData().is_owner = true;
                            cout << "Nuevo dueño: compañero del maestro: "; tio->print();
                            return;
                        }
                    }
            } else if (maestro->getData().is_dead) {
                // Prioridad 4: Maestro muerto, volver a reglas generales
                Node<T>* nuevo = buscarDescendiente(root, esVivoElementalOUnique);
                if (!nuevo) nuevo = buscarDescendiente(root, esVivoMixed);
                if (!nuevo) nuevo = buscarDescendiente(root, esHombreVivo);
                if (nuevo) {
                    nuevo->getData().is_owner = true;
                    cout << "Nuevo dueño por reglas generales: "; nuevo->print();
                    return;
                }
            }
        }
        // Prioridad 5: Mujer más joven con discípulos y magia mixed, cuyo maestro fue owner
        Node<T>* mejor = nullptr;
        int min_edad = 200;
        buscarMujerJoven(root, mejor, min_edad);
        if (mejor) {
            mejor->getData().is_owner = true;
            cout << "Nuevo dueño: mujer más joven con discípulos y magia mixed: "; mejor->print();
            return;
        }
        // Si no, mujer más joven viva
        mejor = nullptr;
        min_edad = 200;
        buscarMujerJovenViva(root, mejor, min_edad);
        if (mejor) {
            mejor->getData().is_owner = true;
            cout << "Nuevo dueño: mujer más joven viva: "; mejor->print();
            return;
        }
        cout << "No se encontró un nuevo dueño según las reglas.\n";
    } else {
        cout << "El dueño actual sigue siendo:\n";
        owner->print();
    }
}

template<class T>
Node<T>* Tree<T>::buscarDescendiente(Node<T>* nodo, bool (*condicion)(const T&)) {
    if (!nodo) return nullptr;
    if (condicion(nodo->getData())) return nodo;
    Node<T>* izq = buscarDescendiente(nodo->getLeft(), condicion);
    if (izq) return izq;
    return buscarDescendiente(nodo->getRight(), condicion);
}
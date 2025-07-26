#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "src/node.h" // Enlaza la clase Node
using namespace std;

enum ramas { IZQ, DER };

template<class T>
class Tree {
private:
    Node<T>* root = nullptr;

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
    Tree() : root(nullptr) {}
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

    ifstream file("binary_tree.csv");
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
void Tree<T>::actualizarOwner() {
    Node<T>* owner = buscarOwner(root);
    if (owner && owner->getData().is_dead) {
        owner->getData().is_owner = false;
        reasignarOwnerPorReglas(owner);
    }
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
    auto buscarMujerJoven = [&](Node<T>* n) {
        if (!n) return;
        auto& d = n->getData();
        if (d.gender == 'F' && !d.is_dead && d.age < min_edad) {
            mejor = n;
            min_edad = d.age;
        }
        buscarMujerJoven(n->getLeft());
        buscarMujerJoven(n->getRight());
    };
    buscarMujerJoven(root);
    if (mejor) {
        mejor->getData().is_owner = true;
        cout << "Nuevo dueño: ";
        mejor->print();
    }
}

template<class T>
void Tree<T>::editarNodo(int id, const T& nuevosDatos) {
    // No hay id_map, así que buscamos el nodo por id
    function<void(Node<T>*)> buscarYEditar = [&](Node<T>* n) {
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
        buscarYEditar(n->getLeft());
        buscarYEditar(n->getRight());
    };
    buscarYEditar(root);
}


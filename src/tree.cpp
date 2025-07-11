#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include "src/tree.h" // Enlaza la clase Node
using namespace std;

template<class T>
void Tree<T>::print(int tipo) {
    // 0: inOrden, 1: preOrden, 2: postOrden
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

// Construir árbol desde CSV SIN vector/map, nombre fijo "binary_tree.csv"
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

// Mostrar lista de sucesión de los vivos SIN vector
template<class T>
void Tree<T>::mostrarSucesionVivos() {
    const int MAX_VIVOS = 1000;
    Node<T>* vivos[MAX_VIVOS];
    int n_vivos = 0;
    sucesionVivos(root, vivos, n_vivos, MAX_VIVOS);
    cout << "Lista de sucesión (vivos):" << endl;
    for (int i = 0; i < n_vivos; ++i) vivos[i]->print();
}

// Recorrer inOrden y guardar vivos en arreglo
template<class T>
void Tree<T>::sucesionVivos(Node<T>* nodo, Node<T>** vivos, int& n_vivos, int max_vivos) {
    if (!nodo || n_vivos >= max_vivos) return;
    sucesionVivos(nodo->getLeft(), vivos, n_vivos, max_vivos);
    if (!nodo->getData().is_dead && n_vivos < max_vivos) vivos[n_vivos++] = nodo;
    sucesionVivos(nodo->getRight(), vivos, n_vivos, max_vivos);
}

// Buscar nodo dueño actual
template<class T>
Node<T>* Tree<T>::buscarOwner(Node<T>* nodo) {
    if (!nodo) return nullptr;
    if (nodo->getData().is_owner) return nodo;
    Node<T>* izq = buscarOwner(nodo->getLeft());
    if (izq) return izq;
    return buscarOwner(nodo->getRight());
}

// Actualizar dueño del hechizo si el actual murió
template<class T>
void Tree<T>::actualizarOwner() {
    Node<T>* owner = buscarOwner(root);
    if (owner && owner->getData().is_dead) {
        owner->getData().is_owner = false;
        reasignarOwnerPorReglas(owner);
    }
}

// Implementar reglas de reasignación (simplificado, puedes expandir)
template<class T>
void Tree<T>::reasignarOwnerPorReglas(Node<T>* nodo) {
    // Buscar primer hijo vivo con magia elemental o unique
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
    // Si no, buscar primer hijo vivo con magia mixed
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
    // Si no, buscar primer hombre vivo
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
    // Si no, buscar mujer más joven viva
    Node<T>* mejor = nullptr;
    int min_edad = 200;
    vector<Node<T>*> vivos;
    sucesionVivos(root, vivos);
    for (auto n : vivos) {
        auto& d = n->getData();
        if (d.gender == 'F' && d.age < min_edad) {
            mejor = n;
            min_edad = d.age;
        }
    }
    if (mejor) {
        mejor->getData().is_owner = true;
        cout << "Nuevo dueño: ";
        mejor->print();
    }
}

// Editar datos de un nodo (excepto id y id_father)
template<class T>
void Tree<T>::editarNodo(int id, const T& nuevosDatos) {
    if (id_map.count(id)) {
        T& d = id_map[id]->getData();
        // d.id y d.id_father no se modifican
        d.first_name = nuevosDatos.first_name;
        d.last_name = nuevosDatos.last_name;
        d.gender = nuevosDatos.gender;
        d.age = nuevosDatos.age;
        d.is_dead = nuevosDatos.is_dead;
        d.type_magic = nuevosDatos.type_magic;
        d.is_owner = nuevosDatos.is_owner;
    }
}
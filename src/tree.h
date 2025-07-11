#pragma once
#include <iostream>
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
        // Usar setLeft para sincronizar correctamente
        nodo->setLeft(eliminar(nodo->getLeft(), valor));
    } else if (valor > nodo->getData()) {
        // Usar setRight para sincronizar correctamente
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


#pragma once
#include <iostream>
#include <string>
using namespace std;

// Estructura para los datos del mago
struct Wizard {
    int id;
    string first_name;
    string last_name;
    char gender;
    int age;
    int id_father;
    bool is_dead;
    string type_magic;
    bool is_owner;

    void mostrar() const {
        cout << "ID: " << id << ", Nombre: " << first_name << " " << last_name
             << ", Género: " << gender << ", Edad: " << age
             << ", Padre ID: " << id_father << ", Muerto: " << (is_dead ? "Sí" : "No")
             << ", Tipo de magia: " << type_magic
             << ", Dueño: " << (is_owner ? "Sí" : "No") << endl;
    }
};

template<class T>
class Node {
private:
    T data;
    Node<T>* left;
    Node<T>* right;
    Node<T>* parent;  // Añadido para rastrear padre
    int height;

public:
    Node(T value, Node<T>* l = nullptr, Node<T>* r = nullptr, Node<T>* p = nullptr)
        : data(value), left(l), right(r), parent(p), height(1) {}
        
    ~Node() {
        // No liberar memoria aquí, el árbol se encargará
    }

    T getData() { return data; }
    void setData(T value) { data = value; }
    
    Node<T>* getLeft() { return left; }
    Node<T>* getRight() { return right; }
    Node<T>* getParent() { return parent; }  // Nuevo
    
    void setLeft(Node<T>* node) { 
        left = node; 
        if (node) node->parent = this;  // Establecer padre automáticamente
    }
    
    void setRight(Node<T>* node) { 
        right = node; 
        if (node) node->parent = this;  // Establecer padre automáticamente
    }
    
    void setParent(Node<T>* node) { parent = node; }  // Nuevo
    
    void print() {
        cout << data.first_name << " " << data.last_name;
        if (data.is_owner) cout << " (OWNER)";
        if (data.is_dead) cout << " [DEAD]";
        cout << endl;
    }
    
    bool hoja() {
        return left == nullptr && right == nullptr;
    }
    
    int getHeight() { return height; }
    void setHeight(int h) { height = h; }
    
    void setChildren(Node<T>* l, Node<T>* r) {
        setLeft(l);
        setRight(r);
    }
    
    // Buscar nodo por ID (recursivo)
    Node<T>* findNodeById(int id) {
        if (data.id == id) return this;
        
        Node<T>* found = nullptr;
        if (left) found = left->findNodeById(id);
        if (found) return found;
        
        if (right) found = right->findNodeById(id);
        return found;
    }
};

template<class T>
    void setData(T value) {
        // Bloquea modificación de ID y padre
        value.id = data.id;
        value.id_father = data.id_father;
        data = value;
    }

    template<class T>
    Node<T> getData() { return data; }

    template<class T>
    Node<T>* getLeft() { return left; }

    template<class T>
    Node<T>* getRight() { return right; }

    template<class T>
    Node<T>* getParent() { return parent; }

    template<class T>
    void setLeft(Node<T>* node) { 
        left = node;
        if(node) node->parent = this;
    }
    template<class T>
    void setRight(Node<T>* node) { 
        right = node;
        if(node) node->parent = this;
    }

    template<class T>
    void setParent(Node<T>* node) { parent = node; }

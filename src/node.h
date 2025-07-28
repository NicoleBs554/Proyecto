#pragma once
#include <iostream>
#include <string>
using namespace std;

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
    Node<T>* parent;

public:
    Node(T value, Node<T>* l = nullptr, Node<T>* r = nullptr, Node<T>* p = nullptr)
        : data(value), left(l), right(r), parent(p) {}
        
    ~Node() {}

    T& getData() { return data; }
    const T& getData() const { return data; }
    void setData(T value) { 
        value.id = data.id;
        value.id_father = data.id_father;
        data = value;
    }
    
    Node<T>* getLeft() { return left; }
    Node<T>* getRight() { return right; }
    Node<T>* getParent() { return parent; }
    
    void setLeft(Node<T>* node) { 
        left = node; 
        if (node) node->parent = this;
    }
    
    void setRight(Node<T>* node) { 
        right = node; 
        if (node) node->parent = this;
    }
    
    void setParent(Node<T>* node) { parent = node; }
    
    void print() {
        cout << data.first_name << " " << data.last_name;
        if (data.is_owner) cout << " (OWNER)";
        if (data.is_dead) cout << " [DEAD]";
    }
    
    bool hoja() {
        return left == nullptr && right == nullptr;
    }
};
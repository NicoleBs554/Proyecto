#pragma once
#include <iostream>
#include <string>
using namespace std;

// Estructura para los datos de la persona
struct PersonData {
    int id;
    string first_name;
    string last_name;
    char gender;
    int age;
    int id_father;
    bool is_dead;
    string type_magic;
    bool is_owner;

    PersonData() : id(0), gender('M'), age(0), id_father(0), is_dead(false), is_owner(false) {}
};

template<class T>
class Node {
private:
    T data;
    Node<T>* left;
    Node<T>* right;
    int heigth = 1; // Altura del nodo
    int fe = 0;     // Factor de equilibrio
public:
    Node(T value, Node<T>* l = nullptr, Node<T>* r = nullptr)
        : data(value), left(l), right(r) {}
    ~Node() {}

    T getData() { return data; }
    void setData(T value) { data = value; }
    Node<T>* getLeft() { return left; }
    Node<T>* getRight() { return right; }
    void setLeft(Node<T>* node) { left = node; }
    void setRight(Node<T>* node) { right = node; }
    void print() { 
        // Imprimir nombre y estado de dueño
        cout << data.first_name << " " << data.last_name;
        if (data.is_owner) cout << " (DUEÑO)";
        if (data.is_dead) cout << " [MUERTO]";
        cout << endl;
    }

    bool hoja() { return left == nullptr && right == nullptr; }

    int getHeigth() { return heigth; }
    void setHeigth(int h) { heigth = h; }
    int getFe() { return fe; }
    void setFe(int f) { fe = f; }
    void setFe();

    Node<T>* getChildren(int child) {
        if (child == 0) return left;
        if (child == 1) return right;
        return nullptr;
    }
    void setChildren(Node<T>* l, Node<T>* r) {
        left = l;
        right = r;
    }

    int alturaAuto();
    int calculate_Heigth(Node<T>* node);
};
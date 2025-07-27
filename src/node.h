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
    int heigth; // Altura del nodo
    int fe;     // Factor de equilibrio
public:
     Node(T value, Node<T>* l = nullptr, Node<T>* r = nullptr)
        : data(value), left(l), right(r), heigth(1), fe(0) {}
    ~Node();

    T& getData();
    void setData(T value);
    Node<T>* getLeft();
    Node<T>* getRight();
    void setLeft(Node<T>* node);
    void setRight(Node<T>* node);
    void print();

    bool hoja();

    int getHeigth();
    void setHeigth(int h);
    int getFe();
    void setFe(int f);
    void setFe();

    Node<T>* getChildren(int child);
    void setChildren(Node<T>* l, Node<T>* r);

    int alturaAuto();
    int calculate_Heigth(Node<T>* node);
};

// --- Implementación de métodos ---

template <class T>
Node<T>::~Node() {
    this->right = nullptr;
    this->left = nullptr;
}

template <class T>
T& Node<T>::getData() {
    return data;
}

template <class T>
void Node<T>::setData(T value) {
    data = value;
}

template <class T>
Node<T>* Node<T>::getLeft() {
    return left;
}

template <class T>
Node<T>* Node<T>::getRight() {
    return right;
}

template <class T>
void Node<T>::setLeft(Node<T>* node) {
    left = node;
    heigth = alturaAuto();
}

template <class T>
void Node<T>::setRight(Node<T>* node) {
    right = node;
    heigth = alturaAuto();
}

template <class T>
void Node<T>::print() {
    cout << data.first_name << " " << data.last_name;
    if (data.is_owner) cout << " (DUEÑO)";
    if (data.is_dead) cout << " [MUERTO]";
    cout << endl;
}

template <class T>
bool Node<T>::hoja() {
    return left == nullptr && right == nullptr;
}

template <class T>
int Node<T>::getHeigth() {
    return heigth;
}

template <class T>
void Node<T>::setHeigth(int h) {
    heigth = h;
}

template <class T>
int Node<T>::getFe() {
    return fe;
}

template <class T>
void Node<T>::setFe(int f) {
    fe = f;
}

template <class T>
void Node<T>::setFe() {
    if (right == nullptr && left == nullptr) {
        fe = 0;
    }
    else if (right != nullptr && left != nullptr) {
        fe = (right->heigth) - (left->heigth);
    } else if (right == nullptr) {
        fe = -(left->heigth);
    } else if (left == nullptr) {
        fe = (right->heigth);
    }
}

template <class T>
Node<T>* Node<T>::getChildren(int child) {
    if (child == 0) return left;
    if (child == 1) return right;
    return nullptr;
}

template <class T>
void Node<T>::setChildren(Node<T>* l, Node<T>* r) {
    left = l;
    right = r;
}

template <class T>
int Node<T>::alturaAuto() {
    int h_left = calculate_Heigth(left);
    int h_right = calculate_Heigth(right);
    return h_left < h_right ? h_right : h_left;
}

template <class T>
int Node<T>::calculate_Heigth(Node<T>* node) {
    if (node == nullptr) return 0;
    else {
        int heigthLeft = calculate_Heigth(node->left);
        int heigthRigth = calculate_Heigth(node->right);
        if (heigthLeft > heigthRigth)
            return heigthLeft + 1;
        else
            return heigthRigth + 1;
    }
}
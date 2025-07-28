#pragma once
#include "node.h"
#include <iostream>
#include <fstream>
using namespace std;

const int MAX_NODOS = 1000;

template<class T>
class Tree {
private:
    Node<T>* root;
    Node<T>* current_owner;
    Node<T>* nodos[MAX_NODOS];
    int contadorNodos;

    // ----- FUNCIONES AUXILIARES PRIVADAS -----
    void deleteTree(Node<T>* node) {
        if(node) {
            deleteTree(node->getLeft());
            deleteTree(node->getRight());
            delete node;
        }
    }

    Node<T>* buscarPorId(int id) {
        for(int i = 0; i < contadorNodos; i++) {
            if(nodos[i]->getData().id == id) return nodos[i];
        }
        return nullptr;
    }

    Node<T>* encontrarDiscipuloValido(Node<T>* nodo, bool incluirHombres = true) {
        if(!nodo) return nullptr;
        
        Node<T>* hijos[2] = {nodo->getLeft(), nodo->getRight()};
        
        // Prioridad 1: Magia elemental o única
        for(int i = 0; i < 2; i++) {
            if(hijos[i] && !hijos[i]->getData().is_dead) {
                string magia = hijos[i]->getData().type_magic;
                if(magia == "elemental" || magia == "unique") return hijos[i];
            }
        }
        
        // Prioridad 2: Magia mixta
        for(int i = 0; i < 2; i++) {
            if(hijos[i] && !hijos[i]->getData().is_dead) {
                if(hijos[i]->getData().type_magic == "mixed") return hijos[i];
            }
        }
        
        // Prioridad 3: Cualquier hombre vivo (si está permitido)
        if(incluirHombres) {
            for(int i = 0; i < 2; i++) {
                if(hijos[i] && !hijos[i]->getData().is_dead) {
                    if(hijos[i]->getData().gender == 'M') return hijos[i];
                }
            }
        }
        
        return nullptr;
    }

    Node<T>* encontrarMujerJoven(bool conDiscipulos = false) {
        Node<T>* mujerJoven = nullptr;
        for(int i = 0; i < contadorNodos; i++) {
            Wizard& datos = nodos[i]->getData();
            if(!datos.is_dead && datos.gender == 'F') {
                bool cumpleCondicion = conDiscipulos ? 
                    (nodos[i]->getLeft() || nodos[i]->getRight()) && datos.type_magic == "mixed" : 
                    true;
                
                if(cumpleCondicion && (!mujerJoven || datos.age < mujerJoven->getData().age)) {
                    mujerJoven = nodos[i];
                }
            }
        }
        return mujerJoven;
    }

public:
    // ----- CONSTRUCTOR Y DESTRUCTOR -----
    Tree() : root(nullptr), current_owner(nullptr), contadorNodos(0) {}

    ~Tree() {
        deleteTree(root);
    }

    // ----- CARGA DE DATOS -----
    void buildFromCSV(const string& filename = "bin/binary_tree.csv") {
        ifstream file(filename);
        if(!file) {
            cerr << "Error abriendo archivo: " << filename << endl;
            return;
        }

        string line;
        getline(file, line); // Ignorar cabecera

        contadorNodos = 0;
        while(getline(file, line) && contadorNodos < MAX_NODOS) {
            Wizard w;
            size_t pos = 0;
            string tokens[9];
            int idx = 0;
            
            while((pos = line.find(',')) != string::npos) {
                tokens[idx++] = line.substr(0, pos);
                line.erase(0, pos + 1);
            }
            tokens[idx] = line;
            
            w.id = stoi(tokens[0]);
            w.first_name = tokens[1];
            w.last_name = tokens[2];
            w.gender = tokens[3][0];
            w.age = stoi(tokens[4]);
            w.id_father = stoi(tokens[5]);
            w.is_dead = (tokens[6] == "1");
            w.type_magic = tokens[7];
            w.is_owner = (tokens[8] == "1");
            
            nodos[contadorNodos] = new Node<Wizard>(w);
            if(w.is_owner) current_owner = nodos[contadorNodos];
            contadorNodos++;
        }

        // Construir relaciones padre-hijo
        for(int i = 0; i < contadorNodos; i++) {
            int padreId = nodos[i]->getData().id_father;
            if(padreId == 0) {
                root = nodos[i];
                continue;
            }
            
            for(int j = 0; j < contadorNodos; j++) {
                if(nodos[j]->getData().id == padreId) {
                    if(!nodos[j]->getLeft()) {
                        nodos[j]->setLeft(nodos[i]);
                    } else if(!nodos[j]->getRight()) {
                        nodos[j]->setRight(nodos[i]);
                    }
                    break;
                }
            }
        }
    }

    // ----- FUNCIONALIDADES PRINCIPALES -----
    void mostrarLineaSucesionVivos() {
        if(!current_owner) {
            cout << "No hay dueño actual" << endl;
            return;
        }
        
        Node<T>* camino[100];
        int contador = 0;
        Node<T>* actual = current_owner;
        
        while(actual && contador < 100) {
            if(!actual->getData().is_dead) {
                camino[contador++] = actual;
            }
            actual = actual->getParent();
        }
        
        cout << "Línea de sucesión (vivos):" << endl;
        for(int i = contador-1; i >= 0; i--) {
            camino[i]->print();
            if(i > 0) cout << " -> ";
        }
        cout << endl;
    }

    void actualizarOwner() {
        if(!current_owner) return;
        
        Wizard& datosOwner = current_owner->getData();
        if(!datosOwner.is_dead && datosOwner.age <= 70) return;

        datosOwner.is_owner = false;
        Node<T>* nuevoOwner = nullptr;

        // REGLA ESPECIAL: >70 años
        if(datosOwner.age > 70) {
            // Buscar discípulo con misma magia
            Node<T>* discipulo = encontrarDiscipuloValido(current_owner, false);
            if(discipulo && discipulo->getData().type_magic == datosOwner.type_magic) {
                nuevoOwner = discipulo;
            }
            // Si no, buscar el más viejo
            else {
                Node<T>* masViejo = nullptr;
                Node<T>* hijos[2] = {current_owner->getLeft(), current_owner->getRight()};
                for(int i = 0; i < 2; i++) {
                    if(hijos[i] && !hijos[i]->getData().is_dead) {
                        if(!masViejo || hijos[i]->getData().age > masViejo->getData().age) {
                            masViejo = hijos[i];
                        }
                    }
                }
                nuevoOwner = masViejo;
            }
        }
        
        // REGLAS NORMALES
        if(!nuevoOwner) {
            // REGLA 1-3: Búsqueda en discípulos
            nuevoOwner = encontrarDiscipuloValido(current_owner);
            
            // REGLA 4: Compañero discípulo
            if(!nuevoOwner && current_owner->getParent()) {
                Node<T>* padre = current_owner->getParent();
                Node<T>* companero = (padre->getLeft() == current_owner) 
                                   ? padre->getRight() 
                                   : padre->getLeft();
                
                if(companero && !companero->getData().is_dead) {
                    if(companero->getData().type_magic == datosOwner.type_magic) {
                        nuevoOwner = companero;
                    } else {
                        nuevoOwner = encontrarDiscipuloValido(companero);
                    }
                }
            }
            
            // REGLA 5: Compañero del maestro
            if(!nuevoOwner && current_owner->getParent() && current_owner->getParent()->getParent()) {
                Node<T>* abuelo = current_owner->getParent()->getParent();
                Node<T>* tio = (abuelo->getLeft() == current_owner->getParent())
                             ? abuelo->getRight()
                             : abuelo->getLeft();
                if(tio && !tio->getData().is_dead) nuevoOwner = tio;
            }
            
            // REGLA 6-7: Mujeres jóvenes
            if(!nuevoOwner) {
                nuevoOwner = encontrarMujerJoven(true);  // Primero con discípulos y magia mixed
                if(!nuevoOwner) nuevoOwner = encontrarMujerJoven(false);  // Cualquier mujer joven
            }
        }

        if(nuevoOwner) {
            nuevoOwner->getData().is_owner = true;
            current_owner = nuevoOwner;
            cout << "Nuevo dueño asignado: ";
            nuevoOwner->print();
        } else {
            cout << "No se pudo asignar nuevo dueño" << endl;
        }
    }

    void editarNodo(int id, const T& nuevosDatos) {
        Node<T>* nodo = buscarPorId(id);
        if(nodo) {
            T datos = nodo->getData();
            nuevosDatos.id = datos.id;
            nuevosDatos.id_father = datos.id_father;
            nodo->setData(nuevosDatos);
            
            if(nodo == current_owner && !nuevosDatos.is_owner) {
                current_owner = nullptr;
            }
        } else {
            cout << "Mago no encontrado" << endl;
        }
    }

    void mostrarHechizos(int id) {
        Node<T>* mago = buscarPorId(id);
        if(mago) {
            cout << "Tipo de magia: " << mago->getData().type_magic << endl;
            cout << "Herencia mágica:" << endl;
            
            Node<T>* actual = mago;
            while(actual) {
                cout << "-> " << actual->getData().first_name 
                     << " (" << actual->getData().type_magic << ")" << endl;
                actual = actual->getParent();
            }
        } else {
            cout << "Mago no encontrado" << endl;
        }
    }

    // ----- FUNCIONES AUXILIARES PÚBLICAS -----
    Node<T>* getRoot() { return root; }
    Node<T>* getCurrentOwner() { return current_owner; }
    int size() { return contadorNodos; }

    void guardarCSV(const string& filename = "bin/binary_tree.csv") {
        ofstream file(filename);
        file << "id,first_name,last_name,gender,age,id_father,is_dead,type_magic,is_owner\n";
        
        for(int i = 0; i < contadorNodos; i++) {
            Wizard& p = nodos[i]->getData();
            file << p.id << ","
                 << p.first_name << ","
                 << p.last_name << ","
                 << p.gender << ","
                 << p.age << ","
                 << p.id_father << ","
                 << (p.is_dead ? "1" : "0") << ","
                 << p.type_magic << ","
                 << (p.is_owner ? "1" : "0") << "\n";
        }
    }
};
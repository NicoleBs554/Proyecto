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
    string filename;

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

    Node<T>* encontrarSucesor(Node<T>* start) {
        if (!start) return nullptr;
        
        Node<T>* hijos[2] = {start->getLeft(), start->getRight()};
        
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
        
        // Prioridad 3: Cualquier hombre vivo
        for(int i = 0; i < 2; i++) {
            if(hijos[i] && !hijos[i]->getData().is_dead) {
                if(hijos[i]->getData().gender == 'M') return hijos[i];
            }
        }
        
        return nullptr;
    }

    Node<T>* buscarEnArbol(Node<T>* start) {
        if (!start) return nullptr;
        
        Node<T>* queue[MAX_NODOS];
        int front = 0, rear = 0;
        queue[rear++] = start;
        
        while (front < rear) {
            Node<T>* current = queue[front++];
            
            if (current != start && !current->getData().is_dead) {
                string magia = current->getData().type_magic;
                if(magia == "elemental" || magia == "unique") return current;
            }
            
            if (current->getLeft()) queue[rear++] = current->getLeft();
            if (current->getRight()) queue[rear++] = current->getRight();
        }
        return nullptr;
    }

    Node<T>* encontrarMujerJoven(bool conDiscipulos) {
        Node<T>* mujerJoven = nullptr;
        for(int i = 0; i < contadorNodos; i++) {
            const Wizard& datos = nodos[i]->getData();
            if(!datos.is_dead && datos.gender == 'F') {
                bool cumple = true;
                if (conDiscipulos) {
                    cumple = (nodos[i]->getLeft() || nodos[i]->getRight()) && 
                             datos.type_magic == "mixed";
                }
                
                if(cumple && (!mujerJoven || datos.age < mujerJoven->getData().age)) {
                    mujerJoven = nodos[i];
                }
            }
        }
        return mujerJoven;
    }

public:
    Tree(const string& file = "bin/binary_tree.csv") : root(nullptr), current_owner(nullptr), contadorNodos(0), filename(file) {}

    ~Tree() {
        deleteTree(root);
    }

    void buildFromCSV(const string& filename = "bin/binary_tree.csv") {
        ifstream file(filename);
        if(!file) {
            cerr << "Error abriendo archivo: " << filename << endl;
            return;
        }
        
         guardarCSV();

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
    void cargarDatos() {
        buildFromCSV(filename);
        cout << "Datos cargados correctamente desde " << filename << endl;
    }   
        
    void setOwner(int id) {
        Node<T>* nodo = buscarPorId(id);
        if (nodo) {
            if (current_owner) {
                current_owner->getData().is_owner = false;
            }
            current_owner = nodo;
            current_owner->getData().is_owner = true;
            guardarCSV(); // Persistir cambio inmediatamente
            cout << "Dueño actualizado y guardado en archivo\n";
        } else {
            cout << "Mago no encontrado" << endl;
        }
    }

    void guardarCSV(const string& filename = "bin/binary_tree.csv") {
        ofstream file(filename);
        file << "id,first_name,last_name,gender,age,id_father,is_dead,type_magic,is_owner\n";
        
        for(int i = 0; i < contadorNodos; i++) {
            const Wizard& p = nodos[i]->getData();
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
        file.close();
        cout << "Datos guardados en " << filename << endl;
    }

    void mostrarLineaSucesionVivos() {
        if(!current_owner) {
            cout << "No hay dueño actual del hechizo" << endl;
            return;
        }
        
        Node<T>* linea[MAX_NODOS];
        int contadorLinea = 0;
        Node<T>* actual = current_owner;
        
        while(actual && contadorLinea < MAX_NODOS) {
            if(!actual->getData().is_dead) {
                linea[contadorLinea++] = actual;
            }
            actual = actual->getParent();
        }
        
        cout << "\nLínea de Sucesión:" << endl;
        for(int i = contadorLinea-1; i >= 0; i--) {
            linea[i]->print();
            if(i > 0) cout << " -> ";
        }
        cout << endl;
    }

    void actualizarOwner() {
        if(!current_owner) {
            cout << "No hay dueño actual" << endl;
            return;
        }
        
        Wizard& datosOwner = current_owner->getData();
        Node<T>* nuevoOwner = nullptr;

        // REGLA ESPECIAL: >70 años
        if(!datosOwner.is_dead && datosOwner.age > 70) {
            Node<T>* hijos[2] = {current_owner->getLeft(), current_owner->getRight()};
            Node<T>* discipulo = nullptr;
            
            for(int i = 0; i < 2; i++) {
                if(hijos[i] && !hijos[i]->getData().is_dead) {
                    if(hijos[i]->getData().type_magic == datosOwner.type_magic) {
                        discipulo = hijos[i];
                        break;
                    }
                }
            }
            
            if(!discipulo) {
                for(int i = 0; i < 2; i++) {
                    if(hijos[i] && !hijos[i]->getData().is_dead) {
                        if(!discipulo || hijos[i]->getData().age > discipulo->getData().age) {
                            discipulo = hijos[i];
                        }
                    }
                }
            }
            nuevoOwner = discipulo;
        }
        
        // REGLAS PARA MUERTE
        if(datosOwner.is_dead || !nuevoOwner) {
            nuevoOwner = encontrarSucesor(current_owner);
            
            if(!nuevoOwner && current_owner->getParent()) {
                Node<T>* padre = current_owner->getParent();
                Node<T>* companero = (padre->getLeft() == current_owner) 
                                   ? padre->getRight() 
                                   : padre->getLeft();
                
                if(companero && !companero->getData().is_dead) {
                    if(companero->getData().type_magic == datosOwner.type_magic) {
                        nuevoOwner = companero;
                    } else {
                        nuevoOwner = buscarEnArbol(companero);
                    }
                }
            }
            
            if(!nuevoOwner && current_owner->getParent() && 
               current_owner->getParent()->getParent()) {
                Node<T>* abuelo = current_owner->getParent()->getParent();
                Node<T>* tio = (abuelo->getLeft() == current_owner->getParent())
                             ? abuelo->getRight()
                             : abuelo->getLeft();
                if(tio && !tio->getData().is_dead) nuevoOwner = tio;
            }
            
            if(!nuevoOwner) {
                nuevoOwner = encontrarMujerJoven(true);
                if(!nuevoOwner) nuevoOwner = encontrarMujerJoven(false);
            }
        }

        if(nuevoOwner) {
            datosOwner.is_owner = false;
            nuevoOwner->getData().is_owner = true;
            current_owner = nuevoOwner;
            cout << "Nuevo dueño: ";
            current_owner->print();
            cout << endl;
            guardarCSV();
        } else {
            cout << "No se pudo asignar nuevo dueño" << endl;
        }
    }

    void editarNodo(int id, const T& nuevosDatos) {
        Node<T>* nodo = buscarPorId(id);
        if(nodo) {
            T& datos = nodo->getData();
            datos.first_name = nuevosDatos.first_name;
            datos.last_name = nuevosDatos.last_name;
            datos.gender = nuevosDatos.gender;
            datos.age = nuevosDatos.age;
            datos.is_dead = nuevosDatos.is_dead;
            datos.type_magic = nuevosDatos.type_magic;
            
            if(nodo == current_owner && datos.is_dead) {
                datos.is_owner = false;
                actualizarOwner();
            }
            
            cout << "Datos actualizados" << endl;
            guardarCSV();
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

    Node<T>* getRoot() { return root; }
    Node<T>* getCurrentOwner() { return current_owner; }
    int size() { return contadorNodos; }
};
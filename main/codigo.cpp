#include <iostream>
#include <string>
#include <fstream>

using namespace std;

// Estructura para los datos de la Solicitud (Ruta Segura)
struct Solicitud {
    int codigo;
    string cliente;
    string telefono;
    string vehiculo;
    string ubicacion;
    string motivo;
    string destino;
    string estado; // "Pendiente", "En Cola", "Atendido"
};

// 1. ESTRUCTURAS DE NODOS
struct NodoLista {
    Solicitud info;
    NodoLista* siguiente;
};

struct NodoCola {
    Solicitud info;
    NodoCola* siguiente;
};

struct NodoPila {
    string accion;
    NodoPila* siguiente;
};

// ==========================================
// FUNCIONES DE ORDENACIÓN Y BÚSQUEDA BINARIA
// ==========================================

void intercambiar(Solicitud &a, Solicitud &b) {
    Solicitud temp = a;
    a = b;
    b = temp;
}

void ordenacionBurbuja(NodoLista* lista) {
    if (lista == NULL) return;
    NodoLista* i;
    NodoLista* j;
    for (i = lista; i != NULL; i = i->siguiente) {
        for (j = i->siguiente; j != NULL; j = j->siguiente) {
            if (i->info.codigo > j->info.codigo) {
                intercambiar(i->info, j->info);
            }
        }
    }
}

NodoLista* obtenerMedio(NodoLista* inicio, NodoLista* fin) {
    if (inicio == NULL) return NULL;
    
    NodoLista* lento = inicio;
    NodoLista* rapido = inicio->siguiente;

    while (rapido != fin) {
        rapido = rapido->siguiente;
        if (rapido != fin) {
            lento = lento->siguiente;
            rapido = rapido->siguiente;
        }
    }
    return lento;
}

bool busquedaBinaria(NodoLista* lista, int codigoBuscado) {
    if (lista == NULL) return false;

    ordenacionBurbuja(lista);

    NodoLista* inicio = lista;
    NodoLista* fin = NULL;

    while (fin == NULL || fin != inicio) {
        NodoLista* medio = obtenerMedio(inicio, fin);

        if (medio == NULL) return false;

        if (medio->info.codigo == codigoBuscado) {
            return true;
        }
        else if (medio->info.codigo > codigoBuscado) {
            fin = medio;
        }
        else {
            inicio = medio->siguiente;
        }
    }
    return false;
}


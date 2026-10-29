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

void registrarSolicitud(NodoLista*& lista, int cod, string cli, string tel, string veh, string ubi, string mot, string des) {
    if (busquedaBinaria(lista, cod)) {
        cout << "Error: El codigo " << cod << " ya existe en el sistema." << endl;
        return;
    }
    NodoLista* nuevoNodo = new NodoLista();
    nuevoNodo->info.codigo = cod;
    nuevoNodo->info.cliente = cli;
    nuevoNodo->info.telefono = tel;
    nuevoNodo->info.vehiculo = veh;
    nuevoNodo->info.ubicacion = ubi;
    nuevoNodo->info.motivo = mot;
    nuevoNodo->info.destino = des;
    nuevoNodo->info.estado = "Pendiente";
    
    nuevoNodo->siguiente = lista;
    lista = nuevoNodo;
    cout << "Solicitud registrada con exito." << endl;
}

void mostrarLista(NodoLista* lista) {
    if (lista == NULL) {
        cout << "La lista de solicitudes esta vacia." << endl;
        return;
    }
    NodoLista* actual = lista;
    cout << "\n--- LISTA GENERAL DE SOLICITUDES ---" << endl;
    while (actual != NULL) {
        cout << "Cod: " << actual->info.codigo 
             << " | Cliente: " << actual->info.cliente 
             << " | Ubicacion: " << actual->info.ubicacion 
             << " | Estado: " << actual->info.estado << endl;
        actual = actual->siguiente;
    }
}

void actualizarUbicacion(NodoLista* lista, int cod, string nuevaUbicacion) {
    NodoLista* actual = lista;
    while (actual != NULL) {
        if (actual->info.codigo == cod) {
            actual->info.ubicacion = nuevaUbicacion;
            cout << "Ubicacion de la solicitud " << cod << " actualizada a " << nuevaUbicacion << "." << endl;
            return;
        }
        actual = actual->siguiente;
    }
    cout << "Solicitud no encontrada." << endl;
}

// ==========================================
// OPERACIONES EN PILAS
// ==========================================

void pushPila(NodoPila*& cima, string accion) {
    NodoPila* nuevoNodo = new NodoPila();
    nuevoNodo->accion = accion;
    nuevoNodo->siguiente = cima;
    cima = nuevoNodo;
}

void popPila(NodoPila*& cima) {
    if (cima != NULL) {
        NodoPila* temp = cima;
        cima = cima->siguiente;
        delete temp;
    } else {
        cout << "La pila esta vacia." << endl;
    }
}

string topPila(NodoPila* cima) {
    if (cima != NULL) {
        return cima->accion;
    }
    return "Pila vacia";
}

bool buscarEnPila(NodoPila*& cima, string accionBuscada) {
    NodoPila* pilaAuxiliar = NULL;
    NodoPila* actual = cima;
    bool encontrado = false;

    while (actual != NULL) {
        if (actual->accion == accionBuscada) {
            encontrado = true;
            break;
        }
        pushPila(pilaAuxiliar, topPila(cima));
        popPila(cima);
        actual = cima;
    }

    while (pilaAuxiliar != NULL) {
        pushPila(cima, topPila(pilaAuxiliar));
        popPila(pilaAuxiliar);
    }

    return encontrado;
}

// ==========================================
// OPERACIONES EN COLAS
// ==========================================

void encolar(NodoCola*& frente, NodoCola*& final, Solicitud s) {
    NodoCola* nuevoNodo = new NodoCola();
    nuevoNodo->info = s;
    nuevoNodo->siguiente = NULL;
    
    if (frente == NULL) {
        frente = nuevoNodo;
    } else {
        final->siguiente = nuevoNodo;
    }
    final = nuevoNodo;
    cout << "Solicitud de " << s.cliente << " agregada a la cola de atencion de gruas." << endl;
}

// DESENCOLAR: Busca el nodo en la LISTA GENERAL y le cambia el estado a "Atendido"
void desencolar(NodoCola*& frente, NodoCola*& final, NodoLista* listaGeneral) {
    if (frente == NULL) {
        cout << "La cola de atencion esta vacia." << endl;
        return;
    }
    NodoCola* temp = frente;
    int codigoAtendido = temp->info.codigo;

    // Recorremos la lista general para modificar el puntero de esa solicitud
    NodoLista* auxLista = listaGeneral;
    while (auxLista != NULL) {
        if (auxLista->info.codigo == codigoAtendido) {
            auxLista->info.estado = "Atendido"; // <-- Cambio directo en el registro
            break;
        }
        auxLista = auxLista->siguiente;
    }

    cout << "Atendiendo grua para el cliente: " << temp->info.cliente 
         << " (Ubicacion: " << temp->info.ubicacion << ")" << endl;
    cout << "-> ¡Estado cambiado a ATENDIDO en la Lista General con exito!" << endl;
         
    frente = frente->siguiente;
    if (frente == NULL) {
        final = NULL;
    }
    delete temp;
}

void consultarFrente(NodoCola* frente) {
    if (frente != NULL) {
        cout << "Siguiente en ser atendido: " << frente->info.cliente 
             << " [Cod: " << frente->info.codigo << "]" << endl;
    } else {
        cout << "La cola esta vacia." << endl;
    }
}

// Persistencia en archivos de texto
void guardarDatos(NodoLista* lista) {
    ofstream archivo("solicitudes_ruta_segura.txt");
    NodoLista* temp = lista;
    while (temp != NULL) {
        archivo << temp->info.codigo << "|" << temp->info.cliente << "|" 
                << temp->info.telefono << "|" << temp->info.vehiculo << "|" 
                << temp->info.ubicacion << "|" << temp->info.motivo << "|" 
                << temp->info.destino << "|" << temp->info.estado << endl;
        temp = temp->siguiente;
    }
    archivo.close();
    cout << "Datos guardados en 'solicitudes_ruta_segura.txt'" << endl;
}

void cargarDatos(NodoLista*& lista) {
    ifstream archivo("solicitudes_ruta_segura.txt");
    if (!archivo) { 
        cout << "No se encontro archivo previo de datos." << endl; 
        return; 
    }
    
    int cod;
    string cli, tel, veh, ubi, mot, des, est;
    int contador = 0;
    char delimitador;
    
    while (archivo >> cod >> delimitador) {
        getline(archivo, cli, '|');
        getline(archivo, tel, '|');
        getline(archivo, veh, '|');
        getline(archivo, ubi, '|');
        getline(archivo, mot, '|');
        getline(archivo, des, '|');
        getline(archivo, est);
        
        NodoLista* nuevoNodo = new NodoLista();
        nuevoNodo->info.codigo = cod;
        nuevoNodo->info.cliente = cli;
        nuevoNodo->info.telefono = tel;
        nuevoNodo->info.vehiculo = veh;
        nuevoNodo->info.ubicacion = ubi;
        nuevoNodo->info.motivo = mot;
        nuevoNodo->info.destino = des;
        nuevoNodo->info.estado = est;
        nuevoNodo->siguiente = lista;
        lista = nuevoNodo;

        contador++;
    }
    archivo.close();
    cout << "Se cargaron " << contador << " solicitud(es) desde el archivo." << endl;
}

// ==========================================
// MENÚ PRINCIPAL
// ==========================================
int main() {
    NodoLista* listaGeneral = NULL;
    NodoCola* frenteCola = NULL;
    NodoCola* finalCola = NULL;
    NodoPila* pilaHistorial = NULL;

    int opcion, cod;
    string cli, tel, veh, ubi, mot, des, accionBuscar;

    do {
        cout << "\n==========================================" << endl;
        cout << "        SISTEMA RUTA SEGURA              " << endl;
        cout << "==========================================" << endl;
        cout << "1. Registrar Solicitud (Lista)" << endl;
        cout << "2. Mostrar Registro General (Lista)" << endl;
        cout << "3. Buscar Solicitud por Codigo (Busqueda Binaria)" << endl;
        cout << "4. Actualizar Ubicacion (Lista)" << endl;
        cout << "5. Ordenar Solicitudes por Codigo (Burbuja)" << endl;
        cout << "6. Agregar a Cola de Gruas (Encolar)" << endl;
        cout << "7. Atender Siguiente Grua (Desencolar)" << endl;
        cout << "8. Ver Proxima Grua (Consultar Frente)" << endl;
        cout << "9. Ver Ultimo Cambio (Top Pila)" << endl;
        cout << "10. Buscar Accion en Historial (Pila Auxiliar)" << endl;
        cout << "11. Guardar Datos en Archivo" << endl;
        cout << "12. Cargar Datos desde Archivo" << endl;
        cout << "13. Salir" << endl;
        cout << "Seleccione una opcion: ";
        
        if (!(cin >> opcion)) {
            cout << "Error: Debe ingresar un numero entero, no una letra." << endl;
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        switch (opcion) {
            case 1:
                cout << "Ingrese Codigo: "; cin >> cod;
                cin.ignore();
                cout << "Nombre del Cliente: "; getline(cin, cli);
                cout << "Telefono: "; getline(cin, tel);
                cout << "Vehiculo: "; getline(cin, veh);
                cout << "Ubicacion Emergencia: "; getline(cin, ubi);
                cout << "Motivo: "; getline(cin, mot);
                cout << "Destino: "; getline(cin, des);
                
                registrarSolicitud(listaGeneral, cod, cli, tel, veh, ubi, mot, des);
                pushPila(pilaHistorial, "Registro_de_Solicitud");
                break;

            case 2:
                mostrarLista(listaGeneral);
                break;

            case 3:
                cout << "Ingrese codigo a buscar: "; cin >> cod;
                if (busquedaBinaria(listaGeneral, cod)) {
                    cout << "-> [BUSQUEDA BINARIA]: La solicitud SI se encuentra registrada." << endl;
                } else {
                    cout << "-> [BUSQUEDA BINARIA]: La solicitud NO existe." << endl;
                }
                break;

            case 4:
                cout << "Ingrese codigo de la solicitud: "; cin >> cod;
                cin.ignore();
                cout << "Ingrese nueva ubicacion: "; getline(cin, ubi);
                actualizarUbicacion(listaGeneral, cod, ubi);
                pushPila(pilaHistorial, "Actualizacion_de_Ubicacion");
                break;

            case 5:
                ordenacionBurbuja(listaGeneral);
                cout << "Lista ordenada por algoritmo Burbuja:" << endl;
                mostrarLista(listaGeneral);
                break;

            case 6: {
                cout << "Ingrese codigo de la solicitud a pasar a Grua: "; cin >> cod;
                NodoLista* aux = listaGeneral;
                bool hallado = false;
                while (aux != NULL) {
                    if (aux->info.codigo == cod) {
                        aux->info.estado = "En Cola"; // Modifica la lista general
                        encolar(frenteCola, finalCola, aux->info);
                        hallado = true;
                        break;
                    }
                    aux = aux->siguiente;
                }
                if (!hallado) cout << "No se encontro la solicitud en la lista." << endl;
                break;
            }

            case 7:
                // Pasa la listaGeneral como 3er parámetro para poder actualizar el estado
                desencolar(frenteCola, finalCola, listaGeneral);
                break;

            case 8:
                consultarFrente(frenteCola);
                break;

            case 9:
                cout << "Ultima accion en el historial: " << topPila(pilaHistorial) << endl;
                break;

            case 10:
                cin.ignore();
                cout << "Ingrese la accion a buscar en la Pila: "; getline(cin, accionBuscar);
                if (buscarEnPila(pilaHistorial, accionBuscar)) {
                    cout << "La accion SI existe en el historial de la Pila." << endl;
                } else {
                    cout << "La accion NO fue encontrada en la Pila." << endl;
                }
                break;

            case 11:
                guardarDatos(listaGeneral);
                break;

            case 12:
                cargarDatos(listaGeneral);
                break;

            case 13:
                cout << "Saliendo del programa..." << endl;
                break;

            default:
                cout << "Opcion no valida." << endl;
        }
    } while (opcion != 13);

    return 0;
}

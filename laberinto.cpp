//***************************************************************************
// File:   laberinto.cpp
// Author: Programación II. Universidad de Zaragoza
// Date:   March 27, 2019
// Coms:   Implementación del tipo "Laberinto" para la práctica 3 de la asignatura
//***************************************************************************

#include <iostream>
#include <unistd.h> // para "usleep"
#include <fstream>  // para "ficheros"
#include <stdlib.h> // para "rand"
#include <time.h>
#include "laberinto.hpp"

//*************************************************************************
// Encontrar un camino en el laberinto
//*************************************************************************
bool buscarCaminoR(Laberinto& lab, const int fila, const int columna) {
    //Laberinto Resuelto => Lab.mapa[lab.ancho-2][lab.ancho-2]
    if ((fila == lab.alto-2) && (columna == lab.ancho-2)) {
        lab.mapa[fila][columna] = CAMINO;
        //mostrarLaberintoR(lab); // Mostrar Recursivo
        //mostrarLaberinto(lab);  // Mostrar Iterativo
        return true;
    } else {
    //Aplicamos recursividad y buscamos caminos
        if ((fila >= 1) && (columna >= 1) && (fila <= lab.alto-2) && (columna <= lab.ancho-2) && (lab.mapa[fila][columna] == LIBRE)) {
            lab.mapa[fila][columna] = CAMINO;
            //mostrarLaberintoR(lab); // Mostrar Recursivo
            //mostrarLaberinto(lab);  // Mostrar Iterativo

            //Avanzamos hacia abajo
            if (buscarCaminoR(lab, fila+1, columna)) {
                return true;
            }
            //Avanzamos hacia la derecha
            if (buscarCaminoR(lab, fila, columna+1)) {
                return true;
            }
            //Avanzamos hacia arriba
            if (buscarCaminoR(lab, fila-1, columna)) {
                return true;
            }
            //Avanzamos hacia la izquierda
            if (buscarCaminoR(lab, fila, columna-1)) {
                return true;
            }
            //Backtracking si no llegamos a solucion
            lab.mapa[fila][columna] = IMPOSIBLE;
            //mostrarLaberintoR(lab); // Mostrar Recursivo
            //mostrarLaberinto(lab);  // Mostrar Iterativo
            return false;
        }
        return false;
    }
}

void buscarCamino(Laberinto& lab, bool& encontrado) {
    encontrado = buscarCaminoR(lab, 1, 1);
}

//*************************************************************************
// Generar el laberinto
//*************************************************************************

void cargarLaberintoR(ifstream &archivo, Laberinto &lab, int fila, int columna) {
    char caracter = archivo.get();

    if (!archivo.eof()) {
        if (caracter == '\n') {
            lab.ancho = columna-1;
            cargarLaberintoR(archivo, lab, fila+1, 0);
        } else {
            lab.mapa[fila][columna] = caracter;
            cargarLaberintoR(archivo, lab, fila, columna+1);
        }
    } else {
        lab.alto = fila+1;
    }
}

void cargarLaberinto(const char nombreFichero[], Laberinto& lab) {
    ifstream archivo(nombreFichero);
    if (archivo.is_open()) {
        cargarLaberintoR(archivo, lab, 0, 0);
        archivo.close();
    } else {
        cerr << "ERROR > Imposible de abrir el fichero" << endl;
    }
}

void generarLaberinto(Laberinto &lab, double densidad, int fila, int col) {
    if (fila <= lab.alto-1 && col <= lab.ancho-1) {
        //Itera hasta el ultimo '#' lab.mapa[lab.alto-1][lab.ancho-1]
        if ((fila == 0) || (fila == lab.alto-1) || (col == lab.ancho-1)) {
            // Rellena los muros |TOP| && |BOTTON| && |RIGHT|
            lab.mapa[fila][col] = MURO;
            if(col == lab.ancho-1) {
                //Relleno el muro |LEFT|
                if (fila < lab.alto-1) {
                    // Si todavía no hemos llegado a la ultima fila:
                    lab.mapa[fila+1][0] = MURO;                 //  Rellenamos  el muro |LEFT|
                    generarLaberinto(lab, densidad, fila+1, 1); //  Saltamos a la siguiente fila a la [fila+1][1]
                } else {
                    //  Coloca el último '#'
                    lab.mapa[lab.alto-1][lab.ancho-1] = MURO;
                }
            } else {
                generarLaberinto(lab, densidad, fila, col+1);
            }

        } else {
            // Relleno el interior
            if (((double)rand()/(RAND_MAX)) <= (densidad)) {
                lab.mapa[fila][col] = MURO;
            } else {
                lab.mapa[fila][col] = LIBRE;
            }
            generarLaberinto(lab, densidad, fila, col+1);
        }
    }

}

//*************************************************************************
// Visualizar el camino encontrado
//*************************************************************************

// Pre:   "lab" es un laberinto correcto, según la especificación dada para el tipo
// Post:  Se haa mostrado el laberinto por la salida estándar
// Coms:  Versión iterativa
void mostrarLaberinto(const Laberinto& lab) {
    system("clear");
    for (int i = 0; i < lab.alto; i++) {
        for (int j = 0; j < lab.ancho; j++) {
            if (lab.mapa[i][j] == CAMINO) {
                cout << "\033[1;36;46m" << lab.mapa[i][j] << "\033[0m";
            } else if (lab.mapa[i][j] == IMPOSIBLE) {
                cout << "\033[1;36;41m" << lab.mapa[i][j] << "\033[0m";
            } else {
                cout << lab.mapa[i][j];
            }
        }
        // Al acabar una fila, se cambia de línea
        cout << endl;
    }
    // Despues de terminar todas las filas
    usleep(RETRASO_MOSTRAR); //microsegundos
}

// Pre:  "lab" es un laberinto correcto, según la especificación en el enunciado
// Post:  Se ha mostrado el laberinto por la salida estándar
// Coms:  Versión recursiva
void mostrarLaberintoRAux(const Laberinto& lab, int fila, int columna) {
    if ((fila < lab.alto) && (columna <= lab.ancho)) {
        if (columna == lab.ancho) { // Cambio de Línea
            cout << endl;                               // Imprime -> Fila + 1
            mostrarLaberintoRAux(lab, fila+1, 0);       // Suma 1 a nuestro contador fila
        } else {                    // Avanzo en columna siempre que columna != lab.ancho
            if (lab.mapa[fila][columna] == CAMINO) {
                cout << "\033[1;36;46m" << lab.mapa[fila][columna] << "\033[0m";
            } else if (lab.mapa[fila][columna] == IMPOSIBLE) {
                cout << "\033[1;36;41m" << lab.mapa[fila][columna] << "\033[0m";
            } else {
                cout << lab.mapa[fila][columna];
            }
            mostrarLaberintoRAux(lab, fila, columna+1); // Avanza 1 en la columna
        }
    }
}

void mostrarLaberintoR(const Laberinto& lab) {
    system("clear");
    mostrarLaberintoRAux(lab, 0, 0);
    usleep(RETRASO_MOSTRAR); //microsegundos
}
#include <iostream>
#include <stdbool.h> // asegurar los booleanos
#include <time.h>
#include <fstream>
#include "laberinto.hpp"

using namespace std;

void generarGrafica() {
    system("gnuplot -e \"set terminal gif; set style data lines; plot 'tdens.txt' \" > tdens.gif");
}

void generarDatos(string nombreFichero, Laberinto &lab, const double dMin, const double dMax, const double Step, const int nRep) {    
    ofstream datos(nombreFichero);
    if (datos.is_open()) {
        double densidad = dMin;
        bool encontrado;
        while (densidad < dMax ) {  // Calculo los labs para todas las densidades segun Step
            clock_t TICKS = 0;
            int rep = 0;
            while (rep < nRep) {   // Repito los laberintos nRep veces para cada densidad
                encontrado = false;
                //time_t seed = time(nullptr);
                //srand(seed);
                generarLaberinto(lab, densidad, 0, 0);
                lab.mapa[1][1] = LIBRE;
                lab.mapa[lab.alto-2][lab.ancho-2] = LIBRE;
                clock_t TICKS1 = clock();   // Primera medida del tiempo
                buscarCamino(lab, encontrado);
                clock_t TICKS2 = clock();   // Segunda medida del tiempo
                if (encontrado) {   // Solo los cuento si tienen solucion
                    TICKS += (TICKS2 - TICKS1); // Tiempo en TICKS
                    rep++;
                }
            }
            datos << densidad << "\t" << double(1000000/CLOCKS_PER_SEC * double(TICKS) / nRep) << endl;
            densidad += Step;         // Modifico el valor de densidad
        }
        datos.close();

    } else {
        cerr << "No se creo el archivo" << endl;
    }
}

int main(int numArg, char* args[]) {
    if ((numArg == 2) || (numArg == 7)) {
        if (numArg == 2) {      // Generar la grafica
            string nombreFichero = args[1];
            if (nombreFichero == "DEVELOPER") {
                Laberinto lab;          //  Defino una 'Var' Laberinto
                lab.alto  = 80;         //  Preparo un ejemplo de Laberinto
                lab.ancho = 80;         //

                string nombreFichero = "tdens.txt";     //  Archivo para guardar los datos  |Densidad|  |Tiempos|
                double dMin = 0.1;                      //  Densidad Minima
                double dMax = 0.45;                     //  Densidad Maxima
                double Step = 0.01;                     //  Parametro para variar la densidad
                int nRep = 100;                         //  Parametro para obtener nRep tiempos para una misma densidad 
                generarDatos(nombreFichero, lab, dMin, dMax, Step, nRep);
                generarGrafica();
            } else {
                generarGrafica();
            }
        } else {                // Generar los Datos
            Laberinto lab;          //  Defino una 'Var' Laberinto
            lab.alto  = atoi(args[2]);         //  Preparo un ejemplo de Laberinto
            lab.ancho = atoi(args[2]);         //

            string nombreFichero = args[1];    //  Archivo para guardar los datos  |Densidad|  |Tiempos|
            double dMin = stod(args[3]);       //  Densidad Minima
            double dMax = stod(args[4]);       //  Densidad Maxima
            double Step = stod(args[5]);       //  Parametro para variar la densidad
            int nRep = atoi(args[6]);          //  Parametro para obtener nRep tiempos para una misma densidad 

            generarDatos(nombreFichero, lab, dMin, dMax, Step, nRep);
        }
    } else {
        cerr << "ERROR > Ha habido un error al introducir los parametros:" << endl;
        cerr << "~Grafica  ->  ./costeLab <nombreFichero.txt>" << endl;
        cerr << "~Datos    ->  ./costeLab <nombreFichero.txt> <3 <= Lab.Dim <= 120> <[dMin >= 0]> <[dMax <= 1]> <Step> <nRep>" << endl;
    }
    return 0;
}

//Desktop/798095~Unizar/1.2.3~Prog2/Practicas/Practica4
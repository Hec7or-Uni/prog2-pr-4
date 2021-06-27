# include <fstream>
# include <iostream>
# include <time.h>
# include "laberinto.hpp"
using namespace std;

// Dimensión de los laberintos
const int M = 80;

int main(int argc, char* argv[]) {
    if (argc != 5) { // Número de parámetros incorrecto.
        cout << "Número de parámetros incorrecto." << endl;
        return 1;
    }
    else {
        double dMin = atof(argv[1]);
        double dMax = atof(argv[2]);
        double step = atof(argv[3]);
        int nRep = atoi(argv[4]);
        if (0 <= dMin && dMin <= dMax && dMax < 1
                && 0 < step && step < 1 && 0 < nRep ) { // Parámetros en intervalo correcto.
            ofstream f("tdens.txt");
            if (f.is_open()) {
                Laberinto lab;
                lab.ancho = M;
                lab.alto = M;
                bool encontrado;
                double d = dMin;
                while (d < dMax + step) {
                	clock_t t = 0;
                    int i = 0;
                    while (i < nRep) {
                        encontrado = false;
                        time_t semilla = time(nullptr);
                        srand(semilla);
                        generarLaberinto(lab, d, 0, 0);
                        clock_t t1 = clock();
                        buscarCamino(lab, encontrado);
                        clock_t t2 = clock();
                        if (encontrado) {
                        	t += t2 - t1;
                            i++;
                        }
                    }
                    f << d << '\t' << double(1000000/CLOCKS_PER_SEC * double(t) / nRep) << endl;
                    d+=step;
                }
                f.close();
                system("gnuplot -e \"set terminal gif; set style data lines; plot 'tdens.txt'\" > tdens.gif");
                return 0;
            }
            else { 
                cout << "No se ha podido abrir el fichero para crear la gráfica." << endl;
                return 3;
            }
        }
        else { // Parámetros en intervalo incorrecto.
            cout << "Algún parámetro en intervalo incorrecto." << endl;
            return 2;
        }
    }
}

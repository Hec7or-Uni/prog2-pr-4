#***************************************************************************
# File:   Makefile
# Author: Programación II. Universidad de Zaragoza
# Date:   March 27, 2019
# Coms:   Ejemplo de Makefile
#         Ejecutar 'make' para compilar (o 'make -f Makefile')
#         Ejecutar 'make' clean para limpiar (borrar lo que genere)
#***************************************************************************

#---------------------------------------------------------
# macros
RM = /bin/rm -f  # para limpiar
CC = g++         # para compilar
#---------------------------------------------------------
# opciones de compilación
CPPFLAGS=-I. -std=c++11 -fmax-errors=1 # opciones compilación
#---------------------------------------------------------
costeLab: costeLab.o laberinto.o
	$(CC) costeLab.o laberinto.o -o costeLab $(CPPFLAGS)
#---------------------------------------------------------
costeLab.o: costeLab.cpp
	$(CC) costeLab.cpp -c $(CPPFLAGS)
#---------------------------------------------------------
laberinto.o: laberinto.cpp laberinto.hpp
	$(CC) laberinto.cpp -c $(CPPFLAGS)
#---------------------------------------------------------
# Cuidado con lo que se pone aquí, que se borra sin preguntar
clean:
	$(RM) costeLab laberinto.o
	$(RM) costeLab costeLab.o
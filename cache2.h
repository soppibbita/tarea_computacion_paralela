#include <iostream>
#include <chrono>
#include <omp.h>
#include <vector>
#include <filesystem>
#include <list>
#include <fstream>
#include <typeinfo>
#include <filesystem>
#include <cmath>
#include "crear_matriz_cache.h"


using namespace std;

using MatrixS = vector<vector<int>>;

//funcion de multiplicacion amigable con cache2
int* multiplicar_cache2(int* A, int* B, int cols, int rows) {
    // Creamos C inicializada en cero
    int* C = new int[cols * rows];
    for (int i = 0; i < cols * rows; i++) {
        C[i] = 0;
    }

    // Transponemos B para mejorar la localidad de memoria
    int* B_T = new int[cols * rows];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            B_T[j * rows + i] = B[i * cols + j];  // B_T[j][i] = B[i][j]
        }
    }

    // Multiplicación
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < rows; j++) {
            int sum = 0;
            for (int k = 0; k < cols; k++) {
                sum += A[i * cols + k] * B_T[j * rows + k]; 
            }
            C[i * rows + j] = sum;
        }
    }

    delete[] B_T;  // Liberamos memoria 
    return C;
}

void experimento_cache2() {

    list<int> n_experimentos;
    list<double> t_ejecucion;

    // Creacion de archivos para guardar los resultados
    ofstream tamano("tamano_cache.txt");
    ofstream tiempos("tiempos_cache2.txt");
    for (int i = 2; i < 12; i++) {
        // por simplicidad se asumen matrices cuadradas
        int rows = pow(2, i);
        //se crean matrices con los mismos valores para A y B pero distintas estructuras según el enfoque
        int* A = crearMatriz_cache(rows, rows, 1);
        int* B = crearMatriz_cache(rows, rows, 2);
        int contador = 0;
        while (contador < 10) {
            auto start2 = chrono::steady_clock::now();
            int* C = multiplicar_cache2(A, B, rows, rows);
            auto end2 = chrono::steady_clock::now();
            auto tcache = chrono::duration_cast<chrono::nanoseconds>(end2 - start2).count();
            delete[] C;
            contador++;
            n_experimentos.insert(n_experimentos.end(), rows);
            t_ejecucion.insert(t_ejecucion.end(), tcache);
        }

        delete[] A;
        delete[] B;
    }

    for (auto i : n_experimentos) {
        tamano << i << endl;
    }
    for (auto i : t_ejecucion) {
        tiempos << i << endl;
    }
    tamano.close();
    tiempos.close();
}
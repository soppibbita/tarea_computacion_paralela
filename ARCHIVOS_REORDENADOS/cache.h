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

//funcion de multiplicacion amigable con cache
int* multiplicar_cache(int* A, int* B, int cols, int rows) {

    // Creo matriz en forma de arreglo, inicializada en cero
    int* C = new int[cols * rows];
    for (int i = 0; i < cols * rows; i++) {
        C[i] = 0;
    }
    //auto start = chrono::steady_clock::now();
    // Multiplicamos
    for (int i = 0; i < rows; i++) {
        for (int k = 0; k < cols; k++) {
            int a_ik = A[i * cols + k]; // A[i][k]
            for (int j = 0; j < rows; j++) {
                C[i * rows + j] += a_ik * B[k * rows + j];//  C[i][j] = A[i][k] * B[k][j]
            }
        }
    }
    //auto end = chrono::steady_clock::now();
    //auto tinitns = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    //cout << " tiempo multiplicacion " << tinitns << " ns " << tinitns / 1000000000.0 << " s" << endl;

    return C;
}

void experimento_cache() {

    list<int> n_experimentos;
    list<double> t_ejecucion;

    // Creacion de archivos para guardar los resultados
    ofstream tamano("tamano_cache.txt");
    ofstream tiempos("tiempos_cache.txt");
    for (int i = 2; i < 12; i++) {
        // por simplicidad se asumen matrices cuadradas
        int rows = pow(2, i);
        //se crean matrices con los mismos valores para A y B pero distintas estructuras según el enfoque
        int* A = crearMatriz_cache(rows, rows, 1);
        int* B = crearMatriz_cache(rows, rows, 2);
        int contador = 0;
        while (contador < 10) {
            auto start2 = chrono::steady_clock::now();
            int* C = multiplicar_cache(A, B, rows, rows);
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
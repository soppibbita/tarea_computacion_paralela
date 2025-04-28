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
#include "crear_matriz.h"
#include "borrar_matriz.h"

using namespace std;

using MatrixS = vector<vector<int>>;

//funcion basica de multiplicacion de matrices con omp parallel
int** multiplicar_paralelo(int** A, int** B, int rows, int cols) {
    // memoria en 2D
    int** matrix = new int* [rows];

    // memoria para las filas
    for (int i = 0; i < rows; i++) {
        matrix[i] = new int[rows];
    }


    //#pragma omp parallel
    //{
    //#pragma omp master
      //  {
    //        cout << "Numero de hilos: " << omp_get_num_threads() << endl;
        //}
    //}

    //multiplicar
#pragma omp parallel for
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < rows; j++) {
            matrix[i][j] = 0;
            for (int k = 0; k < cols; k++) {
                matrix[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    //auto end = chrono::steady_clock::now();
    //auto tinitns = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    //cout << " tiempo multiplicacion " << tinitns << " ns " << tinitns / 1000000000.0 << " s" << endl;
    return matrix;
}

void experimento_paralelo() {
    list<int> n_experimentos;
    list<double> t_ejecucion;

    // Creacion de archivos para guardar los resultados
    ofstream tamano("tamano_paralelo.txt");
    ofstream tiempos("tiempos_paralelo.txt");
    for (int i = 2; i < 12; i++) {
        // por simplicidad se asumen matrices cuadradas
        int rows = pow(2, i);
        //se crean matrices con los mismos valores para A y B pero distintas estructuras según el enfoque
        int** A = crearMatriz(rows, rows, 1);
        int** B = crearMatriz(rows, rows, 2);

        int contador = 0;
        while (contador < 10) {
            auto start = chrono::steady_clock::now();
            int** matrix_C = multiplicar_paralelo(A, B, rows, rows);
            auto end = chrono::steady_clock::now();
            auto tparalelo = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
            borrarMatriz(matrix_C, rows);
            contador++;
            n_experimentos.insert(n_experimentos.end(), rows);
            t_ejecucion.insert(t_ejecucion.end(), tparalelo);
        }

        borrarMatriz(A, rows);
        borrarMatriz(B, rows);
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
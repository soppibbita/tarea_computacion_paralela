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

//funcion basica de multiplicacion de matrices secuencial
int** multiplicar_secuencial(int** A, int** B, int rows, int cols) {
    // Paso 1: Pide memoria din�mica{ para un arreglo de punteros a filas
    // esto usa una representacion 2D
    int** matrix = new int* [rows];

    // Paso 2: Pide memoria para cada arreglo de fila
    for (int i = 0; i < rows; i++) {
        matrix[i] = new int[rows];
    }
    //matriz C es de dimensi�n [rows][rows] por la multiplicaci�n

    // Paso 3: Multiplica
    //auto start = chrono::steady_clock::now();

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

void experimento_secuencial() {
    list<int> n_experimentos;
    list<double> t_ejecucion;

    // Creacion de archivos para guardar los resultados
    ofstream tamano("tamano_secuencial.txt");
    ofstream tiempos("tiempos_secuencial.txt");
    for (int i = 2; i < 12; i++) {
        // por simplicidad se asumen matrices cuadradas
        int rows = pow(2, i);
        //se crean matrices con los mismos valores para A y B pero distintas estructuras según el enfoque
        int** A = crearMatriz(rows, rows, 1);
        int** B = crearMatriz(rows, rows, 2);

        int contador = 0;
        while (contador < 10) {
            auto start = chrono::steady_clock::now();
            int** matrix_C = multiplicar_secuencial(A, B, rows, rows);
            auto end = chrono::steady_clock::now();
            auto tsecuencial = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
            borrarMatriz(matrix_C, rows);
            contador++;
            n_experimentos.insert(n_experimentos.end(), rows);
            t_ejecucion.insert(t_ejecucion.end(), tsecuencial);
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
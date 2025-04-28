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
#include "sumar_matriz.h"
#include "crear_matriz_vector.h"



using namespace std;

using MatrixS = vector<vector<int>>;

//multiplicacion con recursivo
MatrixS multiplicar_recursivo(const MatrixS& A, const MatrixS& B) {
    int size = A.size();
    MatrixS C(size, vector<int>(size, 0));
    //caso base recursivo
    if (size == 1) {
        C[0][0] = A[0][0] * B[0][0];
        return C;
    }

    int newSize = size / 2;

    // Crear submatrices
    MatrixS A11(newSize, vector<int>(newSize));
    MatrixS A12(newSize, vector<int>(newSize));
    MatrixS A21(newSize, vector<int>(newSize));
    MatrixS A22(newSize, vector<int>(newSize));
    MatrixS B11(newSize, vector<int>(newSize));
    MatrixS B12(newSize, vector<int>(newSize));
    MatrixS B21(newSize, vector<int>(newSize));
    MatrixS B22(newSize, vector<int>(newSize));

    // Llenar submatrices
    for (int i = 0; i < newSize; i++)
        for (int j = 0; j < newSize; j++) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + newSize];
            A21[i][j] = A[i + newSize][j];
            A22[i][j] = A[i + newSize][j + newSize];

            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + newSize];
            B21[i][j] = B[i + newSize][j];
            B22[i][j] = B[i + newSize][j + newSize];
        }

    // Multiplicaciones recursivas normales (8)
    MatrixS P1 = multiplicar_recursivo(A11, B11);
    MatrixS P2 = multiplicar_recursivo(A12, B21);
    MatrixS P3 = multiplicar_recursivo(A11, B12);
    MatrixS P4 = multiplicar_recursivo(A12, B22);
    MatrixS P5 = multiplicar_recursivo(A21, B11);
    MatrixS P6 = multiplicar_recursivo(A22, B21);
    MatrixS P7 = multiplicar_recursivo(A21, B12);
    MatrixS P8 = multiplicar_recursivo(A22, B22);

    // Calcular cuadrantes de C
    MatrixS C11 = sumar(P1, P2);
    MatrixS C12 = sumar(P3, P4);
    MatrixS C21 = sumar(P5, P6);
    MatrixS C22 = sumar(P7, P8);

    // Combinar los cuadrantes en C
    for (int i = 0; i < newSize; i++)
        for (int j = 0; j < newSize; j++) {
            C[i][j] = C11[i][j];
            C[i][j + newSize] = C12[i][j];
            C[i + newSize][j] = C21[i][j];
            C[i + newSize][j + newSize] = C22[i][j];
        }

    return C;
}

void experimento_recursivo() {
    list<int> n_experimentos;
    list<double> t_ejecucion;

    // Creacion de archivos para guardar los resultados
    ofstream tamano("tamano_recursivo.txt");
    ofstream tiempos("tiempos_recursivo.txt");
    for (int i = 2; i < 10; i++) {
        // por simplicidad se asumen matrices cuadradas
        int rows = pow(2, i);
        //se crean matrices con los mismos valores para A y B pero distintas estructuras según el enfoque
        MatrixS A = crear_matriz_vector(rows, 1);
        MatrixS B = crear_matriz_vector(rows, 2);
        int contador = 0;
        while (contador < 10) {
            auto start3 = chrono::steady_clock::now();
            MatrixS C = multiplicar_recursivo(A, B);
            auto end3 = chrono::steady_clock::now();
            auto trecursivo = chrono::duration_cast<chrono::nanoseconds>(end3 - start3).count();
            contador++;
            n_experimentos.insert(n_experimentos.end(), rows);
            t_ejecucion.insert(t_ejecucion.end(), trecursivo);
        }

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
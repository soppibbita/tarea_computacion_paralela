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
#include "restar_matriz.h"


using namespace std;

using MatrixS = vector<vector<int>>;

// Multiplicacion con Strassen < O(n3) cómo mido el tiempo en la recursividad ? este también debería paralelizarlo 
MatrixS multiplicar_strassen(const MatrixS& A, const MatrixS& B) {
    int size = A.size();
    MatrixS C(size, vector<int>(size, 0));
    //caso base recursividad
    if (size == 1) {
        C[0][0] = A[0][0] * B[0][0];
        return C;
    }
    //sino subdividimos a matrices mas pequeñas de 4x4
    int newSize = size / 2;
    MatrixS A11(newSize, vector<int>(newSize));
    MatrixS A12(newSize, vector<int>(newSize));
    MatrixS A21(newSize, vector<int>(newSize));
    MatrixS A22(newSize, vector<int>(newSize));
    MatrixS B11(newSize, vector<int>(newSize));
    MatrixS B12(newSize, vector<int>(newSize));
    MatrixS B21(newSize, vector<int>(newSize));
    MatrixS B22(newSize, vector<int>(newSize));

    // completamos las matrices
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

    // 7 multiplicaciones recursivas
    MatrixS M1 = multiplicar_strassen(sumar(A11, A22), sumar(B11, B22));
    MatrixS M2 = multiplicar_strassen(sumar(A21, A22), B11);
    MatrixS M3 = multiplicar_strassen(A11, restar(B12, B22));
    MatrixS M4 = multiplicar_strassen(A22, restar(B21, B11));
    MatrixS M5 = multiplicar_strassen(sumar(A11, A12), B22);
    MatrixS M6 = multiplicar_strassen(restar(A21, A11), sumar(B11, B12));
    MatrixS M7 = multiplicar_strassen(restar(A12, A22), sumar(B21, B22));

    // Combinar resultados
    MatrixS C11 = sumar(restar(sumar(M1, M4), M5), M7);
    MatrixS C12 = sumar(M3, M5);
    MatrixS C21 = sumar(M2, M4);
    MatrixS C22 = sumar(restar(sumar(M1, M3), M2), M6);

    // Juntar submatrices
    for (int i = 0; i < newSize; i++)
        for (int j = 0; j < newSize; j++) {
            C[i][j] = C11[i][j];
            C[i][j + newSize] = C12[i][j];
            C[i + newSize][j] = C21[i][j];
            C[i + newSize][j + newSize] = C22[i][j];
        }

    return C;
}

MatrixS multiplicar_strassen_paralelo(const MatrixS& A, const MatrixS& B) {
    int size = A.size();
    MatrixS C(size, vector<int>(size, 0));
    if (size == 1) {
        C[0][0] = A[0][0] * B[0][0];
        return C;
    }

    int newSize = size / 2;
    MatrixS A11(newSize, vector<int>(newSize)), A12(newSize, vector<int>(newSize)),
        A21(newSize, vector<int>(newSize)), A22(newSize, vector<int>(newSize)),
        B11(newSize, vector<int>(newSize)), B12(newSize, vector<int>(newSize)),
        B21(newSize, vector<int>(newSize)), B22(newSize, vector<int>(newSize));

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

    MatrixS M1, M2, M3, M4, M5, M6, M7;

#pragma omp parallel sections
    {
#pragma omp section
        { M1 = multiplicar_strassen(sumar(A11, A22), sumar(B11, B22)); }

#pragma omp section
        { M2 = multiplicar_strassen(sumar(A21, A22), B11); }

#pragma omp section
        { M3 = multiplicar_strassen(A11, restar(B12, B22)); }

#pragma omp section
        { M4 = multiplicar_strassen(A22, restar(B21, B11)); }

#pragma omp section
        { M5 = multiplicar_strassen(sumar(A11, A12), B22); }

#pragma omp section
        { M6 = multiplicar_strassen(restar(A21, A11), sumar(B11, B12)); }

#pragma omp section
        { M7 = multiplicar_strassen(restar(A12, A22), sumar(B21, B22)); }
    }

    MatrixS C11 = sumar(restar(sumar(M1, M4), M5), M7);
    MatrixS C12 = sumar(M3, M5);
    MatrixS C21 = sumar(M2, M4);
    MatrixS C22 = sumar(restar(sumar(M1, M3), M2), M6);

    for (int i = 0; i < newSize; i++)
        for (int j = 0; j < newSize; j++) {
            C[i][j] = C11[i][j];
            C[i][j + newSize] = C12[i][j];
            C[i + newSize][j] = C21[i][j];
            C[i + newSize][j + newSize] = C22[i][j];
        }

    return C;
}
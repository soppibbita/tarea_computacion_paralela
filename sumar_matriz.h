#ifndef _sumar_matriz_h
#define _sumar_matriz_h
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


using namespace std;

using MatrixS = vector<vector<int>>;

// Suma de matrices para vector
MatrixS sumar(const MatrixS& A, const MatrixS& B) {
    int size = A.size();
    MatrixS C(size, vector<int>(size, 0));
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            C[i][j] = A[i][j] + B[i][j];
    return C;
}

#endif
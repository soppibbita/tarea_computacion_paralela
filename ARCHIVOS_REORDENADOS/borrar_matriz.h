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

// Libera memoria dinamica (primero cada fila, luego el arreglo matrix )
void borrarMatriz(int** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        delete[] matrix[i]; //libera memoria de cada fila
    }
    delete[] matrix; //libera la memoria de los punteros en matrix
}
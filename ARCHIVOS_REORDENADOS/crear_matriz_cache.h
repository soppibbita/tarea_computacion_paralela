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

int* crearMatriz_cache(int rows, int cols, int k) {
    int* matrix = new int[rows * cols]; //creamos arreglo 

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i * cols + j] = (i + k) * (j + k);
        }
    }
    return matrix;

}
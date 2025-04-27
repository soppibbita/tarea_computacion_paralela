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
#include <crear_matriz.h>
#include "crear_matriz_cache.h"
#include "crear_matriz_vector.h"
#include "secuencial.h"
#include "paralelo.h"
#include "cache.h"
#include "cache2.h"
#include "recursivo.h"
#include "recursivo_paralelo.h"
#include "strassen.h"
#include "strassen_paralelo.h"

using namespace std;

using MatrixS = vector<vector<int>>;

int main() {
    int* A = crearMatriz_cache(rows, cols, 1);
    int* B = crearMatriz_cache(cols, rows, 2);
    MatrixS vA = crear_matriz_vector(rows, 1);
    MatrixS vB = crear_matriz_vector(rows, 2);

    experimento_secuencial();
    experimento_paralelo();
    cout << "STRASSEN" << endl;
    experimento_cache();
    experimento_cache2();
    experimento_recursivo();
    experimento_recursivo_paralelo();
    experimento_strassen();
    experimento_recursivo_paralelo();
    experimento_strassen_paralelo(vA, vB);

    delete[] A;
    delete[] B;


    return 0;
}
﻿#include <iostream>
#include <chrono>
#include <omp.h>
#include <vector>

using namespace std;

using MatrixS = vector<vector<int>>;

int** crearMatriz(int rows, int cols, int k) {
    // Paso 1: Pide memoria din�mica para un arreglo de punteros a filas
    // esto usa una representacion 2D
    int** matrix = new int* [rows];

    // Paso 2: Pide memoria para cada arreglo de fila
    for (int i = 0; i < rows; i++) {
        matrix[i] = new int[cols];
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = (i + k) * (j + k); // segun lo que necesite
        }
    }

    
    return matrix;
}

int* crearMatriz_cache(int rows, int cols, int k) {
    int* matrix = new int[rows * cols]; //creamos arreglo 

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i * cols + j] = (i + k) * (j + k);
        }
    }
    return matrix;

}

MatrixS crear_matriz_vector(int size, int k) {
    MatrixS matrix(size, vector<int>(size));
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            matrix[i][j] = (i + k) * (j + k);  
    return matrix;
}


void imprimirMatriz(int** matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

// Libera memoria dinamica (primero cada fila, luego el arreglo matrix )
void borrarMatriz(int** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        delete[] matrix[i]; //libera memoria de cada fila
    }
    delete[] matrix; //libera la memoria de los punteros en matrix
}

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

//funcion basica de multiplicacion de matrices con omp parallel
int** multiplicar_paralelo(int** A, int** B, int rows, int cols) {
    // Paso 1: Pide memoria din�mica{ para un arreglo de punteros a filas
    // esto usa una representacion 2D
    int** matrix = new int* [rows];

    // Paso 2: Pide memoria para cada arreglo de fila
    for (int i = 0; i < rows; i++) {
        matrix[i] = new int[rows];
    }
    //matriz C es de dimension [rows][rows] por la multiplicacion

    // Paso 3: Multiplica

    #pragma omp parallel
        {
    #pragma omp master
            {
                cout << "Numero de hilos: " << omp_get_num_threads() << endl;
            }
        }

        //auto start = chrono::steady_clock::now();
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


//funcion para multiplicar matrices dividir y conquistar 
// Suma de matrices para vector
MatrixS sumar(const MatrixS& A, const MatrixS& B) {
    int size = A.size();
    MatrixS C(size, vector<int>(size, 0));
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            C[i][j] = A[i][j] + B[i][j];
    return C;
}

// Resta de matrices para vector
MatrixS restar(const MatrixS& A, const MatrixS& B) {
    int size = A.size();
    MatrixS C(size, vector<int>(size, 0));
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            C[i][j] = A[i][j] - B[i][j];
    return C;
}

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


int main() {
    // por simplicidad se asumen matrices cuadradas
    int rows = 16;
    int cols = rows;
    //se crean matrices con los mismos valores para A y B pero distintas estructuras según el enfoque
    int** matrix_A = crearMatriz(rows, cols, 1);
    int** matrix_B = crearMatriz(cols, rows, 2);
    int* A = crearMatriz_cache(rows, cols, 1);
    int* B = crearMatriz_cache(cols, rows, 2);
    MatrixS vA = crear_matriz_vector(rows, 1);
    MatrixS vB = crear_matriz_vector(rows, 2);

    //cout << "matriz A"<<endl;
    //imprimirMatriz(matrix_A, rows, cols);
    //cout << "matriz B" << endl;
    //imprimirMatriz(matrix_B, cols, rows);

    cout << "SECUENCIAL" << endl;
    auto start = chrono::steady_clock::now();
    int** matrix_C = multiplicar_secuencial(matrix_A, matrix_B, rows, cols);
    auto end = chrono::steady_clock::now();
    auto tsecuencial = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    cout << " tiempo multiplicacion " << tsecuencial << " ns " << tsecuencial / 1000000000.0 << " s" << endl;
    
    cout << "PARALELO" << endl;
    auto start1 = chrono::steady_clock::now();
    matrix_C = multiplicar_paralelo(matrix_A, matrix_B, rows, cols);
    auto end1 = chrono::steady_clock::now();
    auto tparalelo = chrono::duration_cast<chrono::nanoseconds>(end1 - start1).count();
    cout << " tiempo multiplicacion " << tparalelo << " ns " << tparalelo / 1000000000.0 << " s" << endl;
    
    cout << "CACHE" << endl;
    auto start2 = chrono::steady_clock::now();
    int* C = multiplicar_cache(A, B, rows, cols);
    auto end2 = chrono::steady_clock::now();
    auto tcache = chrono::duration_cast<chrono::nanoseconds>(end2 - start2).count();
    cout << " tiempo multiplicacion " << tcache << " ns " << tcache / 1000000000.0 << " s" << endl;
    
    cout << "Recursivo" << endl;
    auto start3 = chrono::steady_clock::now();
    MatrixS vC = multiplicar_recursivo(vA,vB);
    auto end3 = chrono::steady_clock::now();
    auto trecursivo = chrono::duration_cast<chrono::nanoseconds>(end3 - start3).count();
    cout << " tiempo multiplicacion " << trecursivo << " ns " << trecursivo / 1000000000.0 << " s" << endl;

    cout << "Strassen" << endl;
    auto start4 = chrono::steady_clock::now();
    vC = multiplicar_strassen(vA, vB);
    auto end4 = chrono::steady_clock::now();
    auto tstrassen = chrono::duration_cast<chrono::nanoseconds>(end4 - start4).count();
    cout << " tiempo multiplicacion " << tstrassen << " ns " << tstrassen / 1000000000.0 << " s" << endl;
    //cout << "matriz C" << endl;
    //imprimirMatriz(matrix_C, rows, rows);

    borrarMatriz(matrix_A, rows);
    borrarMatriz(matrix_B, cols);
    borrarMatriz(matrix_C, rows);
    delete[] A;
    delete[] B;
    delete[] C;
    return 0;
}

//comentario para hacer un commit
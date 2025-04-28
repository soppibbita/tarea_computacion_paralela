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

int main(int argc, char* argv[]) {
    // Check if a command-line argument is provided
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <experiment_name>" << endl;
        cout << "Available experiments:" << endl;
        cout << "1. secuencial (sequential)" << endl;
        cout << "2. paralelo (parallel)" << endl;
        cout << "3. cache (sequential)" << endl;
        cout << "4. cache2 (sequential)" << endl;
        cout << "5. recursivo (sequential)" << endl;
        cout << "6. recursivo_paralelo (parallel)" << endl;
        cout << "7. strassen (sequential)" << endl;
        cout << "8. strassen_paralelo (parallel)" << endl;
        return 1;
    }

    // Convert argument to string for comparison
    string experiment = argv[1];

    // Set number of threads based on whether the experiment is parallel
    if (experiment == "secuencial" || experiment == "cache" || 
        experiment == "cache2" || experiment == "recursivo" || 
        experiment == "strassen") {
        omp_set_num_threads(1); // Force sequential execution
        cout << "Running sequential experiment with 1 thread" << endl;
    } else {
        // For parallel experiments, use default or set a specific number of threads
        int num_threads = omp_get_max_threads(); // Or set a specific number, e.g., 4
        omp_set_num_threads(num_threads);
        cout << "Running parallel experiment with " << num_threads << " threads" << endl;
    }

    // Run the selected experiment
    if (experiment == "secuencial") {
        experimento_secuencial();
    } else if (experiment == "paralelo") {
        experimento_paralelo();
    } else if (experiment == "cache") {
        experimento_cache();
    } else if (experiment == "cache2") {
        experimento_cache2();
    } else if (experiment == "recursivo") {
        experimento_recursivo();
    } else if (experiment == "recursivo_paralelo") {
        experimento_recursivo_paralelo();
    } else if (experiment == "strassen") {
        experimento_strassen();
    } else if (experiment == "strassen_paralelo") {
        experimento_strassen_paralelo();
    } else {
        cout << "Invalid experiment name. Available experiments:" << endl;
        cout << "1. secuencial (sequential)" << endl;
        cout << "2. paralelo (parallel)" << endl;
        cout << "3. cache (sequential)" << endl;
        cout << "4. cache2 (sequential)" << endl;
        cout << "5. recursivo (sequential)" << endl;
        cout << "6. recursivo_paralelo (parallel)" << endl;
        cout << "7. strassen (sequential)" << endl;
        cout << "8. strassen_paralelo (parallel)" << endl;
        return 1;
    }

    return 0;
}
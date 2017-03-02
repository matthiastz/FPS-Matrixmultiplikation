#ifndef FPS_MATRIXMULTIPLIKATION_MATRIX_H
#define FPS_MATRIXMULTIPLIKATION_MATRIX_H

#define COMPARE_EPSILON 0.000001
#define AVX_VECTOR_SIZE 8

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>
#include <immintrin.h>  // AVX header

// =======================================================================
// = ACCESS AND HELP FUNCTIONS
// =======================================================================


/**
 * create a matrix with pseudo random numbers filled. since the explicit
 * values in out matrices are not that much important, pseudo random should be ok
 *
 * @param N
 * @return
 */
float* createRandomizedMatrix_f(int N);


/**
 * compare the given matrices to make sure calculations are correct
 *
 * @param stdAlgorithm result matrix from standard algorithm
 * @param a other matrix to compare with
 * @return true / false if all matrix elements are the same or not
 */
bool compareResultMatrices_f(float* stdAlgorithm, float* a, int N);


/**
 * "compare" two float values with a given epsilon
 * found at: floating-point-gui.de/errors/comparison/
 *
 * @param a
 * @param b
 * @param epsilon
 * @return boolean if values are equal or not
 */
bool nearlyEqual(float a, float b, float epsilon);

// =======================================================================
// = MATRIX MULTIPLICATION
// =======================================================================

/**
 * Naive implementation of a matrix multiplication
 */
int standardMatrixMul_f(float* a, float* b, float* result, int N);


/**
 * Cache optimized matrix multiplication:
 * make the execution of loops more efficient, by increasing the locality of reference
 */
int optimizedMatrixMul_f(float* a, float* b, float* result, int N, int blockSize);



/**
 * Parallel implementation of a matrix multiplication using AVX
 */
int parallelMatrixMul_AVX_f(float* a, float* b, float* result, int N);

#endif //FPS_MATRIXMULTIPLIKATION_MATRIX_H

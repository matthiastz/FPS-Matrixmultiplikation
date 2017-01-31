//
// thanks to @benediktg for creating this interface :-)
//

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

/**
 * Represents a matrix.
 */
typedef struct Matrix {
    /** count of rows */
    unsigned int rowCount;
    /** count of columns */
    unsigned int columnCount;
    /** the actual matrix which is supposed to have a size of
     * count of rows times columns */
    float *data;

} Matrix;

// =======================================================================
// = ACCESS AND HELP FUNCTIONS
// =======================================================================

/**
 * Allocates space for the result of a matrix multiplication.
 */
Matrix callocMatrix(Matrix a, Matrix b);

/**
 * creates a matrix with given dimension (rows, columns)
 */
Matrix createRandomizedMatrix(unsigned int rowCount, unsigned int columnCount);

/**
 *
 * @param a the matrix ready to init.
 */
void initMatrixWithZeros(Matrix a);

/**
 * Frees up the space of the matrix data.
 */
int freeMatrix(Matrix* matrix);

/**
 * Checks if the matrix is similar to a null object.
 */
bool isNullMatrix(Matrix matrix);

/**
 * Checks if the matrix is square.
 */
bool isSquareMatrix(Matrix matrix);

/**
 * Retrieves a value of a matrix element.
 *
 * @param matrix the matrix instance (dimension M × N)
 * @param i the row number (must be in the interval 0..M-1)
 * @param j the column number (must be in the interval 0..N-1)
 * @returns the value of the given element
 */
float getElementValue(Matrix matrix, int i, int j);

/**
 * Sets a value of a matrix element.
 *
 * @param matrix the matrix instance (dimension M × N)
 * @param i the row number (must be in the interval 0..M–1)
 * @param j the column number (must be in the interval 0..N–1)
 * @param value the value to set
 */
void setElementValue(Matrix *matrix, int i, int j, float value);

/**
 * Adds a value to the current value of a matrix element.
 *
 * @param matrix the matrix instance (dimension M × N)
 * @param i the row number (must be in the interval 0..M–1)
 * @param j the column number (must be in the interval 0..N–1)
 * @param value the value to set
 */
void addToElemValue(Matrix *matrix, int i, int j, float value);

/**
 * Prints a representation of the matrix to stdout.
 */
int prettyPrint(Matrix matrix);
int prettyPrint_16(Matrix matrix);


/**
 * compare the given matrices to make sure calculations are correct
 *
 * @param stdAlgorithm result matrix from standard algorithm
 * @param a other matrix to compare with
 * @return true / false if matrix elements are the same or not
 */
bool compareResultMatrices(Matrix stdAlgorithm, Matrix a);

/**
 * "compare" two float values with a given epsilon
 * found at: floating-point-gui.de/errors/comparison/
 *
 * @param a
 * @param b
 * @param epsilon
 * @return
 */
bool nearlyEqual(float a, float b, float epsilon);

// =======================================================================
// = MATRIX MULTIPLICATION
// =======================================================================

/**
 * Naive implementation of a matrix multiplication
 */
int standardMatrixMul(Matrix a, Matrix b, Matrix *result);

/**
 * Cache optimized implemetation of a matrix multiplication (n x n)
 */
int optimizedMatrixMul_old(Matrix a, Matrix b, Matrix *result, int blockSize);

/**
 * wikipedia: make the execution of loops more efficient, by increasing the locality of reference
 */
int optimizedMatrixMul_DirectAccess(Matrix a, Matrix b, Matrix *result, int blockSize);


/**
 * Parallel implementation of a matrix multiplication
 */
int parallelMatrixMul(Matrix a, Matrix b, Matrix *result);


int parallelMatrixMul_AVX(Matrix a, Matrix b, Matrix *result);

#endif //FPS_MATRIXMULTIPLIKATION_MATRIX_H

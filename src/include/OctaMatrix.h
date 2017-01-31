//
// Created by matthias on 27.12.16.
//

#ifndef FPS_MATRIXMULTIPLIKATION_OCTAMATRIX_H
#define FPS_MATRIXMULTIPLIKATION_OCTAMATRIX_H

#include "Matrix.h"
#include <math.h>

/**
 * Represents a matrix containing arrays with 8 floats each.
 */
typedef struct OctaMatrix {
    unsigned int sections_count;
    float** data;
} OctaMatrix;

// TODO: change data structure of matrix to packed float arrays of 8 elements
// TODO: so we can load 8 elements in 1 "rutsch"
//                unaligned_floats = a.data[Ni_plus_Avx];


/**
 * Allocates space for the result of a matrix multiplication.
 */
OctaMatrix callocOctaMatrix(Matrix a, Matrix b);

/**
 * creates a matrix with given dimension (rows, columns)
 *
 * @param a
 * @param order
 * @return
 */
OctaMatrix createOctaMatrix(Matrix a, bool order);

/**
 *
 * @param a
 * @param b
 * @param result
 * @return error
 */
int multOct(OctaMatrix a, OctaMatrix b, Matrix *result);

/**
 *
 * @param a
 */
void printOctaMatrix(OctaMatrix a);

/**
 *
 * @param a
 * @return
 */
int freeOctMatrix(OctaMatrix* a);


#endif //FPS_MATRIXMULTIPLIKATION_OCTAMATRIX_H

//
// Created by matthias on 27.12.16.
//

#ifndef FPS_MATRIXMULTIPLIKATION_OCTAMATRIX_H
#define FPS_MATRIXMULTIPLIKATION_OCTAMATRIX_H

#include "Matrix.h"

/**
 * Represents a matrix containing arrays with 8 floats each.
 */
typedef struct OctaMatrix {
    /** count of rows */
    int rowCount;
    /** count of columns */
    int columnCount;
    /** the actual matrix which is supposed to have a size of
     * count of rows times columns */
    float*** data;  // each element in that matrix is a array of 8 floats
} OctaMatrix;

// TODO: change data structure of matrix to packed float arrays of 8 elements
// TODO: so we can load 8 elements in 1 "rutsch"
//                unaligned_floats = a.data[Ni_plus_Avx];

// old matrix:
//                1 1 1
//                2 2 2
//                .....
//                8 8 8
//                .....
//                ....

// new matrix:
//                [1        [1      [1
// ...      ...     ...
// 8]       8]      8]
//                ...
//                ...

/**
 * Allocates space for the result of a matrix multiplication.
 */
OctaMatrix allocOctaMatrix(Matrix a, Matrix b);

/**
 * creates a matrix with given dimension (rows, columns)
 */
OctaMatrix createOctaMatrix(Matrix a);

/**
 *
 * @param a
 * @param b
 * @param result
 * @return error
 */
int multOct(OctaMatrix a, OctaMatrix b, OctaMatrix *result);


#endif //FPS_MATRIXMULTIPLIKATION_OCTAMATRIX_H

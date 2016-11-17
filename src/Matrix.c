//
// Created by matthias on 08.11.16.
//

#include "include/Matrix.h"

#include <stdio.h>
#include <stdlib.h>


/*****************************
 * Access and help functions *
 *****************************/

Matrix allocMatrix(Matrix a, Matrix b) {
    Matrix result;

    if (isSquareMatrix(a) && isSquareMatrix(b)) {
        result.rowCount = a.rowCount;
        result.columnCount = a.columnCount;
        result.data = malloc(result.rowCount * result.columnCount * sizeof (float));

    } else if (1) {

    } else {
        // error
    }
    return result;
}

/**
 * create a matrix with pseudo random numbers filled. since the explicit
 * values in out matrices are not that much important, pseudo random should be ok
 *
 * @param rowCount ouf our generated matrix
 * @param columnCount ouf our generated matrix
 * @return the created matrix
 */
Matrix createRandomizedMatrix(int rowCount, int columnCount) {
    float value;
    Matrix result;
    // TODO: think we could need some testing here?
    result.rowCount = rowCount;
    result.columnCount = columnCount;
    result.data = malloc(result.rowCount * result.columnCount * sizeof (float));

    for (int i = 0; i < result.rowCount; ++i) {
        for (int j = 0; j < result.columnCount; ++j) {
            value = rand() % 100;   // value in the range 0 to 99
            setElementValue(&result, i, j, value);
        }
    }
    return result;
}

int freeMatrix(Matrix *matrix) {
    free(matrix->data);
    matrix->data = NULL;
    return 0;
}

bool isNullMatrix(Matrix matrix) {

}

bool isSquareMatrix(Matrix matrix) {
    return (matrix.columnCount == matrix.rowCount);
}

float getElementValue(Matrix matrix, int i, int j) {

    int index1D = (matrix.columnCount * i) + j;
    return matrix.data[index1D];
}

void setElementValue(Matrix *matrix, int i, int j, float value) {

    // 1 dimensional ... offset of column count ...

    int index1D = (matrix->columnCount * i) + j;
    matrix->data[index1D] = value;

}

int prettyPrint(Matrix matrix) {

    for (int i = 0; i < matrix.rowCount; ++i) {
        for (int j = 0; j < matrix.columnCount ; ++j) {
            printf("%.1f  ", getElementValue(matrix, i, j));
        }
        printf("\n");
    }

}

/*************************
 * Matrix multiplication *
 *************************/

int standardMatrixMul(Matrix a, Matrix b, Matrix *result) {

    // TODO check dimensions of matrices
    // case 1 : a.row == b.row && a.col == b.col
    // case 2 : a.row == b.col && a.col == b.row
    // else error

    float calc;

    for (int i = 0; i < a.rowCount ; ++i) {
        for (int j = 0; j < b.columnCount ; ++j) {

            calc = 0.0;
            for (int k = 0; k < b.rowCount ; ++k) {
                calc += getElementValue(a, i, j) * getElementValue(b,j, k);
            }
            setElementValue(result, i, j, calc);
        }
    }
}

int min(int a, int b) {
    return a > b ? a : b;
}

/**
 * wikipedia: make the execution of loops more efficient, by increasing the locality of reference
 *
 * @param a
 * @param b
 * @param result
 * @param blockSize
 * @return
 */
int optimizedMatrixMul(Matrix a, Matrix b, Matrix *result, int blockSize) {
    // TODO: fehler/trivial behandlung blocksize variable

    /**
     *
     * meh.................
     *
     */

/*
    int K, N, M;
    K = a.rowCount;
    M = K;
    N = K;
    int BS = blockSize;

    for(int i=0; i<N; i++) {
        for(int j=0; j<K; j++) {
            result->data[K*i + j] = 0;
        }
    }


    for(int l2=0; l2<M; l2+=BS) {
        for(int j2=0; j2<K; j2+=BS) {
            for(int i=0; i<N; i++) {
                for(int l=l2; l<min(M, l2+BS); l++) {
                    for(int j=j2; j<min(K, j2+BS); j++) {
                        result->data[K*i + j] += a.data[M*i+l]*b.data[K*l+j];
                    }
                }
            }
        }
    }

    */

    // tiled (parallel) matrix multiplication
// from /sys/devices/system/cpu/cpu0/cache/index0
// cat coherency_line_size returns 64;
// thus I will use 64 as the blocking size;

    int incr = blockSize;
    int i,j,k,x,y,z;
    int row = a.rowCount;
    int col = a.columnCount;

    for (i = 0; i < row; i += incr) {
        for (j = 0; j < col; j += incr) {
            result->data[i*col+j] = 0.0;
            for (k = 0; k < row; k += incr) {
                for (x = i; x < min( i + incr, row ); x++) {
                    for (y = j; y < min( j + incr, col ); y++) {
                        for (z = k; z < min( k + incr, row ); z++) {

                            result->data[ x * col + y ] +=  a.data[ x * col + z ] * b.data[ z * col  + y  ];

                        }
                    }
                }
            }
        }
    }
}

int parallelMatrixMul(Matrix a, Matrix b, Matrix *result) {

}

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
            value = rand() % 10;   // value in the range 0 to 9
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
                calc += getElementValue(a, i, k) * getElementValue(b, k, j);
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
     * meh.................to slow
     *
     */

    int N = a.rowCount;
    int TILE = blockSize;

    for (int i=0; i<N; i+=TILE) {
        for (int j = 0; j < N; j += TILE) {
            for (int k = 0; k < N; k += TILE) {
                /* Regular multiply inside the tiles */
                for (int y = i; y < i + TILE; y++) {
                    for (int x = j; x < j + TILE; x++) {

                        for (int z = k; z < k + TILE; z++) {
                            setElementValue(result, y, x, getElementValue(*result, y, x) +
                                                          (getElementValue(a, y, z) * getElementValue(b, z, x)));
                        }
                    }
                }
            }
        }
    }
}

int parallelMatrixMul(Matrix a, Matrix b, Matrix *result) {

}

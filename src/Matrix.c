//
// Created by matthias on 08.11.16.
//

#include "include/Matrix.h"


/*****************************
 * Access and help functions *
 *****************************/

Matrix allocMatrix(Matrix a, Matrix b) {
    Matrix result;

    if (isSquareMatrix(a) && isSquareMatrix(b)) {
        result.rowCount = a.rowCount;
        result.columnCount = a.columnCount;
        result.data = calloc(result.rowCount * result.columnCount, sizeof (result.data));

    } else {
        // TODO: error
        printf("error: wrong dimensions!\n");

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
Matrix createRandomizedMatrix(unsigned int rowCount, unsigned int columnCount) {
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

void initMatrixWithZeros(Matrix a) {
    for (int i = 0; i < a.rowCount; ++i) {
        for (int j = 0; j < a.rowCount; ++j) {
            setElementValue(&a, i, j, 0.0);
        }
    }
}


int freeMatrix(Matrix *matrix) {
    free(matrix->data);
    matrix->data = NULL;
    return 0;
}

bool isNullMatrix(Matrix matrix) {
    for (int i = 0; i < matrix.rowCount; ++i) {
        for (int j = 0; j < matrix.columnCount; ++j) {

            // TODO: redundant

            if (!(nearlyEqual(matrix.data[(matrix.columnCount * i) + j],
                0.0f, COMPARE_EPSILON))) {
                return false;
            }
        }
    }
    return true;
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

void addToElemValue(Matrix *matrix, int i, int j, float value) {
    int index1D = (matrix->columnCount * i) + j;
    matrix->data[index1D] += value;
}

int prettyPrint(Matrix matrix) {

    // if matrix is really big don't print it
    if (matrix.columnCount > 10 || matrix.rowCount > 10) {
        return 0;
    }

    for (int i = 0; i < matrix.rowCount; ++i) {
        for (int j = 0; j < matrix.columnCount ; ++j) {
            printf("%.1f  ", getElementValue(matrix, i, j));
        }
        printf("\n");
    }
    return 0;
}

bool compareResultMatrices(Matrix stdAlgorithm, Matrix a) {

    for (int i = 0; i < stdAlgorithm.rowCount ; ++i) {
        for (int j = 0; j < a.columnCount ; ++j) {

            // element found where values are different

            if (!(nearlyEqual(getElementValue(stdAlgorithm, i, j),
                            getElementValue(a, i, j), COMPARE_EPSILON))) {
                // TODO: remove printf line
                printf("false! : %f -- %f\n",getElementValue(stdAlgorithm, i, j),
                       getElementValue(a, i, j));
                return false;
            }
        }
    }
    return true;
}

bool nearlyEqual(float a, float b, float epsilon) {

    // TODO: use static float variables?
    float absA = fabsf(a);
    float absB = fabsf(b);
    float diff = fabsf(a - b);

    if (a == b) { // handles infinities
        return true;
    } else if (a == 0 || b == 0 || diff < FLT_EPSILON) {
        // a or b is zero or both are extremely close to it
        // relative error is less meaningful here
        return diff < (epsilon * FLT_EPSILON);
    } else { // use relative error
        return diff / fminf((absA + absB), FLT_MAX) < epsilon;
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
    // TODO:
    return 0;
}

int optimizedMatrixMul_old(Matrix a, Matrix b, Matrix *result, int blockSize) {
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
                            addToElemValue(result, y, x, (getElementValue(a, y, z) * getElementValue(b, z, x)));
                        }
                    }
                }
            }
        }
    }
    // TODO:
    return 0;
}

int optimizedMatrixMul_DirectAccess(Matrix a, Matrix b, Matrix *result, int blockSize) {
    // TODO: fehler/trivial behandlung blocksize variable

    int N = a.rowCount;
    int TILE = blockSize;

    for (int i=0; i<N; i+=TILE) {
        for (int j = 0; j < N; j += TILE) {
            for (int k = 0; k < N; k += TILE) {

                /* Regular multiply inside the tiles */
                for (int y = i; y < i + TILE; y++) {
                    for (int x = j; x < j + TILE; x++) {


                        for (int z = k; z < k + TILE; z++) {
                            result->data[(N * y) + x] +=
                                    a.data[(N * y) + z] * b.data[(N * z) + x];
                        }
                    }
                }
            }
        }
    }
    // TODO:
    return 0;
}

int parallelMatrixMul(Matrix a, Matrix b, Matrix *result) {

    if (!(isSquareMatrix(a) && isSquareMatrix(b))) {
        printf("avx impl.: matrix a & b have to be square (N x N)\n");
        return -1;
    } else if (a.rowCount != b.rowCount) {
        printf("matrix a & b have different dimensions\n");
        return -1;
    } else if (a.rowCount % AVX_VECTOR_SIZE != 0) {
        printf("matrix dimension mod %d has to be 0\n", AVX_VECTOR_SIZE);
        return -1;
    }

    int N = a.rowCount;
    int multiplication_count = N / AVX_VECTOR_SIZE;
    __m256 vA, vB, vC;
    float calc;
    float* f;


    for (int i = 0; i < N ; ++i) {
        int Ni = N * i;

        for (int j = 0; j < N ; ++j) {

            // reset calc for next loop of multiplication
            calc = 0.0;

            // dim/vector_size-times vector_size parallel multiplication
            for (int l = 0; l < multiplication_count; l++) {

                int lAvx = l*AVX_VECTOR_SIZE;
                int Ni_plus_Avx = Ni + lAvx;

//                float* unaligned_floats = (float*)malloc(64 * sizeof(float));
//                __m256 vec = _mm256_loadu_ps(unaligned_floats);


                // TODO: set steps are overhead !
                vA = _mm256_setr_ps(
                        a.data[Ni_plus_Avx + 0],
                        a.data[Ni_plus_Avx + 1],
                        a.data[Ni_plus_Avx + 2],
                        a.data[Ni_plus_Avx + 3],
                        a.data[Ni_plus_Avx + 4],
                        a.data[Ni_plus_Avx + 5],
                        a.data[Ni_plus_Avx + 6],
                        a.data[Ni_plus_Avx + 7]);

                vB = _mm256_setr_ps(
                        b.data[(N * (lAvx + 0)) + j],
                        b.data[(N * (lAvx + 1)) + j],
                        b.data[(N * (lAvx + 2)) + j],
                        b.data[(N * (lAvx + 3)) + j],
                        b.data[(N * (lAvx + 4)) + j],
                        b.data[(N * (lAvx + 5)) + j],
                        b.data[(N * (lAvx + 6)) + j],
                        b.data[(N * (lAvx + 7)) + j]);

                // multiplication (8*8 vectors)
                vC = _mm256_mul_ps(vA, vB);

                // TODO: overhead !
                f = (float*)&vC;
                for (int m = 0; m < AVX_VECTOR_SIZE ; ++m) {
                    calc += f[m];
                }

            }
            result->data[N * i + j] += calc;
        }
    }

    // TODO:
    return 0;
}
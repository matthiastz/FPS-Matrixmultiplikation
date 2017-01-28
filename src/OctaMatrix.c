//
// Created by matthias on 27.12.16.
//

#include "include/OctaMatrix.h"

// TODO: remove that data type ???

OctaMatrix createOctaMatrix(Matrix a) {
    OctaMatrix result;
    result.rowCount = a.rowCount / AVX_VECTOR_SIZE;
    result.columnCount = a.columnCount;

    int ROW = result.rowCount;
    int COL = result.columnCount;
    int N = COL;

    // alloc mem
    result.data = malloc(COL * sizeof(float**));
    for (int l = 0; l < COL; ++l) {
        result.data[l] = malloc(ROW * sizeof(float*));

        for (int m = 0; m < ROW; ++m) {
            result.data[m][l] = malloc(AVX_VECTOR_SIZE * sizeof(float));
        }
    }

    // errors
    if (!isSquareMatrix(a)) {
        printf("matrix is not a square matrix\n");
        // TODO:
        return result;
    } else if (a.rowCount % AVX_VECTOR_SIZE != 0) {
        printf("matrix.rowcount mod %d != 0\n",AVX_VECTOR_SIZE);
        // TODO:
        return result;
    }

    // put values
    for (int i = 0; i < N; ++i) {

        for (int j = 0; j < result.rowCount; ++j) {

//            float* temp = malloc(AVX_VECTOR_SIZE * sizeof(float));
            float temp[AVX_VECTOR_SIZE];
            for (int k = 0; k < AVX_VECTOR_SIZE; ++k) {
                temp[k] = a.data[(N * j) + (i * AVX_VECTOR_SIZE + k)];
            }
            result.data[j][i] = temp;
        }
    }
    return result;
}

OctaMatrix allocOctaMatrix(Matrix a, Matrix b) {
    OctaMatrix result;

    if (!isSquareMatrix(a) || !isSquareMatrix(b)) {
        printf("matrix a or matrix b is NOT a square matrix!\n");
        return result;
    }

    int N = a.rowCount;

    result.rowCount = a.rowCount / AVX_VECTOR_SIZE;
    result.columnCount = a.columnCount;
    int ROW = result.rowCount;
    int COL = result.columnCount;

    // alloc mem
    result.data = calloc(COL,  sizeof(float**));
    for (int l = 0; l < COL; ++l) {
        result.data[l] = calloc(ROW, sizeof(float*));

        for (int m = 0; m < ROW; ++m) {
            result.data[m][l] = calloc(AVX_VECTOR_SIZE, sizeof(float));
        }
    }

    // put values
    for (int i = 0; i < N; ++i) {

        for (int j = 0; j < result.rowCount; ++j) {

//            float* temp = malloc(AVX_VECTOR_SIZE * sizeof(float));
            float temp[AVX_VECTOR_SIZE];
            for (int k = 0; k < AVX_VECTOR_SIZE; ++k) {
                temp[k] = 0;
            }
            result.data[j][i] = temp;
        }
    }
    return result;
}

int multOct(OctaMatrix a, OctaMatrix b, OctaMatrix *result) {
    int N = a.rowCount;
    __m256 vec_A, vec_B, vec_C;

    for (int i = 0; i < N; ++i) {

        for (int j = 0; j < result->rowCount; ++j) {

            vec_A = _mm256_load_ps(a.data[i][j]);
            vec_B = _mm256_load_ps(b.data[i][j]);
            vec_C = _mm256_mul_ps(vec_A, vec_B);

            float* f = (float*)&vec_C;
            result->data[i][j] = f;
        }
    }

    // TODO:
    return 0;
}
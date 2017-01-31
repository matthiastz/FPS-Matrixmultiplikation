//
// Created by matthias on 27.12.16.
//

#include "include/OctaMatrix.h"

OctaMatrix createOctaMatrix(Matrix a, bool order) {
    OctaMatrix result;
    result.sections_count = (a.rowCount * a.columnCount) / AVX_VECTOR_SIZE;

    // alloc mem (section_count * array[8])
    result.data = malloc(result.sections_count * sizeof(float*));
    for (int l = 0; l < result.sections_count; ++l) {
        result.data[l] = malloc(AVX_VECTOR_SIZE * sizeof(float));
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

    // bool: row-major, col-major

    // put values
    for (int i = 0; i < a.rowCount; ++i) {
        for (int j = 0; j < a.columnCount; ++j) {
            float temp;
//            = getElementValue(a, i, j);

//            if (order) {
//                result.data[i][j] = temp;
//            } else {
//                result.data[j][i] = temp;
//            }

            if (order) {
                temp = getElementValue(a, j, i);
            } else {
                temp = getElementValue(a, i, j);
            }
            result.data[i][j] = temp;
        }
    }
    return result;
}

// TODO: write multiplication function in this file !!!!!!!!!!!!!

void printOctaMatrix(OctaMatrix a) {

    for (int l = 0; l < a.sections_count; ++l) {
        for (int i = 0; i < AVX_VECTOR_SIZE ; ++i) {
            printf("%.1f  ", a.data[l][i]);
        }
        printf("\n");
    }
}


OctaMatrix callocOctaMatrix(Matrix a, Matrix b) {
    OctaMatrix result;
    result.sections_count = (a.rowCount * a.columnCount) / AVX_VECTOR_SIZE;

    // alloc mem (section_count * array[8])
    result.data = malloc(result.sections_count * sizeof(float*));
    for (int l = 0; l < result.sections_count; ++l) {
        result.data[l] = calloc(AVX_VECTOR_SIZE, sizeof(float));
    }

    if (!isSquareMatrix(a) || !isSquareMatrix(b)) {
        printf("matrix a or matrix b is NOT a square matrix!\n");
        return result;
    }
    return result;
}

int freeOctMatrix(OctaMatrix* a) {
    int N = a->sections_count;

    // alloc mem (section_count * array[8])
    for (int i = 0; i < N; ++i) {
        free(a->data[i]);
        a->data[i] = NULL;
    }
    free(a->data);
    a->data = NULL;

    // TODO:
    return 0;
}


int multOct(OctaMatrix a, OctaMatrix b, Matrix *result) {

    // TODO: meh…
    int N = (int) sqrt(a.sections_count * AVX_VECTOR_SIZE);
    int vec_mm_count = N / AVX_VECTOR_SIZE;
    float calc;
    float* f;
    __m256 vec_A, vec_B, vec_C;

    if (a.sections_count != b.sections_count) {
        printf("OctaMatrix a and b have different section counts!\n");
        return -1;
    }

    /* calculation */
    for (int i = 0; i < N; ++i) {

        calc = 0.0;
        for (int j = 0; j < N; ++j) {

            for (int k = 0; k < vec_mm_count; ++k) {

//                calc += getElementValue(a, i, k) * getElementValue(b, k, j);

                vec_A = _mm256_loadu_ps(a.data[i + k]);
                vec_B = _mm256_loadu_ps(b.data[(N * k) + i]);

                if (j==0){
                    // debug:
                    float* fa = (float*)&vec_A;
                    float* fb = (float*)&vec_B;
                    for (int m = 0; m < AVX_VECTOR_SIZE ; ++m) {
                        printf("a: %.2f ", fa[m]);
                    }
                    printf("\n");
                    for (int m = 0; m < AVX_VECTOR_SIZE ; ++m) {
                        printf("b: %.2f ", fb[m]);
                    }
                    printf("\n");
                }

                vec_C = _mm256_mul_ps(vec_A, vec_B);

                // TODO: overhead !
                f = (float*)&vec_C;
                for (int m = 0; m < AVX_VECTOR_SIZE ; ++m) {
                    calc += f[m];
                }
            }
            result->data[(N * i) + j] += calc;
        }
    }

    // TODO:
    return 0;
}
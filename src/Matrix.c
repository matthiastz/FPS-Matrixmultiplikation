//
// Created by matthias on 08.11.16.
//

#include "include/Matrix.h"


// =======================================================================
// = ACCESS AND HELP FUNCTIONS
// =======================================================================

/**
 * Set / Get Values from matrix:
 *
 * int index1D = (N * i) + j;
 * matrix->data[index1D] = value;
 */

float* createRandomizedMatrix_f(int N) {
    float value;

    float* result = malloc(N * N * sizeof (float));

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            value = rand() % 10;   // value in the range 0 to 9
            result[(N * i) + j] = value;
        }
    }
    return result;
}


bool compareResultMatrices_f(float* stdAlgorithm, float* a, int N) {
    for (int i = 0; i < N ; ++i) {
        for (int j = 0; j < N ; ++j) {

            // element found where values are different

            if (!(nearlyEqual(stdAlgorithm[(N * i) + j],
                              a[(N * i) + j], COMPARE_EPSILON))) {

                printf("false! : %f -- %f\n",stdAlgorithm[(N * i) + j],
                       a[(N * i) + j]);
                return false;
            }
        }
    }
    return true;
}


bool nearlyEqual(float a, float b, float epsilon) {

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


// =======================================================================
// = MATRIX MULTIPLICATION
// =======================================================================


int standardMatrixMul_f(float* a, float* b, float* result, int N) {
    float calc;

    for (int i = 0; i < N ; ++i) {
        for (int j = 0; j < N ; ++j) {

            calc = 0.0;
            for (int k = 0; k < N ; ++k) {
                calc += a[(N * i) + k] * b[(N * k) + j];
            }
            result[(N * i) + j] = calc;
        }
    }
    // TODO:
    return 0;
}

int optimizedMatrixMul_f(float* a, float* b, float* result, int N, int blockSize) {
    int BLOCK = blockSize;

    for (int i = 0; i < N; i += BLOCK) {
        for (int j = 0; j < N; j += BLOCK) {
            for (int k = 0; k < N; k += BLOCK) {

                // Regular multiply inside the blocks
                for (int y = i; y < i + BLOCK; y++) {
                    for (int x = j; x < j + BLOCK; x++) {

                        for (int z = k; z < k + BLOCK; z++) {
                            result[(N * y) + x] +=
                                    a[(N * y) + z] * b[(N * z) + x];
                        }
                    }
                }
            }
        }
    }
    // TODO:
    return 0;
}


int parallelMatrixMul_AVX_f(float* a, float* b, float* result, int N) {

    int calculation_count = N / AVX_VECTOR_SIZE;

    // (III.)
    for (int i = 0; i < N; ++i) {

        int iN = (i * N);

        // "rows" of matrix B separated in blocks (II.)
        for (int j = 0; j < calculation_count; ++j) {

            int jAVX = j * AVX_VECTOR_SIZE;
            float* result_address = &result[iN + jAVX];


            // (I.)
            for (int k = 0; k < N; ++k) {

                // calculation:
                // __m256 broad_A = _mm256_set1_ps(a.data[k + (i * N)]);
                // __m256 vector_B = _mm256_load_ps(&b.data[j * AVX_VECTOR_SIZE + (k * N)]);
                // __m256 temp = _mm256_mul_ps(broad_A, vector_B);
                // result[] += (broad_A * vector_B)

                // TODO: aufaddieren (+=) <-> OVERHEAD
                _mm256_storeu_ps(result_address,
                                 _mm256_add_ps(
                                         _mm256_mul_ps(
                                                 _mm256_set1_ps(a[k + iN]),
                                                 _mm256_loadu_ps(&b[jAVX + (k * N)])),
                                         _mm256_loadu_ps(result_address)));
            }
        }
    }
    // TODO:
    return 0;
}


#include <stdio.h>
#include <time.h>
#include <cblas.h>// libatlas-dev (3.10.2-7)

#include "include/Matrix.h"
#include "include/HelpFunctions.h"

// file output
char* RESULT_FOLDER_PATH = "results/";
struct stat ST = {0};


int main() {

    // TODO: 2er Potenzen meint benedikt (512, 1024, 2048) -> #define?
    // TODO: auch kleine matrix größen betrachten! -> bezug L1-, L2-cache

    // TODO: remove unnecessary functions (old version: not plain float matrices)

    // VARIABLES

    clock_t start, end; // measure time
    bool calc_correct[3] = {false, false, false}; // correct calculations?
    double overall_times[4] = {0,0,0,0}; // save different overall times in array

    unsigned int N = 512; // dimension (N x N)
    int BS = 8; // block size
    int REPETITIONS = 5;

    // INIT.

    float* m1_f = createRandomizedMatrix_f(N);
    float* m2_f = createRandomizedMatrix_f(N);
    float* stdAlgorithm_F = calloc(N * N, sizeof (float));

    // TEST FOR DIMENSIONS

    if (N % AVX_VECTOR_SIZE != 0 || N < AVX_VECTOR_SIZE) {
        printf("> ERROR! dimension N (%d) has to be: N mod %d == 0\n",N,AVX_VECTOR_SIZE);
        return EXIT_FAILURE;
    }
    printHeader(N, REPETITIONS);

    // === CALCULATIONS ==================================================================

    // STANDARD ALGORITHM

    printf("executing standard algorithm multiplications …\n");

    for (int i = 0; i < REPETITIONS; ++i) {
        float* result_f = calloc(N * N, sizeof (float));

        // one multiplication
        start = clock();
        standardMatrixMul_f(m1_f, m2_f, result_f, N);
        end = clock();

        // copy values from other matrix
        if (i == 0) {
            for (int j = 0; j < N; ++j) {
                for (int k = 0; k < N; ++k) {
                    stdAlgorithm_F[(N * j) + k] = result_f[(N * j) + k];
                }
            }
        }

        overall_times[0] += ((double) (end - start)) / CLOCKS_PER_SEC;

        free(result_f);
    }


    // CACHE OPTIMIZED

    printf("executing cache optimized multiplications …\n");

    for (int i = 0; i < REPETITIONS; ++i) {
        float* result_cache_f = calloc(N * N, sizeof (float));

        start = clock();
        optimizedMatrixMul_DirectAccess_f(m1_f, m2_f, result_cache_f, N, BS);
        end = clock();

        overall_times[1] += ((double) (end - start)) / CLOCKS_PER_SEC;
        calc_correct[0] = compareResultMatrices_F(stdAlgorithm_F, result_cache_f, N);

        free(result_cache_f);
    }


    // PARALLEL (AVX)

    printf("executing parallel multiplications …\n");

    for (int i = 0; i < REPETITIONS; ++i) {
        float* result_para_f = calloc(N * N, sizeof (float));

        start = clock();
        parallelMatrixMul_AVX_f(m1_f, m2_f, result_para_f, N);
        end = clock();

        overall_times[2] += ((double) (end - start)) / CLOCKS_PER_SEC;
        calc_correct[1] = compareResultMatrices_F(stdAlgorithm_F, result_para_f, N);

        free(result_para_f);
    }


    // CBLAS (ATLAS)

    printf("executing multiplications using BLAS …\n");
    float ALPHA = 1;
    float BETA = 0;

    //  These parameters are different that those for pure Fortran
    int LDA=N;
    int LDB=N;
    int LDC=N;

    /* Single Precision, General Matrix Multiplication */
    for (int i = 0; i < REPETITIONS; ++i) {
        float* result_cblas_f = calloc(N * N, sizeof (float));


        start = clock();
        cblas_sgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,N,N,N,ALPHA,
                    m1_f,LDA,
                    m2_f,LDB,BETA,
                    result_cblas_f,LDC);
        end = clock();

        overall_times[3] += ((double) (end - start)) / CLOCKS_PER_SEC;
        calc_correct[2] = compareResultMatrices_F(stdAlgorithm_F, result_cblas_f, N);

        free(result_cblas_f);
    }

    // ===== cleanup allocated memory ===================================

    free(m1_f);
    free(m2_f);
    free(stdAlgorithm_F);


    // TODO: diagram matrix größe - ausführungszeit
    // diagramm blocksize by cache optimierung
    // mehrfach ausführen, durchschnitt


    // === FILE OUTPUT ===================================================================

    createResultFolder(RESULT_FOLDER_PATH, ST);
    char* filename =
            createResultFile(RESULT_FOLDER_PATH, N, REPETITIONS, overall_times, calc_correct);
    printf("\nwriting results to %s …\n", filename);
    printf("done.\n");

    free(filename);

    return 0;
}
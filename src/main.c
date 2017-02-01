#include <stdio.h>
#include <time.h>

#include "include/Matrix.h"
#include "include/HelpFunctions.h"


#include <cblas.h>// libatlas-dev (3.10.2-7)

char* RESULT_FOLDER_PATH = "results/";
struct stat ST = {0};


int main() {
    // measure time, bool variables
    clock_t start, end;
    bool calc_correct[3] = {false, false, false};
    double overall_times[4] = {0,0,0,0};

    // TODO: 2er Potenzen meint benedikt (512, 1024, 2048) -> #define?
    // TODO: auch kleine matrix größen betrachten! -> bezug L1-, L2-cache

    unsigned int N = 1025;
    unsigned int BS = 8;
    unsigned int REPETITIONS = 5;

    if (N % AVX_VECTOR_SIZE != 0 || N < AVX_VECTOR_SIZE) {
        printf("> ERROR! dimension N (%d) has to be: N mod %d == 0\n",N,AVX_VECTOR_SIZE);
        return EXIT_FAILURE;
    }


    printHeader(N, REPETITIONS);

//
    Matrix m1 = createRandomizedMatrix(N, N);
    Matrix m2 = createRandomizedMatrix(N, N);
    Matrix result = callocMatrix(m1, m2);
    Matrix resultCache2 = callocMatrix(m1, m2);
//    Matrix result_Parall = callocMatrix(m1, m2);

    // TODO: outsource to function
    float* m1_f = createRandomizedMatrix_f(N);
    float* m2_f = createRandomizedMatrix_f(N);


    float* stdAlgorithm_F = calloc(N * N, sizeof (float));
    float* result_f = calloc(N * N, sizeof (float));
    float* result_cache_f = calloc(N * N, sizeof (float));
    float* result_para_f = calloc(N * N, sizeof (float));
    float* result_cblas_f = calloc(N * N, sizeof (float));


    // ==================================================================
    // ===== standard algorithm multiplication ==========================
    // ==================================================================
    printf("executing standard algorithm multiplications …\n");
    Matrix stdAlgorithmResult = callocMatrix(m1, m2);

    for (int i = 0; i < REPETITIONS; ++i) {

        start = clock();
//        standardMatrixMul(m1, m2, &result);
        standardMatrixMul_f(m1_f, m2_f, result_f, N);
        end = clock();

        if (i == 0) {
            // save result of standard algorithm for comparison
            stdAlgorithm_F = result_f;
        }

        overall_times[0] += ((double) (end - start)) / CLOCKS_PER_SEC;

//        freeMatrix(&result);
//        result = callocMatrix(m1, m2);
        result_f = calloc(N * N, sizeof (float));
    }


    // ==================================================================
    // ===== cache optimized multiplication 2 ===========================
    // ==================================================================
    printf("executing cache optimized multiplications …\n");

    for (int i = 0; i < REPETITIONS; ++i) {

        start = clock();
//        optimizedMatrixMul_DirectAccess(m1, m2, &resultCache2, BS);
        optimizedMatrixMul_DirectAccess_f(m1_f, m2_f, result_cache_f, N, BS);
        end = clock();

        overall_times[1] += ((double) (end - start)) / CLOCKS_PER_SEC;
        calc_correct[0] = compareResultMatrices_F(stdAlgorithm_F, result_cache_f, N);

//        freeMatrix(&resultCache2);
//        resultCache2 = callocMatrix(m1, m2);
        result_cache_f = calloc(N * N, sizeof (float));
    }



    // ==================================================================
    // ===== parallel (AVX) =============================================
    // ==================================================================
    printf("executing parallel multiplications …\n");

//    Matrix avx_result = callocMatrix(m1, m2);
    for (int i = 0; i < REPETITIONS; ++i) {

        start = clock();
//        parallelMatrixMul_AVX(m1, m2, &avx_result);
        parallelMatrixMul_AVX_f(m1_f, m2_f, result_para_f, N);
        end = clock();

        overall_times[2] += ((double) (end - start)) / CLOCKS_PER_SEC;
        calc_correct[1] = compareResultMatrices_F(stdAlgorithm_F, result_para_f, N);
//
//        freeMatrix(&avx_result);
//        avx_result = callocMatrix(m1, m2);
        result_para_f = calloc(N * N, sizeof (float));
    }


    // ==================================================================
    // ===== multiplication using CBLAS (ATLAS-dev 3.10) ================
    // ==================================================================
    printf("executing multiplications using BLAS …\n");
//    Matrix result_Cblas = callocMatrix(m1, m2);
    float ALPHA = 1;
    float BETA = 0;

    //  These parameters are different that those for pure Fortran
    int LDA=N;
    int LDB=N;
    int LDC=N;

    /* Single Precision, General Matrix Multiplication */
    for (int i = 0; i < REPETITIONS; ++i) {

        start = clock();
        cblas_sgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,N,N,N,ALPHA,
                    m1_f,LDA,
                    m2_f,LDB,BETA,
                    result_cblas_f,LDC);
        end = clock();

        overall_times[3] += ((double) (end - start)) / CLOCKS_PER_SEC;
        calc_correct[2] = compareResultMatrices_F(stdAlgorithm_F, result_cblas_f, N);

//        freeMatrix(&result_Cblas);
        result_cblas_f = calloc(N * N, sizeof (float));
    }

//
//    // ==================================================================
//    // ===== cleanup allocated memory ===================================
//    // ==================================================================
    // TODO: clean up float matrices
//
    freeMatrix(&m1);
    freeMatrix(&m2);
    freeMatrix(&result);
//    freeMatrix(&resultCache2);
//    freeMatrix(&result_Parall);
//    freeMatrix(&result_Cblas);
//    freeMatrix(&avx_result);

//    freeMatrix(&resultOct);
//    freeOctMatrix(&oct1);
//    freeOctMatrix(&oct2);


    // TODO: diagram matrix größe - ausführungszeit
    // diagramm blocksize by cache optimierung
    // mehrfach ausführen, durchschnitt


    // ===== FILE OUTPUT ================================
    createResultFolder(RESULT_FOLDER_PATH, ST);
    char* filename =
            createResultFile(RESULT_FOLDER_PATH, N, REPETITIONS, overall_times, calc_correct);
    printf("\nwriting results to %s …\n", filename);
    printf("done.\n");

    return 0;
}


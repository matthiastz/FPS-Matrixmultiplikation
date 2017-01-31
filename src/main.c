#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

#include "include/Matrix.h"
#include "include/HelpFunctions.h"

#include "OctaMatrix.h"

#include <cblas.h>// libatlas-dev (3.10.2-7)

const char* RESULT_FOLDER_PATH = "results/";
struct stat ST = {0};



int main() {
    // measure time variables
    clock_t start, end;
    double cpu_time_used;
    bool calc_correct[3] = {false, false, false};
    double overall_times[4] = {0,0,0,0};

    // TODO: 2er Potenzen meint benedikt (512, 1024, 2048) -> #define?
    // TODO: auch kleine matrix größen betrachten! -> bezug L1-, L2-cache

    unsigned int N = 512;
    unsigned int BS = 8;
    unsigned int REPETITIONS = 100;
    printf("matrix multiplication: standard, cache-optimized, parallel, blas\n");
    printf("N: %d\n", N);
    printf("REPETITIONS for each algorithm: %d\n", REPETITIONS);
    printf("----------------------------------------------------------------\n");

//
    Matrix m1 = createRandomizedMatrix(N, N);
    Matrix m2 = createRandomizedMatrix(N, N);
    Matrix result = callocMatrix(m1, m2);
    Matrix resultCache2 = callocMatrix(m1, m2);
//    Matrix result_Parall = callocMatrix(m1, m2);


    // ==================================================================
    // ===== standard algorithm multiplication ==========================
    // ==================================================================
    printf("executing standard algorithm multiplication…\n");
    Matrix stdAlgorithmResult;

    for (int i = 0; i < REPETITIONS; ++i) {

        start = clock();
        standardMatrixMul(m1, m2, &result);
        end = clock();

        if (i == 1) {
            // save result of standard algorithm for comparison
            Matrix stdAlgorithmResult = result;
        }

        overall_times[0] += ((double) (end - start)) / CLOCKS_PER_SEC;
        result = callocMatrix(m1, m2);
    }


    // ==================================================================
    // ===== cache optimized multiplication 2 ===========================
    // ==================================================================
    printf("executing cache optimized multiplication…\n");

    for (int i = 0; i < REPETITIONS; ++i) {

        start = clock();
        optimizedMatrixMul_DirectAccess(m1, m2, &resultCache2, BS);
        end = clock();

        overall_times[1] += ((double) (end - start)) / CLOCKS_PER_SEC;
        calc_correct[0] = compareResultMatrices(stdAlgorithmResult, resultCache2);
        resultCache2 = callocMatrix(m1, m2);
    }

    // ==================================================================
    // ===== parallel (AVX) =============================================
    // ==================================================================
    printf("executing parallel multiplication…\n");

    Matrix avx_result = callocMatrix(m1, m2);
    for (int i = 0; i < REPETITIONS; ++i) {

        start = clock();
        parallelMatrixMul_AVX(m1, m2, &avx_result);
        end = clock();

        overall_times[2] += ((double) (end - start)) / CLOCKS_PER_SEC;
        calc_correct[1] = compareResultMatrices(stdAlgorithmResult, avx_result);
        avx_result = callocMatrix(m1, m2);
    }

    // ==================================================================
    // ===== multiplication using CBLAS (ATLAS-dev 3.10) ================
    // ==================================================================
    printf("executing multiplication using BLAS …\n");
    Matrix result_Cblas = callocMatrix(m1, m2);
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
                    m1.data,LDA,
                    m2.data,LDB,BETA,
                    result_Cblas.data,LDC);
        end = clock();

        overall_times[3] += ((double) (end - start)) / CLOCKS_PER_SEC;
        calc_correct[2] = compareResultMatrices(stdAlgorithmResult, result_Cblas);
        result_Cblas = callocMatrix(m1, m2);
    }

//
//    // ==================================================================
//    // ===== cleanup allocated memory ===================================
//    // ==================================================================
//
    freeMatrix(&m1);
    freeMatrix(&m2);
    freeMatrix(&result);
//    freeMatrix(&resultCache2);
//    freeMatrix(&result_Parall);
    freeMatrix(&result_Cblas);
    freeMatrix(&avx_result);

//    freeMatrix(&resultOct);
//    freeOctMatrix(&oct1);
//    freeOctMatrix(&oct2);


    // TODO: diagram matrix größe - ausführungszeit
    // diagramm blocksize by cache optimierung
    // mehrfach ausführen, durchschnitt

    // TODO:
    printf("%.2f - %.2f - %.2f - %.2f\n", overall_times[0], overall_times[1],
    overall_times[2], overall_times[3]);

    // ===== FILE OUTPUT ================================
    printf("writing results to folder results/…");
    createResultFolder(RESULT_FOLDER_PATH, ST);
    createResultFile(RESULT_FOLDER_PATH, N, REPETITIONS, overall_times, calc_correct);


    printf("done.\n");

    return 0;
}


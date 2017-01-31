#include <stdio.h>
#include <time.h>

#include "include/Matrix.h"

#include "OctaMatrix.h"

#include <cblas.h>// libatlas-dev (3.10.2-7)

int main() {

    // measure time variables
    clock_t start, end;
    double cpu_time_used;

    // TODO: 2er Potenzen meint benedikt (512, 1024, 2048) -> #define?
    // TODO: auch kleine matrix größen betrachten! -> bezug L1-, L2-cache

    unsigned int N = 1024;
    unsigned int BS = 32;
    printf("N: %d\n", N);
//
    Matrix m1 = createRandomizedMatrix(N, N);
    Matrix m2 = createRandomizedMatrix(N, N);
    Matrix result = callocMatrix(m1, m2);
    Matrix resultCache2 = callocMatrix(m1, m2);
//    Matrix result_Parall = callocMatrix(m1, m2);


    // ==================================================================
    // ===== standard algorithm multiplication ==========================
    // ==================================================================

    start = clock();
    standardMatrixMul(m1, m2, &result);
    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("std MM time used: %f seconds\n",cpu_time_used);
//    prettyPrint(result);
    printf("--------------------------------\n");

    // save result of standard algorithm for comparison
    Matrix stdAlgorithmResult = result;


    // ==================================================================
    // ===== cache optimized multiplication 2 ===========================
    // ==================================================================
    start = clock();
    optimizedMatrixMul_DirectAccess(m1, m2, &resultCache2, BS);
    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("cacheopt MM 2 (direct array access) time used: %f seconds\n",cpu_time_used);
    prettyPrint(resultCache2);
    printf("calculation correct? : %s\n",
           compareResultMatrices(stdAlgorithmResult, resultCache2) ? "true" : "false");
    printf("--------------------------------\n");


    // ==================================================================
    // ===== parallel multiplication 256 bit I ==========================
    // ==================================================================

//    start = clock();
//    parallelMatrixMul(m1, m2, &result_Parall);
//    end = clock();
//
//    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
//    printf("parallel MM (AVX) time used: %f seconds\n",cpu_time_used);
//    prettyPrint(result_Parall);
//    printf("calculation correct? : %s\n",
//           compareResultMatrices(stdAlgorithmResult, result_Parall) ? "true" : "false");
//    printf("--------------------------------\n");


    // ==================================================================
    // ===== parallel multiplication 256 bit II (octa) ==================
    // ==================================================================

//    OctaMatrix oct1 = createOctaMatrix(m1, true);
//    OctaMatrix oct2 = createOctaMatrix(m2, false);
//
//    Matrix resultOct = callocMatrix(m1, m2);
//
//    printf("normal matrices:\n");
//    prettyPrint(m1);
//    printf("\n");
//    prettyPrint(m2);
//
//    printf("octa matrices:\n");
//    printOctaMatrix(oct1);
//    printf("\n");
//    printOctaMatrix(oct2);
//
//
//    start = clock();
//    multOct(oct1, oct2, &resultOct);
//    end = clock();
//
//    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
//    printf("parallel MM (AVX oct) time used: %f seconds\n",cpu_time_used);
//    printf("calculation correct? : %s\n",
//           compareResultMatrices(stdAlgorithmResult, resultOct) ? "true" : "false");
//    printf("--------------------------------\n");
//
//    // TODO: free oct matrix !!!!!!!!!!!!
//    int COL = oct1.columnCount;
//    int ROW = oct1.rowCount;
//
//    for (int l = 0; l < COL; ++l) {
//        for (int m = 0; m < ROW; ++m) {
//            free(oct1.data[l][m]);
//        }
//        free(oct1.data[l]);
//    }
//    free(oct1.data);
//    for (int l = 0; l < COL; ++l) {
//        for (int m = 0; m < ROW; ++m) {
//            free(oct2.data[l][m]);
//        }
//        free(oct2.data[l]);
//    }
//    free(oct2.data);
//    for (int l = 0; l < COL; ++l) {
//        for (int m = 0; m < ROW; ++m) {
//            free(resultOct.data[l][m]);
//        }
//        free(resultOct.data[l]);
//    }
//    free(resultOct.data);

    // ==================================================================
    // ===== AVX TRY  ===========================
    // ==================================================================

    Matrix avx_result = callocMatrix(m1, m2);
    start = clock();
    parallelMatrixMul_AVX(m1, m2, &avx_result);
    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("AVX TRY time used: %f seconds\n",cpu_time_used);
    printf("calculation correct? : %s\n",
           compareResultMatrices(stdAlgorithmResult, avx_result) ? "true" : "false");
    printf("--------------------------------\n");
    freeMatrix(&avx_result);


    // ==================================================================
    // ===== multiplication using CBLAS (ATLAS-dev 3.10) ================
    // ==================================================================
    Matrix result_Cblas = callocMatrix(m1, m2);
    float ALPHA = 1;
    float BETA = 0;

    //  These parameters are different that those for pure Fortran
    int LDA=N;
    int LDB=N;
    int LDC=N;

    /* Single Precision, General Matrix Multiplication */
    start = clock();
    cblas_sgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,N,N,N,ALPHA,
                m1.data,LDA,
                m2.data,LDB,BETA,
                result_Cblas.data,LDC);
    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("CBLAS MM time used: %f seconds\n",cpu_time_used);
    printf("calculation correct? : %s\n",
           compareResultMatrices(stdAlgorithmResult, result_Cblas) ? "true" : "false");
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


    return 0;
}


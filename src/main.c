#include <stdio.h>
#include <time.h>

#include "include/Matrix.h"

int main() {

    //--------------------------------------------------------------

    // measure time variables
    clock_t start, end;
    double cpu_time_used;

    // TODO: 2er Potenzen meint benedikt (512, 1024, 2048) -> #define?
    // TODO: auch kleine matrix größen betrachten! -> bezug L1-, L2-cache

    int N = 1024;
    int BS = 16;
    printf("N: %d\n", N);

    Matrix m1 = createRandomizedMatrix(N, N);
    Matrix m2 = createRandomizedMatrix(N, N);
    Matrix result = allocMatrix(m1, m2);
    Matrix resultCache1 = allocMatrix(m1, m2);
    initMatrixWithZeros(resultCache1);
    Matrix resultCache2 = allocMatrix(m1, m2);
    initMatrixWithZeros(resultCache2);
    Matrix result_Parall = allocMatrix(m1, m2);
    initMatrixWithZeros(result_Parall);


    // ==================================================================
    // ===== standard algorithm multiplication ==========================
    // ==================================================================

    start = clock();
    // do the work
    standardMatrixMul(m1, m2, &result);
    Matrix stdAlgorithmResult = result;
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("std MM time used: %f seconds\n",cpu_time_used);
    prettyPrint(result);
    printf("--------------------------------\n");

    // ==================================================================
    // ===== cache optimized multiplication 1 ===========================
    // ==================================================================

    // TODO: implementierung robuster -> siehe OpenTuner framework
    // TODO: um nicht nur log(2)-Größen sondern alle zu unterstützen

    start = clock();
    // do the work
    optimizedMatrixMul_old(m1, m2, &resultCache1, BS);

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("cacheopt MM time used: %f seconds\n",cpu_time_used);
    prettyPrint(resultCache1);
//    printf("calculation correct? : %s\n",
//           compareResultMatrices(stdAlgorithmResult, resultCache1) ? "true" : "false");
    printf("--------------------------------\n");

    // ==================================================================
    // ===== cache optimized multiplication 2 ===========================
    // ==================================================================
    start = clock();
    // do the work
    optimizedMatrixMul_DirectAccess(m1, m2, &resultCache2, BS);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("cacheopt MM 2 (direct array access) time used: %f seconds\n",cpu_time_used);
    prettyPrint(resultCache2);
//    printf("calculation correct? : %s\n",
//           compareResultMatrices(stdAlgorithmResult, resultCache2) ? "true" : "false");


    // ==================================================================
    // ===== parallel multiplication 256 bit ============================
    // ==================================================================

    start = clock();
    parallelMatrixMul(m1, m2, &result_Parall);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("parallel MM (AVX) time used: %f seconds\n",cpu_time_used);
    prettyPrint(result_Parall);

    // TODO: correct calc sicherstellen


    // cleanup allocated memory
    freeMatrix(&m1);
    freeMatrix(&m2);
    freeMatrix(&result);

    // TODO: diagram matrix größe - ausführungszeit
    // diagramm blocksize by cache optimierung
    // mehrfach ausführen, durchschnitt

    return 0;
}


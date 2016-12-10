#include <stdio.h>
#include <immintrin.h>  // AVX header
#include <time.h>

#include "include/Matrix.h"

int main() {


    /*

    __m512 tmp = _mm512_set_ps(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);

    float* f = (float*)&tmp;

    for (int i = 0; i < 16 ; ++i) {
        printf("%f\n", f[i]);
    }

    __m512

    /**
     * z-style speicherung, bezug vorbereitung / performance
     */

    //--------------------------------------------------------------

    // measure time variables
    clock_t start, end;
    double cpu_time_used;

    // TODO: 2er Potenzen meint benedikt (512, 1024, 2048) -> #define?
    // TODO: auch kleine matrix größen betrachten! -> bezug L1-, L2-cache

    Matrix m1 = createRandomizedMatrix(4, 4);
    Matrix m2 = createRandomizedMatrix(4, 4);
    Matrix result = allocMatrix(m1, m2);


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

    Matrix resultCache1 = allocMatrix(m1, m2);
    initMatrixWithZeros(resultCache1);

    start = clock();
    // do the work
    optimizedMatrixMul_old(m1, m2, &resultCache1, 2);

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("cacheopt MM time used: %f seconds\n",cpu_time_used);
    prettyPrint(resultCache1);
    printf("calculation correct? : %s\n",
           compareResultMatrices(stdAlgorithmResult, resultCache1) ? "true" : "false");
    printf("--------------------------------\n");

    // ==================================================================
    // ===== cache optimized multiplication 2 ===========================
    // ==================================================================
    Matrix resultCache2 = allocMatrix(m1, m2);
    initMatrixWithZeros(resultCache2);
    start = clock();
    // do the work
    optimizedMatrixMul_DirectAccess(m1, m2, &resultCache2, 2);

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("cacheopt MM 2 (direct array access) time used: %f seconds\n",cpu_time_used);
    prettyPrint(resultCache2);
    printf("calculation correct? : %s\n",
           compareResultMatrices(stdAlgorithmResult, resultCache2) ? "true" : "false");

    // cleanup allocated memory
    freeMatrix(&m1);
    freeMatrix(&m2);
    freeMatrix(&result);

    // TODO: diagram matrix größe - ausführungszeit
    // diagramm blocksize by cache optimierung
    // mehrfach ausführen, durchschnitt

    return 0;
}


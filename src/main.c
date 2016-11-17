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

    Matrix m1, m2;
    //m1 = createRandomizedMatrix(4, 4);
    //m2 = createRandomizedMatrix(4, 4);

    m1.rowCount = 2;
    m1.columnCount = 2;
    m1.data = malloc(4* sizeof (float));
    setElementValue(&m1, 0, 0, 1);
    setElementValue(&m1, 0, 1, 2);
    setElementValue(&m1, 1, 0, 3);
    setElementValue(&m1, 1, 1, 4);

    m2.rowCount = 2;
    m2.columnCount = 2;
    m2.data = malloc(4* sizeof (float));
    setElementValue(&m2, 0, 0, 1);
    setElementValue(&m2, 0, 1, 2);
    setElementValue(&m2, 1, 0, 3);
    setElementValue(&m2, 1, 1, 4);


    Matrix result = allocMatrix(m1, m2);


    //===== standard multiplication ==================

    start = clock();
    // do the work
    standardMatrixMul(m1, m2, &result);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("std MM time used: %f seconds\n",cpu_time_used);
    prettyPrint(result);

    printf("\n--------------------------------\n");

    //===== cache optimized multiplication ==================
    // TODO: wrong calculation !!!

    // TODO: result -> to slow...
    // TODO: idea: we could use blocked style to save / access matrices, maybe performance boost from that?
    // TODO: implement blocked struct Matrix_Blocked


    start = clock();
    // do the work

    optimizedMatrixMul(m1, m2, &result, 2);


    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("cacheopt MM time used: %f seconds\n",cpu_time_used);
    prettyPrint(result);

    // cleanup allocated memory
    freeMatrix(&m1);
    freeMatrix(&m2);
    freeMatrix(&result);

    // TODO: diagram matrix größe - ausführungszeit
    // diagramm blocksize by cache optimierung
    // mehrfach ausführen, durchschnitt

    // TODO: 2er Potenzen meint benedikt (512, 1024, 2048) -> #define?
    // iwie davon überzeugen, dass multiplikation korrekt war (std. algo -> == "überladung")

    return 0;
}


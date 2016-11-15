#include <stdio.h>
#include <immintrin.h>
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
    m1 = createRandomizedMatrix(1000, 1000);
    m2 = createRandomizedMatrix(1000, 1000);
    Matrix result = allocMatrix(m1, m2);

    start = clock();
    // do the work
    standardMatrixMul(m1, m2, &result);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    //prettyPrint(result);
    printf("standard multiplication - time used: %f seconds\n",cpu_time_used);

    // cleanup allocated memory
    freeMatrix(&m1);
    freeMatrix(&m2);
    freeMatrix(&result);

    // TODO: diagram matrix größe - ausführungszeit
    // 2er Potenzen meint benedikt (512, 1024, 2048)
    // iwie davon überzeugen, dass multiplikation korrekt war (std. algo -> == "überladung")

    return 0;
}


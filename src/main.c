#include <stdio.h>
#include <immintrin.h>
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



    // TODO: implement createRandomizedMatrix()

    Matrix m1;
    m1.rowCount = 2;
    m1.columnCount = 2;
    m1.data = malloc(m1.rowCount * m1.columnCount * sizeof(float));

    setElementValue(&m1, 0, 0, 1.0);
    setElementValue(&m1, 0, 1, 2.0);
    setElementValue(&m1, 1, 0, 3.0);
    setElementValue(&m1, 1, 1, 4.0);

    // TODO: implement createRandomizedMatrix()

    Matrix m2;
    m2.rowCount = 2;
    m2.columnCount = 2;
    m2.data = malloc(m2.rowCount * m2.columnCount * sizeof(float));

    setElementValue(&m2, 0, 0, 2.0);
    setElementValue(&m2, 0, 1, 2.0);
    setElementValue(&m2, 1, 0, 2.0);
    setElementValue(&m2, 1, 1, 2.0);

    Matrix result = allocMatrix(m1, m2);
    standardMatrixMul(m1, m2, &result);
    prettyPrint(result);

    return 0;
}


# FSP-Matrixmultiplikation
Forschungspraktikum Parallele und Verteilte Systeme - TU Chemnitz

# Verwendung der BLAS-Bibliothek (ATLAS)
[math-atlas.sourceforge.net](http://math-atlas.sourceforge.net/)

# Installation
    sudo apt-get install libatlas-base-dev
    #include <cblas.h>
    Linker Flag: -lcblas -lm

# Kompilieren
    make -j4  # -j4 ist optional, ggf. noch -B, um garantiert zu rekompilieren

# Quellen
[Intel Intrinsics](https://software.intel.com/sites/landingpage/IntrinsicsGuide)

[AVX Intro](https://www.codeproject.com/articles/874396/crunching-numbers-with-avx-and-avx)

[Efficient matrix multiplication](http://stackoverflow.com/questions/18499971/efficient-4x4-matrix-multiplication-c-vs-assembly)





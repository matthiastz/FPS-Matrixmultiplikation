# FSP-Matrixmultiplikation
Forschungspraktikum Parallele und Verteilte Systeme - TU Chemnitz

# Verwendung der BLAS-Bibliothek (ATLAS)
[math-atlas.sourceforge.net](http://math-atlas.sourceforge.net/)

# Installation
    sudo apt-get install libatlas-base-dev
    #include <cblas.h>
    Flag: -lcblas

# Kompilieren
    make -j4  # -j4 ist optional, ggf. noch -B, um garantiert zu rekompilieren

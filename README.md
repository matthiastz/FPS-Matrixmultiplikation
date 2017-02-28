# FSP-Matrixmultiplikation
Forschungspraktikum Parallele und Verteilte Systeme - TU Chemnitz

Ziel ist es, verschiedene Verfahren für die Matrizenmultiplikation zu realisieren
und deren Performance messen und vergleichen zu können.
Die folgenden vier Verfahren stehen zur Verfügung:
- 1. Standardalgorithmus
- 2. Cache-optimiertes Verfahren (Loop tiling, BS)
- 3. Paralleles Verfahren (AVX Intrinsics)
- 4. BLAS-Bibliothek (ATLAS)

Es wurde eine Einschränkung auf quadratische Matrizen (N x N) vorgenommen.
Die Werte für Matrixgröße (N) und Blockgröße (BS) müssen *Zweierpotenzen* sein,
um Verfahren 2 und 3 fehlerfrei ausführen zu können.

    N >= 8
    BS <= N # sinnvoll ist natürlich BS < (N/2)
    AVX_VECTOR_SIZE = 8 # Größe des float-Vectors für Verfahren 3

# Verwendung der BLAS-Bibliothek (ATLAS)
[math-atlas.sourceforge.net](http://math-atlas.sourceforge.net/)

# Installation
    sudo apt-get install libatlas-base-dev
    #include <cblas.h>
    Linker Flag: -lcblas -lm

# Kompilieren
    make -j4  # -j4 ist optional, ggf. noch -B, um garantiert zu rekompilieren

# Ausführen
Die drei notwendigen Parameter (Matrixgröße, Blockgröße, Wiederholungsanzahl) werden direkt per Kommandozeile übergeben, 
sodass man schnell und einfach verschiedene Parameter ausprobieren kann.
Eine Ausführung mit N = 1024, BS = 8 und REPETITIONS = 5 sieht dann so aus: 

    ./a.out 1024 8 5

# Quellen
[Intel Intrinsics](https://software.intel.com/sites/landingpage/IntrinsicsGuide)

[AVX Intro](https://www.codeproject.com/articles/874396/crunching-numbers-with-avx-and-avx)

[Efficient matrix multiplication](http://stackoverflow.com/questions/18499971/efficient-4x4-matrix-multiplication-c-vs-assembly)





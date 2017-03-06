set style data histogram
set style histogram cluster gap 1

set style fill solid border rgb "black"
set auto x

# explizit yrange setzen -> mehr platz
set yrange [0:400]

set xlabel "Matrixgröße (N)"
set ylabel "benötigte Zeit (Sekunden)"


plot 'test1.dat' using 2:xtic(1) title col, \
        '' using 3:xtic(1) title col, \
        '' using 4:xtic(1) title col, \
        '' using 5:xtic(1) title col, \


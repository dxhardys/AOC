set terminal pngcairo enhanced font 'Arial,12' size 800,600
set output 'execution_times.png'

set style data histograms
set style fill solid border -1
set boxwidth 0.8
set grid ytics

set ylabel 'Temps (s)'
set xlabel 'Version'
set xtics rotate by -45
set title 'Comparaison des Temps dexécution par Version (Lower is Better)'

set datafile separator "\t"

$DATA << EOD
Baseline	291.28
OPT0 (Compilation)	59.22
OPT1 (Scanf)	54.78
OPT2 (No Qsort)	16.67
OPT3 (Ptr 1D)	12.83
EOD

plot $DATA using 2:xtic(1) title 'Temps (s)' linecolor rgb '#696969'

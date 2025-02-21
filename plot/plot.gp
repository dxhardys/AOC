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
Baseline	318.83
OPT0 (Compilation)	317.56
OPT1 (Invariant)	150.89
OPT2 (Fast-expo Shraudolph)	51.11
OPT2 (Fast-expo Taylor)	50.96
OPT3 (Loop-change)	19.50
OPT4 (Complexity-Reduction)	15.56
OPT5 (OpenMp 4 Threads)	7.58
OPT5 (OpenMp 8 Threads)	5.33
OPT6 (Inverse 4 Threads)	7.46
OPT6 (Inverse 8 Threads)	5.30
EOD

plot $DATA using 2:xtic(1) title 'Temps (s) ' linecolor rgb '#696969'

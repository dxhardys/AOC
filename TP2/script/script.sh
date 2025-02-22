rm -rf baseline RESULTS* opt1-compil opt2-scanf opt3-noqsort opt4-single-ptr
rm *.o


gcc -g -O2 baseline.c -o baseline.o
taskset -c 3 maqao OV -R1 xp=baseline -- ./baseline.o 50 2000 3000
cp -r baseline/RESULTS RESULTS_baseline

gcc -Ofast -g -Wall -lm -ftree-vectorize -funroll-loops -march=native -fopenmp -fno-omit-frame-pointer baseline.c -o baseline.o
taskset -c 3 maqao OV -R1 xp=opt0-compil -- ./baseline.o 50 2000 3000
cp -r opt0-compil/RESULTS RESULTS_opt0-compil

gcc -Ofast -g -Wall -lm -ftree-vectorize -funroll-loops -march=native -fopenmp -fno-omit-frame-pointer opt1.c -o opt1.o
taskset -c 3 maqao OV -R1 xp=opt1-scanf -- ./opt1.o 50 2000 3000
cp -r opt1-scanf/RESULTS RESULTS_opt1-scanf

gcc -Ofast -g -Wall -lm -ftree-vectorize -funroll-loops -march=native -fopenmp -fno-omit-frame-pointer opt2.c -o opt2.o
taskset -c 3 maqao OV -R1 xp=opt2-noqsort -- ./opt2.o 50 2000 3000
cp -r opt2-noqsort/RESULTS RESULTS_opt2-noqsort

gcc -Ofast -g -Wall -lm -ftree-vectorize -funroll-loops -march=native -fopenmp -fno-omit-frame-pointer opt3.c -o opt3.o
taskset -c 3 maqao OV -R1 xp=opt3-single-ptr -- ./opt3.o 50 2000 3000
cp -r opt3-single-ptr/RESULTS RESULTS_opt3-single-ptr


rm *.o

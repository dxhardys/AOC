rm -rf baseline RESULTS* opt1-compil_clang opt2-scanf_clang opt3-noqsort_clang opt4-single-ptr_clang
rm -f *.o


clang -g -O2 baseline.c -o baseline
taskset -c 3 maqao OV -R1 xp=baseline_clang -- ./baseline 50 2000 3000
cp -r baseline_clang/RESULTS RESULTS_baseline_clang

clang -Ofast -g -Wall -march=native -fno-omit-frame-pointer -lm opt0-compil.c -o opt0-compil
taskset -c 3 maqao OV -R1 xp=opt0-compil_clang -- ./opt0-compil 50 2000 3000
cp -r opt0-compil_clang/RESULTS RESULTS_opt0-compil_clang

clang -Ofast -g -Wall -march=native -fno-omit-frame-pointer -lm opt1.c -o opt1
taskset -c 3 maqao OV -R1 xp=opt1-scanf_clang -- ./opt1 50 2000 3000
cp -r opt1-scanf_clang/RESULTS RESULTS_opt1-scanf_clang

clang -Ofast -g -Wall -march=native -fno-omit-frame-pointer -lm opt2.c -o opt2
taskset -c 3 maqao OV -R1 xp=opt2-noqsort_clang -- ./opt2 50 2000 3000
cp -r opt2-noqsort_clang/RESULTS RESULTS_opt2-noqsort_clang

clang -Ofast -g -Wall -march=native -fno-omit-frame-pointer -lm opt3.c -o opt3
taskset -c 3 maqao OV -R1 xp=opt3-single-ptr_clang -- ./opt3 50 2000 3000
cp -r opt3-single-ptr_clang/RESULTS RESULTS_opt3-single-ptr_clang

rm -f *.o

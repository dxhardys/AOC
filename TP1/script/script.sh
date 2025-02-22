rm -rf baseline*
rm -rf opt*
rm *.txt

make clean
make OPT=NOOPT OPTFLAGS="-O2 -g -Wall -lm" CFLAGS="-O2 -g -Wall -lm"
taskset -c 3 maqao OV -R1 xp=baseline -- ./measure 2000 100 100000
cp -r baseline/RESULTS RESULTS_baseline
./check 2000 baseline.txt

make clean
make OPT=NOOPT
taskset -c 3 maqao OV -R1 xp=opt0-compilation -- ./measure 2000 100 100000
cp -r opt0-compilation/RESULTS RESULTS_opt0-compilation

make clean 
make OPT=OPT1
taskset -c 3 maqao OV -R1 xp=opt1_invariant -- ./measure 2000 100 100000
cp -r opt1_invariant/RESULTS RESULTS_opt1_invariant
./check 2000 opt1_invariant.txt

make clean 
make OPT=OPT2
taskset -c 3 maqao OV -R1 xp=opt2_fast_expo_taylor -- ./measure 2000 100 100000
cp -r opt2_fast_expo_taylor/RESULTS RESULTS_opt2_fast_expo_taylor
./check 2000 opt2_taylor.txt

make clean 
make OPT=OPT22
taskset -c 3 maqao OV -R1 xp=opt2_fast_expo_shraudolph -- ./measure 2000 100 100000
cp -r opt2_fast_expo_shraudolph/RESULTS RESULTS_opt2_fast_expo_shraudolph
./check 2000 opt2_shraudolph.txt


make clean 
make OPT=OPT3
taskset -c 3 maqao OV -R1 xp=opt3_loop_interchange -- ./measure 2000 100 100000
cp -r opt3_loop_interchange/RESULTS RESULTS_opt3_loop_interchange
./check 2000 opt3_loop_interchange.txt


make clean 
make OPT=OPT4
taskset -c 3 maqao OV -R1 xp=opt4_flow -- ./measure 2000 100 100000
cp -r opt4_flow/RESULTS RESULTS_opt4_flow
./check 2000 opt4_flow.txt


make clean 
make OPT=OPT5
export OMP_NUM_THREADS=4
maqao OV -R1 xp=opt5_omp_4 -- ./measure 2000 100 100000
cp -r opt5_omp_4/RESULTS RESULTS_opt5_omp_4
./check 2000 opt5_omp_4.txt

make clean 
make OPT=OPT5
export OMP_NUM_THREADS=8
maqao OV -R1 xp=opt5_omp8 -- ./measure 2000 100 100000
cp -r opt5_omp8/RESULTS RESULTS_opt5_omp8
./check 2000 opt5_omp8.txt

make clean 
make OPT=OPT6
export OMP_NUM_THREADS=4
maqao OV -R1 xp=opt6_inverse_omp4 -- ./measure 2000 100 100000
cp -r opt6_inverse_omp4/RESULTS RESULTS_opt6_inverse_omp4
./check 2000 opt6_inverse_omp4.txt

make clean 
make OPT=OPT6
export OMP_NUM_THREADS=8
maqao OV -R1 xp=opt6_inverse_omp8 -- ./measure 2000 100 100000
cp -r opt6_inverse_omp8/RESULTS RESULTS_opt6_inverse_omp8
./check 2000 opt6_inverse_omp8.txt



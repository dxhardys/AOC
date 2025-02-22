rm -rf opt5_* opt6_*
rm -rf opt5*.txt opt6*.txt
rm -rf RESULTS_opt5* RESULTS_opt6*
make clean 
make OPT=OPT5
export OMP_NUM_THREADS=4
export OMP_PLACES=cores
export OMP_PROC_BIND=close
maqao OV -R1 xp=opt5_omp_4 -- ./measure 2000 100 100000
cp -r opt5_omp_4/RESULTS RESULTS_opt5_omp_4
./check 2000 opt5_omp_4.txt

make clean 
make OPT=OPT5
export OMP_NUM_THREADS=8
export OMP_PLACES=cores
export OMP_PROC_BIND=close
maqao OV -R1 xp=opt5_omp8 -- ./measure 2000 100 100000
cp -r opt5_omp8/RESULTS RESULTS_opt5_omp8
./check 2000 opt5_omp8.txt

make clean 
make OPT=OPT6
export OMP_NUM_THREADS=4
export OMP_PLACES=cores
export OMP_PROC_BIND=close
maqao OV -R1 xp=opt6_inverse_omp4 -- ./measure 2000 100 100000
cp -r opt6_inverse_omp4/RESULTS RESULTS_opt6_inverse_omp4
./check 2000 opt6_inverse_omp4.txt

make clean 
make OPT=OPT6
export OMP_NUM_THREADS=8
export OMP_PLACES=cores
export OMP_PROC_BIND=close
maqao OV -R1 xp=opt6_inverse_omp8 -- ./measure 2000 100 100000
cp -r opt6_inverse_omp8/RESULTS RESULTS_opt6_inverse_omp8
./check 2000 opt6_inverse_omp8.txt


#Longer Runtime : 


make clean 
make OPT=OPT5
export OMP_NUM_THREADS=4
export OMP_PLACES=cores
export OMP_PROC_BIND=close
maqao OV -R1 xp=opt5_omp_4_longerruntime -- ./measure 2000 100 500000
cp -r opt5_omp_4_longerruntime/RESULTS RESULTS_opt5_omp_4_longerruntime
./check 2000 opt5_omp_4_longerruntime.txt

make clean 
make OPT=OPT5
export OMP_NUM_THREADS=8
export OMP_PLACES=cores
export OMP_PROC_BIND=close
maqao OV -R1 xp=opt5_omp8_longerruntime -- ./measure 2000 100 500000
cp -r opt5_omp8_longerruntime/RESULTS RESULTS_opt5_omp8_longerruntime
./check 2000 opt5_omp8_longerruntime.txt

make clean 
make OPT=OPT6
export OMP_NUM_THREADS=4
export OMP_PLACES=cores
export OMP_PROC_BIND=close
maqao OV -R1 xp=opt6_inverse_omp4_longer_runtime -- ./measure 2000 100 500000
cp -r opt6_inverse_omp4_longer_runtime/RESULTS RESULTS_opt6_inverse_omp4_longerruntime
./check 2000 opt6_inverse_omp4_longer_runtime.txt

make clean 
make OPT=OPT6
export OMP_NUM_THREADS=8
export OMP_PLACES=cores
export OMP_PROC_BIND=close
maqao OV -R1 xp=opt6_inverse_omp8_longerruntime -- ./measure 2000 100 500000
cp -r opt6_inverse_omp8_longerruntime/RESULTS RESULTS_opt6_inverse_omp8_longerruntime
./check 2000 opt6_inverse_omp8_longerruntime.txt


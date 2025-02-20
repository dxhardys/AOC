#include <math.h>
#ifdef OPT1

/* Removing of store to load dependency (array ref replaced by scalar) */
void kernel ( unsigned n , double a[n] ,const float b [n] ,const float c[n] ,const float d [12]) {
   unsigned k, i ;
   for(k=0; k<12; k++){
      for(i=0 ; i<n ; i++){
         if(b[i] >= 0.0 && b[i] < 1.0){
           a [i] += exp ( b[i] + d[k]) / c [i];
         } else if (b[i] >= 1.0) {
            a[i] += (b[i] * d[k]) / c[i] ;
         }
      }
   }

}

#elif defined OPT2

#include <string.h> // memset

/* ijk -> ikj permutation to make stride 1 the innermost loop */
void kernel (unsigned n, float a[n][n], float b[n][n], float c[n][n]) {
   int i, j, k;

   memset (c, 0, n * n * sizeof c[0][0]);

   for (i=0; i<n; i++)
      for (k=0; k<n; k++)
         for (j=0; j<n; j++)
            c[i][j] += a[i][k] * b[k][j];
}

#else

/* original */
void kernel(unsigned n, double a[n], const float b[n] ,const float c[n] ,const float d [12]) {
   unsigned k, i ;

   for(k=0; k<12; k++){
      // take out const from loop
      const float dk = d[k] ;
      for(i=0 ; i<n ; i++){
         if(b[i] >= 0.0 && b[i] < 1.0){
           a [i] += exp ( b[i] + dk) / c [i];
         } else if (b[i] >= 1.0) {
            a[i] += (b[i] * dk) / c[i] ;
         }
      }
   }
}
#endif

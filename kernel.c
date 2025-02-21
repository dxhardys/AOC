#include <math.h>

// Taylor approximation
static inline float fast_exp(float x) {
    return 1.0f + x * (1.0f + x * (0.5f + x * (1.0f / 6.0f)));
}

// Schraudolph approximation
// not very accurate for x big
// but since in our kernel we only compute
static inline float fast_exp2(float x) {
    union { float f; int i; } v;
    v.i = (int)(12102203.0f * x + 1065353216.0f);
    return v.f;
}

#ifdef OPT1

void kernel(unsigned n, double* restrict a, const float* restrict b ,const float* restrict c ,const float* restrict d) {
   unsigned k, i ;

   for(k=0; k<12; k++){
      // take out const from loop
      const float d_k = d[k] ;
      for(i=0 ; i<n ; i++){

         if(b[i] >= 0.0 && b[i] < 1.0){
           a [i] += exp(b[i] + d_k) / c [i];
         } else if (b[i] >= 1.0) {
            a[i] += (b[i] * d_k) / c[i] ;
         }
      }
   }
}

#elif defined OPT2

#include <string.h> // memset

void kernel(unsigned n, double* restrict a, const float* restrict b ,const float* restrict c ,const float* restrict d) {
   unsigned k, i ;

   for(k=0; k<12; k++){
      // take out const from loop
      const float d_k = d[k] ;
      for(i=0 ; i<n ; i++){

         if(b[i] >= 0.0 && b[i] < 1.0){
           a [i] += fast_exp(b[i] + d_k) / c [i];
         } else if (b[i] >= 1.0) {
            a[i] += (b[i] * d_k) / c[i] ;
         }
      }
   }
}
#elif defined OPT22

void kernel(unsigned n, double* restrict a, const float* restrict b ,const float* restrict c ,const float* restrict d) {
   unsigned k, i ;

   for(k=0; k<12; k++){
      // take out const from loop
      const float d_k = d[k] ;
      for(i=0 ; i<n ; i++){

         if(b[i] >= 0.0 && b[i] < 1.0){
           a [i] += fast_exp2(b[i] + d_k) / c [i];
         } else if (b[i] >= 1.0) {
            a[i] += (b[i] * d_k) / c[i] ;
         }
      }
   }
}

#elif defined OPT3
// I will perform my optims in this kernel
/* original */
void kernel(unsigned n, double* restrict a, const float* restrict b ,const float* restrict c ,const float* restrict d) {
   unsigned k, i ;


for(i=0 ; i<n ; i++){
      const float b_i = b[i] ;
      const float c_i = c[i] ;
      double sum = 0.0 ;
      for(k=0; k<12; k++){
         if(b_i >= 0.0 && b_i < 1.0){
           sum += fast_exp2(b_i + d[k]) / c_i;
         } else if (b_i >= 1.0) {
            sum += (b_i * d[k]) / c_i ;
         }
      }
      a[i] = sum ;
   }
}
#elif defined OPT4 
void kernel(unsigned n, double* restrict a, const float* restrict b ,const float* restrict c ,const float* restrict d) {
   unsigned k, i ;

for (i = 0; i < n; i++) {
        const float b_i = b[i];
        const float c_i = c[i];
        double sum = 0.0;

        const float mask1 = (b_i >= 0.0f && b_i < 1.0f) ? 1.0f : 0.0f;
        const float mask2 = (b_i >= 1.0f) ? 1.0f : 0.0f;

        for (k = 0; k < 12; k++) {
            sum += mask1 * (fast_exp2(b_i + d[k]) / c_i) + 
                   mask2 * ((b_i * d[k]) / c_i);
        }

        a[i] = sum;
    }
}

#elif defined OPT5
void kernel(unsigned n, double* restrict a, const float* restrict b ,const float* restrict c ,const float* restrict d) {
   unsigned k, i ;

#pragma omp parallel for
for (i = 0; i < n; i++) {
        const float b_i = b[i];
        const float c_i = c[i];
        double sum = 0.0;

        const float mask1 = (b_i >= 0.0f && b_i < 1.0f) ? 1.0f : 0.0f;
        const float mask2 = (b_i >= 1.0f) ? 1.0f : 0.0f;

        for (k = 0; k < 12; k++) {
            sum += mask1 * (fast_exp2(b_i + d[k]) / c_i) + 
                   mask2 * ((b_i * d[k]) / c_i);
        }

        a[i] = sum;
    }
}

#elif defined OPT6
void kernel(unsigned n, double* restrict a, const float* restrict b ,const float* restrict c ,const float* restrict d) {
   unsigned k, i ;
#pragma omp parallel for
for (i = 0; i < n; i++) {
        const float b_i = b[i];
        const float c_i = c[i];
        double sum = 0.0;

        const float mask1 = (b_i >= 0.0f && b_i < 1.0f) ? 1.0f : 0.0f;
        const float mask2 = (b_i >= 1.0f) ? 1.0f : 0.0f;
        const float c_i_inv = 1.0 / c_i ; 
        for (k = 0; k < 12; k++) {
            sum += mask1 * (fast_exp2(b_i + d[k]) * c_i_inv) + 
                   mask2 * ((b_i * d[k]) * c_i_inv);
        }

        a[i] = sum;
    }
}


#else



// I will perform my optims in this kernel
/* original */
void kernel(unsigned n, double* restrict a, const float* restrict b ,const float* restrict c ,const float* restrict d) {
   unsigned k, i ;

   for(k=0; k<12; k++){
      for(i=0 ; i<n ; i++){
         if(b[i] >= 0.0 && b[i] < 1.0){
           a[i] += exp(b[i] + d[k]) / c[i];
         } else if (b[i] >= 1.0) {
            a[i] += (b[i] * d[k]) / c[i] ;
         }
      }
   }
}

#endif

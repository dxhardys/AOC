#include <stdio.h>
#include <stdlib.h> // atoi, qsort
#include <stdint.h>
#include <time.h> // nanosleep

#define NB_METAS 5

extern uint64_t rdtsc ();

// TODO: adjust for each kernel
extern void kernel(unsigned n , double a[n] ,
const float b[n] ,
const float c[n] ,
const float d[12]);

// TODO: adjust for each kernel
static void init_array_double(unsigned n , double* a) {
   int i;

   for (i=0; i<n; i++){
         a[i] = (double) rand() / RAND_MAX;
   }
}

static void init_array_float(unsigned n , float* a) {
   int i;

   for (i=0; i<n; i++){
         a[i] = (float) rand() / RAND_MAX;
   }
}

static int cmp_uint64 (const void *a, const void *b) {
   const uint64_t va = *((uint64_t *) a);
   const uint64_t vb = *((uint64_t *) b);

   if (va < vb) return -1;
   if (va > vb) return 1;
   return 0;
}

int main (int argc, char *argv[]) {

   /* check command line arguments */
   if (argc != 3) {
      fprintf (stderr, "Usage: %s <size> <nb measures>\n", argv[0]);
      return EXIT_FAILURE;
   }

   /* get command line arguments */
   const unsigned size = atoi (argv[1]); /* problem size */
   const unsigned repm = atoi (argv[2]); /* number of repetitions during measurement */

   uint64_t (*tdiff)[NB_METAS] = malloc (repm * sizeof tdiff[0]);

   unsigned m;
   for (m=0; m<NB_METAS; m++) {
      printf ("Metarepetition %u/%d: running %u instances\n", m+1, NB_METAS, repm);

      unsigned i;

      /* allocate arrays. TODO: adjust for each kernel */
      double *a = (double*)malloc(size * sizeof(double));
      float *b  = (float*)malloc (size * sizeof(float));
      float *c  = (float*)malloc (size * sizeof(float));
      float *d  = (float*)malloc(12 * sizeof(float) ) ; 

      /* init arrays */
      srand(0);
      init_array_double (size, a);
      init_array_float (size, b);
      init_array_float (size, c) ;
      init_array_float(12, d) ;

      // No warmup, measure individual instances
      for (i=0; i<repm; i++) {
         const uint64_t t1 = rdtsc();
         kernel (size, a, b, c, d);
         const uint64_t t2 = rdtsc();
         tdiff[i][m] = t2 - t1;
      }

      /* free arrays. TODO: adjust for each kernel */
      free (a);
      free (b);
      free (c);
      free(d) ;

      /* Let some time for processor to cool down and allow capturing stability via next meta-repetitions */
      const struct timespec two_seconds = { .tv_sec = 2, .tv_nsec = 0 };
      nanosleep (&two_seconds, NULL);
   }

   const unsigned nb_inner_iters = size * size * size; // TODO adjust for each kernel
   int i;
   for (i=0; i<repm; i++) {
      printf ("Instance %u/%u\n", i+1, repm);

      qsort (tdiff[i], NB_METAS, sizeof tdiff[i][0], cmp_uint64);

      // Minimum value: should be at least 2000 RDTSC-cycles
      const uint64_t min = tdiff[i][0];
      printf ("MIN %lu RDTSC-cycles (%.2f per inner-iter)\n",
              min, (float) min / nb_inner_iters);

      // Median value
      const uint64_t med = tdiff[i][NB_METAS/2];
      if (med < 500) {
         printf ("Warning: median time is less than 500 RDTSC-cycles. Accurary is limited for that instance\n");
      }
      printf ("MED %lu RDTSC-cycles (%.2f per inner-iter)\n",
              med, (float) med / nb_inner_iters);

      // Stability: (med-min)/min
      const float stab = (med - min) * 100.0f / min;
      if (stab >= 10)
         printf ("BAD STABILITY: %.2f %%\n", stab);
      else if (stab >= 5)
         printf ("AVERAGE STABILITY: %.2f %%\n", stab);
      else
         printf ("GOOD STABILITY: %.2f %%\n", stab);
   }

   return EXIT_SUCCESS;
}

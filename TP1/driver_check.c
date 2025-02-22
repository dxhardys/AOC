#include <stdio.h>
#include <stdlib.h> // atoi
#include <stdint.h>

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

// TODO: adjust for each kernel
static void print_array (int n, float a[n], const char *output_file_name) {
   int i ;

   FILE *fp = fopen (output_file_name, "w");
   if (fp == NULL) {
      fprintf (stderr, "Cannot write to %s\n", output_file_name);
      return;
   }

   for (i=0; i<n; i++)
         fprintf (fp, "%f\n", a[i]);

   fclose (fp);
}

int main (int argc, char *argv[]) {
   /* check command line arguments */
   if (argc != 3) {
      fprintf (stderr, "Usage: %s <size> <output file name>\n", argv[0]);
      return EXIT_FAILURE;
   }

   /* get command line arguments */
   const unsigned size = atoi (argv[1]); /* problem size */
   const char *output_file_name = argv[2];

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

   /* print output */
   kernel (size, a, b, c, d);
   print_array (size, c, output_file_name);

   /* free arrays. TODO: adjust for each kernel */
   free (a);
   free (b);
   free (c);

   return EXIT_SUCCESS;
}

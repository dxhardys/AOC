/* Author: Emmanuel OSERET, University of Versailles Saint-Quentin-en-Yvelines, France
 * This program pseudo-randomly generates pairs of values (v1,v2) in a 2D grid
 * and then computes and prints maximum (v1) and maximum (v2) and x-y position
 * Execution:
 * - Usage: ./exe <nb repetitions> <nb points X> <nb points Y>
 * - Nb repetitions: number of times the experiment is repeated (from file loading to memory release), allows to increase runtime for more precise sampling-based profiling
 * - Nb points X: input size along X
 * - Nb points Y: input size along Y
 * - Recommended: with the baseline implementation, good starting point is 2000 x 3000
 */

#include <stdio.h>  // printf, fopen etc.
#include <stdlib.h> // atoi, qsort, malloc etc.

// Abstract values, entry for a values_array_t array
typedef struct {
   float v1, v2;
} value_t;

// Dynamic array of values
typedef struct {
   unsigned nx, ny;  // number of values along X, Y
   value_t *entries; // array of pointers to values (flat)
} value_grid_t;

// Structure to relate values and position in the grid
typedef struct {
   unsigned x, y; // position in the 2D grid
   float v1, v2;
} pos_val_t;

// Dynamic array of pos_val_t entries
typedef struct {
   unsigned nx, ny; // number of values along X, Y
   pos_val_t *entries; // array of pointers to pos_val_t structures (flat)
} pos_val_grid_t;

size_t sum_bytes;       // Cumulated sum of allocated bytes (malloc, realloc)

// Pseudo-randomly generates 'n' values and writes them to a text file
int generate_random_values (const char *file_name, unsigned nx, unsigned ny)
{
   printf ("Generate %u x %u values and dump them to %s...\n", nx, ny, file_name);

   // Open/create output file
   FILE *fp = fopen (file_name, "w");
   if (!fp) {
      fprintf (stderr, "Cannot write %s\n", file_name);
      return -1;
   }

   // Save nx and ny on the first line
   if (fprintf (fp, "%u %u\n", nx, ny) <= 0)
      return -2;

   // Generate values (one per line)
   unsigned i, j;
   for (i=0; i<nx; i++) {
      for (j=0; j<ny; j++) {
         const float v1 = (float) rand() / RAND_MAX;
         const float v2 = (float) rand() / RAND_MAX;

         if (fprintf (fp, "%lf %lf\n", v1, v2) <= 0)
            return -2;
      }
   }

   // Close output file
   fclose (fp);

   return 0;
}
// Loads values from a file written by generate_random_values() to the grid
int load_values(const char *file_name, value_grid_t *val_grid) {
    printf("Load values from %s...\n", file_name);

    // Open input file
    FILE *fp = fopen(file_name, "r");
    if (!fp) {
        fprintf(stderr, "Cannot read %s\n", file_name);
        return -1;
    }

    // Read grid size (first line)
    if (fscanf(fp, "%u %u", &val_grid->nx, &val_grid->ny) != 2) {
        fprintf(stderr, "Failed to parse the first line from the input file\n");
        fclose(fp);
        return 1;
    }

    unsigned total_values = val_grid->nx * val_grid->ny;

    val_grid->entries = malloc(total_values * sizeof(value_t));
    if (!val_grid->entries) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(fp);
        return -1;
    }
    sum_bytes += total_values * sizeof(value_t);

    for (unsigned i = 0; i < total_values; i++) {
        if (fscanf(fp, "%f %f", &val_grid->entries[i].v1, &val_grid->entries[i].v2) != 2) {
            fprintf(stderr, "Failed to parse a line from the input file\n");
            free(val_grid->entries);
            fclose(fp);
            return 1;
        }
    }

    // Close file
    fclose(fp);

    return 0;
}


// Relate pairs to coordinates
void load_positions(value_grid_t src, pos_val_grid_t *dst) {
    dst->nx = src.nx;
    dst->ny = src.ny;
    dst->entries = malloc(src.nx * src.ny * sizeof(pos_val_t));
    if (!dst->entries) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    sum_bytes += src.nx * src.ny * sizeof(pos_val_t);

    for (unsigned i = 0; i < src.nx; i++) {
        for (unsigned j = 0; j < src.ny; j++) {
            unsigned idx = i * src.ny + j;
            dst->entries[idx].x = i;
            dst->entries[idx].y = j;
            dst->entries[idx].v1 = src.entries[idx].v1;
            dst->entries[idx].v2 = src.entries[idx].v2;
        }
    }
}


// Computes maximum v1+v2 (and save related points) from a row of pairs
pos_val_t *find_max_v1(const pos_val_grid_t *pv_grid)
{
    printf("Compute maximum v1...\n");

    pos_val_t *max_val = &pv_grid->entries[0];  
    for (unsigned i = 1; i < pv_grid->nx * pv_grid->ny; i++) {
        if (pv_grid->entries[i].v1 > max_val->v1) {
            max_val = &pv_grid->entries[i];  
        }
    }

    return max_val;
}



// Computes maximum v1+v2 (and save related points) from a row of pairs
pos_val_t *find_max_v2(const pos_val_grid_t *pv_grid)
{
    printf("Compute maximum v2...\n");

    pos_val_t *max_val = &pv_grid->entries[0];  
    for (unsigned i = 1; i < pv_grid->nx * pv_grid->ny; i++) {
        if (pv_grid->entries[i].v2 > max_val->v2) {
            max_val = &pv_grid->entries[i];  
        }
    }

    return max_val;
}



// Frees memory that was allocated to save distances
void free_pos_val_grid(pos_val_grid_t pv_grid) {
    printf("Free memory allocated for positions+values (%u x %u entries)...\n",
           pv_grid.nx, pv_grid.ny);

    free(pv_grid.entries);
    sum_bytes -= pv_grid.nx * pv_grid.ny * sizeof(pos_val_t);
}


// Frees memory that was allocated to save points/coordinates
void free_value_grid(value_grid_t val_grid) {
    printf("Free memory allocated for coordinates (%u x %u entries)...\n",
           val_grid.nx, val_grid.ny);

    free(val_grid.entries);
    sum_bytes -= val_grid.nx * val_grid.ny * sizeof(value_t);
}


// Program entry point: CF comments on top of this file + README
int main (int argc, char *argv[])
{
   // Check arguments number
   if (argc < 4) {
      fprintf (stderr, "Usage: %s <nb repetitions> <nb points X> <nb points Y>\n", argv[0]);
      return EXIT_FAILURE;
   }

   // Read arguments from command line
   unsigned nrep = (unsigned) atoi (argv[1]);
   unsigned nx   = (unsigned) atoi (argv[2]);
   unsigned ny   = (unsigned) atoi (argv[3]);

   // Generate points and save them to a text file named "values.txt"
   const char *input_file_name = "values.txt";
   if (generate_random_values (input_file_name, nx, ny) != 0) {
      fprintf (stderr, "Failed to write %u x %u coordinates to %s\n",
               nx, ny, input_file_name);
      return EXIT_FAILURE;
   }

   sum_bytes = 0;

   // Main part: repeated nrep times
   unsigned r;
   for (r=0; r<nrep; r++) {
      value_grid_t value_grid;
      pos_val_grid_t pos_val_grid;

      // Load coordinates from disk to memory
      if (load_values (input_file_name, &value_grid) != 0) {
         fprintf (stderr, "Failed to load coordinates\n");
         return EXIT_FAILURE;
      }

      // Relate pairs to coordinates
      load_positions (value_grid, &pos_val_grid);

      // Compute maximum (v1 and v2)
      const pos_val_t *pos_v1_max = find_max_v1 (&pos_val_grid);
      const pos_val_t *pos_v2_max = find_max_v2 (&pos_val_grid);

      // Print maximum (v1 and v2)
      printf ("Max v1: x=%u, y=%u, v1=%f\n",
              pos_v1_max->x, pos_v1_max->y, pos_v1_max->v1);
      printf ("Max v2: x=%u, y=%u, v2=%f\n",
              pos_v2_max->x, pos_v2_max->y, pos_v2_max->v2);

      // Free allocated memory
      free_pos_val_grid (pos_val_grid);
      free_value_grid (value_grid);
   }

   // Delete text file
   remove (input_file_name);

   return EXIT_SUCCESS;
}

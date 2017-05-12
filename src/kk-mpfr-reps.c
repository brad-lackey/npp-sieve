//
//  kk-mpfr-reps.c
//  
//  Created by Brad Lackey on 3/22/17.
//  Modified by Stephen Jordan on 5/12/17
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpfr.h>

int precision;
unsigned long seed = 0;

void populateArray(mpfr_t *array, unsigned int sz, gmp_randstate_t *rand);
mpfr_t *initializeBuffer(unsigned int sz);
void clearBuffer(mpz_t *array, unsigned int sz);

int main(int argc, char **argv){
  int a,b;
  unsigned int array_size, buffer_size;
  mpfr_t *array;
  mpfr_t *buffer;
  mpfr_t max;
  clock_t beginning, end; //for timing
  double time_spent; //for timing
  unsigned int N, t, reps;
  gmp_randstate_t rand;

  if( argc != 5 ) {
    printf("Usage: karmarkar-karp N BITS SEED REPS\n");
    return 0;
  }
  beginning = clock();
  N = atoi(argv[1]);
  precision = atoi(argv[2]);
  seed = atoi(argv[3]);
  reps = atoi(argv[4]);

  printf("N = %i\tbits = %i\tseed = %i\treps = %i\n", N, precision, seed, reps);
  buffer_size = N/2;
  buffer = initializeBuffer(buffer_size);
  mpfr_init2(max, precision);
  array = initializeBuffer(N);

  gmp_randinit_mt(rand);
  gmp_randseed_ui(rand, seed);

  for(t = 0; t < reps; t++) {
    array_size = N;
    clearBuffer(buffer, buffer_size);
    populateArray(array, N, &rand);

    while( array_size >= 2){
      mpfr_sub(buffer[0], array[array_size-1], array[array_size-2], MPFR_RNDN);
      mpfr_set(max, buffer[0], MPFR_RNDN);
      //printf("(%d-%d,%d) %zu\n", array_size-1, array_size-2, 0, mpz_sizeinbase(buffer[0],2));
      a = array_size-4;
      b = 1;
      while ((a >= 0) && (b < buffer_size) && (mpfr_cmp(max,array[a+1]) < 0)) {
        mpfr_sub(buffer[b], array[a+1], array[a], MPFR_RNDN);
        //printf("(%d-%d,%d) %zu %zu\n", a+1, a, b, mpz_sizeinbase(buffer[b],2), mpz_sizeinbase(max,2));
        if ( mpfr_cmp(max, buffer[b]) < 0 ) mpfr_set(max, buffer[b], MPFR_RNDN);
        b += 1;
        a -= 2;
      }
      //printf("Array size: %d, Array segment: %d, buffer size: %d\n", array_size, a+2, b);
      for (a+=2,--b; b>=0; ++a, --b){
        mpfr_set(array[a],buffer[b], MPFR_RNDN);
      }
      array_size = a;
      qsort(array, array_size, sizeof(mpfr_t), mpfr_cmp);
     //printf("\n\n");
    }  
    //printf("residual bits: %zu\n", mpz_sizeinbase(buffer[0],2));
    //printf("value: ");
    //mpfr_printf("%Rf\n",array[0]);
    mpfr_out_str(stdout, 10, 10, buffer[0], MPFR_RNDN); printf("\n");
    fflush(stdout);
  }
  gmp_randclear(rand);
  end = clock();
  time_spent = (double)(end-beginning)/CLOCKS_PER_SEC;
  printf("Walltime = %f\n", time_spent);
  return 0;
}

void clearBuffer(mpz_t *array, unsigned int sz) {
  int i;
  for(i = 0; i < sz; i++) mpz_set_str(array[i], "0", 10);
}

mpfr_t *initializeBuffer(unsigned int sz){
  int i;
  mpfr_t *array = calloc(sz, sizeof(mpfr_t));
  for(i=0; i<sz; ++i) mpfr_init2(array[i], precision);
  return array;
}

void populateArray(mpfr_t *array, unsigned int sz, gmp_randstate_t *rand) {
  int i;
  mpfr_t temp;
  mpfr_init2(temp,precision);
  for (i=0; i<sz; ++i) {
    mpfr_urandomb(temp,rand);
    mpfr_mul_2ui(array[i], temp, 1ul, MPFR_RNDN);
  }
  qsort(array, sz, sizeof(mpfr_t), mpfr_cmp);
}



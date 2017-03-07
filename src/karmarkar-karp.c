//
//  karmarker-karp.c
//  
//
//  Created by Brad Lackey on 1/16/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gmp.h>

int precision;
//const int size = 27;

mpz_t *populateArray(unsigned int sz, unsigned long int seed);
mpz_t *initializeBuffer(unsigned int sz);


int main(int argc, char **argv){
  int a,b;
  unsigned int array_size, buffer_size;
  mpz_t *array;
  mpz_t *buffer;
  mpz_t max;
  clock_t beginning, end; //for timing
  double time_spent; //for timing
  unsigned long int seed; //for rng
  if(argc != 4) {
    printf("Usage: karmarkar-karp N BITS seed\n");
    return 0;
  }
  beginning = clock();
  array_size = atoi(argv[1]);
  precision = atoi(argv[2]);
  seed = atoi(argv[3]);

  printf("seed = %lu\n", seed);
  printf("N = %u\n", array_size);
  printf("bits = %d\n", precision);
  buffer_size = array_size/3;
  array = populateArray(array_size, seed);
  buffer = initializeBuffer(buffer_size);
  mpz_init2(max, precision);

  
  while( array_size >= 2){
    mpz_sub(buffer[0], array[array_size-1], array[array_size-2]);
    mpz_set(max, buffer[0]);
//    printf("(%d-%d,%d) %zu\n", array_size-1, array_size-2, 0, mpz_sizeinbase(buffer[0],2));
    a = array_size-4;
    b = 1;
    while ((a >= 0) && (b < buffer_size) && (mpz_cmp(max,array[a+1]) < 0)) {
      mpz_sub(buffer[b], array[a+1], array[a]);
//      printf("(%d-%d,%d) %zu %zu\n", a+1, a, b, mpz_sizeinbase(buffer[b],2), mpz_sizeinbase(max,2));
     if ( mpz_cmp(max, buffer[b]) < 0 )
        mpz_set(max, buffer[b]);
      b += 1;
      a -= 2;
    }
//    printf("Array size: %d, Array segment: %d, buffer size: %d\n", array_size, a+2, b);
    for (a+=2,--b; b>=0;)
      mpz_set(array[a++],buffer[b--]);
    array_size = a;
    qsort(array, array_size, sizeof(mpz_t), mpz_cmp);
//    printf("\n\n");
  }
  
  printf("residual bits: %zu\n", mpz_sizeinbase(buffer[0],2));
  printf("value: ");
  mpz_out_str(stdout, 10, array[0]); printf("\n");
  end = clock();
  time_spent = (double)(end-beginning)/CLOCKS_PER_SEC;
  printf("Walltime = %f\n", time_spent);
  return 0;
}

mpz_t *initializeBuffer(unsigned int sz){
  int i;
  mpz_t *array = calloc(sz, sizeof(mpz_t));

  for(i=0; i<sz; ++i) mpz_init2(array[i], precision);
  
  return array;
}

mpz_t *populateArray(unsigned int sz, unsigned long int seed){
  int i;
  gmp_randstate_t rand;
  mpz_t *array = calloc(sz, sizeof(mpz_t));
  
  for(i=0; i<sz; ++i) mpz_init2(array[i], precision);
  
  gmp_randinit_mt(rand);
  gmp_randseed_ui(rand, seed);
  for (i=0; i<sz; ++i)
    mpz_urandomb(array[i],rand,precision);

  gmp_randclear(rand);
  qsort(array, sz, sizeof(mpz_t), mpz_cmp);

  return array;
}

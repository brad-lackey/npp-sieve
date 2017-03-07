//
//  karmarker-karp.c
//  
//
//  Created by Brad Lackey on 1/16/17.
//  Minor modifications by Stephen Jordan Feb/March 2017.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gmp.h>

int precision;
//const int size = 27;

mpz_t *populateArray(unsigned int sz, gmp_randstate_t *rand);
mpz_t *initializeBuffer(unsigned int sz);
void free_mpz_array(mpz_t *array, unsigned int sz);
int trial(unsigned int array_size, gmp_randstate_t *rand);

int main(int *argc, char *argv[]) {
  int trials, t;
  unsigned int array_size;
  unsigned long int seed; //for rng
  gmp_randstate_t rand;   //for rng
  clock_t beginning, end; //for timing
  double time_spent;      //for timing
  int success_tally;
  if(argc != 5) {
    printf("Usage: karmarkar-karp N BITS seed trials\n");
    return 0;
  }
  beginning = clock();
  array_size = atoi(argv[1]);
  precision = atoi(argv[2]);
  seed = atoi(argv[3]);
  trials = atoi(argv[4]);
  printf("seed = %lu\n", seed);
  printf("N = %u\n", array_size);
  printf("bits = %d\n", precision);
  printf("trials = %d\n", trials);
  //itialize the rng
  gmp_randinit_mt(rand);
  gmp_randseed_ui(rand, seed);
  success_tally = 0;
  for(t = 1; t <= trials; t++) {
    printf("trial %d--------------------------\n", t);
    success_tally += trial(array_size, &rand);
    fflush(stdout); //on long runs, we don't want output languishing in the output buffer
  }
  gmp_randclear(rand);
  end = clock();
  time_spent = (double)(end-beginning)/CLOCKS_PER_SEC;
  printf("Walltime = %f\n", time_spent);
  printf("Walltime per trial = %f\n", time_spent/(double)trials);
  printf("Success fraction %d/%d = %f\n", success_tally, trials, (double)success_tally/(double)trials);
  return 0;
}

int trial(unsigned int array_size, gmp_randstate_t *rand){
  int a,b;
  unsigned int buffer_size;
  mpz_t *array;
  mpz_t *buffer;
  mpz_t max;
  int success; //1 if final value is 0 or 1, otherwise 0

  buffer_size = array_size/3;
  array = populateArray(array_size, rand);
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
  success = 0;
  if(mpz_cmp_ui(array[0], 0)==0 || mpz_cmp_ui(array[0], 1)==0) success = 1;
  printf("success = %d\n", success);
  mpz_clear(max);
  free_mpz_array(buffer, buffer_size);
  free_mpz_array(array, array_size);
  return success; //1 if karmarkar-karp found minimal solution (0 or 1)
}

mpz_t *initializeBuffer(unsigned int sz){
  int i;
  mpz_t *array = calloc(sz, sizeof(mpz_t));
  for(i=0; i<sz; ++i) mpz_init2(array[i], precision);
  return array;
}

void free_mpz_array(mpz_t *array, unsigned int sz) {
  int i;
  for(i=0; i<sz; ++i) mpz_clear(array[i]);
  free(array);
}

mpz_t *populateArray(unsigned int sz, gmp_randstate_t *rand){
  int i;
  mpz_t *array = calloc(sz, sizeof(mpz_t));
  for(i=0; i<sz; ++i) mpz_init2(array[i], precision);
  for (i=0; i<sz; ++i) mpz_urandomb(array[i],rand,precision);
  qsort(array, sz, sizeof(mpz_t), mpz_cmp);
  return array;
}


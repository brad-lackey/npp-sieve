//
//  npp-tree.c
//  
//
//  Created by Brad Lackey on 1/19/17.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gmp.h>

const int precision = 400;
const int size = 4;
static mpz_t *array;
static mpz_t best;


int main(int argc, char **argv){
  int i;
  mpz_t sum;
  gmp_randstate_t rand;
  
  printf("Seed = %ld\n",time(0));
  array = calloc(1ul<<size,sizeof(mpz_t));

  gmp_randinit_mt(rand);
  gmp_randseed_ui(rand, (unsigned long) time(0));
  
  mpz_init(sum);
  for(i=0; i<(1<<size); ++i) mpz_init2(array[i], precision);
  
  for (i=0; i<(1<<size); ++i){
    mpz_urandomb(array[i],rand,precision);
    mpz_add(sum,sum,array[i]);
  }
  mpz_tdiv_q_2exp(sum,sum,1);
  mpz_init_set(best, sum);
  qsort(array, 1<<size, sizeof(mpz_t), mpz_cmp);
 
  for (i=0; i<(1<<size); ++i){
    mpz_out_str(stdout, 10, array[i]);
    printf("\n");
  }
  
  printf("\n");
  mpz_out_str(stdout, 10, best);
  printf("\n");

  
  return 0;
}

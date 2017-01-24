//
//  karmarker-karp.c
//  
//
//  Created by Brad Lackey on 1/16/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

const int precision = 400;
const int size = 16;

int main(int argc, char ** argv){
  int i;
  mpz_t *array = calloc(1ul<<size,sizeof(mpz_t));
  gmp_randstate_t rand;

  for(i=0; i<(1<<size); ++i) mpz_init2(array[i], precision);
  
  gmp_randinit_mt(rand);
  for (i=0; i<(1<<size); ++i)
    mpz_urandomb(array[i],rand,precision);
  qsort(array, 1<<size, sizeof(mpz_t), mpz_cmp);

  for (i=(1<<size)-2; i>=0; --i) {
    mpz_sub(array[i], array[i+1], array[i]);
    heapsort(array, i+1, sizeof(mpz_t), mpz_cmp);
    printf("%d %zu  ", i, mpz_sizeinbase(array[i],2));
    mpz_out_str(stdout, 10, array[i]); printf("\n");
  }
  
  printf("%zu  ", mpz_sizeinbase(array[0],2));
  mpz_out_str(stdout, 10, array[0]); printf("\n");
  
  return 0;
}

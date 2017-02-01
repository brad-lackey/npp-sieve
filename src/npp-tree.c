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

int readFile(char *filename);

const int precision = 400;
static mpz_t *array;
static mpz_t best;


int main(int argc, char **argv){
  int i,size;
  mpz_t sum;
  gmp_randstate_t rand;
  
  if (argc != 2) {
    fprintf(stderr,"Usage: %s <file of numbers>\n\n", argv[0]);
    return 2;
  }
  
  printf("Seed = %ld\n",time(0));
  gmp_randinit_mt(rand);
  gmp_randseed_ui(rand, (unsigned long) time(0));
  
  mpz_init(sum);
  size = readFile(argv[1]);
//  for(i=0; i<(1<<size); ++i) mpz_init2(array[i], precision);
//
//  for (i=0; i<(1<<size); ++i){
//    mpz_urandomb(array[i],rand,precision);
//    mpz_add(sum,sum,array[i]);
//  }
//  mpz_tdiv_q_2exp(sum,sum,1);
//  mpz_init_set(best, sum);
 
  for (i=0; i<size; ++i){
    mpz_out_str(stdout, 10, array[i]);
    printf("\n");
  }
  
//  printf("\n");
//  mpz_out_str(stdout, 10, best);
// printf("\n");

  
  return 0;
}

int readFile(char *filename){
  int size = 0;
  mpz_t in;
  FILE *fp = fopen(filename, "r");
  
  if (fp == NULL) return 0;

  mpz_init(in);
  
  while ( mpz_inp_str(in, fp, 0) > 0 ) {
    ++size;
    array = realloc(array, size*sizeof(mpz_t));
    mpz_init_set(array[size-1],in);
  }
  
  qsort(array, 1<<size, sizeof(mpz_t), mpz_cmp);
  mpz_clear(in);
  return size;
}
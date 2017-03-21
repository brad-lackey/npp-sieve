//
//  npp-tree.c
//  
//
//  Created by Brad Lackey on 3/21/17.
//
//

#include "npp-tree.h"


int main(int argc, char **argv){
  array_t array;
  number_t best;
  FILE *fp;
  
  
  if (argc < 2) {
    fprintf(stderr,"Usage: %s <file of numbers> [<seed>]\n\n", argv[0]);
    return 2;
  }
  
  fp = fopen(argv[1], "r");
  scanArray(fp, &array);
  fclose(fp);
  
//  printf("Size = %u\n", array.size);
  
  if ( argc > 2 )
    sscanf(argv[2],"%u", &seed);
  else
    seed = time(0);
//  printf("Seed = %u\n", seed);

//  lambda = 1.0;
//  populateArray(array, array_size, uniform);
  
//  printArray(stdout, array); printf("\n");

  NUMBER_INIT(best, array.precision);
  runHorowitzSahni(array, best);

  freeArray(array);
  NUMBER_PRINT(stdout, best);
  printf(" ");
  NUMBER_LOG2(best, best);
  
  NUMBER_PRINT(stdout, best);
  printf("\n");

  return 0;
}


int runHorowitzSahni(array_t array, number_t best){
  int i,j,k;
  unsigned int block_size;
  number_t sum, temp;
  number_t *block;

  NUMBER_INIT(sum,array.precision);
  NUMBER_ADD(sum, array.number[0], array.number[1]);
  for (i=2; i<array.size; ++i) NUMBER_ADD(sum, sum, array.number[i]);
  NUMBER_HALF(sum,sum);

  
  block_size = 1u<<(array.size/2);
  block = (number_t *) malloc(block_size*sizeof(number_t));
  
  NUMBER_INIT(temp, array.precision);
  NUMBER_SET(temp, sum);
  
  NUMBER_NEG(best, temp);
  NUMBER_INIT(block[0], array.precision);
  NUMBER_SET(block[0], temp);
  
  for (i=j=1; i<block_size; ++i) {
    k = decode(i);
    if ( k < 0 )
      NUMBER_ADD(temp, temp, array.number[array.size+k]);
    else
      NUMBER_SUB(temp, temp, array.number[array.size-k]);
    if ( NUMBER_SGN(temp) < 0 )
    {
      if ( NUMBER_CMP(best, temp) < 0 ) NUMBER_SET(best, temp);
      --block_size;
    } else {
      NUMBER_INIT(block[j], array.precision);
      NUMBER_SET(block[j++], temp);
    }
  }
  block_size = j;
  block = (number_t *) realloc(block, block_size*sizeof(number_t));
  qsort(block, block_size, sizeof(number_t), NUMBER_CMP);
  
  NUMBER_ZERO(temp);
  for (i=1; i<1u<<(array.size/2); ++i) {
    k = decode(i);
    if ( k < 0 )
      NUMBER_SUB(temp, temp, array.number[(-k)-1]);
    else
      NUMBER_ADD(temp, temp, array.number[k-1]);
    
    j = -1;
    k = block_size-1;
    while ( (k - j > 0) ){
      if ( NUMBER_CMP(block[k],temp) > 0 ) {
        k = j + (k-j)/2;
      } else {
        j = (k-j+1)/2;
        k += j;
        j = k-j;
      }
    }
    if ( k == -1 ) NUMBER_NEG(sum, temp);
    else NUMBER_SUB(sum, block[k], temp);
    if ( NUMBER_CMP(best, sum) < 0 ) NUMBER_SET(best, sum);
  }
  
  for (i=0; i<block_size; ++i)
    NUMBER_CLEAR(block[i]);
  free(block);
  
  NUMBER_CLEAR(sum);
  NUMBER_NEG(best, best);
  NUMBER_DUB(best, best);
  
  NUMBER_CLEAR(temp);

  return 0;
}






void populateArray(number_t *array, unsigned int size, int (*set_random)(number_t n, random_t r)){
  int i;
  gmp_randstate_t rand;
  
  gmp_randinit_mt(rand);
  gmp_randseed_ui(rand, seed);
  for (i=0; i<size; ++i) set_random(array[i], rand);
  
  gmp_randclear(rand);
  qsort(array, size, sizeof(number_t), NUMBER_CMP);
}

int exponential(mpfr_t n, gmp_randstate_t r){
  mpfr_urandomb(n, r);
  mpfr_log(n, n, MPFR_RNDN);
  mpfr_neg(n, n, MPFR_RNDN);
  mpfr_div_d(n, n, lambda, MPFR_RNDN);
  return 0;
}

int uniform(mpfr_t n, gmp_randstate_t r){
  mpfr_urandomb(n, r);
  mpfr_mul_d(n, n, lambda, MPFR_RNDN);
  return 0;
}


int mpfr_compare(void *a, void *b){
  return mpfr_cmp((mpfr_t *) a, (mpfr_t *) b);
}

int mpfr_reverse(void *a, void *b){
  return -mpfr_cmp((mpfr_t *) a, (mpfr_t *) b);
}

int decode(int index){
  int depth = 1;
  if (index == 0) return 0;
  while ( (index&1) == 0 ) {
    ++depth;
    index >>= 1;
  }
  if ((index >> 1) & 1)
    return -depth;
  else
    return depth;
}

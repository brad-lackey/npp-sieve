//
//  npp-tree.c
//  
//
//  Created by Brad Lackey on 1/19/17.
//
//

#include "npp-tree.h"

int main(int argc, char **argv){
  int i,j,k;
  unsigned int array_size, block_size;
  number_t *array, *block;
  number_t sum, temp, best;
//  clock_t beginning, end;
//  double time_spent;
  
  
  if (argc < 3) {
    fprintf(stderr,"Usage: %s <number of numbers> <precision> [<seed>]\n\n", argv[0]);
    return 2;
  }
  
//  beginning = clock();
  sscanf(argv[1],"%u", &array_size);
//  printf("Size = %u\n", array_size);
  sscanf(argv[2],"%u", &precision);
//  printf("Precision = %u\n", precision);
  if ( argc > 3 )
    sscanf(argv[3],"%u", &seed);
  else
    seed = time(0);
//  printf("Seed = %u\n", seed);

  array = initArray(array_size);

//  lambda = 2.0;
//  populateArray(array, array_size, uniform);
  
  lambda = 1.0;
  populateArray(array, array_size, exponential);
  
  computeSum(sum, array, array_size);

  printArray(stdout, array, array_size); printf("\n");
  
//  printf("Target: ");
//  NUMBER_PRINT(stdout, sum);
//  printf("\n");

  block_size = 1u<<(array_size/2);
  block = (number_t *) malloc(block_size*sizeof(number_t));

  NUMBER_INIT(temp);
  NUMBER_SET(temp, sum);
  
  NUMBER_INIT(best);
  NUMBER_NEG(best, temp);
  NUMBER_INIT(block[0]);
  NUMBER_SET(block[0], temp);
  
  for (i=j=1; i<block_size; ++i) {
    k = decode(i);
    if ( k < 0 )
      NUMBER_ADD(temp, temp, array[array_size+k]);
    else
      NUMBER_SUB(temp, temp, array[array_size-k]);
    if ( NUMBER_SGN(temp) < 0 )
    {
      if ( NUMBER_CMP(best, temp) < 0 ) NUMBER_SET(best, temp);
      --block_size;
    } else {
      NUMBER_INIT(block[j]);
      NUMBER_SET(block[j++], temp);
    }
  }
  block_size = j;
  block = (number_t *) realloc(block, block_size*sizeof(number_t));
  qsort(block, block_size, sizeof(number_t), NUMBER_CMP);
//  printArray(stdout, block, block_size); printf("\n");

//  end = clock();
//  time_spent = (double)(end-beginning)/CLOCKS_PER_SEC;
//  printf("Constructed block. Walltime = %f\n", time_spent);

//  printf("Best = ");
//  NUMBER_PRINT(stdout, best);
//  printf("\n");

  NUMBER_ZERO(temp);
  for (i=1; i<1u<<(array_size/2); ++i) {
    k = decode(i);
    if ( k < 0 )
      NUMBER_SUB(temp, temp, array[(-k)-1]);
    else
      NUMBER_ADD(temp, temp, array[k-1]);
    
    j = -1;
    k = block_size-1;
    while ( (k - j > 0) ){
//      printf("%d %d\n", j, k);

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

//    printf("%d %d: ", j, k);
//    NUMBER_PRINT(stdout, temp);
//    printf("  diff = ");
//    NUMBER_PRINT(stdout, sum);
//    printf("  best = ");
//    NUMBER_PRINT(stdout, best);
//    printf("\n");
  }
//  end = clock();
//  time_spent = (double)(end-beginning)/CLOCKS_PER_SEC;
//  printf("Completed search. Walltime = %f\n", time_spent);

  NUMBER_CLEAR(sum);
  NUMBER_NEG(best, best);
  NUMBER_DUB(best, best);
  NUMBER_PRINT(stdout, best);
//  printf(" ");
//  NUMBER_LOG2(temp, best);
//  NUMBER_PRINT(stdout, temp);
  printf("\n");
  NUMBER_CLEAR(temp);
  NUMBER_CLEAR(best);
  freeArray(&array, array_size);

//  end = clock();
//  time_spent = (double)(end-beginning)/CLOCKS_PER_SEC;
//  printf("Cleaned up. Walltime = %f\n", time_spent);

  return 0;
}

number_t *initArray(unsigned int size){
  int i;
  number_t *arr = (number_t *) malloc(size*sizeof(number_t));
  if ( arr != NULL ) for (i=0; i<size; ++i) NUMBER_INIT(arr[i]);
  return arr;
}

void freeArray(number_t **arr_ptr, unsigned int size){
  int i;
  if ( (*arr_ptr) != NULL ) for (i=0; i<size; ++i) NUMBER_CLEAR((*arr_ptr)[i]);
  *arr_ptr = NULL;
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

void printArray(FILE *stream, number_t *array, unsigned int size){
  int i;
  for (i=0; i<size; ++i) {
    fprintf(stream, "%d ", i);
    NUMBER_PRINT(stream, array[i]);
    fprintf(stream, "\n");
  }
}

void computeSum(number_t sum, number_t *array, unsigned int size){
  int i;
  NUMBER_INIT(sum);
  NUMBER_ADD(sum, array[0], array[1]);
  for (i=2; i<size; ++i) NUMBER_ADD(sum, sum, array[i]);
  NUMBER_HALF(sum,sum);
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

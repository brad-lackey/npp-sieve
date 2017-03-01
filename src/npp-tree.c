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
#include <strings.h>
#include <gmp.h>

void readFile(char *filename);
int decode(int index);
void randomSubset();

int intcompare(void *a, void *b){ return *((int *) a) - *((int *) b); }
int mpzbestcompare(void *a, void *b);

const int precision = 400;
static int *idx;
static int index_size;
static mpz_t *array;
static int array_size;
static mpz_t *block;
static int block_size;
static mpz_t best;
static mpz_t data;

int main(int argc, char **argv){
  int i,j;
  mpz_t sum,temp;
  mpz_t *searchpointer;
  
  if (argc != 3) {
    fprintf(stderr,"Usage: %s <file of numbers> <size>\n\n", argv[0]);
    return 2;
  }
  
  i = (int) time(0);
  printf("Seed = %d\n", i);
  srand48(i);
  sscanf(argv[2],"%d", &index_size);
  printf("Size = %d\n", index_size);
  idx = (int *) malloc(index_size*sizeof(int));
  block_size = 1<<(index_size/2);
  block = (mpz_t *) malloc(block_size*sizeof(mpz_t));
  for (i=0; i<block_size; ++i)
    mpz_init(block[i]);

  mpz_init(sum);
  mpz_init(best);
  mpz_init(temp);
  mpz_init(data);
  readFile(argv[1]);
 
  randomSubset();

  for (i=0; i<index_size; ++i){
    mpz_add(sum, sum, array[idx[i]]);
    printf("%4d ", idx[i]);
    mpz_out_str(stdout, 10, array[idx[i]]);
    printf(" (%zu)\n", mpz_sizeinbase(array[idx[i]], 2));
  }
  
  mpz_fdiv_q_2exp(sum, sum, 1);

  mpz_set(block[0], sum);
  for (i=1; i<block_size; ++i) {
    j = decode(i);
    if ( j < 0 )
      mpz_add(block[i], block[i-1], array[idx[index_size+j]]);
    else
      mpz_sub(block[i], block[i-1], array[idx[index_size-j]]);
  }
  qsort(block, block_size, sizeof(mpz_t), mpz_cmp);

  for (i=0; i<block_size; ++i) {
    if ( mpz_sgn(block[i]) < 0 ) {
      mpz_neg(best, block[i]);
    } else {
      break;
    }
  }
  
  printf("\n");
  mpz_out_str(stdout, 10, best);
  printf(" (%zu)\n", mpz_sizeinbase(best,2));
  
  for (i=1; i<block_size; ++i) {
    j = decode(i);
    if ( j < 0 )
      mpz_sub(temp, temp, array[idx[(-j)-1]]);
    else
      mpz_add(temp, temp, array[idx[j-1]]);
    if ( bsearch(&temp, block, block_size, sizeof(mpz_t), mpzbestcompare) != NULL ){
      mpz_set(best, data);
      mpz_out_str(stdout, 10, best);
      printf(" (%zu)\n", mpz_sizeinbase(best,2));
    }
  }

  return 0;
}

int mpzbestcompare(void *a, void *b){
  mpz_sub(data, *((mpz_t *) a), *((mpz_t *) b));
  if ( mpz_sgn(data) < 0 ) {
    return -1;
  } else {
    if ( mpz_cmp(best, data) < 0 )
      return 1;
    else
      return 0;
  }
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


void randomSubset(){
  int j,k;
  
  j = lrand48() % array_size;
  idx[0] = j;
  k = 1;
  while (k < index_size) {
    j = lrand48() % array_size;
    if ( bsearch(&j, idx, k, sizeof(int), intcompare) == NULL ) {
      idx[k++] = j;
      mergesort(idx, k, sizeof(int), intcompare);
    }
  }
}


void readFile(char *filename){
  mpz_t in;
  FILE *fp = fopen(filename, "r");
  
  array_size = 0;
  if (fp == NULL) return;

  mpz_init(in);
  
  while ( mpz_inp_str(in, fp, 0) > 0 ) {
    ++array_size;
    array = realloc(array, array_size*sizeof(mpz_t));
    mpz_init_set(array[array_size-1],in);
  }
  
  qsort(array, array_size, sizeof(mpz_t), mpz_cmp);
  mpz_clear(in);
}

//
//  algorithm.c
//  
//
//  Created by Brad Lackey on 11/7/16.
//
//

#include "algorithm.h"

static int size;
static int *solution;
static uint64_t *numbers;

int main(int argc, char **argv){
  int i,j;
  uint64_t target = 0;
  uint64_t sumcheck = 0;			// Added to verify result
  
  // Check that the file of numbers was given.
  if (argc != 2) { fprintf(stderr, "Usage: %s <numbers.file>\n", argv[0]); return 2; }
  
  // Read in the numbers.
  if ( loadFile(argv[1], &numbers, &size) ) return 2;
  solution = (int *) malloc(size*sizeof(int));
  
  // Set the target value.
  for (i=0; i<size; ++i) target += numbers[i];

  target /= 2;
  printf("Size: %d\nTarget: %llu\n",size,target);
  
  j = foo(target, 0, 0);
  if ( j < 0 ) {
    printf("No solution.\n");
  } else {
 //   for (i=0; i<j; ++i)	// Original line
 //   print solution indices until we hit a trailing 0
	  i=0;
		do 
		{ 	sumcheck+=numbers[solution[i]];
			printf("%d,",solution[i]);
	  	} while (solution[++i]);
    printf("\nsumcheck = %d \n",sumcheck);
  }
  
  return 0;
}

int foo(uint64_t tgt, int num_idx, int soln_idx){
  int i;
  
  if ( tgt == 0ull ) return soln_idx;
  if ( num_idx == size ) return -1;
  if ( numbers[num_idx] > tgt ) return foo(tgt, num_idx+1, soln_idx);
  
  solution[soln_idx] = num_idx;
  i = foo(tgt-numbers[num_idx], num_idx+1, soln_idx+1);
  
  if ( i < 0 ) return foo(tgt, num_idx+1, soln_idx);
  
  return soln_idx+1;
}

int loadFile(char *filename, uint64_t **numbers_ptr, int *size_ptr){
  int sz;
  uint64_t nbrs[64];
  FILE *fp = fopen(filename, "r");
  
  if ( fp == NULL ) {
    fprintf(stderr, "Could not open file %s", filename);
    return 2;
  }
  
  for (sz=0; sz<64; ++sz) if ( fscanf(fp,"%llu", nbrs+sz) != 1 ) break;
  *numbers_ptr = (uint64_t *) malloc(sz*sizeof(uint64_t));
  bcopy(nbrs, *numbers_ptr, sz*sizeof(uint64_t));
  *size_ptr = sz;
  return 0;
}

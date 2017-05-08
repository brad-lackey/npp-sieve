//
//  npp-tree.c
//  
//
//  Created by Brad Lackey on 3/21/17. Last modified 3/22/17.
//
//

#include "npp-tree.h"

int runHorowitzSahni(array_t array, number_t best){
  int i,j,k;
  unsigned int block_size;
  number_t sum, temp;
  number_t *block;

//  printf("Array Size = %u\n", array.size);

  // Sort the array in order to build the search trees.
  qsort(array.number, array.size, sizeof(number_t), NUMBER_CMP);
  
  // Compute the target vector, i.e. convert NPP to subset-sum problem.
  NUMBER_INIT(sum,array.precision);
  NUMBER_ADD(sum, array.number[0], array.number[1]);
  for (i=2; i<array.size; ++i) NUMBER_ADD(sum, sum, array.number[i]);
  NUMBER_HALF(sum,sum);

  // Create memory to hold one half the meet-in-the-middle.
  // Note that if the array size is odd, this side takes the smaller half.
  block_size = 1u<<(array.size/2);
  block = (number_t *) malloc(block_size*sizeof(number_t));
  
  // Initialize a temporary register.
  NUMBER_INIT(temp, array.precision);
  NUMBER_SET(temp, sum);
  
  // Initialize the array with the target.
  NUMBER_NEG(best, temp);
  NUMBER_INIT(block[0], array.precision);
  NUMBER_SET(block[0], temp);
  
  // Fill in the rest of the array using a Gray code ordering.
  // Only relevant values are added to the array.
  // At the end sort the array for lookups into it.
//  printf("First half block size = %u\n", block_size);
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
  
  // Now loop over the other half also using Gray code ordering.
  // If the array size is odd, this takes the larger half.
  // Lookup into the table (manually did the search).
  // If we find a better solution store it off.
  NUMBER_ZERO(temp);
//  printf("Second half block size = %u\n", 1u<<((array.size+1)/2));
  for (i=1; i<1u<<((array.size+1)/2); ++i) {
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
    if ( NUMBER_CMP(best, sum) < 0 ){
//      printf("%d ",i);
      NUMBER_SET(best, sum);
//      NUMBER_PRINT(stdout, best); printf("\n");
    }
  }
  
  // Free memory
  for (i=0; i<block_size; ++i)
    NUMBER_CLEAR(block[i]);
  free(block);
  
  NUMBER_CLEAR(sum);
  NUMBER_NEG(best, best);
  NUMBER_DUB(best, best);
  
  NUMBER_CLEAR(temp);

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

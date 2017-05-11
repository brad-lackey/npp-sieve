//
//  create_numbers.c
//  
//
//  Created by Brad Lackey on 3/22/17.
//
//

#include <time.h>
#include <gmp.h>
#include <mpfr.h>
#include "types.h"
#include "npp-io.h"

int main(int argc, char **argv){
  int i;
  unsigned int size, precision, seed;
  gmp_randstate_t rand;
  array_t array;
  
  
  if (argc < 3) {
    fprintf(stderr,"Usage: %s <size of list> <precision> [<seed>]\n\n", argv[0]);
    return 2;
  }
  
  sscanf(argv[1],"%u", &(size));
  fprintf(stderr, "Size = %u\n", size);
  sscanf(argv[2],"%u", &(precision));
  fprintf(stderr, "Precision = %u\n", precision);
  if ( argc > 3 )
    sscanf(argv[3],"%ul", &seed);
  else
    seed = time(0);
  fprintf(stderr,"Seed = %u\n", seed);

  initArray(&array,size,precision);
  
  gmp_randinit_mt(rand);
  gmp_randseed_ui(rand, seed);
  for (i=0; i<size; ++i){
    mpfr_urandomb(array.number[i], rand);
    
    // The next line creates the uniform distribution with mean 1.
    //mpfr_mul_2ui(array.number[i], array.number[i], 1ul, MPFR_RNDN);
    
    // The next two lines create the exponential distribution with mean 1.
    mpfr_log(array.number[i], array.number[i], MPFR_RNDN);
    mpfr_neg(array.number[i], array.number[i], MPFR_RNDN);
  }
  
  gmp_randclear(rand);
//  qsort(array.number, size, sizeof(number_t), mpfr_cmp);
  
  printArray(stdout, array);
  freeArray(array);
  
  return 0;
}

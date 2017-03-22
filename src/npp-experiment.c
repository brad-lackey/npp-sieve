//
//  npp-experiment.c
//  
//
//  Created by Brad Lackey on 3/22/17.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <strings.h>
#include <mpfr.h>
#include "types.h"
#include "npp-io.h"
#include "npp-tree.h"



int main(int argc, char **argv){
  unsigned int i;
  array_t input, block, output;
  number_t best;
  FILE *fp;
  
  if (argc < 2) {
    fprintf(stderr,"Usage: %s <file of numbers> <block size>\n\n", argv[0]);
    return 2;
  }
  
  fp = fopen(argv[1], "r");
  scanArray(fp, &input);
  fclose(fp);
  
  sscanf(argv[2],"%u", &i);
  block.size = i;
  block.precision = input.precision;
  block.number = (number_t *) malloc(i*sizeof(number_t));

//  printf("List Size = %u\n", input.size);
//  printf("Block Size = %u\n", block.size);
  
  initArray(&output, input.size/block.size, input.precision);
//  printf("Output Size = %u\n", output.size);

  NUMBER_INIT(best, input.precision);
  for (i=0; i<output.size; ++i) {
    bcopy(input.number + i*block.size, block.number, block.size*sizeof(number_t));
    runHorowitzSahni(block, output.number[i]);
  }
  free(block.number);
  freeArray(input);
  
//  qsort(output.number, output.size, sizeof(number_t), NUMBER_CMP);
  printArray(stdout,output);
  freeArray(output);

  return 0;
}

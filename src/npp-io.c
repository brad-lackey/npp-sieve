//
//  npp-io.c
//  
//
//  Created by Brad Lackey on 3/21/17.
//
//

#include "npp-io.h"

int initArray(array_t *array_ptr, unsigned int size, unsigned int precision){
  int i;
  array_t array;

  array.size = size;
  array.precision = precision;
  array.number = (number_t *) malloc(size*sizeof(number_t));
  for (i=0; i<size; ++i) NUMBER_INIT(array.number[i], precision);
  
  *array_ptr = array;
  
  return 0;
}

void freeArray(array_t array){
  int i;
  
  if ( array.number != NULL ) {
    for (i=0; i<array.size; ++i)
      NUMBER_CLEAR(array.number[i]);
    free(array.number);
  }
  array.size = 0;
  array.precision = 0;
}



void printArray(FILE *stream, array_t array){
  int i;
  fprintf(stream, "npp %d %d\n", array.size, array.precision);
  for (i=0; i<array.size; ++i) {
    fprintf(stream, "n ");
    NUMBER_PRINT(stream, array.number[i]);
    fprintf(stream, "\n");
  }
}

int scanArray(FILE *stream, array_t *array_ptr){
  int size;
  int precision;
  int i;
  char *line = NULL;
  size_t linecap = 0;
  array_t array;
  
  while ( getline(&line, &linecap, stream) > 0 )
    if ( sscanf(line, "npp %d %d", &size, &precision ) == 2 )
      break;
  
  initArray(&array, size, precision);
  for (i=0; i<size; ++i){
    if ( fgetc(stream) == 'n' )
      NUMBER_SCAN(stream, array.number[i]);
    getline(&line, &linecap, stream);
  }
  
  *array_ptr = array;
  return i;
}

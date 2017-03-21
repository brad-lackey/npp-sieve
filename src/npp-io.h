//
//  npp-io.h
//  
//
//  Created by Brad Lackey on 3/21/17.
//
//

#ifndef npp_io_h
#define npp_io_h

#include <stdio.h>
#include <stdlib.h>
#include "types.h"

int initArray(array_t *array_ptr, unsigned int size, unsigned int precision);
void freeArray(array_t array);


void printArray(FILE *stream, array_t array);
int scanArray(FILE *stream, array_t *array_ptr);

#endif /* npp_io_h */

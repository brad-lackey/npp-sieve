//
//  binaryGrayCode.c
//  
//
//  Created by Brad Lackey on 2/4/17.
//
//

#include <stdio.h>
#include <stdlib.h>
#include "binaryGrayCode.h"

const int n = 4;
int *array;


int main(){
  int i;
  array = (int *) calloc((1<<n), sizeof(int));
  array[0] = 0;
  code(1,n);
  for (i=0; i<(1<<n); ++i) {
    printf("%d %3d %3d\n",i,array[i],decode(i));
  }
  return 0;
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

int code(int index, int depth){
  if (depth != 0) {
    index = code(index, abs(depth)-1);
    if (depth > 0)
      array[index] = depth;
    else
      array[index] = depth;
    index = code(index+1, -(abs(depth)-1));
  }
  return index;
}
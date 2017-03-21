//
//  npp-tree.h
//  
//
//  Created by Brad Lackey on 3/21/17.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <strings.h>
#include "types.h"
#include "npp-io.h"

#ifndef npp_tree_h
#define npp_tree_h

unsigned int seed;      // This value characterizes this thread.
double lambda;          // This is a global parameter that can be used in defining initial distributions.



typedef gmp_randstate_t random_t;

void populateArray(number_t *array, unsigned int size, int (*set_random)(number_t n, random_t r));

int decode(int index);

int exponential(number_t n, random_t r); // Use global parameter lambda to set the (inverse) mean.
int uniform(number_t n, random_t r);     // Use global parameter lambda: U(0,lambda).

int mpfr_compare(void *a, void *b);
int mpfr_reverse(void *a, void *b);

int runHorowitzSahni(array_t block, number_t best);


#endif /* npp_tree_h */

//
//  npp-tree.h
//  
//
//  Created by Brad Lackey on 3/21/17. Last modified 3/22/17.
//
//

#include "types.h"
#include "npp-io.h"

#ifndef npp_tree_h
#define npp_tree_h


int decode(int index);

int mpfr_compare(void *a, void *b);
int mpfr_reverse(void *a, void *b);

int runHorowitzSahni(array_t block, number_t best);


#endif /* npp_tree_h */

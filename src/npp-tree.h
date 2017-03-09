//
//  npp-tree.h
//  
//
//  Created by Brad Lackey on 3/2/17.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <strings.h>
#include <mpfr.h>

#ifndef npp_tree_h
#define npp_tree_h

unsigned int seed;      // This value characterizes this thread.
unsigned int precision;


typedef mpfr_t number_t;
#define NUMBER_CMP mpfr_compare
#define NUMBER_REV mpfr_reverse
#define NUMBER_INIT(n) mpfr_init2(n,precision)
#define NUMBER_CLEAR mpfr_clear
#define NUMBER_ZERO(n) mpfr_set_zero(n,1);
#define NUMBER_SET(o,i) mpfr_set(o,i,MPFR_RNDN)
#define NUMBER_PRINT(s,n) mpfr_out_str(s,10,10,n,MPFR_RNDN)
#define NUMBER_ADD(o,i1,i2) mpfr_add(o,i1,i2,MPFR_RNDN)
#define NUMBER_SUB(o,i1,i2) mpfr_sub(o,i1,i2,MPFR_RNDN)
#define NUMBER_HALF(o,i) mpfr_div_2ui(o,i,1ul,MPFR_RNDN)
#define NUMBER_DUB(o,i) mpfr_mul_2ui(o,i,1ul,MPFR_RNDN)
#define NUMBER_NEG(o,i) mpfr_neg(o,i,MPFR_RNDN)
#define NUMBER_SGN mpfr_sgn
#define NUMBER_LOG2(o,i) mpfr_log2(o,i,MPFR_RNDN)

typedef gmp_randstate_t random_t;

number_t *initArray(unsigned int size);
void freeArray(number_t **arr_ptr, unsigned int size);
void populateArray(number_t *array, unsigned int size, int *(set_random)(number_t n, random_t r));
void printArray(FILE *stream, number_t *array, unsigned int size);

void computeSum(number_t sum, number_t *array, unsigned int size);
int createDataBlock(number_t **block_ptr, number_t *array, unsigned int size);


int decode(int index);
int unit_exponential(number_t n, random_t r);

int mpfr_compare(void *a, void *b);
int mpfr_reverse(void *a, void *b);



#endif /* npp_tree_h */

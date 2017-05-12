//
//  types.h
//  
//
//  Created by Brad Lackey on 3/21/17.
//
//

#ifndef types_h
#define types_h

#include <mpfr.h>

typedef mpfr_t number_t;
#define NUMBER_CMP mpfr_compare
#define NUMBER_REV mpfr_reverse
#define NUMBER_INIT(n,p) mpfr_init2(n,p)
#define NUMBER_CLEAR mpfr_clear
#define NUMBER_ZERO(n) mpfr_set_zero(n,1);
#define NUMBER_SET(o,i) mpfr_set(o,i,MPFR_RNDN)
#define NUMBER_SET_D(o,i) mpfr_set_d(o,i,MPFR_RNDN)
#define NUMBER_PRINT(s,n) mpfr_out_str(s,10,0,n,MPFR_RNDN)
#define NUMBER_SCAN(s,n) mpfr_inp_str(n,s,10,MPFR_RNDN)
#define NUMBER_ADD(o,i1,i2) mpfr_add(o,i1,i2,MPFR_RNDN)
#define NUMBER_SUB(o,i1,i2) mpfr_sub(o,i1,i2,MPFR_RNDN)
#define NUMBER_MUL_D(o,i1,i2) mpfr_mul_d(o,i1,i2,MPFR_RNDN)
#define NUMBER_HALF(o,i) mpfr_div_2ui(o,i,1ul,MPFR_RNDN)
#define NUMBER_DUB(o,i) mpfr_mul_2ui(o,i,1ul,MPFR_RNDN)
#define NUMBER_NEG(o,i) mpfr_neg(o,i,MPFR_RNDN)
#define NUMBER_SGN mpfr_sgn
#define NUMBER_LOG(o,i) mpfr_log(o,i,MPFR_RNDN)
#define NUMBER_LOG2(o,i) mpfr_log2(o,i,MPFR_RNDN)
#define NUMBER_EXP2(o,i) mpfr_exp2(o,i,MPFR_RNDN)

typedef struct {
  unsigned int size;
  unsigned int precision;
  number_t *number;
} array_t;

#endif /* types_h */

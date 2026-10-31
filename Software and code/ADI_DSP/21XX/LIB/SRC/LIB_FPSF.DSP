/*
	This routine converts an floating point number in the internal
	format to an IEEE-single precision number.

	This routine is ONLY called from within the CRTL.

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 5/29/93

		Calling Parameters
	    AX0 Contains exponent of X
	MR1:MR0 Contains mantissa of X

		Result Registers
	     AR Contains exponent of result
	SR1:SR0 Contains mantissa of result

		Altered Registers
	AR, AF, AX0, AY1,
	MR2, MR1, MR0, 
	SR1, SR0, SI, SE 

*/

.MODULE/IMAGE	__Internal_FP_to_sf;

#include "lib_glob.h"
#include "ifp_glob.h"

.ENTRY	___lib_libfp_sf;

___lib_libfp_sf:AF=PASS MR1;
		AR=MR0 OR AF;		/* Test for zero input		*/
		IF EQ JUMP __zero_input;/* Input is zero, return zero	*/
		AF=PASS MR0;		/* Load this for double negate	*/
		AR=ABS MR1;		/* Take absolute value of input	*/
		IF NEG AF=-AF;		/* Need to do negation in double*/
		IF NEG AR=-MR1+C-1;	/* precision			*/
		MR1=AR, AR=PASS AF;	/* Place positive result back	*/
		MR0=AR, SE=EXP MR1 (HI);/*  into MR.  Check for shift	*/
		SE=EXP MR0 (LO);	/* Need to check for small num	*/
		SR=NORM MR1 (HI);	/* Normalize input for placement*/
		SR=SR OR NORM MR0 (LO);	/* don't forget LSW		*/
		AR=PASS SR0;		/* Hold LSW for a moment	*/
		SR=LSHIFT SR1 BY 2 (HI);/* Remove MSB			*/
		SR=SR OR LSHIFT AR BY 2 (LO);/* Place LSW		*/
		AR=PASS SR1;		/* Hold MSW for a moment	*/
		SR=LSHIFT SR0 BY -9 (LO);/* Place mantissa bits		*/
		SR=SR OR LSHIFT AR BY -9 (HI);/* Place mantissa bits	*/
		AR=SE;			/* Fetch number of sign bits	*/
		AY1=F_EXP_BIAS-1;	/* Load exponent bias		*/
		AY1=AX0, AR=AR+AY1;	/* Compute biased exponent	*/
		AR=AR+AY1;		/* Compute final biased exp	*/
		SR=SR OR LSHIFT AR BY 7 (HI);/* Place exponent 		*/
		AR=-1;			/* Used to place sign		*/
		IF POS AR=PASS 0;	/* If abs of input was positive	*/
		SR=SR OR LSHIFT AR BY 15 (HI);/* Place sign bit		*/
		RTS;

__zero_input:	SR=LSHIFT MR0 BY -127 (HI);/* Zero out SR1:SR0		*/
		RTS;

.ENDMOD;

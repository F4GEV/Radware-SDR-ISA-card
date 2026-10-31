/*
	This routine converts an IEEE-single precision number to the
	internal format used by the CRTL.

	This routine is ONLY called from within the CRTL.

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 5/29/93

		Calling Parameters
	MR1:MR0 Contains IEEE-single representation of X

		Result Registers
	     AR Contains exponent of result
	SR1:SR0 Contains mantissa of result

		Altered Registers
	AR, AF, AX0, AY1,
	MR2, MR1, MR0, 
	SR1, SR0, SI, SE 

*/

.MODULE/IMAGE	__Internal_FP_from_sf;

#include "lib_glob.h"
#include "ifp_glob.h"

.ENTRY	___lib_sf_libfp;

___lib_sf_libfp:SR0=MR0, AR=ABS MR1;	/* Get sign of input		*/
		SR1=MR1, AF=PASS MR0;	/* Test for zero input, LSW	*/
		AF=MR1 OR AF;		/* Test for zero input, MSW	*/
		IF EQ JUMP __return_zero;
		SR=LSHIFT SR1 BY 1 (LO);/* Clear away sign bit		*/
		SR=LSHIFT SR0 BY -8 (LO);/* Isolate exp into LSBs	*/
		AY1=F_EXP_BIAS;		/* Load bias of IEEE float	*/
		AR=SR0-AY1;		/* AR holds unbiased exponent	*/
		AY1=0x7f;		/* Mask out sign and exponent	*/
		SR0=AR, AR=MR1 AND AY1;	
		AY1=AR, AR=AY1+1;	/* Now make implicit 1 explicit	*/
		SI=MR2, AR=AR OR AY1;	/* Zero out SI for not guard bit*/
		MR1=AR, AF=PASS MR0;	/* MR now holds denorm value	*/
		IF NEG AF=-AF;		/* Set proper sign		*/
		IF NEG AR=-AR+C-1;
		MR1=AR, AR=PASS AF;		
		MR0=AR, AF=PASS SR0;	/* Copy unbiased exponent 	*/
		AR=8;			/* Offset for a normalized num	*/
		AR=AR+AF;		/* Compute exponent for norm	*/
		JUMP ___lib_libfpnorm;	/* Normalized result		*/

__return_zero:	AR=0x8000;		/* Smallest exponent		*/
		SR=LSHIFT AR BY -100 (LO);/* Zero fraction		*/
		RTS;

.ENDMOD;

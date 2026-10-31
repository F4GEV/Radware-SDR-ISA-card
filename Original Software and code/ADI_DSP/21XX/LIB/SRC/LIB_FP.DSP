/*
	These routines provide an internal floating point format that
	provides 16-bits of exponent and 32-bits on mantissa.  This
	format allows greater than IEEE precision for math functions.

	These routines are ONLY called from within the CRTL.

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 5/29/93

		Calling Parameters
	    AX0 Contains exponent of X
	MR1:MR0 Contains mantissa of X

	    AY0 Contains exponent of Y
	MY1:MY0 Contains mantissa of Y

		Result Registers
	     AR Contains exponent of result
	SR1:SR0 Contains mantissa of result

		Altered Registers
	AR, AF, AX0, AY1,
	MR2, MR1, MR0, 
	SR1, SR0, SI, SE 

*/

.MODULE/IMAGE	__Internal_Extended_FP;

#include "lib_glob.h"
#include "flt_glob.h"

.ENTRY	___lib_libfpmult;
.ENTRY 	___lib_libfpadd, ___lib_libfpsub;
.ENTRY	___lib_libfpnorm;
.ENTRY	___lib_sf_libfp, ___lib_libfp_sf;


___lib_libfpmult:SR0=MR0, AF=AX0+AY0;	/* Compute new exponent		*/
		SR1=MR1, MR=SR0*MY0 (UU);/* Compute LSW of product	*/
		MR0=MR1, AR=AF+1;	/* Compensate for integer mult	*/
		MR1=MR2;
		MR=MR+SR1*MY0 (SU);	
		MR=MR+SR0*MY1 (US);
		SI=MR0;			/* Hold third word for norm op	*/
		MR0=MR1;
		MR1=MR2;
		MR=MR+SR1*MY1 (SS);
		JUMP ___lib_libfpnorm;
	
___lib_libfpsub:
___lib_libfpadd:SI=MY0, AR=AX0-AY0;	/* Check relative exponents	*/
		IF GT JUMP __shift_y;	/* Shift Yop if result is pos	*/

__shift_x:	SI=MR0, AR=PASS AR;	/* Need to shift Xop		*/
		SE=AR, AR=PASS MR1;	/* Hold Xop in temp registers	*/
		SR=LSHIFT SI (HI);	/* SR0 now contains guard bits	*/
		SI=SR0, SR=LSHIFT SI (LO);/* Hold guard bits, make LSW	*/
		MR0=MY0, SR=SR OR ASHIFT AR (HI);/* Shift MSW into place*/
		MR1=MY1, AR=PASS AY0;	/* Copy exponent into place	*/
		JUMP __add_core;	/* Now add operands		*/

__shift_y:	SR1=MY1, AR=-AR;	/* Copy value for shift		*/
		SE=AR, AR=PASS SR1;	/* Hold MSW of value to shift	*/
		SR=LSHIFT SI (HI);	/* Shift guard bits 		*/
		SI=SR0, SR=LSHIFT SI (LO);/* Hold guard bits 		*/
		AR=AX0, SR=SR OR ASHIFT AR (HI);/* Shift MSW into place	*/
		JUMP __add_core;	/* Now add operands		*/

__add_core:	AY1=MR0, AF=PASS AR;	/* Hold exponent 		*/
		AY1=MR1, AR=SR0+AY1;	/* Compute new LSW		*/
		MR0=AR, AR=SR1+AY1+C;	/* Compute new MSW		*/
		MR1=AR; 		/* Sign extension into MR2 needs*/
		IF AV JUMP __downshift;	/* Overflow!, need to downshift	*/
		AR=PASS AF;		/* MR holds mant, AR holds exp	*/
		JUMP ___lib_libfpnorm;	/* Normalize result		*/

___lib_libfpnorm:IF MV JUMP __downshift;/* The result overflowed	*/
		SE=EXP MR1 (HI);	/* Check for underflow		*/
		SE=EXP MR0 (LO);	/* Check in LSW also		*/
		AY1=SE;			/* Find out amount shift	*/
		AR=AR+AY1;		/* Compute new exponent		*/
		SR=NORM SI (LO);	/* Shift guard bits 		*/
		SR0=SR1;		/* Downshift guard bits		*/
		SR1=0;			/* Zero out upper word		*/
		SR=SR OR NORM MR1 (HI);/* Shift MSW into place	*/
		SR=SR OR NORM MR0 (LO);/* Shift LSW into place	*/
		AF=PASS SR0;		/* Need to test for zero result	*/
		AF=SR1 OR AF;
		IF EQ AR=PASS 0;	/* Zero exponent for zero result*/
		RTS;
		
__downshift:	AR=NOT MR2;
		SR=ASHIFT AR BY 15 (HI);/* Place sign bit of result	*/
		SR=SR OR LSHIFT MR1 BY -1 (HI);/* Place MSW of result	*/
		SR=SR OR LSHIFT MR0 BY -1 (LO);/* Place LSW of result	*/
		AR=AF+1;
		RTS;


___lib_sf_libfp:SR0=MR0, AR=ABS MR1;	/* Get sign of input		*/
		SR1=MR1;
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

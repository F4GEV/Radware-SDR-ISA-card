/*
	This routine performs a single precision IEEE floating point
	normalization.

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 3/11/93


		Calling Parameters
	MR1:MR0 Contains unnormalized number
	SR0	Contains exponent of unnormalized number
	The ABS condition code must be set with the sign
	of the result.

		Result Registers
	SR1:SR0	Contains Product of X and Y

*/
	
.MODULE/IMAGE		IEEE_Single_Normalize;

#include "lib_glob.h"
#include "flt_glob.h"

.ENTRY	___lib_fpnorm;

___lib_fpnorm:	IF POS JUMP __norm_pos;
		AR=-MR0;		/* Take negative of res		*/
		MR0=AR, AR=-MR1+C-1;	/* in double precision		*/
		MR1=AR;			/* Set AR to unsigned value	*/

__norm_pos:	AF=PASS MR1;		/* Test for a zero		*/
		AR=MR0 OR AF;		/* If this equals zero, return 0*/
		IF EQ JUMP __return_zero;
		SE=EXP MR1 (HI);	/* Check number of signs	*/
		SE=EXP MR0 (LO);	/* Check lower word also	*/
		AY0=7;			/* For normalized number	*/
		AX0=SE;			/* Number of sign bits		*/
		AR=AX0+AY0;
		SE=AR, AF=PASS SR0;	/* Get ready for shift		*/
		SR=NORM SI (LO);
		SR0=SR1;
		SR1=0;
		AX0=AR, SR=SR OR NORM MR0 (LO);/* Place LSW of result		*/
		SE=AY0, SR=SR OR NORM MR1 (HI);/* Place MSW of result	*/
		AY0=0x7f;		/* Mask out implicit 1		*/
		AR=SR1 AND AY0;		/* 1 is removed			*/
		SR1=AR, AR=AX0+AF;	/* Compute new exponent		*/
		AR=SE, SR=SR OR LSHIFT AR (HI);/* Place exponent	*/
		IF POS AR=PASS 0;	/* AR was set to 7 above	*/
		SR=SR OR LSHIFT AR BY 15 (HI);/* Place sign bit		*/
		RTS;

__return_zero:	SR=LSHIFT MR0 BY 100 (HI);
		RTS;
.ENDMOD;


/*
	This routine performs a single precision IEEE floating point multiply

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 3/1/93
*/
	

.MODULE/IMAGE		IEEE_Single_Multiply;

/*		Calling Parameters
	MX1:MX0	Contains X parameter
	MY1:MY0	Contains Y parameter

		Result Registers
	SR1:SR0	Contains Product of X and Y

	
*/

#include "lib_glob.h"
#include "flt_glob.h"

.ENTRY	___lib_fpmult;

/*     Here's the scoop.  We have two IEEE 32-bit floating point numbers that 	*/
/* need to be multiplied.  The first thing we need to do is determine the sign	*/
/* which is based on the XOR of the two input sign bits.  Next we isolate the 	*/
/* mantissa of the inputs and do an unsigned multiply.				*/


___lib_fpmult:	SE=1;				/* Another constant		*/
		MR1=0x0100;			/* Constant used alot		*/
		AY1=F_EXP_BIAS;			/* Load exponent bias		*/
		SI=MX1, MF=MR;			/* Pass to MF for later		*/
		SI=MY1, SR=ASHIFT SI (LO);	/* Shift sign into SR1		*/
		AY0=SR1, MR=SR0*MF (UU);	/* Copy exp to MR1, sign to AY0	*/
		MX1=MR0, SR=ASHIFT SI (LO);	/* Copy Mantissa part 		*/
		MR0=MR2, AR=SR1 XOR AY0;	/* Clear out lower MR register	*/
		AR=ABS AR;			/* Set AS bit			*/
		AF=PASS MR1;			/* Test for X == zero		*/
		IF NE AF=PASS SR0;		/* Test for Y == zero		*/
		IF EQ JUMP __return_zero;	/* Return signed zero		*/
		AY0=MR1, MR=MR+SR0*MF (UU);	/* Added exponents together 	*/
		MY1=MR0, AR=MR1-AY1;		/* Biased exponent in AR 	*/
		AY1=9;				/* Need to bias exp for EXP op	*/
		MR0=MR2, AR=AR+AY1;		/* Bias exponent for normal	*/
		MR1=0x0080;
		MF=MR;
		MR=MR+MR1*MY1(UU);
		MY1=MR1, MR=0;
		MR1=0x0080;
		MR=MR+MX1*MF (UU);

__compute_prod:	MX1=MR1, MR=MX0*MY0 (UU);	/* Compute lower word		*/
		MR0=MR1;			/* Downshift by 16 bits		*/
		MR1=MR2;			/* Downshift by 16 bits		*/
		MR=MR+MX0*MY1 (UU);		/* Compute lowera*upperb	*/
		MR=MR+MX1*MY0 (UU);		/* Compute uppera*lowerb	*/
		SI=MR0;				/* Used for guard bits		*/
		MR0=MR1;			/* Downshift 16 bits		*/
		MR1=MR2;			/* Downshift 16 bits		*/
		SR0=AR, MR=MR+MX1*MY1 (UU);	/* Compute uppera*upperb	*/
		AR=MR1, AF=PASS MR0;
		IF NEG AF=-AF;
		IF NEG AR=-AR+C-1;
		MR1=AR, AR=PASS AF;
		MR0=AR;
		JUMP ___lib_fpnorm;

__return_zero:	SR=LSHIFT AR BY 15 (HI);	/* Set sign of return		*/
		RTS;
.ENDMOD;


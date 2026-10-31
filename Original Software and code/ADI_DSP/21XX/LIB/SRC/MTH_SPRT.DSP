/*
	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 3/1/93
*/
	

.MODULE		IEEE_Single_Multiply;

/*		Calling Parameters
	MX1:MX0	Contains X parameter
	MY1:MY0	Contains Y parameter

		Result Registers
	SR1:SR0	Contains Product of X and Y

	
*/

#include "flt_glob.h"

.ENTRY	___lib_fpmult;

/*     Here's the scoop.  We have two IEEE 32-bit floating point numbers that 	*/
/* need to be multiplied.  The first thing we need to do is determine the sign	*/
/* which is based on the XOR of the two input sign bits.  Next we isolate the 	*/
/* mantissa of the inputs and do an unsigned multiply.				*/


___lib_fpmult:	SE=1;				/* Another constant		*/
		MR1=0x0080;			/* Constant used alot		*/
		AY1=F_EXP_BIAS;			/* Load exponent bias		*/
		SI=MX1, MF=MR;			/* Pass to MF for later		*/
		SI=MY1, SR=LSHIFT SI (LO);	/* Shift sign into SR1		*/
		AY0=SR1, MR=SR0*MF (UU);	/* Copy exp to MR1, sign to AY0	*/
		MX1=MR0, SR=LSHIFT SI (LO);	/* Copy Mantissa part 		*/
		MR0=MR2, AF=SR1 XOR AY0;	/* Clear out lower MR register	*/
		MR=MR+SR0*MF (UU);		/* Add exponents together 	*/
		AR=MR1-AY1;			/* Biased exponent in AR 	*/
		MY1=0x0040;			/* Shift for mantissa part	*/
		MR1=0x0080;			/* Copy explicit 1 to MR	*/
		MF=MR+MR0*MY1 (UU);		/* Make 1. explicit for Y MSM	*/
		MR=MR+MX1*MY1 (UU);		/* Make 1 explicit for X MSM	*/

compute_prod:	MX1=MR1, MR=MX0*MY0 (UU);	/* Compute lower word		*/
		MR0=MR1;			/* Downshift by 16 bits		*/
		MR1=MR2;			/* Downshift by 16 bits		*/
		MR=MR+MX0*MF (UU);		/* Compute lowera*upperb	*/
		MR=MR+MX1*MY0 (UU);		/* Compute uppera*lowerb	*/
		MR0=MR1;			/* Downshift 16 bits		*/
		MR1=MR2;			/* Downshift 16 bits		*/
		MR=MR+MX1*MF (UU);		/* Compute uppera*upperb	*/

normalize_prod:	SE=EXP MR1 (HI);		/* Determine redundant sign bits*/
		AY0=9;
		AR=SE;
		AR=AR+AY0;
		SE=AR;
		SR=LSHIFT MR0 (LO);
		SR=SR OR LSHIFT MR1 (HI);
		AY0=0

.ENDMOD;


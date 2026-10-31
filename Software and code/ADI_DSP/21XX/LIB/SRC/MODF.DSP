/*
    	This C callable subroutine breaks a floating point number into
	integral and fractional parts.

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

    	Created on 5/30/93

	#include <math.h>			<Header>	
	float modff(float x, float *ptr);	<Prototype>
	double modf(double x, double *ptr);	<Prototype>
*/

.MODULE/IMAGE	__modf_;

#include	"lib_glob.h";
#include 	"flt_glob.h";

.ENTRY		modff_, modf_;

modf_:
modff_:		MR1=TOPPCSTACK;
		CALL ___lib_save_small_frame;

		reads(MR1,I6,M5);
		reads(MR0,I6,M5);
		SR=LSHIFT MR1 BY 1 (HI);	/* Move exp up		*/
		SR=LSHIFT SR1 BY -8 (LO);	/* Move exp into MSBs	*/
		AY1=F_EXP_BIAS;
		AR=SR0-AY1, reads(MY1,I6,M5);	/* Compute unbiased exp	*/
		I6=MY1;				/* Hold pointer 	*/
		IF LT JUMP __all_fraction;	/* No integer part	*/
		AY1=22;				/* Check for no 	*/
		AF=AR-AY1;			/*  fractional part	*/
		IF GT JUMP __no_fraction;	/* All integer number	*/
		MY1=AR, AR=AR-AY1;		/* Compute mask shift	*/
		AR=-AR;
		SE=AR, AR=PASS -1; 		/* Mask out fract part  */
		AY1=MR0, SR=ASHIFT AR (LO);
		AY1=MR1, AR=SR0 AND AY1;
		SR0=AR, AR=SR1 AND AY1;
		SR1=AR;
		DM(I6,M5)=SR1;
		DM(I6,M5)=SR0;

		MX1=MR1;
		MX0=MR0;
		AY1=0x8000;
		MY0=SR0, AR=SR1 XOR AY1;
		MY1=AR;
		CALL ___lib_fpadd;
		
__restore_state:JUMP ___lib_restore_small_frame;

__all_fraction:	DM(I6,M5)=0;
		DM(I6,M5)=0;
		SR1=MR1;
		SR0=MR0;
		JUMP __restore_state;


__no_fraction:	DM(I6,M5)=MR1;
		DM(I6,M5)=MR0;
		SR=LSHIFT MR0 BY -100 (LO);
		JUMP __restore_state;
.ENDMOD;

		

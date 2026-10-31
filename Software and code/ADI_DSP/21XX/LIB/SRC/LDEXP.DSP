/*
    	This C callable function adds an integer value to a floating
	point exponent.

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

    	Created on 5/31/93

	#include <math.h>			<Header>	
	float ldexpf(float x, int exp);		<Prototype>
	double ldexp(double x, int exp);	<Prototype>
*/

.MODULE/IMAGE	__ldexp__;

#include	"lib_glob.h";

.ENTRY		ldexpf_, ldexp_;

ldexp_:
ldexpf_:	MR1=TOPPCSTACK;
		CALL ___lib_frame_swap;

		reads(MR1,I6,M5);		/* Read MSW of float	*/

		AY1=0x7f80;			/* Exponent mask	*/
		AR=MR1 AND AY1, reads(MR0,I6,M5);/* Mask out all but exp*/
		SR=LSHIFT AR BY -7 (LO);	/* Exp now in LSBs	*/
		AF=NOT AY1, reads(AY1,I6,M5);	/* Mask for exp		*/
		AR=MR1 AND AF;			/* Mask out exponent	*/
		SR1=AR, AR=SR0+AY1;		/* Compute new exponent	*/	
		IF LE JUMP __underflow;		/* Value is too small	*/
		AY1=F_EXP_BIAS+F_EXP_BIAS+1;	/* Check for exp to big	*/
		AF=AR-AY1;
		IF GT JUMP __overflow;		/* Exponent is too large*/
		SR=SR OR LSHIFT AR BY 7 (HI);	/* Move exponent into	*/
		SR0=MR0;
		
__restore_state:JUMP ___lib_frame_restore;

__overflow:	SR=ASHIFT SI BY -100 (HI);	/* Fill with 0xffffffff	*/
		JUMP __restore_state;

__underflow:	SR=LSHIFT SI BY -100 (HI);	/* Fill with zeros	*/
		JUMP __restore_state;

.ENDMOD;

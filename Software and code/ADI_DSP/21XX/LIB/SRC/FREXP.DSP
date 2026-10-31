/*
    	This C callable function breaks a floating point number into a 
    	normalized fraction and an integral power of 2.

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

    	Created on 5/31/93

	#include <math.h>			<Header>	
	float frexpf(float x, int *exp);	<Prototype>
	double frexp(double x, int *exp);	<Prototype>
*/

.MODULE/IMAGE	__frexp__;

#include	"lib_glob.h";

.ENTRY		frexpf_, frexp_;

frexp_:
frexpf_:
__frexpD:
__frexpfD:	I1=^__dm_write;
		JUMP __frexp_core;

__dm_write:	I6=MY1;				/* Set address for write*/
		DM(I6,M5)=MR1;			/* Write to data mem	*/
		RTS;				/* Finish routine	*/

__frexpP:	
__frexpfP:	I1=^__pm_write;
		JUMP __frexp_core;

__pm_write:	I6=MY1;				/* Set address for write*/
		PM(I6,M5)=MR1;			/* Write to program mem	*/
		RTS;				/* Finish routine	*/

__return_zeros:	MR=0, reads(MY1,I6,M5);		/* Read in address	*/
		I6=I1;				/* Point to proper write*/
		CALL (I6);			/* Write out a zero	*/
		SR=LSHIFT MR1 BY -100 (LO);	/* Zero out return reg	*/
		JUMP __restore_state;		/* Return to caller	*/

__frexp_core:	MR1=TOPPCSTACK;
		CALL ___lib_frame_swap;

		reads(MR1,I6,M5);		/* Read MSW of float	*/

		AY1=0x7f80;			/* Exponent mask	*/
		AR=MR1 AND AY1, reads(MR0,I6,M5);/* Mask out all but exp*/
		IF EQ JUMP __return_zeros;	/* Input is zero	*/
		SR=LSHIFT AR BY -7 (LO);	/* Exp now in LSBs	*/
		AF=NOT AY1, reads(MY1,I6,M5);	/* Mask for exp		*/
		AY1=F_EXP_BIAS-1;		/* Load exp bias - 1	*/
		SI=AY1, AR=SR0-AY1;		/* Compute exp for save	*/
		MR1=AR, AF=MR1 AND AF;		/* Removed exp from inp	*/
		SR=LSHIFT SI BY 7 (LO);		/* Shift new exponent	*/
		SR0=MR0, AR=SR0 OR AF;		/* Return value [.5,1)	*/
		SR1=AR; 
		I6=I1;				/* Get address of writer*/
		CALL (I6);			/* Write specified mem	*/
		
__restore_state:JUMP ___lib_frame_restore;

.ENDMOD;

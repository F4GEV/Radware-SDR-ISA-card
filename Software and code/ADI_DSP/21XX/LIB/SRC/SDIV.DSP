.MODULE/IMAGE	Signed_Divide;

/* 	This file contains the signed divide routine for 16-bit integers.

	The C Runtime Library for the ADSP21xx family of processors.
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 3/7/93

*/
 	
#include "lib_glob.h"

.ENTRY	___sdiv;

___sdiv:	MR1=TOPPCSTACK;		/* Get return address for save	*/
		CALL ___lib_save_small_frame;
		
		AX0=AR, AR=AR XOR AY1;	/* Get sign of result		*/
		AX1=AR, AR=ABS AX0;	/* Get absolute value of num	*/
		SR=ASHIFT AR BY 1 (LO);	/* Shift into proper format	*/
		AY1=SR1, AR=PASS AY1;	/* Copy den to X-reg		*/
		AY0=SR0, AR=ABS AR;	/* Get absolute value of den	*/
		CALL ___lib_div_core;	/* Execute the divide		*/
		AR=AY0, AF=ABS AX1;	/* Copy to return register	*/
		IF NEG AR=-AY0;		/* Set sign of result		*/

restore_state:	JUMP ___lib_restore_small_frame;

.ENDMOD;




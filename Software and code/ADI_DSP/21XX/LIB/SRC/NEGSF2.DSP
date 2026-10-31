/* 
	This routine negates a single precision floating point number.

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 3/11/93

*/

.MODULE/IMAGE	__negsf2__;

#include "lib_glob.h"

.ENTRY	__negsf2_;

__negsf2_:	MR1=TOPPCSTACK;
		CALL ___lib_frame_swap;

		reads(SR1,I6,M5);		/* Read MSW of input	*/
		reads(SR0,I6,M5);		/* Read LSW of input	*/
		AY1=0x8000;			/* Used to flip sign bit*/
		AR=SR1 XOR AY1;			/* Flip sign of input	*/
		SR1=AR;				/* Set MSW for return	*/

__restore_state:JUMP ___lib_frame_restore;	/* Return to caller	*/

.ENDMOD;


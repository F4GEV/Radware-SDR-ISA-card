/* 
	This routine shifts an unsigned long integer 
	to the right.

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 5/9/93

*/

.MODULE/IMAGE	__lshrdi3__;

#include "lib_glob.h"

.ENTRY	__lshrdi3_;

__lshrdi3_:	MR1=TOPPCSTACK;
		CALL ___lib_frame_swap;

		reads(MR1,I6,M5);		/* Read MSW of X	*/
		reads(MR0,I6,M5);		/* Read LSW of X	*/
		reads(AR,I6,M5);		/* Read MSW of Y	*/
		
		AY1=SE, AR=-AR;			/* Need to negate shift	*/	
		SE=AR;				/* value for right shift*/

		SR=LSHIFT MR0 (LO);		/* Shift in LSW		*/
		SE=AY1, SR=SR OR LSHIFT MR1 (HI);/* Shift in MSW	*/

__restore_state:JUMP ___lib_frame_restore;	/* Return to caller	*/

.ENDMOD;


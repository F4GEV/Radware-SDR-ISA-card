/* 
	This routine multiples two long integer numbers.

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 5/9/93
*/

.MODULE/IMAGE	__muldi3__;

#include "lib_glob.h"

.ENTRY	__muldi3_;

__muldi3_:	MR1=TOPPCSTACK;
		CALL ___lib_frame_swap;

		reads(SR1,I6,M5);		/* Read MSW of X	*/
		reads(SR0,I6,M5);		/* Read LSW of X	*/
		reads(MR1,I6,M5);		/* Read MSW of Y	*/
		MF=MR, reads(MY1,I6,M5);	/* Read LSW of Y	*/

		MR=SR0*MY1 (UU);		/* Compute LSW*LSW	*/
		MR0=MR1, AR=PASS MR0;		/* Shift down by 16	*/
		MR1=MR2;			
		MR=MR+SR1*MY1 (SU);		/* Compute MSW*LSW+above*/
		SR0=AR, MR=MR+SR0*MF (US);	/* Compute LSW*MSW+above*/
		SR1=MR0;			/* Set final return val	*/

__restore_state:JUMP ___lib_frame_restore;	/* Return to caller	*/

.ENDMOD;


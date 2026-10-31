/* 
	This routine subtracts two long integer numbers.

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 4/22/93

*/

.MODULE/IMAGE	__subdi3__;

#include "lib_glob.h"

.ENTRY	__subdi3_;

__subdi3_:	MR1=TOPPCSTACK;
		CALL ___lib_frame_swap;

		reads(SR1,I6,M5);		/* Read MSW of X	*/
		reads(SR0,I6,M5);		/* Read LSW of X	*/
		AF=PASS SR0, reads(AY1,I6,M5);	/* Read MSW of Y	*/
		reads(SR0,I6,M5);		/* Read LSW of Y	*/

		AR=AF-SR0;			/* Subtract two LSWs	*/
		SR0=AR, AR=SR1-AY1+C-1;		/* Subtract two MSWs	*/
		SR1=AR;

__restore_state:JUMP ___lib_frame_restore;	/* Return to caller	*/

.ENDMOD;


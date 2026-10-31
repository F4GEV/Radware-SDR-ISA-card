/* 
	This routine adds two long integer numbers.

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 4/22/93

*/

.MODULE/IMAGE	__adddi3__;

#include "lib_glob.h"

.ENTRY	__adddi3_;

__adddi3_:	MR1=TOPPCSTACK;
		CALL ___lib_frame_swap;

		M5=1;
		I6=1;
		MODIFY(I6,M4);

		reads(SR1,I6,M5);		/* Read MSW of X	*/
		reads(SR0,I6,M5);		/* Read LSW of X	*/
		reads(AY1,I6,M5);		/* Read MSW of Y	*/
		AF=PASS SR0, reads(SR0,I6,M5);	/* Read LSW of Y	*/

		AR=SR0+AF;			/* Add to LSWs		*/
		SR0=AR, AR=SR1+AY1+C;		/* Add to MSWs in double*/
		SR1=AR;

__restore_state:JUMP ___lib_frame_restore;	/* Return to caller	*/

.ENDMOD;


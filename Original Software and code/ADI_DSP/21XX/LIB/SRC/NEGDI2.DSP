/* 
	This routine negates a long integer number.

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 3/11/93

*/

.MODULE/IMAGE	__negdi2__;

#include "lib_glob.h"

.ENTRY	__negdi2_;

__negdi2_:	MR1=TOPPCSTACK;
		CALL ___lib_frame_swap;

		reads(SR1,I6,M5);		/* Read MSW of input	*/
		reads(SR0,I6,M5);		/* Read LSW of input	*/
		AR=-SR0;			/* Negate the number in	*/
		SR0=AR, AR=-SR1+C-1;		/* double precision	*/
		SR1=AR;				/* Set MSW for return	*/

__restore_state:JUMP ___lib_frame_restore;	/* Return to caller	*/

.ENDMOD;


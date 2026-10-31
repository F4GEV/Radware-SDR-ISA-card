/*
    	This C callable subroutine returns the absolute value of
	it's integer input.

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

    	Created on 5/7/93

	#include <stdlib.h>
	int abs(int x);
*/

.MODULE/IMAGE	__abs_;

#include	"lib_glob.h";

.ENTRY		abs_;

abs_:		MR1=TOPPCSTACK;		/* Get return address		*/
		CALL ___lib_frame_swap;	/* Swap stack and frame		*/

		AR=ABS AR;		/* Take absolute value of input	*/

__restore_state:JUMP ___lib_frame_restore;

.ENDMOD;

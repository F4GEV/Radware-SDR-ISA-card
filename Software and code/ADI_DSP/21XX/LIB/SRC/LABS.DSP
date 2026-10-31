/*
    	This C callable subroutine returns the absolute value of
	it's long integer input.

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

    	Created on 5/7/93

	#include <stdlib.h>
	long int labs(long int x);
*/

.MODULE/IMAGE	__labs_;

#include	"lib_glob.h";
#include 	"sdl_glob.h";

.ENTRY		labs_;

labs_:		MR1=TOPPCSTACK;		/* Get return address		*/
		CALL ___lib_frame_swap;	/* Swap stack and frame		*/

		reads(SR1,I6,M5);	/* Read MSW first		*/
		AF=ABS SR1, reads(SR0,I6,M5);/* Read LSW 		*/
		IF POS JUMP __restore_state;
		AR=-SR0;		/* Negate LSW first		*/
		SR0=AR, AR=-SR0+C-1;	/* Now negate MSW in double prec*/
		SR1=AR;			/* Copy to return registers	*/

__restore_state:JUMP ___lib_frame_restore;

.ENDMOD

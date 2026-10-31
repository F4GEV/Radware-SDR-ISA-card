.MODULE/IMAGE	Compare_floats_for_EQ;

/* 
	This routine is part of the floating point support library.  It
	determines if two floating point numbers are equal to each other.

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 3/13/93

	Rewritten on 2/4/94 by AS for better efficiency - only use scratch regs.

*/

#include "lib_glob.h"

.ENTRY 		__eqsf2_;

__eqsf2_:	MR1=TOPPCSTACK;
		CALL ___lib_frame_swap;

		reads(SR1,I6,M5);
		reads(SR0,I6,M5);
		AR=PASS -1, reads(AY1,I6,M5);
		AF=SR1-AY1, reads(AY1,I6,M5);

		IF EQ AF=SR0-AY1;	/* Test LSWs for EQ		*/
		IF EQ AR=PASS 0;	/* Entire word is EQ so EQ	*/

__restore_state:JUMP ___lib_frame_restore;

.ENDMOD;



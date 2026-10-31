/*
	This file converts a floating point number to a long
	precision integer.

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 4/22/93

	Updated on 10/7/93 - (AS) improved (parallelized) 
	instruction execution by one cycle by combining reading 
	the lsw of the float coming in, with taking the abs of 
	the msw.

*/

.MODULE/IMAGE	__fixsfdi__;

#include "lib_glob.h"

.ENTRY __fixsfdi_;

__fixsfdi_:	MR1=TOPPCSTACK;
		CALL ___lib_frame_swap;

		reads(MR1,I6,M5);
		reads(MR0,I6,M5), AF=ABS MR1;/* Get sign of result	*/
		SR=LSHIFT MR1 BY 1 (LO);/* Remove sign		*/
		SR=LSHIFT SR0 BY -8 (LO);/* Place biased exp	*/
		AY1=F_EXP_BIAS+23;
		AR=SR0-AY1;		/* Compute unbiased exponent	*/
		SE=AR;
		SR=LSHIFT MR1 BY 9 (LO);/* Removed sign and exponent	*/
		SR=LSHIFT SR0 BY -9 (LO);/* Place exponent		*/
		AY1=0x0080;		/* Implicit 1			*/
		AR=SR0 OR AY1;		/* made explicit		*/
		SR=LSHIFT MR0 (LO);	/* Place LSW of number		*/
		SR=SR OR LSHIFT AR (HI);/* Place MSW of number		*/
		AR=SR0, AF=PASS SR1;	/* Copy result for possible neg	*/
		IF NEG AR=-SR0;		/* Negate LSW if SGN bit set	*/
		IF NEG AF=-SR1+C-1;	/* Negate MSW if SGN bit set	*/
		SR0=AR, AR=PASS AF;	/* Now return result to result	*/
		SR1=AR;			/* registers			*/

		
__restore_state:JUMP ___lib_frame_restore;

.ENDMOD;

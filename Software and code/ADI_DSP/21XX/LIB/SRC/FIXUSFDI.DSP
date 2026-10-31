/*
	This file converts a floating point number to an unsigned
	double precision integer.

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 5/25/93

	Updated on 2/4/94 by AS.  Inserted frame swap instead of frame save
*/

.MODULE/IMAGE	__fixunssfdi__;

#include "lib_glob.h"

.ENTRY __fixunssfdi_;

__fixunssfdi_:	MR1=TOPPCSTACK;
		CALL ___lib_frame_swap;

		reads(MR1,I6,M5);
		reads(MR0,I6,M5), AF=ABS MR1; /* Get sign of result	*/

		SR=LSHIFT MR1 BY 1 (LO);/* Remove sign			*/
		SR=LSHIFT SR0 BY -8 (LO);/* Place biased exp		*/
		AY1=F_EXP_BIAS;
		AR=SR0-AY1;		/* Compute unbiased exponent	*/
		AY1=23;			/* Compute offset for a 1.0	*/
		AR=AR-AY1;
		SE=AR;
		SR=LSHIFT MR1 BY 9 (LO);/* Removed sign and exponent	*/
		SR=LSHIFT SR0 BY -9 (LO);/* Place exponent		*/
		AY1=0x0080;		/* Implicit 1			*/
		AR=SR0 OR AY1;		/* made explicit		*/
		SR=LSHIFT MR0 (LO);	/* Place LSW of number		*/
		SR=SR OR LSHIFT AR (HI);/* Place MSW of number		*/
		
__restore_state:JUMP ___lib_frame_restore;

.ENDMOD;

/*
	This C callable subroutine tests to see if the input is a digit.

	The C Runtime Library for the ADSP21xx family of processors.
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 3/25/93

    	#include <ctype.h>;
    	int isdigit(int c);
*/

/* NOTE: A copy of this routine exists in fprintf.			*/
 	
.MODULE/IMAGE	__Isdigit_Routine;

#include "lib_glob.h"
#include "cty_glob.h"

.ENTRY		isdigit_;

isdigit_:	MR1=AR, AR=PASS 1;
		AY1=57;				/* 57 == ASC('9')	*/
		AF=MR1-AY1;			/* Test for (c > '9')	*/
		IF GT AR=PASS 0;		/* Return false		*/
		AY1=48;				/* 48 == ASC('0')	*/
		AF=MR1-AY1;			/* test for (c < '0')	*/
		IF LT AR=PASS 0;		/* Return false		*/
__restore_state:RTS;

.ENDMOD;

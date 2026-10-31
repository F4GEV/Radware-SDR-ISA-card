/*
	This C callable subroutine tests to see if the input is alphabetic.

	The C Runtime Library for the ADSP21xx family of processors.
	
	Craig Smilovitz
	Development Tools Engineering

	Created on 7/13/93

    	#include <ctype.h>;
    	int isalpha(int c);
*/
 	
.MODULE/IMAGE	__Isalpha_Routine;

#include "lib_glob.h"
#include "cty_glob.h"

.ENTRY		isalpha_;

isalpha_:	
		AY1=95;
		AR=AR AND AY1;			/* convert to upper case*/ 
		AY1=91;				/* 91 == ASC('Z')	*/
		MR1=AR; AR=PASS 1;		/* set up AR return code*/
		AF=MR1-AY1;			/* Test for (c > 'Z')	*/
		IF GT AR=PASS 0;		/* Return false		*/
		AY1=65;				/* 65 == ASC('A')	*/
		AF=MR1-AY1;			/* test for (c < 'A')	*/
		IF LT AR=PASS 0;		/* Return false		*/
__restore_state:RTS;

.ENDMOD;

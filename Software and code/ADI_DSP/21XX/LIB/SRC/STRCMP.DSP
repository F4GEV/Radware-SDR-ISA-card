/*
	This C callable subroutine compares one dm string to
    	another.

	The C Runtime Library for the ADSP21xx family of processors.
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 3/21/93

    	#include <string.h>;
    	int strcmp(const char *s1, const char *s2);
*/
 	
.MODULE/IMAGE		__String_Comp_Routine;

#include "lib_glob.h"
#include "str_glob.h"

.ENTRY		strcmp_;

strcmp_:	I6=AR;			/* Point to output array	*/
		I1=AY1;			/* Point to input array		*/
		M5=1;			/* Used for copy		*/

		AR=PASS 0, MR1=DM(I6,M5); /* Read first character of s1	*/
		DO __comp_dm_dm UNTIL NE;
		   AR=PASS MR1, AY1=DM(I1,dm_1);/* Test for NULL s1	*/
		   IF NE AR=PASS AY1;	/* Test for NULL s2		*/
		   IF EQ JUMP __eos;	/* End of string found		*/
		   AR=MR1-AY1;
__comp_dm_dm:	   MR1=DM(I6,M5); /* Compare and read char	*/

__restore_state:RTS;

__eos:		AR=MR1-AY1;	
		POP LOOP, POP PC;
		RTS;


.ENDMOD;

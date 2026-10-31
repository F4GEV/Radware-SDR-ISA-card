/*
	This C callable subroutine copies one dm string into
    	another.

	The C Runtime Library for the ADSP21xx family of processors.
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 3/21/93

    	#include <string.h>;
    	char *strcpy(char *s1, const char *s2);
*/
 	
.MODULE/IMAGE		__String_Copy_Routine;

#include "lib_glob.h"
#include "str_glob.h"

.ENTRY		strcpy_;

strcpy_:	I6=AR;			/* Point to output array	*/
		I1=AY1;			/* Point to input array		*/
		M5=1;			/* Used for copy		*/

		AY1=DM(I1,dm_1);	/* Read first character		*/
		DO __copy_dm_dm UNTIL EQ;
		   AF=PASS AY1, DM(I6,M5)=AY1;/* Test for NULL and write*/
__copy_dm_dm:	   AY1=DM(I1,dm_1);	/* Read next character		*/

__restore_state:RTS;

.ENDMOD;

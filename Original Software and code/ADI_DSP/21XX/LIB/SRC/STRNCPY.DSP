/*
	This C callable subroutine copies one dm string onto
    	another with a length limit.

	The C Runtime Library for the ADSP21xx family of processors.
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 6/10/93

    	#include <string.h>;
    	char *strncpy(char *s1, const char *s2, int n);
*/
 	
.MODULE/IMAGE		__strncpy__;

#include "lib_glob.h"
#include "str_glob.h"

.ENTRY		strncpy_;

strncpy_:	MR1=TOPPCSTACK;
		CALL ___lib_frame_swap;

		I1=AY1;			/* Point to input array		*/
		reads(AY1,I6,M5);	/* Read length parameter	*/
		I6=AR;			/* Point to output array	*/

		AR=AY1-1, AY1=DM(I1,dm_1);/* Read first character	*/
		IF EQ JUMP __last_write;/* Write out last character	*/
		CNTR=AR;		/* Only copy N characters	*/
		DO __copy_dm_dm UNTIL CE;
		   AF=PASS AY1, DM(I6,M5)=AY1;/* Test for NULL and write*/
		   IF EQ JUMP __pop_loops;
__copy_dm_dm:	   AY1=DM(I1,dm_1);	/* Read next character		*/

__last_write:	DM(I6,M5)=AY1;		/* Write out last character	*/

__restore_state:JUMP ___lib_frame_restore;

__pop_loops:	POP LOOP, POP PC;
		JUMP __restore_state;
.ENDMOD;

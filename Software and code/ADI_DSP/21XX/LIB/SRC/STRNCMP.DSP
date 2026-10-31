/*
	This C callable subroutine compares one dm string to
    	another with a limited length.

	The C Runtime Library for the ADSP21xx family of processors.
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 6/8/93

	AS - Fixed bug found on 11/9/93 - missing a check once the
	characters have been compared at the end of the do loop.

    	#include <string.h>;
    	int strncmp(const char *s1, const char *s2, int n);
*/
 	
.MODULE/IMAGE		__String_Compare_Routine;

#include "lib_glob.h"
#include "str_glob.h"

.ENTRY		strncmp_;

strncmp_:	MR1=TOPPCSTACK;
		CALL ___lib_frame_swap;

		reads(MR1,I6,M5); 	/* Read length of compare	*/
		I6=AR;                  /* Point to output array        */
		I1=AY1;                 /* Point to input array         */

		CNTR=MR1;		/* Loop for N characters MAX	*/
		AR=PASS MR1, MR1=DM(I6,M5); /* Read first character of s1*/
		IF EQ JUMP __restore_state;
		AR=PASS 0;		/* Reset status flags		*/
		DO __compare_dm_dm UNTIL CE;
		   IF NE JUMP __strn_end;
		   AR=PASS MR1, AY1=DM(I1,dm_1);/* Test for NULL s1	*/
		   IF EQ JUMP __strn_end; /* End of string found	*/
__compare_dm_dm:   AR=MR1-AY1, MR1=DM(I6,M5);/* Compare characters	*/

__restore_state:JUMP ___lib_frame_restore;

__strn_end:	AR=MR1-AY1; 
		POP LOOP, POP PC;
		JUMP __restore_state;


.ENDMOD;

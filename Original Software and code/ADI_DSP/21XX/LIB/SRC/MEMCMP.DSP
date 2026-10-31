/*
	This C callable subroutine compares one memory region to
    	another.

	The C Runtime Library for the ADSP21xx family of processors.
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 5/27/93

    	#include <string.h>;
    	int memcmp(const void *s1, const void *s2, size_t n);
*/
 	
.MODULE/IMAGE		__memcmp__;

#include "lib_glob.h"
#include "str_glob.h"

.ENTRY		memcmp_;

memcmp_:	MR1=TOPPCSTACK;
		CALL ___lib_frame_swap;
		
		AF=PASS 1, reads(MR1,I6,M5);/* Get length of compare	*/
		I6=AR;			/* Point to output array	*/
		I1=AY1;			/* Point to input array		*/

		AR=MR1-AF, MR1=DM(I6,M5);/* Do one less loop, read s1	*/
		AY1=DM(I1, dm_1);	/* Read s2			*/
		IF EQ JUMP __loop_end;	/* Only need to loop once	*/
		CNTR=AR;		/* Set count for non-zero loop	*/
		DO __compare_mem UNTIL CE;
		   AF=MR1-AY1, MR1=DM(I6,M5);/* Compare two words	*/
		   IF NE JUMP __eos;	/* Unequal, terminate loop	*/
__compare_mem:	   AY1=DM(I1,dm_1);	/* Read first word of s2	*/

__loop_end:	AR=MR1-AY1;		/* Compare last two values	*/

__restore_state:JUMP ___lib_frame_restore;

__eos:		POP LOOP, POP PC;
		JUMP __restore_state;

.ENDMOD;

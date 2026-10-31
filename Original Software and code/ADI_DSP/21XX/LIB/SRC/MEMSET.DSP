/*
    	This C callable subroutine sets a range of memory equal to a
    	character.

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

    	Created on 4/27/89

    	#include <stdlib.h>;
    	void *memset(void *s1, const char s2, size_t n);
*/

.MODULE/IMAGE	__memset__;

#include	"lib_glob.h";
#include 	"sdl_glob.h";

.ENTRY		memset_;

memset_:	MR1=TOPPCSTACK;			/* Pop return address	*/
		CALL ___lib_frame_swap;		/* Swap frame/stack only*/

		reads(SI,I6,M5);		/* Read length of set	*/

		pm_ptr=AR;			/* Point to buffer	*/
		CNTR=SI;			/* Set length of write	*/
		DO __set_memory UNTIL CE;
__set_memory:	   DM(I6,M5)=AY1;		/* Set buffer to value	*/

__restore_state:JUMP ___lib_frame_restore;	/* Restore frame, return*/

.ENDMOD;

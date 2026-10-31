/*
    This C callable subroutine does copies of one range of dm to
    another.

    DSP Applications Library.		    Gordon A. Sterling

    Created on 3/22/89

    #include <string.h>;
    void *memcpy(void *s1, const void *s2, size_t n);
*/

.MODULE/IMAGE	Memory_Copy_Routine;

#include	"lib_glob.h";
#include	"str_glob.h";

.ENTRY		memcpy_;

memcpy_:	MR1=TOPPCSTACK;
		CALL ___lib_frame_swap;

		reads(SI,I6,M5);		/*Get copy length*/
	
		I6=AR;
		dm_ptr=AY1;
		CNTR=SI;
		DO __copy_dm_dm UNTIL CE;
		   SI=DM(I1,dm_1);
__copy_dm_dm:	   DM(I6,M5)=SI;
		
__restore_state:JUMP ___lib_frame_restore;

.ENDMOD;

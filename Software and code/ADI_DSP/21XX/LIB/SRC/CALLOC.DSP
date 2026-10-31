/*
    This C callable subroutine allocates a range of memory from the
    free memory list.  The allocated memory is initialized to zero.

    DSP Applications Library.		    Gordon A. Sterling

    Created on 4/2/89
    V 1.1  gnu calling convention      7/21/93   Craig Smilovitz 

    Rewritten to eliminate some bugs and to optimize by AS on 3/15/94

    #include <stdlib.h>;
    void *calloc(int nmemb, size_t size);

*/

.MODULE/IMAGE	Clear_Alloced_Memory;

#include	<lib_glob.h>;
/* #include	<slib_glob.h>; */

.EXTERNAL	malloc_;

.ENTRY		calloc_;

calloc_:	MR1=TOPPCSTACK;
		CALL ___lib_save_small_frame;
		
		MY0=AY1;
		MR=AR*MY0 (UU);

		AR=MR0;				/*Call malloc with SR0*/
		AY0=MR0;
		CALL malloc_;			/*Malloc saves registers!*/

		AF=PASS AR;
		I6=AR;
		IF EQ JUMP restore_state;			/*Malloc returned NULL*/
		CNTR=AY0;
		M5=1;
		DO zero_space UNTIL CE;
zero_space:	   DM(I6,M5)=0;			/*Zero out allocated space*/

restore_state:  JUMP ___lib_restore_small_frame;

.ENDMOD;



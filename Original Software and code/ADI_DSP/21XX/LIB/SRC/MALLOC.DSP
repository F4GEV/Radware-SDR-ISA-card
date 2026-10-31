/*
    This C callable subroutine allocates a range of memory from the
    free memory list. 

    DSP Applications Library.		    Gordon A. Sterling

    Created on 8/18/89
    V 1.1  gnu calling convention      7/21/93   Craig Smilovitz 

    Rewritten by GAS and AS on 3/15/94 to improve performance

    #include <stdlib.h>;
    void *malloc(size_t size);

*/

.MODULE/IMAGE		Memory_Allocator;


#include	<lib_glob.h>;

.EXTERNAL	__mem_heap_space;
.EXTERNAL       ___setup_heap_called,___lib_setup_heap;

.ENTRY		malloc_;

malloc_:	M7=-1;
		MR1=TOPPCSTACK;
		CALL ___lib_save_small_frame;
		AX0 = DM(___setup_heap_called);
		AF = PASS AX0;
		IF NE JUMP skip_setup;
		CAll ___lib_setup_heap;
		MY0=1;
		DM(___setup_heap_called) = MY0; /* set flag for future malloc calls */
skip_setup:

		AY1=2;				/*Useful constant*/
		AR=AR+AY1;			/*Fetch size of space(adjusted
						  for control area */
		AX0=AR;				/*AX0=size of block wanted (incl prefix)*/
		I6=^__mem_heap_space;
		MR1=I6;
		MR0=MR1;

		DO scan_free UNTIL LE;
		   AR=PASS MR0, AY0=dm(I6,M5);	/*AR=previous ptr,AY0=length of this block*/
		   MR0=MR1;			/*Could be changed to MR=MR1*MY0*/
		   AF=AX0-AY0, MR1=DM(I6,M5);	/*AF=amt block < needed, MR1= next address*/
		   IF GT AF=PASS MR1;		/*Test for valid address*/
scan_free:	   I6=MR1;			/*I6=ptr for next block */

		AR=AX0-AY0, SR1=AR;
		IF GT JUMP restore_state;

block_found:					/*Check for close sizes*/
		AF=AR+AY1, AY1=AR;			/*if Free_size-alloc_size=0,1,2*/
		IF GE JUMP same_size;		/* then use it, don't truc. */
		AF=AY1-MR0, SI=DM(I6,M7);	/*AF=new pointer - 2 */
		AR=AY0-AX0,SI=DM(I6,M7);	/* calculate new size */
		DM(I6,M5)=AR, AR=PASS AF;	/* ar is length of shortened block */
						/* don't change next block pointer */
						/* recover AF */
		I6=MR0;				/* Point to current block	*/
		AR=-AF;				/* Find size of block		*/
		DM(I6,M5)=AR;			/* Set new length		*/
		AR=MR0-AF;			/* Point to start of new block	*/

		I6=AR;
		DM(I6,M5)=AY1;			/* Write out length of block	*/

		AR=I6;

restore_state:	JUMP ___lib_restore_small_frame;

same_size:	I6=SR1;				/* Point to oldest block	*/
		MODIFY(I6,M5);			/* Skip over size		*/
		DM(I6,M5)=MR1;			/* Point to next block!		*/
		AF=PASS MR1;
		AR=AF+1;			/*Add 2 to next ptr and return*/
		JUMP restore_state;
 
.ENDMOD;




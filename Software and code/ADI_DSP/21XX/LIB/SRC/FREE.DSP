/*
    This C callable subroutine deallocates a range of memory that 
    was previously allocated by malloc, calloc, or realloc

    DSP Applications Library.		    Gordon A. Sterling

    Created on 4/3/89
    V 1.1  gnu calling convention      7/21/93   Craig Smilovitz 


*/

.MODULE/IMAGE	Free_Memory_Routine;

#include	<lib_glob.h>;
 /* #include	<slib_glob.h>; */

.EXTERNAL	__mem_heap_space;			/*This is defined in malloc*/

.ENTRY		free_;

free_:		MR1=TOPPCSTACK;
		CALL ___lib_save_small_frame;

		AX0=AR;			/*Fetch deallocate pointer*/
		M7=-1;
		
		I6=AX0;
		SI=DM(I6,M7);
		SI=DM(I6,M7);
		SI=DM(I6,M7);			/*SI holds length*/

		I6=^__mem_heap_space;	
		AY0=^__mem_heap_space; 
		AX1=NULL;
		AR=PASS AY0, AY1=DM(I6,M5);
		AY0=DM(I6,M5);
		I6=AY0;
		AF=AX1-AY0;
		IF EQ JUMP end_of_list;
		DO find_post UNTIL EQ;
		   AR=PASS AY0, AY1=DM(I6,M5);
		   AF=AX0-AY0, AY0=DM(I6,M5);
		   IF LT JUMP middle_of_list;
		   AF=AX1-AY0;
find_post:	   I6=AY0;

    /* dealloc->AR AR holds free area after*/

		JUMP end_of_list;

middle_of_list:	POP PC, POP LOOP;
		AY1=SI, AF=PASS AR;
		AR=AX0+AY1;
		AR=AR-AF, AX1=SI;		/*Check for contigious blocks*/
		IF EQ JUMP contigious;
		I6=AX0;				/*Point to dealloc space*/
		AR=PASS AF, AY0=DM(I6,M7);	/*Dummy read to move pointer*/
		DM(I6,M5)=AR;			/*Store next-pointer*/
		JUMP get_precursor;

contigious:	AR=PASS AF;			/*Get pointer from AF*/
		I6=AR;				/*Point to past free space*/
		AY1=DM(I6,M5);			/*Blocks are contigious*/
		AR=AX1+AY1;			/*Add free spaces*/
		AY1=2;		    
		AR=AR+AY1, AY1=DM(I6,M5);	/*System spaces becomes free*/
		I6=AX0;
		AY0=DM(I6,M7);
		DM(I6,M7)=AY1;			/*Store new next-pointer*/
		DM(I6,M7)=AR;			/*Store new length*/
		SI=AR, AR=PASS AF;		/*Also move new length into SI*/

get_precursor:	I6=^__mem_heap_space;	
		AY0=^__mem_heap_space;
		DO find_previous UNTIL LT;
		   AR=PASS AY0, AY0=DM(I6,M5);
		   AY0=DM(I6,M5);		/*Get next space address*/
		   AF=AX0-AY0;
find_previous:	   I6=AY0;
		I6=AR;				/*AR holds previous array add*/

end_of_list:	I6=AX0;				/*Deallocated space pointer*/
		AY0=DM(I6,M7);			/*Dummy read to move pointer*/
		AY0=DM(I6,M7);			/*Read in next-pointer*/
		I6=AR;				/*Point to precursor*/
		AY1=DM(I6,M5);
		AF=AR+AY1;
		AX1=4;
		AF=AX1+AF;
		AR=AF-AX0;
		IF EQ JUMP pre_contig;
		AY1=2;
		AR=AX0-AY1;
		DM(I6,M5)=AR;
		JUMP restore_state;

pre_contig:	AX0=SI;
		AR=AX0+AY1, DM(I6,M7)=AY0;	/*Move next-pointer up */
		AY1=2;
		AR=AR+AY1;			/*Calculate new length*/
		DM(I6,M5)=AR;			/*Store new length*/

restore_state:	AR=AX0;			/*use correct return reg */
		JUMP ___lib_restore_small_frame;

.ENDMOD;




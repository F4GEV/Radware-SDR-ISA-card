/*
    	This function computes the (FIR) filtered response of a given 
	input sample. The function returns the filtered value of the input.

	The C Runtime Library for the ADSP21xx family of processors.
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

    	Created on    8/1/93

    	#include <filters.h>;   
	int fir(int input[], int coeffs[], int state[], int TAPS, int WINDOW_SIZE);
*/

.MODULE/IMAGE		__fir__;

#include "lib_glob.h"
#include "trn_glob.h"

.ENTRY		firbuf_;

firbuf_:	MR1=TOPPCSTACK;		/* Get return address 		*/
		MR0=-1;			/* Now frame space needed	*/
		CALL ___lib_save_frame;/* Save all registers	*/

		I0=AR;			/* Hold input sample		*/
		MR1=AY1;		/* Hold coeff pointer		*/

		reads(SR0,I6,M5);	/* Fetch pointer to state array	*/
		reads(AY1,I6,M5);	/* Fetch number of TAPS		*/
		reads(AX0,I6,M5);	/* Fetch buffer size		*/

		AR=AY1+1;		/* Compute TAPS+1 for read	*/
		dm_ptr=SR0;		/* Point to beginning of state	*/
		dm_lnt=AR;		/* Set length to TAPS+1		*/
		dm_mdf=-1;
	
		MODIFY(dm_ptr, dm_mdf);	/* Modify by -1, point to ptr	*/
		I5=dm_ptr;		/* Hold address of state ptr	*/
		AR=DM(dm_ptr, dm_1);	/* Load state pointer		*/
		AR=PASS AR;		/* Test for first invocation	*/
		IF EQ AR=PASS SR0;	/* Point to beginning of array	*/
		L6=AY1;			/* Set buffer wrap for coeff	*/
		dm_lnt=AY1;		/* Set length to num of delays	*/
		dm_ptr=AR;		/* Point to current location	*/
		AR=AY1-1;		/* Write new pointer into state	*/	 
		I6=MR1;			/* Point to coeff array		*/
		SR0=MSTAT;		/* Save mode for later		*/
		DIS M_MODE;		/* Enter integer mode		*/

		CNTR=AX0;		/* Loop over entire buffer	*/
		DO __window_loop UNTIL CE;
		   SI=DM(I0,M2);	/* Read input from buffer	*/
		   DM(dm_ptr, dm_1)=SI;	/* Write input into delay	*/
		   MR=0, MX0=DM(dm_ptr, dm_1), MY1=PM(I6,M5);
		   CNTR=AR;		/* Loop TAPS-1			*/
		   DO __convolution UNTIL CE;
__convolution:	      MR=MR+MX0*MY1 (SS), MX0=DM(dm_ptr, dm_1), MY1=PM(I6,M5);
		   MR=MR+MX0*MY1 (RND);
		   IF MV SAT MR;
__window_loop:	   DM(I0,M1)=MR1;

		MR0=I0;			/* Get current pointer		*/		
		DM(I5,M5)=MR0;

		dm_lnt=0;
		L6=0;
		MSTAT=SR0;		/* Restore old mode		*/
		
__restore_state:MR0=-1;
		JUMP ___lib_restore_frame;

.ENDMOD;

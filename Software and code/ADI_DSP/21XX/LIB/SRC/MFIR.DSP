/*
    	This function computes the (FIR) filtered response of a given 
	input sample. The function returns the filtered value of the input.

	The C Runtime Library for the ADSP21xx family of processors.
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

    	Created on    8/1/93

    	#include <filters.h>;   
	int fir(int sample, int coeffs[], int state[], int TAPS);
*/

.MODULE/IMAGE		__fir__;

#include "lib_glob.h"
#include "trn_glob.h"

.ENTRY		fir_;

fir_:		MR1=TOPPCSTACK;		/* Get return address 		*/
		CALL ___lib_frame_swap;	/* Setup frame only		*/

		MR0=AR;			/* Hold input sample		*/
		MR1=AY1;		/* Hold coeff pointer		*/

		reads(SR0,I6,M5);	/* Fetch pointer to state array	*/
		reads(AY1,I6,M5);	/* Fetch number of TAPS		*/

		AR=AY1+1;		/* Compute TAPS+1 for read	*/
		dm_ptr=SI;		/* Point to beginning of state	*/
		dm_lnt=AR;		/* Set length to TAPS+1		*/
	
		MODIFY(dm_ptr, dm_mdf);	/* Modify by -1, point to ptr	*/
		I6=dm_ptr;		/* Hold address of state ptr	*/
		AR=DM(dm_ptr, dm_1);	/* Load state pointer		*/
		SR1=MX0, AR=PASS AR;	/* Test for first invocation	*/
		IF EQ AR=PASS SR0;	/* Point to beginning of array	*/
		dm_lnt=AY1;		/* Set length to num of delays	*/
		dm_ptr=AR;		/* Point to current location	*/
		DM(dm_ptr, dm_1)=MR0;	/* Place input into delay line	*/
		MR0=dm_ptr;		/* Copy pointer for write	*/
		AR=AY1-1, DM(I6,M5)=MR0;/* Write new pointer into state	*/	 
		I6=MR1;			/* Point to coeff array		*/
		SR0=MSTAT;		/* Save mode for later		*/
		DIS M_MODE;		/* Enter integer mode		*/
		MR=0, MX0=DM(I0,M1), MY1=PM(I6,M5);/* Load delay, coeff	*/
		CNTR=AR;		/* Loop TAPS-1			*/
		DO __convolution UNTIL CE;
__convolution:	   MR=MR+MX0*MY1 (SS), MX0=DM(I0,M1), MY1=PM(I6,M5);
		MR=MR+MX0*MY1 (RND);
		IF MV SAT MR;
		MX0=SR1, AR=PASS MR1;	/* Restore MX0 for return	*/
		MSTAT=SR0;		/* Restore old mode		*/
		
__restore_state:JUMP ___lib_frame_restore;

.ENDMOD;

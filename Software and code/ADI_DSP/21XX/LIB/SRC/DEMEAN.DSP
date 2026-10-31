/*
    	This function removes the mean from an input buffer.  The function
	accepts the current mean, and returns the new mean value.

	The C Runtime Library for the ADSP21xx family of processors.
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

    	Created on    8/14/93

	#include <filters.h>
	int demean_buf(int *inbuf, int oldmean, int LENGTH);
*/

.MODULE/RAM	__Demean_Buffer__;

#include 	"lib_glob.h"
#include	"trn_glob.h"

.ENTRY		demean_buffer_;

demean_buffer_:	MR1=TOPPCSTACK;		/* Store return address		*/
		CALL ___lib_frame_swap;	/* Only swap frame !		*/

		MR=0;			/* Clear out mac for loop	*/
		MR1=AY1;		/* Initialize value of mean	*/
		reads(AY1,I6,M5);	/* Read length from stack	*/
		MY1=0x0080;		/* Slow update			*/
		I6=AR;			/* Point to in window		*/
		I1=AR;			/* Point to out window		*/
		AR=AY1-1, AY1=DM(I6,M5);/* Loop one less than buffer	*/
		IF EQ JUMP __last_point;/* Only one point in the buffer!*/
		CNTR=AR;		/* Loop over buffer		*/
		DO __demean_buf UNTIL CE;
		   AR=AY1-MR1, AY1=DM(I6,M5);/* Compute output, get next*/
__demean_buf:	   DM(I1,M1)=AR, MR=MR+AR*MY1 (SS);/* Update mean	*/
__last_point:	AR=AY1-MR1;
		DM(I1,M1)=AR, MR=MR+AR*MY1 (RND);
		AR=PASS MR1;
			
__restore_state:JUMP ___lib_frame_restore;

.ENDMOD;


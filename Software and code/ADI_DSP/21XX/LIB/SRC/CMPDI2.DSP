/* 
	This routine compares to unsigned long integer values to 
	see if how they compare.
		
	This Routine returns {0,1,2} instead of the previous {-1,0,1}

	The C Runtime Library
			
	Alexander Soto              (617) 461 - 3822
	Development Tools Engineering
					
	Created on 5/8/93
	Rewritten on 12/17/93
*/
						
.MODULE/IMAGE   __cmpdi2__;
	
#include "lib_glob.h"

.ENTRY          __cmpdi2_;
				
__cmpdi2_:  MR1=TOPPCSTACK;
	    CALL ___lib_frame_swap;

	    reads(SR1,I6,M5), MR=0;	/* Clear MR to set mr0=0		*/
	    reads(SR0,I6,M5), AF=PASS 1;
	    reads(MR1,I6,M5), AR=AF+1;	/* Set AR=2				*/
	    AF=PASS MR1, reads(AY1,I6,M5);

	    AR=SR0-AY1, MR1=AR;		/* Subtract in double precision, mr1=2	*/
	    AF=SR1-AF+C-1, AR=MR0;	/* Subtract MSWs, AR=0 (less than case)	*/
	    IF EQ AF=SR0-AY1;		/* MSW was zero, so compare LSW		*/
	    IF EQ AR=PASS 1;		/* Numbers are equal, so ret ar=1	*/
	    IF AC AR=PASS MR1;		/* Number must be >, so return ar=2	*/
			

__restore_state:JUMP ___lib_frame_restore;

.ENDMOD;

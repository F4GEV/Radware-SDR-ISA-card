/* 
	This routine shifts a long integer to the left

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 4/22/93

*/

.MODULE/IMAGE	__ashldi3__;

#include "lib_glob.h"

.ENTRY	__ashldi3_;

__ashldi3_:	MR1=TOPPCSTACK;
		CALL ___lib_frame_swap;

		reads(MR1,I6,M5);		/* Read MSW of X	*/
		reads(MR0,I6,M5);		/* Read LSW of X	*/
		reads(AR,I6,M5);		/* Read MSW of Y	*/

		AY1=SE;
		SE=AR;
		SR=LSHIFT MR0 (LO);		/* Shift in LSW		*/
		SE=AY1, SR=SR OR ASHIFT MR1 (HI);/* Shift in MSW	*/

__restore_state:JUMP ___lib_frame_restore;	/* Return to caller	*/

.ENDMOD;


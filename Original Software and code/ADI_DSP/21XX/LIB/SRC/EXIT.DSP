.MODULE/IMAGE	Exit_for_echo;

/* 	
	This file contains the exit routine that writes the exit 
	flag and status to the ECHO board.

	The C Runtime Library for the ADSP21xx family of processors.
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 3/13/93

*/
 	
#include "lib_glob.h"
#include "sdl_glob.h"

.ENTRY		exit_;
.EXTERNAL	___lib_prog_term;

exit_:		I6=0x3008;
		AY1=0;
		MR1=0x8000;
check_for_OK:	AF=MR1 AND AY1, AY1=DM(I6,M6);
		IF EQ JUMP check_for_OK;

		AY1=0x4000;		/* EXIT flag		*/
		DM(0x3000) = AR;	/* Write condition code	*/
		DM(0x3008) = AY1;	/* Write EXIT condition	*/
		JUMP ___lib_prog_term;
.ENDMOD;

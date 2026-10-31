/* 
	This routine is a compiler support routine that
	computes the long modulo.

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 5/8/93

*/

.MODULE/IMAGE	__moddi3__;

#include "lib_glob.h"
#include "int_glob.h"

.ENTRY	__moddi3_;

__moddi3_:	MR1=TOPPCSTACK;
		CALL ___lib_save_small_frame;

		reads(MR1,I6,M5);
		reads(MR0,I6,M5);
		reads(AY1,I6,M5);
		reads(AY0,I6,M5);

		CALL ___lib_ldiv;

		CALL ___lib_setup_stack_read;

		reads(AY1,I6,M5);
		reads(AY0,I6,M5);
		reads(MY1,I6,M5);
		reads(MY0,I6,M5);

		MR=SR0*MY0 (UU);
		MR0=MR1, AR=AY0-MR0;
		MR1=MR2;
		MR=MR+SR1*MY0 (SU);
		SR0=AR, MR=MR+SR0*MY1 (US);
		AR=AY1-MR0+C-1;
		SR1=AR;

__restore_state:JUMP ___lib_restore_small_frame;

.ENDMOD;





/* 
	This routine is a compiler support routine that
	computes the division of two long integers.

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 5/9/93
*/

.MODULE/IMAGE	__divdi3__;

#include "lib_glob.h"
#include "int_glob.h"

.ENTRY	__divdi3_;

__divdi3_:	MR1=TOPPCSTACK;
		CALL ___lib_save_small_frame;

		reads(MR1,I6,M5);
		reads(MR0,I6,M5);
		reads(AY1,I6,M5);
		reads(AY0,I6,M5);

		CALL ___lib_ldiv;

__restore_state:JUMP ___lib_restore_small_frame;

.ENDMOD;



.MODULE/IMAGE	Compare_floats_for_LE;

/* 
	This routine is part of the floating point support library.  It
	determines if two floating point numbers are less than or equal to
	each other.

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 3/13/93

*/

#include "lib_glob.h"
#include "flt_glob.h"

.ENTRY 		__lesf2_;

__lesf2_:	MR1=TOPPCSTACK;
		CALL ___lib_save_small_frame;

		reads(MX1,I6,M5);
		reads(MX0,I6,M5);
		reads(MY1,I6,M5);
		reads(MY0,I6,M5);

		CALL ___lib_fpcomp;

__restore_state:JUMP ___lib_restore_small_frame;

.ENDMOD;






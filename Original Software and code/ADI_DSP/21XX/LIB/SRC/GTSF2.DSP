.MODULE/IMAGE	Compare_floats_for_GT;

/* 
	This routine is part of the floating point support library.  It
	determines if two floating point numbers are greater than each other.

	if( arg1 > arg2 )
		return -1;
	else 
		return 0;

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 4/8/93

*/

#include "lib_glob.h"
#include "flt_glob.h"

.ENTRY 	__gtsf2_;

__gtsf2_:	MR1=TOPPCSTACK;
		CALL ___lib_save_small_frame;

		reads(MX1,I6,M5);
		reads(MX0,I6,M5);
		reads(MY1,I6,M5);
		reads(MY0,I6,M5);

		CALL ___lib_fpcomp;

__restore_state:JUMP ___lib_restore_small_frame;		

.ENDMOD;

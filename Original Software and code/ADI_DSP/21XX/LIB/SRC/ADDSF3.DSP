/*
	This file is the compiler support routine that computes
	the addition of two single precision floating point numbers

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 3/11/93
*/

.MODULE/IMAGE	__addsf3__;

#include "lib_glob.h"
#include "flt_glob.h"

.ENTRY	__addsf3_;

__addsf3_:	MR1=TOPPCSTACK;
		M5=-1;
		CALL ___lib_save_small_frame;

		I6=1;
		M5=1;
		MODIFY(I6,M4);

		reads(MX1,I6,M5);
		reads(MX0,I6,M5);
		reads(MY1,I6,M5);
		reads(MY0,I6,M5);

		CALL ___lib_fpadd;

		JUMP ___lib_restore_small_frame;
.ENDMOD;


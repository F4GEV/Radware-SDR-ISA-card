/*
	This file is the compiler support routine that computes
	the multiplication of two single precision floating point numbers

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 3/11/93
*/

.MODULE/IMAGE	__mulsf3__;

#include "lib_glob.h"
#include "flt_glob.h"

.ENTRY	__mulsf3_;

__mulsf3_:	MR1=TOPPCSTACK;
		CALL ___lib_save_small_frame;

		reads(MX1,I6,M5);
		reads(MX0,I6,M5);
		reads(MY1,I6,M5);
		reads(MY0,I6,M5), MR=0;

		CALL ___lib_fpmult;

		JUMP ___lib_restore_small_frame;
.ENDMOD;


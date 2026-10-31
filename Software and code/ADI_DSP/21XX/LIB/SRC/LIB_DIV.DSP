.MODULE/IMAGE	Library_divide_support;

/* 	This file contains the 16 divide instructions that are used to
	compute a fixed point division.  ALL routine that require a divide
	calls this routine, so that code space usage will be minimized.

	The C Runtime Library for the ADSP21xx family of processors.
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 3/7/93

	Calling Parameters
   		AY0	=	LSW of Numerator
		AY1	=	MSW of Numerator
		AX0	=	Denominator

	Result Registers
		AY0     =	Quotient

	Altered Registers
		AY0, AY1, AF, ASTAT

	Cycle Count
		17
*/
 	
#include "lib_glob.h"
#include "spt_glob.h"

.GLOBAL	___lib_div_core;

___lib_div_core:	DIVS AY1, AX0;
			DIVQ AX0;DIVQ AX0;DIVQ AX0;
			DIVQ AX0;DIVQ AX0;DIVQ AX0;
			DIVQ AX0;DIVQ AX0;DIVQ AX0;
			DIVQ AX0;DIVQ AX0;DIVQ AX0;
			DIVQ AX0;DIVQ AX0;DIVQ AX0;
			RTS;
.ENDMOD;

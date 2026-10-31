/*
	This file is the compiler support routine that computes
	the subtraction of two single precision floating point numbers

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 3/11/93
*/

.MODULE/IMAGE	__subsf3__;

#include "lib_glob.h"
#include "flt_glob.h"

.ENTRY	__subsf3_;

__subsf3_:	MR1=TOPPCSTACK;
		CALL ___lib_save_small_frame;

		reads(MX1,I6,M5);
		reads(MX0,I6,M5);
		reads(AY1,I6,M5);
		AF=PASS AY1, reads(MY0,I6,M5);

		AX0=0x8000;		/* Used to flip sign bit	*/
		AX1=MY0, AR=AX0 XOR AY1;/* Flip sign bit of Y		*/
		MY1=AR, AF=AX1 OR AF;	/* Copy back to Y MSW and do add*/
		IF EQ JUMP __return_x;	/* Y is zero, so return x	*/
		CALL ___lib_fpadd;

__restore_state:JUMP ___lib_restore_small_frame;

__return_x:	SR0=MX0;
		SR1=MX1;
		JUMP __restore_state;


.ENDMOD;


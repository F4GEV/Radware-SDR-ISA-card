/*
	This routine is used to normalize a value in MR that is in the
	internal floating point format.

	This routine is ONLY called from within the CRTL.

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 5/29/93

		Calling Parameters
	     AR Contains denormalized exponent
	MR1:MR0 Contains denormalized internal FP number
	     SI Contains 16 guard bits from FP operation

		Result Registers
	     AR Contains exponent of result
	SR1:SR0 Contains mantissa of result

		Altered Registers
	AR, AF, AX0, AY1,
	MR2, MR1, MR0, 
	SR1, SR0, SI, SE 

*/

.MODULE/IMAGE	__Internal_FP_normalize;

#include "lib_glob.h"
#include "flt_glob.h"

.ENTRY	___lib_libfpnorm;

___lib_libfpnorm:IF MV JUMP __downshift;/* The result overflowed	*/
		SE=EXP MR1 (HI);	/* Check for underflow		*/
		SE=EXP MR0 (LO);	/* Check in LSW also		*/
		AY1=SE;			/* Find out amount shift	*/
		AR=AR+AY1;		/* Compute new exponent		*/
		SR=NORM SI (LO);	/* Shift guard bits 		*/
		SR0=SR1;		/* Downshift guard bits		*/
		SR1=0;			/* Zero out upper word		*/
		SR=SR OR NORM MR1 (HI);/* Shift MSW into place	*/
		SR=SR OR NORM MR0 (LO);/* Shift LSW into place	*/
		AF=PASS SR0;		/* Need to test for zero result	*/
		AF=SR1 OR AF;
		IF EQ AR=PASS 0;	/* Zero exponent for zero result*/
		RTS;
		
__downshift:	AR=NOT MR2;
		SR=ASHIFT AR BY 15 (HI);/* Place sign bit of result	*/
		SR=SR OR LSHIFT MR1 BY -1 (HI);/* Place MSW of result	*/
		SR=SR OR LSHIFT MR0 BY -1 (LO);/* Place LSW of result	*/
		AR=AF+1;
		RTS;

.ENDMOD

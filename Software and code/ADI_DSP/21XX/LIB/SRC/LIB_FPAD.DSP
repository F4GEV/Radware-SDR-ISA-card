/*
	This routine computes the sum of two floating point numbers
 	int the internal format.

	These routines are ONLY called from within the CRTL.

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 5/29/93

		Calling Parameters
	    AX0 Contains exponent of X
	MR1:MR0 Contains mantissa of X

	    AY0 Contains exponent of Y
	MY1:MY0 Contains mantissa of Y

		Result Registers
	     AR Contains exponent of result
	SR1:SR0 Contains mantissa of result

		Altered Registers
	AR, AF, AX0, AY1,
	MR2, MR1, MR0, 
	SR1, SR0, SI, SE 

*/

.MODULE/IMAGE	__Internal_FP_add;

#include "lib_glob.h"
#include "flt_glob.h"
#include "ifp_glob.h"


.ENTRY 	___lib_libfpadd;

___lib_libfpadd:SI=MY0, AR=AX0-AY0;	/* Check relative exponents	*/
		IF GT JUMP __shift_y;	/* Shift Yop if result is pos	*/

__shift_x:	SI=MR0, AR=PASS AR;	/* Need to shift Xop		*/
		SE=AR, AR=PASS MR1;	/* Hold Xop in temp registers	*/
		SR=LSHIFT SI (HI);	/* SR0 now contains guard bits	*/
		SI=SR0, SR=LSHIFT SI (LO);/* Hold guard bits, make LSW	*/
		MR0=MY0, SR=SR OR ASHIFT AR (HI);/* Shift MSW into place*/
		MR1=MY1, AR=PASS AY0;	/* Copy exponent into place	*/
		JUMP __add_core;	/* Now add operands		*/

__shift_y:	SR1=MY1, AR=-AR;	/* Copy value for shift		*/
		SE=AR, AR=PASS SR1;	/* Hold MSW of value to shift	*/
		SR=LSHIFT SI (HI);	/* Shift guard bits 		*/
		SI=SR0, SR=LSHIFT SI (LO);/* Hold guard bits 		*/
		AR=AX0, SR=SR OR ASHIFT AR (HI);/* Shift MSW into place	*/
		JUMP __add_core;	/* Now add operands		*/

__add_core:	AY1=MR0, AF=PASS AR;	/* Hold exponent 		*/
		AY1=MR1, AR=SR0+AY1;	/* Compute new LSW		*/
		MR0=AR, AR=SR1+AY1+C;	/* Compute new MSW		*/
		IF AV JUMP __overflow;	/* Sum overflowed, need to fix	*/
		MR1=AR, AR=PASS AF; 	/* Load MR, copy exp for norm	*/
		JUMP ___lib_libfpnorm;	/* Normalize result		*/

__overflow:	SR=LSHIFT MR0 BY -1 (LO);/* Downshift sum for overflow	*/
		SR=SR OR LSHIFT AR BY -1 (HI);/*  in double precision	*/
		MR0=SR0, AR=NOT AR;	/* Need to flip sign bit	*/
		AY1=0x8000;		/* Need sign bit of real result	*/
		AY1=SR1, AR=AR AND AY1;	/* AND away all but sign bit	*/
		AR=AR OR AY1;		/* OR in sign bit with result	*/
		MR1=AR, AR=AF+1;	/* Copy exponent for norm ops	*/
		JUMP ___lib_libfpnorm;	/* Normalize result		*/

.ENDMOD

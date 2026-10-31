/*
	This routine computes the subtrahend of two floating point numbers
 	in the internal format.

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

.MODULE/IMAGE	__Internal_FP_sub;

#include "lib_glob.h"
#include "flt_glob.h"
#include "ifp_glob.h"

.ENTRY 	___lib_libfpsub;

___lib_libfpsub:SI=MY0, AR=AX0-AY0;	/* Check relative exponents	*/
		IF GT JUMP __shift_y;	/* Shift Yop if result is pos	*/

__shift_x:	SI=MR0, AR=PASS AR;	/* Need to shift Xop		*/
		SE=AR, AR=PASS MR1;	/* Hold Xop in temp registers	*/
		SR=LSHIFT SI (HI);	/* SR0 now contains guard bits	*/
		SI=SR0, SR=LSHIFT SI (LO);/* Hold guard bits, make LSW	*/
		MR0=MY0, SR=SR OR ASHIFT AR (HI);/* Shift MSW into place*/
		MR1=MY1, AR=PASS AY0;	/* Copy exponent into place	*/
		AY1=MR0, AF=PASS AR;	/* Hold exponent		*/
		AY1=MR1, AR=SR0-AY1;	/* Compute LSW			*/
		MR0=AR, AR=SR1-AY1;	/* Compute MSW			*/
		JUMP __finish_sub;	/* This saves a few PM addresses*/

__shift_y:	SR1=MY1, AR=-AR;	/* Copy value for shift		*/
		SE=AR, AR=PASS SR1;	/* Hold MSW of value to shift	*/
		SR=LSHIFT SI (HI);	/* Shift guard bits 		*/
		SI=SR0, SR=LSHIFT SI (LO);/* Hold guard bits 		*/
		AR=AX0, SR=SR OR ASHIFT AR (HI);/* Shift MSW into place	*/
		AY1=SI, AF=PASS AR;	/* Hold exponent		*/
		AY1=SR0, AR=-AY1;	/* Negate guard bits		*/
		SI=AR, AR=MR0-AY1+C-1;	/* Subtract LSWs of result	*/
		AY1=SR1;		/* Copy for subtract		*/
		MR0=AR, AR=MR1-AY1+C-1;	/* Compute MSWs for subtrahend	*/

__finish_sub:	IF AV JUMP __overflow;	/* Handle overflow case		*/
		MR1=AR, AR=PASS AF;	/* Copy exponent		*/
		JUMP ___lib_libfpnorm;	/* Normalize result		*/
		
__overflow:	SR=LSHIFT SI BY -1 (LO);/* Need to handle guard bits	*/
		SR=SR OR LSHIFT MR0 BY -1 (HI);/* Save guard bit	*/
		SI=SR0;			/* Save guard bits for norm op	*/
		SR=LSHIFT MR0 BY -1 (LO);/* Downshift sum for overflow	*/
		SR=SR OR LSHIFT AR BY -1 (HI);/*  in double precision	*/
		MR0=SR0, AR=NOT AR;	/* Need to flip sign bit	*/
		AY1=0x8000;		/* Need sign bit of real result	*/
		AY1=SR1, AR=AR AND AY1;	/* AND away all but sign bit	*/
		AR=AR OR AY1;		/* OR in sign bit with result	*/
		MR1=AR, AR=AF+1;	/* Copy exponent for norm ops	*/
		JUMP ___lib_libfpnorm;	/* Normalize result		*/

.ENDMOD

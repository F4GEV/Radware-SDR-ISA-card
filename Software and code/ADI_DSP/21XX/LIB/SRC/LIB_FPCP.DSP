/*
	This routine compares two floating point numbers in the internal format.

	These routines are ONLY called from within the CRTL.

	The C Runtime Library
	
	Alexander Soto
	Development Tools Engineering

	Created on 9/27/93

		Calling Parameters
	    AX0 Contains exponent of X
	MR1:MR0 Contains mantissa of X

	    AY0 Contains exponent of Y
	MY1:MY0 Contains mantissa of Y

		Result Registers
	     AR Contains result (0 if equal)


		Altered Registers
	          AR, AF, AY1

*/

.MODULE/IMAGE	__Internal_FP_comp;

#include "lib_glob.h"
#include "ifp_glob.h"


.ENTRY 	___lib_libfpcp;

___lib_libfpcp: AR=AX0-AY0, AY1=MY1;		/*  Subtract exponents 	*/
		IF NE JUMP __chk_msw;
		
		AR=MR1-AY1, AY1=MY0;		/*  Subtract msws	*/
		IF NE RTS;

		AF=MR0-AY1, AY1=MY1;		/*  Subtract lsw	*/
		AR=MR1-AY1+C-1;			/*  Subtract msw w/C	*/
		IF EQ AR=PASS AF;
		RTS;

__chk_msw:  	AR=MR1;				/*  Return msw of larger*/
		IF GT RTS;
		AR=-AY1;			/*  Return msw of inverted Y*/
		RTS;
.ENDMOD;




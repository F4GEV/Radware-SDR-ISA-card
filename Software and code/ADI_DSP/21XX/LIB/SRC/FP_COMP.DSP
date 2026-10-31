/*
	This routine compares two single precision IEEE floating
	point numbers.  The value returned is +1, -1, or zero.

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 6/3/93

		Calling Parameters
	MX1:MX0	Contains X parameter
	MY1:MY0	Contains Y parameter

		Result Registers
	     AR Contains -1, 0, +1
*/
	

.MODULE/IMAGE	__fp_comp_;

#include "lib_glob.h"
#include "flt_glob.h"

.ENTRY	___lib_fpcomp;

___lib_fpcomp:	SI=MX1;	
		SR=LSHIFT SI BY 1 (LO);	/* Clear away sign bit		*/
		SR=LSHIFT SR0 BY -8(HI);/* Isolate exponent into lsbs	*/
		AR=MY1, AF=PASS SR1;	/* Hold X exponent		*/
		SI=SR0;			/* Hold mantissa for later	*/
		IF EQ JUMP __return_y;	/* X is zero, return Y		*/
		SR=LSHIFT AR BY 1 (LO);	/* Clear away sign bit		*/
		SR=LSHIFT SR0 BY -8(HI);/* Isolate exponent into lsbs	*/
		AX0=MX0, AR=PASS SR1;	/* Check for Y == zero		*/
		IF EQ JUMP __return_x;	/* Y==0, so return X		*/
		AY0=MY0, AR=AF-SR1;	/* Check for largest magnitude	*/
		IF GT JUMP __return_x;	/* X is bigger, return MSW X	*/
		IF LT JUMP __return_y;	/* Y is bigger, return MSW Y	*/
		SR=LSHIFT SR0 BY -9(LO);/* Shift MSW of mantissa 	*/
		AY0=SR0, AF=AX0-AY0;	/* Need to check mantissas	*/
		SR=LSHIFT SI BY -9 (LO);/* Shift MSW of X mantissa	*/
		AY0=MY0, AR=SR0-AY0+C-1;/* Reload Y-reg to compare LSWs	*/
		IF LT JUMP __return_y;	/* If result is neg, return -Y	*/
		IF GT JUMP __return_x;	/* If result is pos, return X	*/
		IF EQ AR=PASS AF;	/* If MSWs==0, check LSWs	*/
		IF EQ RTS;		/* If LSWs==0, return 0		*/

__return_x:	AR=MX1;			/* Return MSW of X		*/
		RTS;

__return_y:	AR=MY1;			/* Return MSW of Y		*/
		AR=-AR;			/* Change sign of Y		*/
		RTS;

.ENDMOD;


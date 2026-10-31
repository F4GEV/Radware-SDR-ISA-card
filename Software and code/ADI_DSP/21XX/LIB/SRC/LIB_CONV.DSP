/*
	This routine converts an internal format to an short (or long) integer.

	This routine is ONLY called from within the CRTL.

	The C Runtime Library
	
	Alexander Soto
	Development Tools Engineering

	Created on 10/18/93

		Calling Parameters
	    AX0 Contains exponent of X
	MR1:MR0 Contains mantissa of X

		Result Registers
	AR has integer result	

		Altered Registers
	AR, AF, AY1, SR1, SR0, SE, SI

*/

.MODULE/IMAGE	__Internal_WD_to_int;

#include "lib_glob.h"
#include "ifp_glob.h"


.ENTRY	___lib_3wdtoi, ___libr_3wdtoi;
.ENTRY  ___lib_ito3wd;


___libr_3wdtoi:	SI=1;
		JUMP __3wd_conv;

___lib_3wdtoi:	SI=0;

__3wd_conv:	AF=PASS MR1;
		IF EQ JUMP __zero_input;
		AY1=15;
		AR=AX0-AY1, AY1=SI;		/*Subtract 15 from exp	*/
		SE=AR;
		SR=LSHIFT MR0 (LO);
		SR=SR OR ASHIFT MR1 (HI);
		AR=SR1, AF=PASS AY1;		/*Place short int into AR*/
		IF EQ JUMP __end;		/*Round or trunc (end)?	*/
		AF=PASS SR0;			/*Check for need of rounding*/
		IF GE JUMP __end;
		AF=PASS AY1;			/*Round.  Check for +/-	*/
		IF LT AF=-AF;
		AR=AR+AF;			/*Rounding adjustment	*/
		
__end:		RTS;
		
__zero_input:	AR=PASS 0;			/* Zero out AR		*/
		RTS;



/*
	This routine converts an integer to the internal format used by 
	the CRTL.

	This routine is ONLY called from within the CRTL.

		Calling Parameters
	AR Contains int X

		Result Registers
	     AR Contains exponent of result
	SR1:SR0 Contains mantissa of result

		Altered Registers
	AR, AF, AY1, SR1, SR0, SE 

*/


___lib_ito3wd: 	AF=PASS AR;
		IF EQ JUMP __return_zero3;
		SE=EXP AR (HI);			/*Returns negative # of redunant*/
		SR=NORM AR (HI), AR=SE;		/*sign bits.			*/
		AY1=15;
		AR=AR+AY1;
		RTS;


__return_zero3:	AR=0x8000;			/* Smallest exponent	*/
		SR=LSHIFT AR BY -100 (LO);	/* Zero fraction	*/
		RTS;

.ENDMOD;

/*
	This routine performs a single precision IEEE floating point addition

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 3/11/93

		Calling Parameters
	MX1:MX0	Contains X parameter
	MY1:MY0	Contains Y parameter

		Result Registers
	SR1:SR0	Contains Product of X and Y

*/
	

.MODULE/IMAGE	__IEEE_Single_Add;

#include "lib_glob.h"
#include "flt_glob.h"

.ENTRY	___lib_fpadd;

___lib_fpadd:	AY0=0x0080;		/* Used for implicit 1 bit	*/
		SI=MX1;	
		SR=LSHIFT SI BY 1 (LO);	/* Clear away sign bit		*/
		SR=LSHIFT SR0 BY -8 (LO);/* Isolate exponent into lsbs	*/
		SI=MY1, AF=PASS SR0;	/* Hold X exponent		*/
		IF EQ JUMP __return_y;	/* X is zero, return Y		*/
		SR=LSHIFT SI BY 1 (LO);	/* Clear away sign bit		*/
		SR=LSHIFT SR0 BY -8 (LO);/* Isolate exponent into lsbs	*/
		AR=PASS SR0;		/* Check for Y == zero		*/
		IF EQ JUMP __return_x;	/* Y==0, so return X		*/
		AX1=SR0, AR=AF-SR0;	/* Check for large magnitude	*/
		AY1=23;	
		SR1=AR, AR=AR-AY1;	
		IF GT JUMP __return_x;
		AR=SR1+AY1;
		IF LT JUMP __return_y;
		AR=AF-SR0;
		IF LT JUMP __shift_y;	/* Need to swap X and Y		*/

__swap_ops:	AX1=AR, AR=PASS AF;
		SR0=MX0, AF=PASS AX1;	/* Swap the X operand with the	*/
		MX0=MY0;		/* Y operand.  It is easier to	*/
		MY0=SR0;		/* always shift the Y operand	*/
		SR0=MX1;		/* so we swap them here if they	*/
		MX1=MY1;		/* are in the wrong order	*/
		MY1=SR0;
		AX1=AR, AR=-AF;

__shift_y:	SE=AR, AR=PASS AR;	/* Get ready for shift		*/
		SI=MY1;			/* Hold MSW for shift		*/
		SR=ASHIFT SI BY 9 (LO);	/* SR1 holds sign, SR0 mantissa	*/
		AF=ABS SR1;		/* Hold sign for a moment	*/
		SR=LSHIFT SR0 BY -9 (LO);/* Now SR0 hold mantissa only	*/
		SI=MY0, AR=SR0 OR AY0;	/* Make implicit 1 explicit	*/
		SR=LSHIFT SI BY 0 (LO);	/* Shift LSW into place		*/
		SR=SR OR LSHIFT AR BY 0 (HI);/* Shift MSW into place		*/
		IF POS JUMP __place_x;	/* Don't need to negate Y	*/
__negate_y:	AR=-SR0;		/* Negate Y in double precision	*/
		SR0=AR, AR=-SR1+C-1;
		SR1=AR;

__place_x:	AY0=SR0, AF=PASS AY0;	/* Copy Y value into Y-regs	*/
		AY1=SR1;
		SI=MX1;			/* Hold MSW for shift		*/
		SR=ASHIFT SI BY 9 (LO);	/* SR1 holds sign, SR0 mantissa	*/
		AR=ABS SR1;		/* Hold sign for a moment	*/
		SR=LSHIFT SR0 BY -9 (LO);/* Now SR0 hold mantissa only	*/
		SI=MX0, AR=SR0 OR AF;	/* Make implicit 1 explicit	*/
		SR=LSHIFT SI (HI);	/* Need to get guard bits	*/
		SI=SR0, SR=LSHIFT SI (LO);/* Shift LSW into place		*/
		SR=SR OR LSHIFT AR (HI);/* Shift MSW into place		*/
		IF POS JUMP __add_core;	/* Don't need to negate Y	*/
__negate_x:	AR=SI;			/* Be sure to use guard bits	*/
		AR=-AR;			
		SI=AR, AR=-SR0+C-1;	/* Negate Y in double precision	*/
		SR0=AR, AR=-SR1+C-1;
		SR1=AR;

__add_core:	SR0=AX1, AR=SR0+AY0;	/* Compute LSW		*/
		MR0=AR, AR=SR1+AY1+C;	/* Compute MSW		*/
		MR1=AR, AF=ABS AR;	/* Test for negative	*/

		JUMP ___lib_fpnorm;	/* Normalize result	*/

__return_y:	SR0=MY0;
		SR1=MY1;
		RTS;

__return_x:	SR0=MX0;
		SR1=MX1;
		RTS;
.ENDMOD;

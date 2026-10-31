/*
	This routine generates a fractional division for the internal
	floating point format;

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

    	Created on 6/5/93

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
	MR2, MR1, MR0, MF,
	SR1, SR0, SI, SE 
	I6
*/

.MODULE/IMAGE	__Internal_FP_divide;

#include "lib_glob.h"
#include "ifp_glob.h"

.ENTRY		___lib_libfpdiv;

___lib_libfpdiv:AY1=MY1, AR=PASS 1;	/* Move Y-op into y-regs        */
		MF=AR*MY1 (SS);		/* Copy sign of Y-op into MF	*/
		AF=PASS MR1;
		MY1=AX0, AR=PASS AY0;	/* Copy X-exp and hold Y-exp	*/
		AY0=MY0, AF=MR0 OR AF;	/* Need to test for divide-by-0	*/
		IF EQ JUMP __divide_err;/* Attempt to divide by zero	*/
		MY0=AR, AR=PASS AY1;	/* Copy Y-op for caller, check for 0*/
		IF EQ JUMP __return_zero;/* X-op is zero, return 0	*/

__compute_sign:	AF=MR1 XOR AY1;		/* Test for sign of result	*/
		I6=^__restore_state;	/* Assume positive sign		*/
		IF GE JUMP __abs_ops;	/* Need to have two positive ops*/
		I6=^__negate_result;	/* Quotient should be negative	*/

__abs_ops:	AR=PASS AY1;		/* Test for sign of Y-op	*/
		IF GE JUMP __abs_x;	/* If >0, test X-op		*/
		AX0=AR, AR=-AY0;	/* Need to have positive Y-op	*/
		AY0=AR, AR=-AX0+C-1;	/* Do negation in double 	*/
		AY1=AR;

__abs_x:	AR=PASS MR1;		/* Test for sign of X-op	*/
		IF GE JUMP __compute_q;	/* X-op is already positive	*/
		AR=-MR0;		/* Need to have positive X-op	*/
		MR0=AR, AR=-MR1+C-1;	/* Do negation in double	*/
		MR1=AR;

__compute_q:	AX0=0;			/* Clear quotient bit		*/
		SE=-15;
		CNTR=32;
		
		SR0=MR0;
		SR1=MR1;

	        DO __ddivu_pos UNTIL CE;
                   AR=ABS AX0;        	/* Check for quotient bit	*/
		   IF POS AF=SR0-AY0;	/* If set, subtract divisor	*/
		   IF POS AR=SR1-AY1+C-1;/* from partial remainder	*/
		   IF NEG AF=SR0+AY0;	/* If not set, add divisor	*/
		   IF NEG AR=SR1+AY1+C;	/* to partial remainder		*/

		   SR=LSHIFT MR0 BY 1 (LO);/* Shift dividend one bit	*/
		   SR=SR OR LSHIFT MR1 BY 1 (HI);/*  in double prec	*/
		   SI=AR, AR=AR XOR AY1;/* Compute quotient bit		*/
		   AX0=AR, AR=NOT AR;	/* Compute next bit for quotient*/
		   SR=SR OR LSHIFT AR (LO);/* Shift in next bit		*/
		   MR0=SR0, AR=PASS AF;	/* Get LSW			*/
		   MR1=SR1, SR=LSHIFT MR1 (LO);/* Place partial 	*/
		   SR=SR OR LSHIFT SI BY 1 (HI);/* remainder for next	*/
__ddivu_pos:	   SR=SR OR LSHIFT AR BY 1 (LO);/* iteration		*/
		AX0=MY1, AR=ABS MR1;	/* Need to make sure that the	*/
		SR0=MR0, AF=PASS 1;	/*  divide did not overflow	*/
		SR1=MR1, AR=PASS AX0;	/* This bit of code checks to	*/
		SE=-1;			/*  make sure that the result	*/
		IF NEG SR=LSHIFT MR0 (LO);/* is positive.  If not, then	*/
		IF NEG SR=SR OR LSHIFT MR1 (HI);/* there was an overflow*/
		IF NEG AR=AR+AF;	/*  and the quotient needs to be*/
		AX0=AR, AR=PASS 1;	/*  downshifted.		*/
		JUMP (I6);		/* Might need to negate sign	*/

__negate_result:AR=-SR0;		/* Negate result		*/
		SR0=AR, AR=-SR1+C-1;	/* in double precision		*/
		SR1=AR, AR=PASS 1;	/* Fall through to return	*/

__restore_state:MR=AR*MF (SS);		/* Get sign of Y-op		*/
		MR0=MY0, AF=ABS MR1;	/* Check if y-regs should be 	*/
		AR=AY1; AF=PASS AY0;	/* Copy y-regs to result regs	*/
		IF NEG AF=-AF;		/* Negate y-reg to reproduce	*/
		IF NEG AR=-AR+C-1;	/* the original Y-op		*/
		MY1=AR, AR=PASS AF;	/* Copy Y-op back for caller	*/
		MY0=AR, AF=PASS MR0;
		AY0=MR0, AR=AX0-AF;	/* Compute exponent of quotient	*/
		MR0=SR0;
		MR1=SR1;
		JUMP ___lib_libfpnorm;	/* Normalize quotient		*/

__divide_err:	
__return_zero:	SR=LSHIFT AR BY -100 (LO);/* Zero out return registers	*/
		AR=0x8000;		/* Smallest possible exponent	*/
		RTS;
.ENDMOD;




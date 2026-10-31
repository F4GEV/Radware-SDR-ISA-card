/*
	This routine computes the product of two floating point numbers
	in the internal format.

	This routine is called from within the CRTL only.

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

.MODULE/IMAGE	__Internal_FP_multiply;

#include "lib_glob.h"
#include "flt_glob.h"
#include "ifp_glob.h"

.ENTRY	___lib_libfpmult;

___lib_libfpmult:SR0=MR0, AF=AX0+AY0;	/* Compute exp of denorm prod	*/
		SR1=MR1, MR=SR0*MY0 (UU);/* Compute LSW of denorm prod	*/
		MR0=MR1, AR=AF+1;	/* Increment exp to handle the	*/
		MR1=MR2;		/*  integer mode multiply of a	*/
		MR=MR+SR1*MY0 (SU);	/*  fractional number		*/
		MR=MR+SR0*MY1 (US);	/* Compute middle words of prod	*/
		SI=MR0;			/* Hold bits 33-48 for use in	*/
		MR0=MR1;		/*  possible shift during norm	*/
		MR1=MR2;		
		MR=MR+SR1*MY1 (SS);	/* Compute final part of prod	*/
		JUMP ___lib_libfpnorm;	/* Normalize product, fix exp	*/

.ENDMOD

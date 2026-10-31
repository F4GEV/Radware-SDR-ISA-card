/* 	This file contains the unsigned modulo routine for 16-bit integers.

	The C Runtime Library for the ADSP21xx family of processors.
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 5/9/93
*/
 	
.MODULE/IMAGE	__umodsi3__;

#include "lib_glob.h"
#include "int_glob.h"

.ENTRY	__umodsi3_;

__umodsi3_:	MR1=TOPPCSTACK;		/* Get return address for save	*/
		CALL ___lib_save_small_frame;
	
		MR1=AY1;
		MR0=AR, MF=MR;		/* Copy for double divide 	*/
		SR=LSHIFT AR BY 1 (LO);	/* Shift numerator into position*/
		AY0=SR0, AR=PASS AY1;
		AF=ABS AR;
		IF NEG JUMP do_in_double;
		AF=PASS SR1;
		ASTAT=0x0;		/* Need to clear AQ for unsigned*/
		DIVQ AR;DIVQ AR;DIVQ AR;DIVQ AR;
		DIVQ AR;DIVQ AR;DIVQ AR;DIVQ AR;
		DIVQ AR;DIVQ AR;DIVQ AR;DIVQ AR;
		DIVQ AR;DIVQ AR;DIVQ AR;DIVQ AR;
		AR=PASS AY0;
		MR1=0;			/* Sign extended Num into MR	*/
		MR=MR-AR*MF (UU);	/* Subtract so Num-Den*Quo	*/
		AR=MR0;

__restore_state:JUMP ___lib_restore_small_frame;

do_in_double:	AY0=AR;
		AY1=0;
		MX1=MR0;		/* Copy Num for later		*/
		MR1=0;
		CALL ___lib_uldiv;
		MR0=MX1;		/* Copy Num into MR		*/
		MR1=0;
		MR=MR-SR0*MF (UU);	/* Subtract so Num-Den*Quo	*/
		AR=PASS MR0;
		JUMP __restore_state;


.ENDMOD;




/*
	This routine computes the unsigned long integer division
	of two unsigned long ints.

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 3/11/93

	Updated on 2/3/94 by AS - made slightly more efficient

        Calling Parameters
                AY0 = LSW of Y
                AY1 = MSW of Y
                MR1 = MSW of X
                MR0 = LSW of X

        Return Values
                MR1 = MSW of Z
                MR0 = LSW of Z

        Altered Registers
                AR AX1 AX0 SR0 SR1
		AF SI


*/

.MODULE/IMAGE	__Unsigned_Long_Division;

#include "lib_glob.h"
#include "int_glob.h"

.ENTRY	___lib_uldiv;

___lib_uldiv:	AF = PASS MR1;
		AR = MR0 OR AF;
		IF NE JUMP do_ludiv;
		SR=LSHIFT AR BY -100 (LO);	/* Zero out SR		*/
		JUMP __restore_state;

do_ludiv:	AF = PASS MR1;
		IF GE JUMP div_dividend;
		SR = LSHIFT MR0 BY 1 (LO);	/* Multiply dividend by two*/
		SR = SR OR LSHIFT MR1 BY 1 (HI);
		MR0 = SR0;
		MR1 = SR1;
		SR0 = 1;
		JUMP do_divide;

div_dividend:	SR = LSHIFT MR0 BY 1 (LO);	/* Multiply dividend by two*/
		SR = SR OR LSHIFT MR1 BY 1 (HI);
		MR0 = SR0;
		MR1 = SR1;
		SR0 = 0;

do_divide:	SR1 = 0;
		AF = PASS 0, AX1=SR1;
		CNTR=32;
		SE = -15;
        	AX0=AY1;
	        DO ddivu_pos UNTIL CE;
                   AR=ABS AX1;        
                   IF POS JUMP aqz_pos;		/*Is quotient bit set?	*/
aqo_pos:           AR=SR0+AY0;        	/*YES, ADD DIVISOR TO PARTIAL REMAINDER*/
                   SI=AR, AF=SR1+AY1+C;
                   JUMP ddivi_pos;
aqz_pos:           AR=SR0-AY0;        /*NO, SUBTRACT DIVISOR FROM PARTIAL REMAINDER*/
                   SI=AR, AF=SR1-AY1+C-1;
ddivi_pos:         SR=LSHIFT MR0 BY 1(LO); /*SHIFT DIVIDEND ONE BIT*/
                   SR=SR OR LSHIFT MR1 BY 1(HI);
                   AR=AX0 XOR AF;     /*COMPUTE QUOTIENT BIT*/
                   AX1=AR;            /*SAVE QUOTIENT BIT*/
                   AR=NOT AX1;
                   SR=SR OR LSHIFT AR(LO); /*SHIFT IN NEW BIT*/
                   MR0=SR0, AR=PASS AF;
		   SR=LSHIFT MR1(LO), MR1=SR1;
                   SR=SR OR LSHIFT SI BY 1(LO);
ddivu_pos:         SR=SR OR LSHIFT AR BY 1(HI);

		SR1=MR1;
		SR0=MR0;
__restore_state:RTS;

.ENDMOD;

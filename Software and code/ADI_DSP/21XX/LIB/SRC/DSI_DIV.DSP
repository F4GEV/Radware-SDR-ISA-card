/*
        Double-Precision Division
                Z = X  / Y

        Calling Parameters
                AY0 = LSW of Y
                AY1 = MSW of Y
                MR1 = MSW of X
                MR0 = LSW of X

        Return Values
                MR1 = MSW of Z
                MR0 = LSW of Z

        Altered Registers
		AY0 AY1 AX0 AX1 AR SR0 SR1
		AF SE SI
	
	Updated by AS on 2/3/94, subst sr1=sr0=0 for 1 instr

*/
.MODULE/IMAGE	ls_divide;

#include	"lib_glob.h"
#include 	"int_glob.h"

.entry	___lib_ldiv;

___lib_ldiv:	AF = PASS MR1;
	AR = MR0 OR AF;
	IF NE JUMP do_lsdiv;
	SR=LSHIFT AR BY -100 (LO);	/* Zero out SR		*/
	JUMP __restore_state;
do_lsdiv:

	AX0 = 0;
	AF = PASS 0;

/* find sign of result and take absolute value of operands*/

	AR = PASS AY1;
	IF GE JUMP sy_pos;
/* take twos compliment of Y */
	AR = - AY0;
	AY0 = AR;
	AR = AX0 - AY1 + C - 1;
	AY1 = AR;

	AR = PASS MR1;
	IF GE JUMP ddivq_neg;

/* take twos compliment of X */
	AR = - MR0;
	MR0 = AR;
	AR = AF - MR1 + C - 1;
	MR1 = AR;

	JUMP ddivq_pos;

sy_pos:
	AR = PASS MR1;
	IF GE JUMP ddivq_pos;

/* take twos compliment of X */
	AR = - MR0;
	MR0 = AR;
	AR = AF - MR1 + C - 1;
	MR1 = AR;

	JUMP ddivq_neg;

ddivq_pos:  CNTR=32;
        AX1=0;
	SE = -15;
        AX0=AY1;

/* multiply dividend by two */

	SR = LSHIFT MR0 BY 1 (LO);
	SR = SR OR LSHIFT MR1 BY 1 (HI);
	MR0 = SR0;
	MR1 = SR1;

	SR=LSHIFT AR BY -100 (LO); 	/* Zero out SR			*/

        DO ddivu_pos UNTIL CE;
                AR=ABS AX1;        
                IF POS JUMP aqz_pos;   /*Is quotient bit set?*/
aqo_pos:            AR=SR0+AY0;        /*Yes, add divisor to partial remainder*/
                SI=AR, AF=SR1+AY1+C;
                JUMP ddivi_pos;
aqz_pos:            AR=SR0-AY0;        /*No, subtract divisor from partial remainder*/
                SI=AR, AF=SR1-AY1+C-1;
ddivi_pos:          SR=LSHIFT MR0 BY 1(LO); /*Shift dividend one bit*/
                SR=SR OR LSHIFT MR1 BY 1(HI);
                AR=AX0 XOR AF;     /*Compute quotient bit*/
                AX1=AR;            /*Save quotient bit*/
                AR=NOT AX1;
                SR=SR OR LSHIFT AR(LO); /*Shift in new bit*/
                MR0=SR0, AR=PASS AF;
		SR=LSHIFT MR1(LO), MR1=SR1;
                SR=SR OR LSHIFT SI BY 1(LO);
ddivu_pos:          SR=SR OR LSHIFT AR BY 1(HI);

		SR0=MR0;
		SR1=MR1;

                JUMP __restore_state;


ddivq_neg:  CNTR=32;
        AX1=0;
	SE = -15;
        AX0=AY1;

/* multiply dividend by two */

	SR = LSHIFT MR0 BY 1 (LO);
	SR = SR OR LSHIFT MR1 BY 1 (HI);
	MR0 = SR0;
	MR1 = SR1;

	SR=LSHIFT AR BY -100 (LO);	/* Zero out SR		*/

        DO ddivu_neg UNTIL CE;
                AR=ABS AX1;        
                IF POS JUMP aqz_neg;   /*Is quotient bit set?*/
aqo_neg:            AR=SR0+AY0;        /*Yes, add divisor to partial remainder*/
                SI=AR, AF=SR1+AY1+C;
                JUMP ddivi_neg;
aqz_neg:            AR=SR0-AY0;        /*No, subtract divisor from partial remainder*/
                SI=AR, AF=SR1-AY1+C-1;
ddivi_neg:          SR=LSHIFT MR0 BY 1(LO); /*Shift dividend one bit*/
                SR=SR OR LSHIFT MR1 BY 1(HI);
                AR=AX0 XOR AF;     /*Compute quotient bit*/
                AX1=AR;            /*Save quotient bit*/
                AR=NOT AX1;
                SR=SR OR LSHIFT AR(LO); /*Shift in new bit*/
                MR0=SR0, AR=PASS AF;
		SR=LSHIFT MR1(LO), MR1=SR1;
                SR=SR OR LSHIFT SI BY 1(LO);
ddivu_neg:          SR=SR OR LSHIFT AR BY 1(HI);

		/* change sign of result */
		AR = - MR0;
		MR0 = AR, AF=PASS 0;
		AR = AF - MR1 + C - 1;
		MR1 = AR;
		
		SR0=MR0;
		SR1=MR1;

__restore_state:RTS;

.ENDMOD;

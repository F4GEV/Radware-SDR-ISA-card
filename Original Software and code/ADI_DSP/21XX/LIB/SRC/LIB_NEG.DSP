/*  This function negates a 3 wd internal format number.  It is intended for
    internal CRTL use only.

    Written by Alexander Soto

    10/20/93

    Registers used:  AF, AR, SR

*/

.MODULE/IMAGE	__Negate_Module;

#include	"lib_glob.h";

.ENTRY		___lib_3wd_neg, ___lib_3wd_abs;


/* Negation Routine  */

___lib_3wd_neg:	AR=-SR0, AY1=AR;
		SR0=AR, AR=-SR1+C-1;
		IF AV JUMP __n_overflow_ck;
		AR=PASS AY1, SR1=AR;
		RTS;

__n_overflow_ck:  SR=LSHIFT SR1 BY -1 (HI);
		AR=AY1+1;
		RTS;


/* Absolute routine  */

___lib_3wd_abs:	AF=ABS SR1;
		IF AV JUMP __a_overflow;
		IF POS RTS;
__compute_abs:	SR0=AR, AF=-SR0;
		AR=-SR1+C-1;
		SR1=AR, AR=PASS AF;
 		SR0=AR, AR=PASS SR0;
		RTS;

__a_overflow:	AF=PASS SR0;
		IF NE JUMP __compute_abs;
		SR=LSHIFT SR1 BY -1 (HI);
		AF=PASS 1;
		AR=AR+AF;
.ENDMOD;

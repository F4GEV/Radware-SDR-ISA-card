/*
    	This C callable subroutine is used to format and print strings.

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

    	Created on 6/5/93

	#include <stdio.h>
	int fprintf(FILE *fp, char *format_string, ...);

*/

.MODULE/IMAGE	__fprintf_;

#include	"lib_glob.h";
#include	"sio_glob.h";

.ENTRY		new_fprintf_;

new_fprintf_:	MR1=TOPPCSTACK;
		MR0=-1;
		CALL ___lib_save_large_frame;

		reads(SI,I6,M5);	/* Ignore FILE pointer	 	*/
		reads(AX0,I6,M5);	/* Read point to format		*/
		
		I5=I6;			/* Holds pointer to va_arg list	*/
		I7=AX0;			/* Point to format string	*/

__read_char:	AX0=DM(I7,M5);		/* Read character of format	*/
		AF=PASS AX0;		/* Check for end-of-string	*/
		IF EQ JUMP __eos_format;/* No more formating, exit now	*/
		AY0=0x25;		/* Check for '%',a format spec	*/
		AF=AX0-AY0;
		IF EQ JUMP __check_format;

__output_char:	AY1=SI, AR=PASS AX0;	/* Setup parameters char, stream*/
		CALL putc_;		/* Output character to stream	*/
		JUMP __read_char;	/* Get next character in format	*/

__check_format:	AX0=DM(I7,M5);		/* Read char after '%'		*/
		AF=AX0-AY0;		/* Check for '%%'		*/
		IF EQ JUMP __output_char;/* If '%%', output '%', continue*/

__check_space:	AY0=0x20;		/* Check for a space		*/
		AF=AX0-AY0;		/* If space, use space for +	*/
		IF NE JUMP __check_plus;/* Check other modifiers	*/
		AX0=DM(I7,M5);		/* Read beyond space		*/
/* Setup to print a space if the sign is +, a - if the sign is -	*/
		JUMP __check_space;	/* Start over again		*/

__check_plus:	AY0=0x2b;		/* Check for a plus		*/
		AF=AX0-AY0;		/* If plus, always output sign	*/
		IF NE JUMP __check_minus;/* Check for minus sign	*/
		AX0=DM(I7,M5);		/* Read beyond plus		*/
/* Setup to print a + or a - based on sign of output			*/
		JUMP __check_space;	/* Start over again		*/

__check_minus:	AY0=0x2d;		/* Check for a minus		*/
		AF=AX0-AY0;		/* If minus, left justify num	*/
		IF NE JUMP __check_hash;/* Check for a hash		*/
		AX0=DM(I7,M5);		/* Read beyond minus sign	*/
/* Setup to left justify the number					*/
		JUMP __check_space;	/* Start over again		*/

__check_hash:	AY0=0x23;		/* Check for hash		*/
		AF=AX0-AY0;		/* If hash, use alternate form	*/
		IF NE JUMP __check_zero;/* Check for a zero		*/
		AX0=DM(I7,M5);		/* Read beyond hash		*/
/* Setup to use alternate form in output				*/
		JUMP __check_space;	/* Start over again		*/

__check_zero:	AY0=0x30;		/* Check for zero		*/
		AF=AX0-AY0;		/* If zero, pad output w/0's	*/
		IF NE JUMP __check_width;/* Check for width of output	*/
		AX0=DM(I7,M5);		/* Read beyond zero		*/
/* Setup to pad output with zeros					*/
		JUMP __check_space;	/* Start over again		*/

__check_width:	AY0=0x2a;		/* Check for '*'		*/
		AF=AX0-AY0;		/* If '*', width is an argument	*/
		IF EQ JUMP __read_width;/* If not, read from format str	*/
		MY0=10;			/* Used to compute width	*/
		AR=PASS 0;		/* Clear sum register		*/
	
__internal_isd1:AY0=57;
		AY1=AX0, AF=AX0-AY0;	/* Test for (c > '9')		*/
		IF GT JUMP __got_width;/* !isdigit(), get precision	*/
		AY0=48;			/* Test for (c < '0')		*/
		AF=AX0-AY0;
		IF LT JUMP __got_width;/* !isdigit(), get precision	*/
		MR=AR*MY0 (SS);		/* Compute width *= 10		*/
		AR=MR0+AF, AX0=DM(I7,M5);/* Compute width * 10 + ch	*/
		JUMP __internal_isd1;	/* Get width until !isdigit()	*/

__read_width:	AX0=DM(I7,M5);		/* Read beyond star		*/
		AX0=DM(I5,M5);		/* Read width from stack	*/
		AR=PASS AX0;		/* Test for -width		*/
		IF GE JUMP __got_width;	/* If width is positive, use it	*/

__negative_width:AR=-AX0;		/* Negate width, and indicate	*/
/* Setup for left justifications					*/

__got_width:	AR=PASS AR;		/* Width is in AR at this point	*/
/* Do something useful with the width specifier				*/

__check_prec:	AY0=0x2e;		/* Check for '.'		*/
		AF=AX0-AY0;		/* If '.', read precision	*/
		IF NE JUMP __check_tmod;/* If not, read from format str	*/
		AX0=DM(I7,M5);		/* Read beyond '.'		*/
		AY0=0x2a;		/* Check for '*'		*/
		AF=AX0-AY0;		/* If '*', read prec from stack	*/
		IF EQ JUMP __read_prec;	/* Read precision from stack	*/
		MY0=10;			/* Used to compute width	*/
		AR=PASS 0;		/* Clear sum register		*/
	
__internal_isd2:AY0=57;
		AY1=AX0, AF=AX0-AY0;	/* Test for (c > '9')		*/
		IF GT JUMP __got_prec;	/* !isdigit(), get precision	*/
		AY0=48;			/* Test for (c < '0')		*/
		AF=AX0-AY0;
		IF LT JUMP __got_prec;	/* !isdigit(), get precision	*/
		MR=AR*MY0 (SS);		/* Compute width *= 10		*/
		AR=MR0+AF, AX0=DM(I7,M5);/* Compute width * 10 + ch	*/
		JUMP __internal_isd2;	/* Get width until !isdigit()	*/

__read_prec:	AX0=DM(I7,M5);		/* Read beyond star		*/
		AX0=DM(I5,M5);		/* Read prec from stack		*/
		AR=PASS AX0;		/* Test for -prec		*/
		IF LE AR=-AX0;		/* If -prec, make positive	*/

__got_prec:	AR=PASS AR;		/* Prec is in AR at this point	*/
/* Do something useful with the precision specifier			*/

__check_tmod:	AY0=0x68;		/* Check for 'h'		*/
		AF=AX0-AY0;
		IF NE JUMP __check_tmod_l;/* If 'h', the value is short	*/
		AX0=DM(I7,M5);		/* Read beyond the 'h'		*/
/* Do something useful with the fact that the value is a short		*/
		JUMP __check_tmod;	/* Start back at 'h'		*/

__check_tmod_l:	AY0=0x6c;		/* Check for 'l'		*/
		AF=AX0-AY0;		/* If 'l' value is long		*/
		IF NE JUMP __check_tmod_L;/* Check for 'L'		*/
		AX0=DM(I7,M5);		/* Read beyond the 'l'		*/
/* Do something useful with the fact that the value is a long		*/
		JUMP __check_tmod;	/* Start back at 'h'		*/

__check_tmod_L:	AY0=0x4c;		/* Check for 'L'		*/
		AF=AX0-AY0;		/* If 'L' value is long long	*/
		IF NE JUMP __check_spec;/* Check for what to print	*/
		AX0=DM(I7,M5);		/* Read beyond 'L'		*/
		JUMP __check_tmod;	/* Start back at 'h'		*/

__check_spec:	
__restore_state:JUMP ___lib_restore_large_frame;

__eos_format:	JUMP __restore_state;
.ENDMOD;

/*
    	This C callable subroutine returns the square root of the floating
	point input

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

    	Created on 5/29/93

	#include <math.h>		<Header>	
	float sqrtf(float x);		<Prototype>
	double sqrt(double x;		<Prototype>

*/

.MODULE/IMAGE	__sqrt_;

#include	"lib_glob.h";
#include 	"mth_glob.h";
#include	"ifp_glob.h";

.ENTRY		sqrtf_, sqrt_;

sqrt_:
sqrtf_:		MR1=TOPPCSTACK;
		CALL ___lib_save_small_frame;
		
		reads(MR1, I6, M5);
		AF=PASS MR1, reads(MR0, I6, M5);
__step_1:	AR=MR0, AF=MR0 OR AF;		/* Test for zero input	*/
		IF EQ JUMP __return_zero;	/* Input == 0.0		*/

__step_2:	AF=ABS MR1;			/* Check for x < 0.0	*/
		IF NEG JUMP __return_edom;	/* Domain error		*/

		CALL ___lib_sf_libfp;		/* Convert IEEE to libfp*/
__step_4:	AX1=AR;				/* Hold N		*/

__step_5:	AY0=0;				/* Set exp of f to zero	*/
		MY0=SR0;			/* Hold mantissa of y	*/
		MY1=SR1;
		MX0=SR0;			/* Hold f for later	*/
		MX1=SR1;			/* Hold LSW of f	*/

__step_6:	AX0=0;				/* Load exp of .59..	*/
		MR1=0x4b8a;			/* Load MSW mant of .59	*/
		MR0=0x5ce5;			/* Load LSW mant of .59	*/
		CALL ___lib_libfpmult;		/* Compute .59 * f	*/
		AY0=AR;
		MY1=SR1;
		MY0=SR0;
		AX0=-1;				/* Load exp of .41	*/
		MR1=0x6AD4;			/* Load MSW mant of .41	*/
		MR0=0xD402;			/* Load LSW mant of .41	*/
		CALL ___lib_libfpadd;		/* Compute Y0		*/

__step_7:	AY0=AR;				/* Move Y0 into Yop	*/
		MY0=SR0;
		MY1=SR1;
		AX0=0;				/* Load f into Xop	*/
		MR0=MX0;
		MR1=MX1;
		CALL ___lib_libfpdiv;		/* Compute f/Y0		*/
		AX0=AR;
		MR0=SR0;
		MR1=SR1;
		CALL ___lib_libfpadd;		/* Compute z = Y0 + f/Y0*/
		AY0=AR;				/* Move z into Yop	*/
		MY0=SR0;
		MY1=SR1;
		AX0=0;				/* Load f into Xop	*/
		MR0=MX0;
		MR1=MX1;
		CALL ___lib_libfpdiv;		/* Compute f/z		*/
		AX0=AR, AF=AY0-1;		/* Copy f/z to Xop	*/
		AR=AF-1;			/* Compute 0.25 * z	*/
		AY0=AR;				/* Now Yop is 0.25 * z	*/
		MR0=SR0;
		MR1=SR1;
		CALL ___lib_libfpadd;		/* Compute Y2=0.25*z+f/z*/
		AY0=AR;				/* Move Y2 into Yop	*/
		MY0=SR0, AR=PASS AX1;		/* Copy N into AR for 	*/
		MY1=SR1;			/*  step 8		*/

#if 0
		AX0=0;				/* This section of code */
		MR0=MX0;			/*  computes Y3.  This	*/
		MR1=MX1;			/*  should increase the	*/
		CALL ___lib_libfpdiv;		/*  accuracy, but it	*/
		AX0=AR;				/*  does not seem to 	*/
		MR0=SR0;			/*  work.  I have ifdef	*/
		MR1=SR1;			/*  it away for now.  	*/
		CALL ___lib_libfpadd;		/* It should be studied	*/
		MY0=SR0, AF=PASS 1;		/*  to determine if it	*/
		MY1=SR1, AR=AR-AF;		/*  should be used.	*/
		AY0=AR, AR=PASS AX1;
#endif

__step_8:	SR=ASHIFT AR BY -1 (HI);	/* Test for N even	*/
		AR=SR1, AF=ABS SR0;		/* IF NEG, N odd	*/
		IF POS JUMP __step_10;
		AX1=SR1;			/* Hold N/2 for now	*/
		AX0=0;				/* Load exp of .707	*/
		MR1=0x5A82;			/* Load mant MSW .707	*/
		MR0=0x7999;			/* Load mant LSW .707	*/		
		CALL ___lib_libfpmult;		/* Compute Y2 *=sqrt(.5)*/
		AY0=AR;				/* Copy result into Yop	*/
		MY0=SR0, AF=PASS AX1;		/* Save new Y2 in Yop	*/
		MY1=SR1, AR=AF+1;		/* Compute (N+1)/2	*/

__step_10:	MR1=MY1, AR=AR+AY0;		/* Compute B**M		*/
		MR0=MY0;			/* Copy y from Yop	*/
		AX0=AR;
		CALL ___lib_libfp_sf;		/* Convert back to IEEE	*/

__restore_state:JUMP ___lib_restore_small_frame;

__return_edom:	SI=EDOM;			/* Set errno == EDOM	*/
		CALL ___lib_set_errno;		/* Sets global errno	*/

/* The __return_edom routines falls through to __return_zero !		*/

__return_zero:	SR=LSHIFT AR BY -100 (LO);	/* Load SR with a zero	*/
		JUMP __restore_state;

.ENDMOD;




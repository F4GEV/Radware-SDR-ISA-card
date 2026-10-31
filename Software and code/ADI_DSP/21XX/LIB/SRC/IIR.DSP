/*
    This function computes the (IIR) filtered response of a given input sample. The
    function returns the filtered value of the input.

    The DSP Applications Library		    Gordon A. Sterling    
    
    Created on    7/20/93

    #include	<math.h>;   
    int iir(int sample, int a_coeffs[], int b_coeffs[], int *delay[], int taps)
*/

.MODULE/IMAGE		__iir__;

#include	"lib_glob.h";
#include	"flt_glob.h";

.ENTRY		iir_;

iir_:		MR1=TOPPCSTACK;
		CALL ___lib_save_small_frame;

		DIS M_MODE;		/* ###### Fractional mode ####### */

			/* read parameters */
		MR1=AR;				/* input sample from stack*/
		MX0=AY1;			/* pointer to a coeff array*/
		reads(MX1,I6,M5);		/* pointer to b coeff array*/
		reads(SI,I6,M5);		/* pointer to pointer to delay line*/
		reads(AX0,I6,M5);		/* number of taps*/
		AY0=I0;
		AY1=I5;   		/*Store some more registers*/

		I0=SI;				/*Set I0 to point to delay line*/
		M3=AX0;
		modify(I0,M3);			/*Move to pointer to delay lines */
		SI=I0;				/*Save addr for delay line ptr */
		AF=PASS 1, MR0=DM(I0,M2);	/*Fetch pointer to delay line*/
		I0=MR0;
		I6=MX0;				/*Set I6 to point to pointer*/
		I5=MX1;
		AR=AX0-AF;			/*Get taps-1*/

		L0=AX0;
		MR0=0;				/*Clear LSW of input word*/
		MX0=DM(I0,M1), MY0=PM(I6,M5);
		CNTR=AR;
		DO pole_loop UNTIL CE;
pole_loop:	   MR=MR+MX0*MY0 (SS), MX0=DM(I0,M1), MY0=PM(I6,M5);
		MR=MR+MX0*MY0 (RND);
		IF MV SAT MR;
		MX1=MR1;
		MR=0, MX0=DM(I0,M1), MY0=PM(I5,M5);
		CNTR=AR;
		DO zero_loop UNTIL CE;
zero_loop:	   MR=MR+MX0*MY0 (SS), MX0=DM(I0,M1), MY0=PM(I5,M5);
		MR=MR+MX0*MY0 (SS), MY0=PM(I5,M5);	/*Move delay pointer +1*/
		MR=MR+MX1*MY0 (RND), DM(I0,M1)=MX1;
		IF MV SAT MR;
		MR0=I0;
		I0=SI;
		DM(I0,M2)=MR0;
		AR=MR1;
		L0=0;

restore_state:	ENA M_MODE;	/* ###### End Fractional mode ####### */
		I0=AY0;   		/* Restore other registers*/
		I5=AY1;
		JUMP ___lib_restore_small_frame;

.ENDMOD;



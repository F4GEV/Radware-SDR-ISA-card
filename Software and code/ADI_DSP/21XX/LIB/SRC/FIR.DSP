/*
    	This C callable function computes the (FIR) filtered 
	response of a given input sample. The function returns
	the filtered value of the input. 

	The C Runtime Library
	
	Craig Smilovitz
	Development Tools Engineering

    	Created on 7/20/93

	#include <math.h>			<Header>	
	int fir(int sample, int coeffs[], int *delay, int taps);	<Prototype>
*/

.MODULE/IMAGE	__fir__;

#include	"lib_glob.h";
#include	"flt_glob.h";

.ENTRY		fir_;

fir_:		MR1=TOPPCSTACK;
		CALL ___lib_save_small_frame;

		DIS M_MODE;		/* ###### Fractional mode ####### */
					/* do save here */

			/* read parameters */
						/*AR is input sample */
						/*AY1 is pointer to coeff array*/
		reads(MR1,I6,M5);		/*MR1 is  pointer to pointer to delay line*/
		reads(AX0,I6,M5);		/*AX0 is  number of taps*/

		I6=MR1;				/*Set I6 to addr of delay_line_ptr */
		M5=AX0;
		modify(I6,M5);			/*Move to pointer to delay lines */
		M5=1;
		MR1=DM(I6,M6);			/*Fetch pointer to delay line*/
		I1=MR1;

		L1=AX0;
		DM(I1,M1)=AR;		/* put sample in delay line (was changed to M2) */
		MR1=I1;
		DM(I6,M6)=MR1;		/* save new delay_line_ptr */
		I6=AY1;
		MR=0, MX0=DM(I1,M1), MY0=PM(I6,M5);   /* I1 seems one step too far here */
		CNTR=AX0;
		DO convolution UNTIL CE;
convolution:	   MR=MR+MX0*MY0 (SS), MX0=DM(I1,M1), MY0=PM(I6,M5);
		MR=MR (RND);
		IF MV SAT MR;
		AR=MR1;
		L1=0;

restore_state:	ENA M_MODE;	 /* ###### End Fractional mode ####### */
		JUMP ___lib_restore_small_frame;

.ENDMOD;


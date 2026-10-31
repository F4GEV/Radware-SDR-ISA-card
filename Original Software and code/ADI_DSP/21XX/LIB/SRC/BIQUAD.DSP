/*
    	This C callable function computes the (FIR) filtered 
	response of a given input sample. The function returns
	the filtered value of the input. 

	The C Runtime Library
	
	Craig Smilovitz
	Development Tools Engineering

    	Created on 7/20/93

	Improved on 3/11/94 by Alex Soto

	#include <math.h>			<Header>	
	int biquad(int sample, int pm coeffs[], int delay, int sections);
*/

.MODULE/IMAGE	__IIR_Biquad_Subroutine__;

#include	"lib_glob.h";

.ENTRY		biquad_;

biquad_:	MR1=TOPPCSTACK;
		CALL ___lib_save_small_frame;

		DIS M_MODE;		/* ###### Fractional mode ####### */

		SE=0;
		reads(AY0,I6,M5);		/*Fetch delay pointer*/
		reads(MY0,I6,M5), SR=ASHIFT AR (HI); /*Fetch number of sections*/
		M3=-3;

		I1=AY0;
		I6=AY1;

    		CNTR=MY0;			/*Loop once per section*/
		DO biquad_sections UNTIL CE;
		   MX0=DM(I1,M1), MY0=PM(I6,M5);
		   MR=MX0*MY0 (SS), MX1=DM(I1,M1), MY0=PM(I6,M5);
		   MR=MR+MX1*MY0 (SS), MY0=PM(I6,M5);
		   MR=MR+SR1*MY0 (SS), MX0=DM(I1,M1),MY0=PM(I6,M5);
		   MR=MR+MX0*MY0 (SS), MX0=DM(I1,M3), MY0=PM(I6,M5);
		   DM(I1,M1)=MX1, MR=MR+MX0*MY0 (RND);
biquad_sections:   DM(I1,M1)=SR1, SR=ASHIFT MR1 (HI); 
		DM(I1,M1)=MX0, AR=PASS SR1;	/* return value */
		DM(I1,M1)=SR1;

		ENA M_MODE; 	/* ###### End Fractional mode ####### */
	
restore_state:	JUMP ___lib_restore_small_frame;

.ENDMOD;



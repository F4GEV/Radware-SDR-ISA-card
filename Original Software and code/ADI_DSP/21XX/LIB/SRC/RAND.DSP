/*
    This C callable subroutine generates a random integer between 
    0 and (2^15)-1.

    DSP Applications Library.		    Gordon A. Sterling

    Created on 2/16/90
    V 1.1  gnu calling convention      7/21/93   Craig Smilovitz 

    #include <stdlib.h>
    int rand(void)

*/

.MODULE/IMAGE		Random_Number_Generator;


#include	<lib_glob.h>;

.VAR/DM/RAM	seed_lsw, seed_msw;		/*Storage for seed value*/
.GLOBAL		seed_lsw, seed_msw;		/*Must be available for srand()*/

.ENTRY		rand_;



rand_:		MR1=TOPPCSTACK;
		CALL ___lib_save_small_frame;

		SR0=DM(seed_lsw);
		SR1=DM(seed_msw);

		MY0=25;				/*Upper half of a*/
		MR=SR0*MY0 (US);		/*MR=a(hi) * x(lo)*/
		MY0=26125;			/*Lower half of a*/
		MR=MR+SR1*MY0 (SU);		/*MR=a(hi) * x(lo) + a(lo) * x(hi)*/
		AR=PASS MR1, MR1=MR0;		/*Shift up 16 bits*/
		MR2=AR;
		MR0=H#FFFE;			/*c=32767 with upshift*/
		MR=MR+SR0*MY0 (UU);		/*MR=mod[(a*x +c),32768*/
		SR=ASHIFT MR2 BY 15 (HI);
		SR=SR OR LSHIFT MR1 BY -1 (HI);
		SR=SR OR LSHIFT MR0 BY -1 (LO);

		DM(seed_lsw)=SR0;
		DM(seed_msw)=SR1;

		AR=ABS SR1;

restore_state:	JUMP ___lib_restore_small_frame;

.ENDMOD;

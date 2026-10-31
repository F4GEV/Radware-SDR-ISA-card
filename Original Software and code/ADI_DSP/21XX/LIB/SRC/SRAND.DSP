/*
    This C callable subroutine sets the seed value for the
    random number generator.

    DSP Applications Library.		    Gordon A. Sterling

    Created on 2/20/89
    V 1.1  gnu calling convention      7/21/93   Craig Smilovitz 

    srand()

*/

.MODULE/IMAGE		Random_Number_Seeder;




#include	<lib_glob.h>;

.EXTERNAL	seed_lsw, seed_msw;		/*Must be defined in rand()*/

.ENTRY		srand_;

srand_:		DM(seed_lsw)=AR;
		SI=0;
		DM(seed_msw)=SI;

		RTS;				/*Return to caller*/

.ENDMOD;

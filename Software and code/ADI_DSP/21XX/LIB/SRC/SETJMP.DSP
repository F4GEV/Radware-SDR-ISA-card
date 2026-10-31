/*
	This file provides for an unexpected return from nested functions

	The C Runtime Library
	
	Alexander Soto
	Development Tools Engineering

	Created on 10/29/93

	Scratch registers altered:
	  AR, SI, M5

*/

.MODULE/IMAGE	__setjmp__;

#include "lib_glob.h"

.ENTRY setjmp_;

setjmp_:	MR1=TOPPCSTACK;
		
		dm_ptr=AR;		/*Save stack and frame registers*/
		dm_mdf=1;
		AR=I4;
		DM(dm_ptr,dm_mdf)=AR;
		AR=M4;
		DM(dm_ptr,dm_mdf)=AR;

		DM(dm_ptr,dm_mdf)=AY0;	/*Save all compiler registers	*/
		DM(dm_ptr,dm_mdf)=AX0;
		DM(dm_ptr,dm_mdf)=AX1;
		DM(dm_ptr,dm_mdf)=MY0;
		DM(dm_ptr,dm_mdf)=MX0;
		DM(dm_ptr,dm_mdf)=MX1;

		AR=I0;			/*Save DAGS			*/
		DM(dm_ptr,dm_mdf)=AR;
		AR=I5;
		DM(dm_ptr,dm_mdf)=AR;
		AR=I7;
		DM(dm_ptr,dm_mdf)=AR;
		AR=M0;
		DM(dm_ptr,dm_mdf)=AR;
		AR=I0;
		DM(dm_ptr,dm_mdf)=AR;
		AR=M7;
		DM(dm_ptr,dm_mdf)=AR;
		AR=L0;
		DM(dm_ptr,dm_mdf)=AR;
		AR=L1;
		DM(dm_ptr,dm_mdf)=AR;
		AR=L5;
		DM(dm_ptr,dm_mdf)=AR;
		AR=L6;
		DM(dm_ptr,dm_mdf)=AR;
		AR=L7;
		DM(dm_ptr,dm_mdf)=AR, AR=PASS 0;/*Return 0		*/
		
		SI=I2;			/*The following will not work with*/
		DM(dm_ptr, dm_mdf)=SI;	/*autobuffering. Reassemble this file*/
		SI=I3;			/*with these lines commented out, if*/
		DM(dm_ptr,dm_mdf)=SI;	/*you want to use autobuffering.*/

		DM(dm_ptr,dm_mdf)=MR1;	/*Save return address		*/
		I6=MR1;
		JUMP (I6);
		
.ENDMOD;

/*
	This file provides for an unexpected return from nested functions

	The C Runtime Library
	
	Alexander Soto
	Development Tools Engineering

	Created on 10/29/93

	Scratch registers altered:
	  AR, SI, M5

*/

.MODULE/IMAGE	__longjmp__;

#include "lib_glob.h"

.ENTRY longjmp_;

longjmp_:	M5=1;
		I6=AR;			/*Point to jmp_buf		*/
		AR=PASS AY1;
		get(SI,M5), AF=PASS AR;
		IF EQ AR=AF+1;		/*Longjmp cannot return 0	*/

		I4=SI;			/*restore stack and frame ptr	*/
		get(SI,M5);
		M4=SI;

		get(AY0,M5);		/*restore all compiler registers*/
		get(AX0,M5);
		get(AX1,M5);
		get(MY0,M5);
		get(MX0,M5);
		get(MX1,M5);

		get(SI,M5);		/*Save DAGS			*/
		I0=SI;
		get(SI,M5);
		I5=SI;
		get(SI,M5);
		I7=SI;
		get(SI,M5);
		M0=SI;
		get(SI,M5);
		I0=SI;
		get(SI,M5);
		M7=SI;
		get(SI,M5);
		L0=SI;
		get(SI,M5);
		L1=SI;
		get(SI,M5);
		L5=SI;
		get(SI,M5);
		L6=SI;
		get(SI,M5);
		L7=SI;
		
		get(SI,M5);		/*The following will not work with*/
		I2=SI;			/*autobuffering. Reassemble this file*/
		get(SI,M5);		/*with these lines commented out, if*/
		I3=SI;			/*you want to use autobuffering.*/
		
		get(SI,M5);		/*Get return address		*/
		I6=SI;
		JUMP (I6);
		
.ENDMOD;

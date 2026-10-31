/*
	This C callable subroutine converts a character to uppercase
	if it is lower case.

	The C Runtime Library for the ADSP21xx family of processors.
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 7/8/93

    	#include <ctype.h>;			<Header>
    	int toupper(int c);			<Prototype>

	toupper: 9 Cycles maximum		<Cycle Count>
	toupper: 9 PM words			<PM words>
	toupper: 0 DM words			<DM words>
	toupper: 0 Hardware loops		<DO-loops>
*/

.MODULE/IMAGE	__toupper___;

#include "lib_glob.h"
#include "cty_glob.h"

.ENTRY		toupper_;

toupper_:	AY1=122;			/* 122 = ASC('z')	*/
		AF=AR-AY1;			/* Test for c > z	*/
		IF GT JUMP __restore_state;	/* c is not lower case	*/
		AY1=97;				/* 97 = ASC('a')	*/
		AF=AR-AY1;			/* Test for c < a	*/
		IF LT JUMP __restore_state;	/* c is not lower case	*/
		AY1=0xffdf;			/* Caps mask		*/
		AR=AR AND AY1;			/* Mask lowercase bit	*/
__restore_state:RTS;				/* Return to caller	*/

.ENDMOD;

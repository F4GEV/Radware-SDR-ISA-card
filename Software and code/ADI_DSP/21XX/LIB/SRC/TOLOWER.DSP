/*
	This C callable subroutine converts a character to lowercase
	if it is upper case.

	The C Runtime Library for the ADSP21xx family of processors.
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 7/8/93

    	#include <ctype.h>;			<Header>
    	int tolower(int c);			<Prototype>

	tolower: 9 Cycles maximum		<Cycle Count>
	tolower: 9 PM words			<PM words>
	tolower: 0 DM words			<DM words>
	tolower: 0 Hardware loops		<DO-loops>
*/

.MODULE/IMAGE	__tolower___;

#include "lib_glob.h"
#include "cty_glob.h"

.ENTRY		tolower_;

tolower_:	AY1=90;				/* 90 = ASC('Z')	*/
		AF=AR-AY1;			/* Test for c > Z	*/
		IF GT JUMP __restore_state;	/* c is not upper case	*/
		AY1=65;				/* 65 = ASC('A')	*/
		AF=AR-AY1;			/* Test for c < A	*/
		IF LT JUMP __restore_state;	/* c is not upper case	*/
		AY1=32;				/* Caps bit		*/
		AR=AR OR AY1;			/* Set lowercase bit	*/
__restore_state:RTS;				/* Return to caller	*/

.ENDMOD;

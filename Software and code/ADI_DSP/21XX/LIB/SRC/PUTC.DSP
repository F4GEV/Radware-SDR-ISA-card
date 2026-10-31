.MODULE/IMAGE	__Putc_for_echo;

/* 	
	This file contains the routine to write a single character to the
	I/O port of the ECHO board.

	The C Runtime Library for the ADSP21xx family of processors.
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 3/13/93

	#include <stdio.h>
	int putc(char c, FILE *stream);
*/
 	
#include "lib_glob.h"
#include "sio_glob.h"

.ENTRY		putc_;

putc_:		I6=0x3008;		/* Point to I/O port	*/
		AY1=0;			/* Clear for first test	*/
		MR1=0x8000;		/* Mask for OK to write	*/
check_for_OK:	AF=MR1 AND AY1, AY1=DM(I6,M6);/* Test for OK write	*/
		IF EQ JUMP check_for_OK;

		DM(0x3000)=AR;		/* Write character	*/
		RTS;
.ENDMOD;

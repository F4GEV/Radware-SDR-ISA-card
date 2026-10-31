/*
	This routine is used to set errno when a routine detects an error

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 5/29/93


		Calling Parameters
	     SI Contains new value for errno

		Result Registers
	   None

*/

.MODULE/IMAGE	__errno_;

#include "lib_glob.h"

.ENTRY		___lib_set_errno;
.GLOBAL		errno_;

.VAR/DM/RAM	errno_;

___lib_set_errno:	DM(errno_)=SI;
			RTS;

.ENDMOD;


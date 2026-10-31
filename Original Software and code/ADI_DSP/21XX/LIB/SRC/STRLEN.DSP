/*
	This C callable subroutine returns the length
    	of its input argument string.

	The C Runtime Library for the ADSP21xx family of processors.
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 3/21/93

    	#include <string.h>;
    	size_t strlen(const char *s1);
*/
 	
.MODULE/IMAGE		__String_Length_Routine;

#include "lib_glob.h"
#include "str_glob.h"

.ENTRY		strlen_;

strlen_:	I6=AR;
		M5=1;

		AF=PASS 0, AY1=DM(I6,M5);
		DO __check_length UNTIL EQ;
		   AR=PASS AY1, AY1=DM(I6,M5);
__check_length:	   IF NE AF=AF+1;

		AR=PASS AF;

__restore_state:RTS;

.ENDMOD;

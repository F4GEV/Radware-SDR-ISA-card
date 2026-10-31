.MODULE/IMAGE	__Setup_to_Read_Paramters;

/* 
	This routine setups up the I6 and M5 registers to read the 
	parameters from the STACK.

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 4/10/93

*/

#include "lib_glob.h"
#include "emu_glob.h"

.ENTRY 	___lib_setup_stack_read;

___lib_setup_stack_read:M5=1;		/* Used to read sequential 	*/
			I6=1;		/* parameters from the stack	*/
			MODIFY(I6,M4);	/* Point to first parameter	*/
			RTS;		/* Return to caller		*/

.ENDMOD;


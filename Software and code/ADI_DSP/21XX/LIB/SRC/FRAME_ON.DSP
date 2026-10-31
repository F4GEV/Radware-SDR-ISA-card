/*
	This routine swaps the stack/frame pointer.  It is usually
	used for functions that do not need to save registers, but
	do need to access the stack.

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 3/11/93

*/

.MODULE/IMAGE	__Swap_Frame_Only;

#include "lib_glob.h"
#include "spt_glob.h"

.ENTRY	___lib_frame_swap;
.ENTRY	___lib_frame_restore;

___lib_frame_swap:		SI=M4;		/* Hold frame pointer		*/
				M4=I4;		/* Swap stack and frame pointer	*/
				M5=-1;		/* Used for stack pushes	*/
				put(SI, M5);	/* Save old frame on stack	*/
				put(MR1, M5);	/* Save parent`s return address	*/
				M5=1;		/* Now setup to read the params	*/
				I6=1;		/* Begin by reading first param	*/
				MODIFY(I6,M4);	/* Point to first param		*/
				RTS;


___lib_frame_restore:		I6=M4;		/* Point to old frame		*/
				M5=-1;		/* Used for stack restores	*/
				get(SI, M5);	/* Read old frame		*/
				get(MR1, M5);	/* Read return address		*/
				I6=MR1;		/* Load return address for JUMP	*/
				I4=M4;		/* Restore old stack pointer	*/
				M4=SI;		/* Restore old frame pointer	*/
				JUMP (I6);


.ENDMOD;


/*
	This routine saves all the non-dag compiler registers on the stack

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 3/7/93

		Calling Parameters
	MR1 --> Function return address

	
*/

.MODULE/IMAGE	Small_Frame_Save;

#include "lib_glob.h"
#include "spt_glob.h"

.ENTRY	___lib_save_small_frame;
.ENTRY	___lib_restore_small_frame;

___lib_save_small_frame:	SI=M4;		/* Hold frame pointer		*/
				M4=I4;		/* Swap stack and frame pointer	*/
				M5=-1;		/* Used for stack pushes	*/
				put(SI, M5);	/* Save old frame on stack	*/
				put(MR1, M5);	/* Save parent`s return address	*/
				put(AY0, M5);
				put(AX0, M5);
				put(AX1, M5);
				put(MY0, M5);
				put(MX0, M5);
				put(MX1, M5);

				I6=1;
				M5=1;
				MODIFY(I6,M4);

				RTS;


___lib_restore_small_frame:	I6=M4;		/* Point to old frame		*/
				M5=-1;		/* Used for stack restores	*/
				get(SI, M5);	/* Read old frame		*/
				get(MR1, M5);	/* Read return address		*/
				get(AY0, M5);
				get(AX0, M5);
				get(AX1, M5);
				get(MY0, M5);
				get(MX0, M5);
				get(MX1, M5);
				I6=MR1;		/* Load return address for JUMP	*/
				I4=M4;		/* Restore old stack pointer	*/
				M4=SI;		/* Restore old frame pointer	*/
				JUMP (I6);


.ENDMOD;


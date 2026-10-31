/*
	This routine saves all the non-dag compiler registers on the stack

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 3/7/93

	Updated on 1/4/94 by Alex Soto

		Calling Parameters
	MR1 --> Function return address
	MR0 ==  Required local space
*/

.MODULE/IMAGE	___Large_Frame_Save;

#include "lib_glob.h"
#include "spt_glob.h"

.ENTRY	___lib_save_frame;
.ENTRY  ___lib_save_large_frame;
.ENTRY	___lib_restore_frame;
.ENTRY  ___lib_restore_large_frame;

___lib_save_frame:
___lib_save_large_frame:	SI=M4;		/* Hold frame pointer		*/
				M4=I4;		/* Swap stack and frame pointer	*/
				M5=-1;		/* Used for stack pushes	*/
				put(SI, M5);	/* Save old frame on stack	*/
				M5=MR0;		/* Move stack pointer for locals*/
				put(MR1, M5);	/* Save parent`s return address	*/
				M5=-1;		/* Save additional registers	*/
				put(AY0, M5);
				put(AX0, M5);
				put(AX1, M5);
				put(MY0, M5);
				put(MX0, M5);
				put(MX1, M5);
				put(SE, M5);
				SR0=I0;
				put(SR0, M5);
				SR0=M0;
				put(SR0, M5);
				SR0=I2;
				put(SR0, M5);
				SR0=I3;
				put(SR0, M5);
				SR0=I5;
				put(SR0, M5);
				SR0=M6;
				put(SR0, M5);
				SR0=I7;
				put(SR0, M5);
				SR0=M7;
				put(SR0, M5);
				RTS;


___lib_restore_frame:
___lib_restore_large_frame:	I6=M4;		/* Point to old frame		*/
				M5=-1;		/* Used for stack restores	*/
				get(SI, M5);	/* Read old frame		*/
				M5=MR0;
				get(MR1, M5);	/* Read return address		*/
				M5=-1;
				get(AY0, M5);
				get(AX0, M5);
				get(AX1, M5);
				get(MY0, M5);
				get(MX0, M5);
				get(MX1, M5);
				get(SE, M5);
				get(AY1,M5);
				I0=AY1;
				get(AY1,M5);
				M0=AY1;
				get(AY1,M5);
				I2=AY1;
				get(AY1,M5);
				I3=AY1;
				get(AY1,M5);
				I5=AY1;
				get(AY1,M5);
				M6=AY1;
				get(AY1,M5);
				I7=AY1;
				get(AY1,M5);
				M7=AY1;

				I6=MR1;		/* Load return address for JUMP	*/
				I4=M4;		/* Restore old stack pointer	*/
				M4=SI;		/* Restore old frame pointer	*/

___lib_cbug_finish:
				JUMP (I6);


.ENDMOD;


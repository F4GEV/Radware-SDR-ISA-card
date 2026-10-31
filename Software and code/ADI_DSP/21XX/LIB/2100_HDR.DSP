/*
The 2100 run time header also includes the 2100 specific frame restores
*/

.MODULE/ABS=0		ADSP2100_Runtime_Header;

#define put(a,b) DM(I4,b)=a
#define get(a,b) a=DM(I6,b)
.ENTRY  ___lib_frame_swap;^M
.ENTRY  ___lib_frame_restore;^M
.ENTRY	___lib_save_frame;
.ENTRY  ___lib_save_large_frame;
.ENTRY	___lib_restore_frame;
.ENTRY  ___lib_restore_large_frame;
.ENTRY	___lib_save_small_frame;
.ENTRY	___lib_restore_small_frame;

.EXTERNAL   main_, ___lib_setup_args, ___lib_setup_environment,___lib_setup_heap;
.EXTERNAL	___lib_int0_ctrl,___lib_int1_ctrl, ___lib_int2_ctrl, ___lib_int3_ctrl;


___interrupt0:		JUMP ___lib_int0_ctrl;
___interrupt1:		JUMP ___lib_int1_ctrl;
___interrupt2:		JUMP ___lib_int2_ctrl;
___interrupt3:		JUMP ___lib_int3_ctrl;

___reset:		CALL ___lib_setup_environment;
		        CALL ___lib_setup_args;
			CALL main_;
			TRAP;



^M

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
				RTS;

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
				RTS;


___lib_frame_swap:              SI=M4;          /* Hold frame pointer           */^M
                                M4=I4;          /* Swap stack and frame pointer */^M
                                M5=-1;          /* Used for stack pushes        */^M
                                put(SI, M5);    /* Save old frame on stack      */^M
                                put(MR1, M5);   /* Save parent`s return address */^M
                                M5=1;           /* Now setup to read the params */^M
                                I6=1;           /* Begin by reading first param */^M
                                MODIFY(I6,M4);  /* Point to first param         */^M
                                RTS;^M
^M
^M
___lib_frame_restore:           I6=M4;          /* Point to old frame           */^M
                                M5=-1;          /* Used for stack restores      */^M
                                get(SI, M5);    /* Read old frame               */^M
                                get(MR1, M5);   /* Read return address          */^M
                                I6=MR1;         /* Load return address for JUMP */^M
                                I4=M4;          /* Restore old stack pointer    */^M
                                M4=SI;          /* Restore old frame pointer    */^M
                                RTS;^M
^M
^M
.ENDMOD;^M

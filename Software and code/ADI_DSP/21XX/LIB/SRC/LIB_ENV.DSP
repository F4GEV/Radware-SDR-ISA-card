.MODULE/IMAGE	Library_Setup_Environment;

.EXTERNAL	____top_of_stack;
.ENTRY		___lib_setup_environment;
.VAR/DM         ___setup_heap_called;
.GLOBAL         ___setup_heap_called;

___lib_setup_environment:	M4=^____top_of_stack;
				I4=^____top_of_stack;
			   	IMASK=0;		/* Disable ints	*/
				L0=0; L1=0; L2=0; L3=0;	/* Clear L-regs	*/
				L4=0; L5=0; L6=0; L7=0;	/* Clear L-regs	*/
				M2=0;			/* Fixed value	*/
				M1=1;			/* Fixed value	*/
				M6=0;			/* Fixed value	*/
				MF=0;			/* For simulation, give it a value */
				DM(___setup_heap_called) = M6; /* clear flag */
			 	RTS;

.ENDMOD;

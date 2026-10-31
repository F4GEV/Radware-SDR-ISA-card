.MODULE/IMAGE	Library_Setup_Processor;

.ENTRY	___lib_setup_processor;

___lib_setup_processor:	ENA M_MODE;
			ICNTL=0x17;  /*  enables nesting (I hope)  */
			RTS;

.ENDMOD;

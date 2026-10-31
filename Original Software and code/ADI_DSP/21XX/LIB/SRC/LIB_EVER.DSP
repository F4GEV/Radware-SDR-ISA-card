.MODULE/IMAGE	Library_Setup_Everything;

.ENTRY	___lib_setup_everything;

.EXTERNAL	___lib_setup_processor;
.EXTERNAL	___lib_setup_environment;
.EXTERNAL	___lib_setup_args;

___lib_setup_everything:CALL ___lib_setup_processor;
			CALL ___lib_setup_environment;
			CALL ___lib_setup_args;
			RTS;
.ENDMOD;

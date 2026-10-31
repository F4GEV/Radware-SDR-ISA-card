/*
    	This routine sets up the command line arguments to main.  The value
	of argc is one, and the value of argv[0] = 'adi'.

	This routine is called once during the enviroment setup.

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

    	Created on 6/9/93

*/

.MODULE/IMAGE	__Library_setup_args_;

#include	"lib_glob.h";

.VAR 	___lib_argv;				/* argv array		*/
.INIT	___lib_argv : ^___lib_arg1;		/* Point to arg1	*/

.VAR	___lib_arg1[5];				/* First arg string	*/
.INIT 	___lib_arg1 : 97, 100, 105, 0, 0;	/* 'a', 'd', 'i', 0, 0	*/


.ENTRY		___lib_setup_args;

___lib_setup_args:	AX0=0;			/* NULL terminate stack	*/
			M5=-1;			/* Used to push down stk*/
			put(AX0, M5), AR= PASS 1;/* argc == 1		*/
			AY1=^___lib_argv;	/* *argv[0] == 'adi'	*/
			RTS;
.ENDMOD;


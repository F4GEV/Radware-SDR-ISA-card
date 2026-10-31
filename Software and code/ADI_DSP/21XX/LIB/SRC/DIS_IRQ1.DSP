/*
    	This is the interrupt dispatcher which is called from the interrupt
   	vector table.  It is the responsibility of the dispatcher to save
	all scratch registers and setup the C runtime environment for the 
	interrupt handler.

	The C Runtime Library
n	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

    	Created on 8/29/93

*/

.MODULE/IMAGE	__Interrupt_1_Dispatcher;

#include	"lib_glob.h";
#include 	"sig_glob.h";

#define INTERRUPT_NAME SIGINT1
#define UNIQUE(a) __lib_SIGINT1_ ## a

.ENTRY		___lib_int1_ctrl;
		
___lib_int1_ctrl:
		
#include "int_ctrl.h"

.ENDMOD;


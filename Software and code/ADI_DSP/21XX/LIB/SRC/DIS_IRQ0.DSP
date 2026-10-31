/*
    	This is the interrupt dispatcher which is called from the interrupt
   	vector table.  It is the responsibility of the dispatcher to save
	all scratch registers and setup the C runtime environment for the 
	interrupt handler.

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

    	Created on 8/29/93

*/

.MODULE/IMAGE	__Interrupt_0_Dispatcher;

#include	"lib_glob.h";
#include 	"sig_glob.h";

#define INTERRUPT_NAME SIGINT0
#define UNIQUE(a) __lib_SIGINT0_ ## a

.ENTRY		___lib_int0_ctrl;
		
___lib_int0_ctrl:
		
#include "int_ctrl.h"

.ENDMOD;


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

.MODULE/IMAGE	__Sport_0_Recv_Dispatcher;

#include	"lib_glob.h";
#include 	"sig_glob.h";

#define INTERRUPT_NAME SIGSPORT0RECV
#define UNIQUE(a) __lib_SIGSPORT0RECV_ ## a

.ENTRY		___lib_sp0r_ctrl;
		
___lib_sp0r_ctrl:
		
#include "int_ctrl.h"

.ENDMOD;


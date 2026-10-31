/*
    	This is the interrupt dispatcher which is called from the interrupt
   	vector table.  It is the responsibility of the dispatcher to save
	all scratch registers and setup the C runtime environment for the 
	interrupt handler.

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

    	Created on 8/29/93

        Modified for 2171 sw int 9/15/93 SBB

*/

.MODULE/IMAGE	__Swi_1_int_Dispatcher;

#include	"lib_glob.h";
#include 	"sig_glob.h";

#define INTERRUPT_NAME SIGSWI1
#define UNIQUE(a) __lib_SIGSWI1_ ## a

.ENTRY		___lib_swi1_ctrl;
		
___lib_swi1_ctrl:
		
#include "int_ctrl.h"

.ENDMOD;


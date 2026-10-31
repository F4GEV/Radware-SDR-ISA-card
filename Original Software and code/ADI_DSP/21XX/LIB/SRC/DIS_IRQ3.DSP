/*
    	This is the interrupt dispatcher which is called from the interrupt
   	vector table.  It is the responsibility of the dispatcher to save
	all scratch registers and setup the C runtime environment for the 
	interrupt handler.  This interrupt is for the 2100 chip only.

	The C Runtime Library
	
	Alexander Soto
	Development Tools Engineering

    	Created on 3/29/94 

*/

.MODULE/IMAGE	__Interrupt_3_Dispatcher;

#include	"lib_glob.h";
#include 	"sig_glob.h";

#define INTERRUPT_NAME SIGINT3
#define UNIQUE(a) __lib_SIGINT3_ ## a

.ENTRY		___lib_int3_ctrl;
		
___lib_int3_ctrl:
		
#include "int_ctrl.h"

.ENDMOD;


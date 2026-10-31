/*
    	This is the interrupt dispatcher which is called from the interrupt
   	vector table.  It is the responsibility of the dispatcher to save
	all scratch registers and setup the C runtime environment for the 
	interrupt handler.

	The C Runtime Library
	
	Alexander Soto
	Development Tools Engineering

    	Created on 2/17/94

        Modified for 21msp50 codec int

*/

.MODULE/IMAGE	__Codec_dacx_Dispatcher;

#include	"lib_glob.h";
#include 	"sig_glob.h";

#define INTERRUPT_NAME SIGCODECXMIT
#define UNIQUE(a) __lib_SIGCODECXMIT_ ## a

.ENTRY		___lib_dacx_ctrl;
		
___lib_dacx_ctrl:
		
#include "int_ctrl.h"

.ENDMOD;


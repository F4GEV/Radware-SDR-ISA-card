/*
    	This is the interrupt dispatcher which is called from the interrupt
   	vector table.  It is the responsibility of the dispatcher to save
	all scratch registers and setup the C runtime environment for the 
	interrupt handler.

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

    	Created on 8/29/93

        Modified for 21msp50 codec int 9/15/93 SBB

*/

.MODULE/IMAGE	__Codec_Recv_Dispatcher;

#include	"lib_glob.h";
#include 	"sig_glob.h";

#define INTERRUPT_NAME SIGCODECRECV
#define UNIQUE(a) __lib_SIGCODECRECV_ ## a

.ENTRY		___lib_adcr_ctrl;
		
___lib_adcr_ctrl:
		
#include "int_ctrl.h"

.ENDMOD;


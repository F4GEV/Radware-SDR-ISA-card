/*
        These C callable subroutines control the timer of the 
	ADSP2101 family.

	The C Runtime Library for the ADSP21xx family of processors.
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 6/27/93

	#include<21xx.h>
    	int timer_set(unsigned int tperiod, unsigned int tcount, int tscale);
    	unsigned int timer_on(void);
    	unsigned int timer_off(void);

*/
 	
.MODULE/IMAGE		__timer_control__;

#include "lib_glob.h"
#include "21_glob.h"

.ENTRY		timer_set_, timer_on_, timer_off_;

timer_on_:	AR=DM(tcount);			/*Fetch current count*/
		ENA TIMER;			/*Start the timer now*/
		RTS;				/*Return*/

timer_off_:	DIS TIMER;			/*Stop the timer now*/
		AR=DM(tcount);			/*Fetch current count*/
		RTS;

timer_set_:	MR1=TOPPCSTACK;
		CALL ___lib_frame_swap;

		DM(tperiod)=AR;			/*Set TPERIOD*/
		DM(tcount)=AY1;			/*Set TCOUNT*/
		reads(AY1,I6,M5), AR=PASS 1;	/*Fetch TSCALE*/
		DM(tscale)=AY1;
		AY1=MSTAT;			/*Read mode status register*/
		SR0=H#0020;			/*Mask for TIMER enable*/
		AF=SR0 AND AY1;	
		IF EQ AR=PASS 0;		/*Set are to 1 if timer enabled*/
		
restore_state:	JUMP ___lib_frame_restore;

.ENDMOD;

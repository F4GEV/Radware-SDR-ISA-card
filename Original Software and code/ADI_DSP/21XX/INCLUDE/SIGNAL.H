#ifndef __SIGNAL_DEFINED__
#define __SIGNAL_DEFINED__

#define SIG_IGN        0x0000
#define SIG_DFL        0x0000
#define SIG_ERR        0x3fff 

#ifdef __ADSP2100__

#define SIGINT3       0x0008
#define SIGINT2       0x0004
#define SIGINT1       0x0002
#define SIGINT0       0x0001

#define __SIG_FORCE_SHIFT 0

#endif

#ifdef __ADSP2101__

#define SIGINT2       0x0020
#define SIGSPORT0XMIT 0x0010
#define SIGSPORT0RECV 0x0008
#define SIGINT1       0x0004
#define SIGINT0       0x0002
#define SIGTIMER      0x0001

#define SIGSPORT1XMIT SIGINT1
#define SIGSPORT1RECV SIGINT0

#define __SIG_FORCE_SHIFT SIGINT2

#endif

#ifdef __ADSP2105__

#define SIGINT2       0x0020
#define SIGSPORT0XMIT 0x0010
#define SIGSPORT0RECV 0x0008
#define SIGINT1       0x0004
#define SIGINT0       0x0002
#define SIGTIMER      0x0001

#define SIGSPORT1XMIT SIGINT1
#define SIGSPORT1RECV SIGINT0

#define __SIG_FORCE_SHIFT SIGINT2

#endif

#ifdef __ADSP2111__

#define SIGINT2       0x0080
#define SIGHIPWRITE   0x0040
#define SIGHIPREAD    0x0020
#define SIGSPORT0XMIT 0x0010
#define SIGSPORT0RECV 0x0008
#define SIGINT1       0x0004
#define SIGINT0       0x0002
#define SIGTIMER      0x0001

#define SIGSPORT1XMIT SIGINT1
#define SIGSPORT1RECV SIGINT0

#define __SIG_FORCE_SHIFT SIGINT2

#endif

#ifdef __ADSP2150__

#define SIGPOWERDOWN  0x0400
#define SIGINT2       0x0200
#define SIGHIPWRITE   0x0100
#define SIGHIPREAD    0x0080
#define SIGSPORT0XMIT 0x0040
#define SIGSPORT0RECV 0x0020
#define SIGCODECXMIT  0x0010
#define SIGCODECRECV  0x0008
#define SIGINT1       0x0004
#define SIGINT0       0x0002
#define SIGTIMER      0x0001

#define SIGSPORT1XMIT SIGINT1
#define SIGSPORT1RECV SIGINT0

#define __SIG_FORCE_SHIFT SIGPOWERDOWN

#endif

#ifdef __ADSP2171__

#define SIGPOWERDOWN  0x0400
#define SIGINT2       0x0200
#define SIGHIPWRITE   0x0100
#define SIGHIPREAD    0x0080
#define SIGSPORT0XMIT 0x0040
#define SIGSPORT0RECV 0x0020
#define SIGSWI1       0x0010
#define SIGSWI0       0x0008
#define SIGINT1       0x0004
#define SIGINT0       0x0002
#define SIGTIMER      0x0001

#define SIGSPORT1XMIT SIGINT1
#define SIGSPORT1RECV SIGINT0

#define __SIG_FORCE_SHIFT SIGPOWERDOWN

#endif

int raise(int);
int clear_interrupt(int);

void (*signal(int, void (*func)(int)))(int);
void (*interrupt(int, void (*func)(int)))(int);

#define signal(a,b)  ({ void (*__signal)(int) = b; \
asm volatile ( \
"/* These EXTERNALs provide this bit of code access to the memory that	*/" \
"/*  holds the addresses of the handlers for the desired signal.	 The 	*/" \
"/*  memory is allocated in the CRTL routine that supports this interrupt*/" \
"/* The `lib_sure_rts' label always points to and RTS instruction.  This	*/" \
"/*  is needed because the ANSI standard says that handlers setup with	*/" \
"/*  the signal() routine will only respond ONCE to an interrupt, so all	*/" \
"/*  other occurances of the same interrupt will be jump to an RTS.	*/" \
".EXTERNAL __lib_" ## #a ## "_handler;" \
".EXTERNAL __lib_" ## #a ## "_next_handler;" \
".EXTERNAL ___lib_sure_rts;" \
"/* The next few lines of code determine if the enable bit of the desired*/" \
"/*  interrupt should be set or cleared.  The first step is to take the 	*/" \
"/*  address of the handler, and see if it is zero.  The address of the 	*/" \
"/*  handler is negated, and the absolute value is taken.  If the address*/" \
"/*  was zero (indicating that the interrupt should be ignored), the AS	*/" \
"/*  flag will be set to POS.  The AS flag is only modified on by an ABS	*/" \
"/*  instruction, so the value of the flag can be used at the end of the	*/" \
"/*  routine.								*/" \
"AY0=%1;" \
"AR=-AY0;" \
"AR=ABS AR;" \
"/* The next two lines of code save the old IMASK, and set IMASK to zero.*/" \
"/* There is some danger that an interrupt will occur in the cycle after	*/" \
"/*  IMASK has been saved, and before interrupts are disabled.  If this 	*/" \
"/*  occurs, it is possible that the Interrupt Service Routine will alter*/" \
"/*  IMASK.  This would be bad.  See the documentation for more details	*/" \
"AR=IMASK;" \
"IMASK=0;" \
"/* Now the interrupt service routine is setup.  These memory locations	*/" \
"/*  hold the address of the handler for this interrupt.  When the 	*/" \
"/*  interrupt occurs, the dispatcher reads the address of the handler	*/" \
"/*  from these locations.  See the documentation for more details.	*/" \
"%0=DM(__lib_" ## #a ## "_handler);" \
"DM(__lib_" ## #a ## "_handler)=%1;" \
"AY0=^___lib_sure_rts;" \
"DM(__lib_" ## #a ## "_next_handler)=AY0;" \
"/* The new value for IMASK is determined in the next few cycles.  These	*/" \
"/*  instructions must be executed after the interrupts have been 	*/" \
"/*  disabled.  The AS flag (that was generated previously) is used to	*/" \
"/*  test if the interrupt should be enabled (OR'ed) or disabled (AND'ed)*/" \
"/* Once the new IMASK has been determined, interrupts are reanabled.	*/" \
"AY0=%2;" \
"AF=NOT AY0;" \
"IF NEG AR=AR OR AY0;" \
"IF POS AR=AR AND AF;" \
"IMASK=AR;" \
: "=y" (__signal) : "y" (__signal), "K" (a) : "AR", "AY0"); \
__signal;})


#define interrupt(a,b)  ({ void (*__interupt)(int) = b; \
asm volatile (\
"/* These EXTERNALs provide this bit of code access to the memory that	*/" \
"/*  holds the addresses of the handlers for the desired signal.	 The 	*/" \
"/*  memory is allocated in the CRTL routine that supports this interrupt*/" \
".EXTERNAL __lib_" ## #a ## "_handler;" \
".EXTERNAL __lib_" ## #a ## "_next_handler;" \
"/* The next few lines of code determine if the enable bit of the desired*/" \
"/*  interrupt should be set or cleared.  The first step is to take the 	*/" \
"/*  address of the handler, and see if it is zero.  The address of the 	*/" \
"/*  handler is negated, and the absolute value is taken.  If the address*/" \
"/*  was zero (indicating that the interrupt should be ignored), the AS	*/" \
"/*  flag will be set to POS.  The AS flag is only modified on by an ABS	*/" \
"/*  instruction, so the value of the flag can be used at the end of the	*/" \
"/*  routine.								*/" \
"AY0=%1;" \
"AR=-AY0;" \
"AR=ABS AR;" \
"/* The next two lines of code save the old IMASK, and set IMASK to zero.*/" \
"/* There is some danger that an interrupt will occur in the cycle after	*/" \
"/*  IMASK has been saved, and before interrupts are disabled.  If this 	*/" \
"/*  occurs, it is possible that the Interrupt Service Routine will alter*/" \
"/*  IMASK.  This would be bad.  See the documentation for more details	*/" \
"AR=IMASK;" \
"IMASK=0;" \
"/* Now the interrupt service routine is setup.  These memory locations	*/" \
"/*  hold the address of the handler for this interrupt.  When the 	*/" \
"/*  interrupt occurs, the dispatcher reads the address of the handler	*/" \
"/*  from these locations.  See the documentation for more details.	*/" \
"%0=DM(__lib_" ## #a ## "_handler);" \
"DM(__lib_" ## #a ## "_handler)=%1;" \
"DM(__lib_" ## #a ## "_next_handler)=%1;" \
"/* The new value for IMASK is determined in the next few cycles.  These	*/" \
"/*  instructions must be executed after the interrupts have been 	*/" \
"/*  disabled.  The AS flag (that was generated previously) is used to	*/" \
"/*  test if the interrupt should be enabled (OR'ed) or disabled (AND'ed)*/" \
"/* Once the new IMASK has been determined, interrupts are reanabled.	*/" \
"AY0=%2;" \
"AF=NOT AY0;" \
"IF NEG AR=AR OR AY0;" \
"IF POS AR=AR AND AF;" \
"IMASK=AR;" \
: "=y" (__interupt) : "y" (__interupt), "K" (a) : "AR", "AY0"); \
__interupt;})

#define raise(a) asm volatile("IFC=%0;nop;nop;" : : "e" (a * 2 * __SIG_FORCE_SHIFT))
#define clear_interrupt(a) asm volatile("IFC=%0;nop; nop;": : "e" (a))

/* "nop"s were added in the lines above to prevent the compiler from moving them
around significantly enough to damage anything  */

#endif


/* @(#)setjmp.h	1.4 12/18/92 1 */

/*	This is the include file for the long jump routines*/


#ifndef __SETJMP_DEFINED
#define __SETJMP_DEFINED


typedef	int jmp_buf[22];

int 	setjmp(jmp_buf env);
void 	longjmp(jmp_buf env, int return_value);


#endif


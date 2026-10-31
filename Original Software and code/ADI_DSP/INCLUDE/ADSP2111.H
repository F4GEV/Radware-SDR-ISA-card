/*
This include file indicates that this program
is intended for use on an ADSP-2111 platform.
*/


#ifndef ___ADSP2111_DEFINED
#define ___ADSP2111_DEFINED

#define	signal(a,b) signal2111(a,b)
#define raise(a) raise2111(a)
#define interrupt(a,b) interrupt2111(a,b)

#endif
  

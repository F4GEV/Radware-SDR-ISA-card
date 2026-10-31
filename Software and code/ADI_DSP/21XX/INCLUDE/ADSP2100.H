/*
This include file indicates that this program
is intended for use on an ADSP-2100 platform.
*/


#ifndef ___ADSP2100_DEFINED
#define ___ADSP2100_DEFINED

#define	signal(a,b) signal2100(a,b)
#define raise(a) raise2100(a)
 
#define interrupt(a,b) interrupt2100(a,b)

#endif


  

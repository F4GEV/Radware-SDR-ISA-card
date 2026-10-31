/*
This include file indicates that this program
is intended for use on an ADSP-2150 platform.
*/


#ifndef ___ADSP2150_DEFINED
#define ___ADSP2150_DEFINED

#define	signal(a,b) signal2150(a,b)
#define raise(a) raise2150(a)
#define interrupt(a,b) interrupt2150(a,b)

#endif 

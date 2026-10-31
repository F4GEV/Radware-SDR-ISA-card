/*
This include file indicates that this program
is intended for use on an ADSP-2101/2 platform.
*/


#ifndef ___ADSP2101_DEFINED
#define ___ADSP2101_DEFINED

#define	signal(a,b) signal2101(a,b)
#define raise(a) raise2101(a)
 
#define interrupt(a,b) interrupt2101(a,b)

#endif
  

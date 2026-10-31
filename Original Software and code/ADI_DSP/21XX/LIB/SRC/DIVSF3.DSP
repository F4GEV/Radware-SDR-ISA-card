/* 
	This routine is a compiler support routine that
	computes the division of two floating point numbers.

	The C Runtime Library
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

	Created on 4/22/93
*/

.MODULE/IMAGE	__divsf3__;

#include "lib_glob.h"
#include "flt_glob.h"
#include "ifp_glob.h"

.ENTRY	__divsf3_;

__divsf3_:	MR1=TOPPCSTACK;
		CALL ___lib_save_small_frame;

		reads(MR1,I6,M5);
		reads(MR0,I6,M5);
	
		CALL ___lib_sf_libfp;

		AY0=AR;
		MY0=SR0;
		MY1=SR1;

		reads(MR1,I6,M5);
		reads(MR0,I6,M5);
	
		CALL ___lib_sf_libfp;

		AX0=AY0;
		MR0=MY0;
		MR1=MY1;
		AY0=AR;
		MY0=SR0;
		MY1=SR1;
		
		CALL ___lib_libfpdiv;
		
		AX0=AR;
		MR0=SR0;
		MR1=SR1;
		
		CALL ___lib_libfp_sf;		
		
__restore_state:JUMP ___lib_restore_small_frame;

.ENDMOD;



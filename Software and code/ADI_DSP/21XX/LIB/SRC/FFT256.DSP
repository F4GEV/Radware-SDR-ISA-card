/*
	This subroutine computes the Decimation in Time (DIT)
	Fast Fourier Transform (FFT) of its fixed point input.
	The integer value of the block exponent is returned
	by fft256.

	The C Applications Library

	Created on 7/20/93 

	#include <ffts.h>;
	int fft256(int in_real[], int out_real[], int out_real[], int out_imag[]);
		NOTE: The input and output arrays must be different & the output
		      arrays must be one circular boundries
*/

.CONST          	N_=256, N_div_2_=128;    /*Const. for 256 points*/
.CONST			nover2_=128, nover4_=64;
.CONST			mod_value_=H#0040;
.CONST			log2Nminus2_=6, Nminus1_=255;

.MODULE/IMAGE		__256_DIT_FFT_Routine;
.VAR/PM/ANY/CIRC        twid_real [N_div_2_];
.VAR/PM/ANY/CIRC        twid_imag [N_div_2_];


#include		<lib_glob.h>;
#include		<fft_glob.h>;

.EXTERNAL		__fftcore;

#include	<t256_rl.h>;
#include	<t256_im.h>;

.ENTRY		fft256_;

fft256_:	I1=I4;		/* push values for fft256.  We haven't
				   done frame stuff, but we'll be okay */
		M3=-2;
		MODIFY(I1,M3);  /* pass over prev frame and return address */
		M3=-1;
		AX1=N_;
		DM(I1,M3)=AX1;
		AX1=mod_value_;
		DM(I1,M3)=AX1;
		AX1=Nminus1_;
		DM(I1,M3)=AX1;
		AX1=nover2_;
		DM(I1,M3)=AX1;
		M3=-3;
		MODIFY(I1,M3);  /* skip over out_* and node_space */
		M3=-1;
		AX1=^twid_real;
		DM(I1,M3)=AX1;
		AX1=^twid_imag;
		DM(I1,M3)=AX1;
		MODIFY(I1,M3);   /* skip over bflys_per_group and groups */
		MODIFY(I1,M3);   
		AX1=nover4_;
		DM(I1,M3)=AX1;
		AX1=log2Nminus2_;
		DM(I1,M3)=AX1;
		JUMP __fftcore;

.ENDMOD;

/* This file is the include file for the ffts. */

/* These are symbolic names for the stack slots 
   we are using for local variables.  Stack slots 
   are used instead of data space for reenterency. */

      /* these order of these stack slots better not
	 be fiddled with without studying fft.dsp and
	 all the fftN and ifftN routines. Those routines
	 are all very dependent on various variables 
	 being in adjacent stack slots. */

#define N               -2
#define mod_value       -3
#define Nminus1         -4
#define nover2          -5
#define out_real        -6   
#define out_imag        -7
#define node_space      -8
#define twiddle_real    -9
#define twiddle_imag    -10
#define bflys_per_group -11
#define groups          -12
#define nover4          -13
#define log2Nminus2     -14
#define blk_exponent    -15   

       /* fft_locals must be (largest stack slot number - 1) */
#define fft_locals    -16







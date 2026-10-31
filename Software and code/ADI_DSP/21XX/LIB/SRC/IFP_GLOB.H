/* This files is included by all routines that use the internal floating
   point format of the ADSP21xx C runtime environment.  The internal format
   is a three word (48-bit) format.  The first 16-bits represent a 
   signed, unbiased exponent.  The next 32 bits represent the signed, 1.31 
   format mantissa.

   This format provides more accuracy than IEEE-single, allowing most math
   routines to return results with less than 1 LSB error.
*/

.EXTERNAL   ___lib_libfpmult, ___lib_libfpdiv;
.EXTERNAL   ___lib_libfpadd, ___lib_libfpsub;
.EXTERNAL   ___lib_libfpnorm;

.EXTERNAL   ___lib_libfp_sf, ___lib_sf_libfp;

.EXTERNAL  ___lib_libfpcp;
.EXTERNAL  ___lib_3wdtoi, ___libr_3wdtoi, ___lib_ito3wd; 
.EXTERNAL  ___lib_3wd_neg, ___lib_3wd_abs;


/* @(#)float.h	1.9 2/15/93 1 */

/* characteristics of floating point numbers */

#ifndef __FLOAT_DEFINED
#define __FLOAT_DEFINED

#define FLT_ROUNDS 1

#define	FLT_RADIX	2

#define	FLT_MANT_DIG	24	
#define	DBL_MANT_DIG	FLT_MANT_DIG
#define	LDBL_MANT_DIG	DBL_MANT_DIG	

#define	FLT_DIG		6
#define	DBL_DIG	        FLT_DIG
#define	LDBL_DIG	DBL_DIG	

#define	FLT_MIN_EXP	-126
#define	DBL_MIN_EXP	FLT_MIN_EXP
#define	LDBL_MIN_EXP	DBL_MIN_EXP	

#define	FLT_MIN_10_EXP	-37
#define	DBL_MIN_10_EXP	FLT_MIN_10_EXP
#define	LDBL_MIN_10_EXP	DBL_MIN_10_EXP

#define	FLT_MAX_EXP	127
#define	DBL_MAX_EXP	FLT_MAX_EXP
#define	LDBL_MAX_EXP	DBL_MAX_EXP	

#define	FLT_MAX_10_EXP	38
#define	DBL_MAX_10_EXP	FLT_MAX_10_EXP
#define	LDBL_MAX_10_EXP	DBL_MAX_10_EXP

#define FLT_MAX 	3.40282346638528860e+38
#define DBL_MAX 	FLT_MAX
#define	LDBL_MAX	DBL_MAX	

#define	FLT_EPSILON	1.1920928955078125E-07F
#define	DBL_EPSILON	FLT_EPSILON
#define	LDBL_EPSILON	DBL_EPSILON

#define	FLT_MIN		1.1754943508222875E-38F
#define	DBL_MIN		FLT_MIN
#define	LDBL_MIN	DBL_MIN
	
#endif



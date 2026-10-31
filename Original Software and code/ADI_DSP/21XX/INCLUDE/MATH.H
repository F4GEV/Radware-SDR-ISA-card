/* @(#)math.h	1.15 5/10/93 1 */


#ifndef __MATH_DEFINED
#define __MATH_DEFINED

#ifndef HUGE_VAL
#define HUGE_VAL    99.e999
#endif

double  acos (double x);
double  asin (double x);
double  atan (double x);
double  atan2(double x, double y);
double  ceil (double x);
double  cos (double x);
double  cosh (double x);
double  exp (double x);
double  fabs (double x);
double  floor (double x);
double  fmod (double x, double y);
double	frexp (double x, int *y);
double	ldexp (double x, int y);
double  log (double x);
double  log10 (double x);
double	modf (double x, double *y);
double  pow (double x, double y);
double  sin (double x);
double  sinh (double x);
double	sqrt (double x);
double  tan (double x);
double  tanh (double x);

#endif

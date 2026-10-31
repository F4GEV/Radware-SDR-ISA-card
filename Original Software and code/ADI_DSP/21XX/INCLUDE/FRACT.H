#define INT_TO_FRACT(a) a*.000030517578125
#define FRACT_TO_INT(a) a*32768.0

#define FRACT_MULTIPLY(a,b)\
({ int __temp;\
asm("MX0=%1;MR=MX0*%2 (SS); MR=MR+MX0*%2 (SS);%0=MR1;"\
: "=e" (__temp)\
: "e" (a), "B" (b)\
: "MX0", "MR0", "MR1", "MR2");\
__temp;})

#define FRACT_MAC(a,b,c)\
({ long __temp_mac = c;\
int __temp_m = ((__temp_mac>>16)&0xffff), __temp_l = (__temp_mac&0xffff);\
asm("MR0=%4;MR1=%5;MX0=%2;MR=MX0*%3 (SS); MR=MR+MX0*%3 (SS);%0=MR1;%1=MR0;"\
: "=e" (__temp_m), "=e" (__temp_l)\
: "e" (a), "B" (b), "e" ( __temp_l), "e" (__temp_m)\
: "MX0", "MR0", "MR1", "MR2");\
__temp_mac = (((long)(__temp_m))<<16)&0xffff | __temp_l&0xffff;})

#ifndef __ASM_SPRT_DEFINED
#define __ASM_SPRT_DEFINED

#define function_entry SI=M4;M4=I4;MR1=TOPPCSTACK;M5=-1;DM(I4,M5)=SI;DM(I4,M5)=MR1
#define leaf_entry     SI=M4;M4=I4;M5=-1;DM(I4,M5)=SI

#define leaf_exit      I6=M4;SI=DM(I6,M6);I4=M4;M4=SI;RTS
#define exit           I6=M4;M5=-1;SI=DM(I6,M5);MR1=DM(I6,M6);I4=M4;M4=SI;I6=MR1;JUMP (I6)

#define readsfirst(a)  M5=1;I6=1;MODIFY(I6,M4);a=DM(I6,M5)     
#define readsnext      DM(I6,M5)

#define putsfirst      M5=-1;DM(I4,M5)
#define putsnext       DM(I4,M5)

#define getsfirst(a)   M5=1;I6=1;MODIFY(I6,M4);a=DM(I6,M5)
#define getsnext       DM(I6,M5)
#define alter(x)       M5=x;MODIFY(I4,M5)

#define save_reg       DM(I4,M5)=AY0;DM(I4,M5)=AX0;DM(I4,M5)=AX1;\
                       DM(I4,M5)=MY0;DM(I4,M5)=MX0;DM(I4,M5)=MX1;\
                       SR0=I0;DM(I4,M5)=SR0;\
                       SR0=M0;DM(I4,M5)=SR0;\
                       SR0=I5;DM(I4,M5)=SR0;\
                       SR0=I7;DM(I4,M5)=SR0;\
                       SR0=M7;DM(I4,M5)=SR0

#define restore_reg    I6=I4;M5=1;MODIFY(I6,M5);\
                       SR0=DM(I6,M5); M7=SR0;\
                       SR0=DM(I6,M5); I7=SR0;\
                       SR0=DM(I6,M5); I5=SR0;\
                       SR0=DM(I6,M5); M0=SR0;\
                       SR0=DM(I6,M5); I0=SR0;\
                       MX1=DM(I6,M5); MX0=DM(I6,M5); MY0=DM(I6,M5);\
                       AX1=DM(I6,M5); AX0=DM(I6,M5); AY0=DM(I6,M5);\
                       M5=11;MODIFY(I4,M5)

#endif


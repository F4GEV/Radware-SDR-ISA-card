/*
    This C callable subroutine produces the ceiling value of it's input.

    DSP Applications Library.		    Alexander Soto

    Created on 11/1/93

    #include <math.h>				<Header>
    double ceil (double v);			<Prototype>
*/

.MODULE/IMAGE	__CEIL_Module__;

#include	"lib_glob.h";
#include	"ifp_glob.h";


.ENTRY		ceil_, ceilf_;

ceil_:
ceilf_:		MR1=TOPPCSTACK;
                CALL ___lib_save_small_frame;

		reads(MR1,I6,M5);		/*Fetch MSW of input V	*/
                reads(MR0,I6,M5), AR=PASS MR1;
		AX1=AR;				/*Save sign		*/
		AY0=31;				/*Used for exp test	*/
		CALL ___lib_sf_libfp;		/*Change V into 3 word	*/
		AX0=1;				/*Load 1 in 3 wd	*/
		MR1=0x4000;
		MR0=0;
		AF=AR-AY0;			/*If > 31 return	*/
		IF GT JUMP __three_to_two;	/*Input contains no fractional part*/
		IF NE JUMP __ceil_cont;
		AF=PASS AR;
		AR=AX0+AF;			/*Must remove an MSB of input*/
		AX0=AR, AR=PASS AF;		/*if big exp (scalb)	*/

__ceil_cont:	AF=PASS AX1;			/*Get original sign	*/
		AF=SR1 XOR AF;			/*Should you negate number?*/
		IF LT CALL ___lib_3wd_neg;	/*Signs are not the same, negate*/
		M5=-1;
		DM(I4,M5)=AX0;			/*Save scale value	*/
                DM(I4,M5)=MR1;
                DM(I4,M5)=MR0;
		AY0=AR;				/*Move V into Y regs	*/
		MY1=SR1;
		MY0=SR0;
		CALL ___lib_libfpsub;		/*Subtract V from scale	*/
		CALL ___lib_3wd_neg;
		AX0=AR;
                MR1=SR1;
                MR0=SR0;
                CALL ___libr_3wdtoi;		/*Convert to fixed pt.	*/
		CALL ___lib_ito3wd;		/*Convert to floating pt*/
		I6=I4;
                M5=1;
                MODIFY(I6,M5);
                MR0=DM(I6,M5);			/*Pop scale value	*/
                MR1=DM(I6,M5);
                AX0=DM(I6,M6);
                I4=I6;
		M5=-1;
		DM(I4,M5)=AY0;			/*Save V value		*/
                DM(I4,M5)=MY1;
                DM(I4,M5)=MY0;
		AY0=AR;				/*Move new float into Y regs*/
		MY1=SR1;
		MY0=SR0;
		CALL ___lib_libfpadd;		/*Add back scale if necessary*/
		I6=I4;
                M5=1;
                MODIFY(I6,M5);
                MR0=DM(I6,M5);			/*Pop V value		*/
                MR1=DM(I6,M5);
                AX0=DM(I6,M6);
                I4=I6;
		AY0=AR;
		MY1=SR1;
		MY0=SR0;
		CALL ___lib_libfpsub;		/*Find sign of input	*/
		AR=PASS AY0, SR0=MY0;		/*Move Y reg to Z reg	*/
		AF=PASS SR1, SR1=MY1;		/*in case you're done	*/
		IF LE JUMP __three_to_two;	/*If MSW <=0 jump	*/
		AX0=1;
		MR1=0x4000;
		MR0=0;
		CALL ___lib_libfpadd;
		
__three_to_two: AX0=AR;
		MR1=SR1;
		MR0=SR0;
		CALL ___lib_libfp_sf;           /*Transform back to IEEE*/

__restore_state: JUMP ___lib_restore_small_frame;

.ENDMOD;

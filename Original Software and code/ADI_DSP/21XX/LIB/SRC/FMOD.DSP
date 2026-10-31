/*
    This C callable subroutine produces the floating pt modulo value of it's
    input.

    DSP Applications Library.		    Alexander Soto

    Created on 11/2/93

    #include <math.h>				<Header>
    double fmod (double v, double u);		<Prototype>
*/

.MODULE/IMAGE	__FMOD_Module__;

#include	"lib_glob.h";
#include	"ifp_glob.h";


.ENTRY		fmod_, fmodf_;

fmod_:
fmodf_:		MR1=TOPPCSTACK;
                CALL ___lib_save_small_frame;

		SR=LSHIFT AR BY -100 (LO);	/*Load SR with 0	*/
		reads(MR1,I6,M5);		/*Fetch MSW of input V	*/
                reads(MR0,I6,M5), AR=PASS MR1;
		IF EQ JUMP __restore_state;	/*Denominator=0		*/
		AX1=AR;				/*Save sign		*/
		CALL ___lib_sf_libfp;
		AY0=AR;				/*Move V into Y regs	*/
		MY1=SR1;
		MY0=SR0;
		reads(MR1,I6,M5);		/*Fetch MSW of input U	*/
                reads(MR0,I6,M5);
		CALL ___lib_sf_libfp;
		AX0=AY0;			/*Move V into X regs	*/
		MR1=MY1;
		MR0=MY0;
		M5=-1;
		DM(I4,M5)=AX0;			/*Save V value (level 1)*/
                DM(I4,M5)=MR1;
                DM(I4,M5)=MR0;
		AY0=AR;				/*Move U into Y regs	*/
		MY1=SR1;
		MY0=SR0;
		CALL ___lib_libfpdiv;		/*Q=V/U			*/
		AY1=31;				/*Used for exp test	*/
		AF=AR-AY1;			/*If > 31 return	*/
		AX0=AY0;			/*Move U into x reg in	*/
		MR1=MY1;			/*case we jump.		*/
		MR0=MY0;
		IF GT JUMP __compute_modulo;	/*Input has all integer	*/
		DM(I4,M5)=AX0;			/*Save U value (level 2)*/
                DM(I4,M5)=MR1;
                DM(I4,M5)=MR0;
		AX0=1;				/*Load 1 in 3 wd	*/
		MR1=0x4000;
		MR0=0;
		IF NE JUMP __fmod_cont;
		AF=PASS AR;
		AR=AX0+AF;			/*Must remove an MSB of input*/
		AX0=AR, AR=PASS AF;		/*if big exp (scalb)	*/

__fmod_cont:	AF=PASS AX1;			/*Get original sign	*/
		AF=SR1 XOR AF;			/*Should you negate number?*/
		IF LT CALL ___lib_3wd_neg;	/*Signs are not the same, negate*/
		M5=-1;
		DM(I4,M5)=AX0;			/*Save scale value (level 3)*/
                DM(I4,M5)=MR1;
                DM(I4,M5)=MR0;
		AY0=AR;				/*Move Q into Y regs	*/
		MY1=SR1;
		MY0=SR0;
		CALL ___lib_libfpsub;		/*Subtract Q from scale	*/
		CALL ___lib_3wd_neg;		/*Chg: sub scale from Q	*/
		AX0=AR;
                MR1=SR1;
                MR0=SR0;
                CALL ___lib_3wdtoi;		/*Convert to fixed pt.	*/
		CALL ___lib_ito3wd;		/*Convert to floating pt*/
		I6=I4;
                M5=1;
                MODIFY(I6,M5);
                MR0=DM(I6,M5);			/*Pop scale value (level 2)*/
                MR1=DM(I6,M5);
                AX0=DM(I6,M6);
                I4=I6;
		AY0=AR;				/*Move new float into Y regs*/
		MY1=SR1;
		MY0=SR0;
		CALL ___lib_libfpadd;		/*Add back scale	*/
		I6=I4;
                M5=1;
                MODIFY(I6,M5);
                MR0=DM(I6,M5);			/*Pop U value (level 1)	*/
                MR1=DM(I6,M5);
                AX0=DM(I6,M6);
                I4=I6;

__compute_modulo: AY0=AR;
		MY1=SR1;
		MY0=SR0;
		CALL ___lib_libfpmult;		/*Compute i*y		*/
		I6=I4;
                M5=1;
                MODIFY(I6,M5);
                MR0=DM(I6,M5);			/*Pop V value (level 0)	*/
                MR1=DM(I6,M5);
                AX0=DM(I6,M6);
                I4=I6;
		AY0=AR;
		MY1=SR1;
		MY0=SR0;
		CALL ___lib_libfpsub;		/*Compute x-i*y		*/
				
__three_to_two: AX0=AR;
		MR1=SR1;
		MR0=SR0;
		CALL ___lib_libfp_sf;           /*Transform back to IEEE*/

__restore_state: JUMP ___lib_restore_small_frame;

.ENDMOD;

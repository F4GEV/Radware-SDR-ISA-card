/*
    This C callable subroutine produces the arctangent of its floating	
    point input.

    DSP Applications Library.		    Alexander Soto

    Created on 9/1/93

    #include <math.h>				<Header>
    double atan(double v);			<Prototype>
    double atan2(double v, double u);		<Prototype>
*/

.MODULE/IMAGE	Arctangent_Module;

#include	"lib_glob.h";
#include	"mth_glob.h";
#include	"ifp_glob.h";

#define F_EXP_BIAS 127

/* The values used in this program are taken from the atan.h
used in the 21k lib/src directory.  The following are they:
	
	sqrt_3 1.7320508075
	sqrt_3_minus_1 0.7320508075
	two_minus_sqrt_3 0.2679491924

	eps 0.000244140625

	p0 -0.144008344874E+1
	p1 -0.720026848898E+0
	q0  0.432025038919E+1
	q1  0.475222584599E+1

*/

.ENTRY		atan_, atan2_;

atan2_:		AY1=1;
		JUMP __begin_compute;

atan_:		AY1=0;

__begin_compute: MR1=TOPPCSTACK;
                CALL ___lib_save_small_frame;

		M3=0;
		reads(MR1,I6,M5), AR=PASS 0;	/*Fetch MSW of input V  */
		reads(MR0,I6,M5), AF=PASS MR1;	/*Fetch LSW of input V  */
		IF LT AR=PASS -1;
		I1=AR;				/*Is V neg? save msw	*/
		AR=PASS AY1;			/*doing atan or atan2 ?	*/
		IF EQ JUMP __change_format;

		reads(AX1,I6,M5), AR=PASS 0;	/*Fetch U msw           */
		reads(AY0,I6,M5), AF=PASS AX1;	/*check for msw of U=0	*/
		IF LT AR=PASS -1;		/*Set flag for neg val	*/
		M3=AR;
		IF NE JUMP __check_overflow;	/*Test U=0, cont otherwise*/
		
		AF=PASS MR1;			/*Test V msw=0		*/
		IF NE JUMP __return_half_PI;	/*If V=0 return PI/2	*/
		SR=LSHIFT AR BY -100 (LO);      /*Load SR with a zero	*/
		SI=EDOM;			/*Set errno=EDOM	*/
		CALL ___lib_set_errno;		/*Set global errno	*/
		JUMP __restore_state;

__change_format: CALL ___lib_sf_libfp;		/*Change V into 3 word	*/
		JUMP __compute_atan;

__check_overflow: AY1=0x7f80;
		AR=AX1 AND AY1;			/*Mask out all but exp 	*/
		SR=LSHIFT AR BY -7 (LO);        /*Exp now in LSBs	*/
		AR=F_EXP_BIAS-1;		/*Load exp bias - 1	*/
		AF=PASS AR;
                AR=SR0-AF;         	    	/*Compute exp for save	*/

		AR=MR1 AND AY1, AY1=AR;		/*Repeat process for V	*/
		SR=LSHIFT AR BY -7 (LO);        /*Exp now in LSBs	*/
		AR=SR0-AF;

		AR=AR-AY1;			/*Subtract exps		*/
		AY1=124;			/*Max exponent - 3	*/
		AF=AR-AY1;
		IF GE JUMP __return_half_PI;	/*Overflowed		*/
		AR=-AY1, AY1=AR;		/*Check for underflow	*/
		AR=AY1-AR;
		IF GT JUMP __do_division;	/*If no underflow, cont	*/
		AR=I1;				/*msw of V neg?		*/
		AF=PASS AR;
		AR=0;
		AY1=-1;
		IF NE AR=AR-AY1;		/*Set flag to flip sign?*/
		AY1=M3;				/*msw of U neg?		*/
		AF=PASS AY1;
		AY1=1;
		IF NE AR=AR+AY1;		/*Set flag to flip sign?*/
		SR1=0x4049;			/*Load PI		*/
		SR0=0x0fdb;
		AF=PASS AR;
		IF NE JUMP __flip_sign;		/*Do iff one sign was neg*/
		JUMP __restore_state;
		

__do_division:	CALL ___lib_sf_libfp;		/*Change V into 3 word	*/
		MR1=AX1;			/*Load U		*/
		MR0=AY0;
		AY0=AR;				/*Store results	temp	*/
		MY1=SR1;
		MY0=SR0;
		CALL ___lib_sf_libfp;		/*Change U into 3 word	*/
		AX0=AY0;			/*Do some swapping to 	*/
		AY0=AR;				/*get values in x and y	*/
		MR1=MY1;			/*respectively		*/
		MY1=SR1;
		MR0=MY0;
		MY0=SR0;
		CALL ___lib_libfpdiv;		/*Divide V/U --> f	*/
		
__compute_atan:	SB=0;				/*N=0 (Cody & Waite)	*/
		MR1=AR;				/*Get ABS of 3 wd	*/
		AR=SR0, AF=ABS SR1;
		IF NEG AR=-SR0;
		IF NEG AF=-SR1+C-1;
		SR0=AR, AR=PASS AF;
		SR1=AR, AR=PASS MR1;
		AY0=AR;				/*Move f		*/
		MY1=SR1;
		MY0=SR0;
		AX0=1;				/*1.0 in 3 wd format	*/
		MR1=0x4000;
		MR0=0;
		CALL ___lib_libfpcp;		/*Cmp 1 to f		*/
		AF=PASS AR, AR=AY0;		/*Restore exp		*/
		IF GT JUMP __test_f;		/*If f < 1 jump		*/
		CALL ___lib_libfpdiv;		/*Divide 1/f		*/
		SB=2;				/*N=2			*/

__test_f:	AY0=AR;				/*Move f		*/
		MY1=SR1;
		MY0=SR0;
		AX0=0xffff;			/*Load 2-sqrt(3) in 3 wd*/
		MR1=0x4498;			/*format		*/
		MR0=0x5170;
		CALL ___lib_libfpcp;		/*Compare bound to f	*/
		AF=PASS AR, AR=AY0;		/*Move exp value back 	*/
		IF GT JUMP __test_eps;		/* iff (f <= bound) jump*/
		AY1=SB;
		AR=AY1+1;			/*N=N+1			*/
		SB=AR;
		AX0=0;				/*Load in sqrt(3)-1 in	*/
		MR1=0x5db3;			/*3-word format (=A).	*/
		MR0=0xd747;
		call ___lib_libfpmult;		/* A*f			*/
		AX0=AR;				/*move answer		*/
		MR1=SR1;
		MR0=SR0;
		M5=-1;
		DM(I4,M5)=AY0;			/*B'cse of register	*/
		DM(I4,M5)=MY1;			/*limitations, we must	*/
		DM(I4,M5)=MY0;			/*push 'f' on stack	*/
		AY0=1;				/*1.0 in 3 wd format	*/
		MY1=0x4000;
		MY0=0;

		/*NOTE:  If not enough precision you should subtract .5	*/
		/*    twice instead.  AY0=0x0,MY1=0x4000,MY0=0x0000.	*/
		/*    This will give you an extra bit of precision.	*/

		CALL ___lib_libfpsub;		/*(A*f)-1		*/
		AX0=AR;				/*move answer		*/
		MR1=SR1;
		MR0=SR0;
		M5=1;
		I6=I4;
		MODIFY(I6,M5);
		MY0=DM(I6,M5);			/*pop previously pushed	*/ 
		MY1=DM(I6,M5);			/*value (f). (Say that	*/
		AY0=DM(I6,M6);			/*10 times)  :)		*/
		CALL ___lib_libfpadd;		/* ((A*f)-1)+f		*/
		M5=-1;
		I4=I6;
		DM(I4,M5)=AR;			/*B'cse of reg limits,	*/
		DM(I4,M5)=SR1;			/*we must push		*/
		DM(I4,M5)=SR0;			/*'((A*f)-1)+f' on stk	*/
		AX0=1;				/*Load sqrt(3) in 3 wd	*/
		MR1=0x6ed9;
		MR0=0xeb99;
		CALL ___lib_libfpadd;		/* sqrt(3)+f		*/
		M5=1;
		I6=I4;
		MODIFY(I6,M5);
		MR0=DM(I6,M5);			/*pop '(A*f)-1)+f'	*/
		MR1=DM(I6,M5);
		AX0=DM(I6,M6);
		AY0=AR;				/*move sqrt(3)+f	*/
		MY1=SR1;			
		MY0=SR0;
		I4=I6;				/*Before I6 gets trashed*/
		CALL ___lib_libfpdiv;		/*((A*f)-1)+f)/(sqrt(3)+f)*/

__test_eps:	M5=-1;
		DM(I4,M5)=AR;			/*B'cse of register	*/
		DM(I4,M5)=SR1;			/*limits, we must push	*/
		DM(I4,M5)=SR0;			/*new f value on stack	*/
		MR1=AR;				/*Do abs of new f	*/
		AR=SR0, AF=ABS SR1;
		IF NEG AR=-SR0;
		IF NEG AF=-SR1+C-1;
		SR0=AR, AR=PASS AF;
		SR1=AR, AR=PASS MR1;
		AX0=AR;				/*Load exp abs (newf)	*/
		MR1=SR1;			/*Load msw abs (newf)	*/
		MR0=SR0;			/*Test for small input	*/
		AY0=0xfff1;			/*Load eps=1.525878e-05	*/
		MY1=0x4000;			/*or 2^-16		*/
		MY0=0x0000;			/*fixed pt. was 2**-8	*/
		CALL ___lib_libfpcp;		/*Cmp abs(newf) and eps */	
		AF=PASS AR;
		IF LE JUMP __check_n;
		M5=1;
		I6=I4;
		MODIFY(I6,M5);
		MY0=DM(I6,M5);			/*pop previously pushed	*/ 
		MY1=DM(I6,M5);			/*value (new f). 	*/
		AY0=DM(I6,M6);
		AX0=AY0;
		MR1=MY1;
		MR0=MY0;
		call ___lib_libfpmult;		/*g=newf*newf		*/
		M5=-1;
		I4=I6;
		DM(I4,M5)=AY0;			/*B'cse of register	*/
		DM(I4,M5)=MY1;			/*limitations, we must	*/
		DM(I4,M5)=MY0;			/*push 'newf' on stack	*/
		AY0=AR;				/*move g (depth 1)	*/
		MY1=SR1;			
		MY0=SR0;
		AX0=0;
		MR1=0xa3d6;
		MR0=0x2904;
		call ___lib_libfpmult;		/*P(g)=p1*g		*/
		DM(I4,M5)=AY0;			/*B'cse of register	*/
		DM(I4,M5)=MY1;			/*limitations, we must	*/
		DM(I4,M5)=MY0;			/*push 'g' on stack	*/
		AY0=AR;				/*move P(g) (depth 2)	*/
		MY1=SR1;			
		MY0=SR0;
		AX0=1;				/*Load p0 in 3 wd.	*/
		MR1=0xa3d5;
		MR0=0xac3c;
		CALL ___lib_libfpadd;		/*P(g)=(p1*g)+p0		*/
		AX0=AR;				/*move P(g)		*/
		MR1=SR1;
		MR0=SR0;
		M5=1;
		I6=I4;
		MODIFY(I6,M5);
		MY0=DM(I6,M5);			/*pop previously pushed	*/ 
		MY1=DM(I6,M5);			/*value (g).  (depth 1)	*/
		AY0=DM(I6,M6);
		call ___lib_libfpmult;		/*P(g)=(p1*g+p0)*g	*/
		M5=-1;
		I4=I6;
		DM(I4,M5)=AR;			/*B'cse of reg limits,	*/
		DM(I4,M5)=SR1;			/*we must push P(g) on	*/
		DM(I4,M5)=SR0;			/*stack	 (depth 2)	*/
		AX0=0x0003;			/*Load q1 in 3 wd.	*/
		MR1=0x4c09;
		MR0=0x1df7;
		CALL ___lib_libfpadd;		/*Q(g)=q1+g		*/
		AX0=AR;				/*move Q(g)		*/
		MR1=SR1;			
		MR0=SR0;
		call ___lib_libfpmult;		/*Q(g)=(q1+g)*g		*/
		AX0=AR;				/*move Q(g)		*/
		MR1=SR1;			
		MR0=SR0;
		AY0=3;				/*Load q0		*/
		MY1=0x451f;
		MY0=0xbedf;
		CALL ___lib_libfpadd;		/*Q(g)=(q1+g)*g+q0	*/
		AY0=AR;
		MY1=SR1;
		MY0=SR0;
		M5=1;
		I6=I4;
		MODIFY(I6,M5);
		MR0=DM(I6,M5);			/*pop previously pushed	*/ 
		MR1=DM(I6,M5);			/*value P(g) (depth 1)	*/
		AX0=DM(I6,M5);
		AX1=I6;				/*fpdiv uses I6		*/
		CALL ___lib_libfpdiv;		/*R=P(g)/Q(g)		*/
		I6=AX1;
		AX0=AR;				/*move R		*/
		MR1=SR1;
		MR0=SR0;
		MY0=DM(I6,M5);			/*pop previously pushed	*/ 
		MY1=DM(I6,M5);			/*value (f).  (depth 0)	*/
		AY0=DM(I6,M6);
		call ___lib_libfpmult;		/*f*R			*/
		AX0=AR;				/*move f*R		*/
		MR1=SR1;
		MR0=SR0;
		CALL ___lib_libfpadd;		/*f+f*R			*/

__check_n:	AY1=SB;				/*Load N		*/
		AF=AY1-1;
		IF LE JUMP __skip_neg;
		AX0=AR;				/*move R		*/
		MR1=SR1;
		MR0=SR0;
		CALL ___lib_3wd_neg;		/*If n>1 negate result	*/
__skip_neg:	AX0=0x8000;			/*load 0 in 3 wd format	*/
		MR=0;
		AF=PASS AY1;
		IF EQ JUMP __adjust_by_AN;
		AF=AF-1, AX0=MR0;		/*Load pi/6 in 3 wd 	*/
		MR1=0x4305;
		MR0=0x48ea;
		IF EQ JUMP __adjust_by_AN;
		AF=AF-1;
		AX0=1;				/*Load pi/2 in 3 wd 	*/
		MR1=0x6487;
		MR0=0xed34;
		IF EQ JUMP __adjust_by_AN;
		MR1=0x4305;			/*Load pi/3 in 3 wd (ax0=1)*/
		MR0=0x4915;

__adjust_by_AN:	AY0=AR;				/* Move answer		*/
		MY1=SR1;
		MY0=SR0;
		CALL ___lib_libfpadd;
		AY1=M3;
		AF=PASS AY1;			/*U msw=pos ?		*/
		IF GE JUMP __check_sign_v;
		
__check_sign_u:	AY0=AR;				/*Move answer		*/
		MY1=SR1;
		MY0=SR0;
		AX0=2;				/*Load pi in 3 wd format*/
		MR1=0x6487;
		MR0=0xed69;
		CALL ___lib_libfpsub;	
						
__check_sign_v:	AX1=I1;
		AF=PASS AX1;			/*Check for V<0		*/
		IF EQ JUMP __three_to_two;	/*I1 doesn't sign extend*/
		AX0=AR;
		MR1=SR1;
		MR0=SR0;
		CALL ___lib_3wd_neg;		/*Negate 3 wd number	*/
			
__three_to_two: AX0=AR;
		MR1=SR1;
		MR0=SR0;
		call ___lib_libfp_sf;		/*Transform back to IEEE*/

__restore_state: JUMP ___lib_restore_small_frame;

__return_half_PI: SR0=0x4049;			/*Load pi/2		*/
		SR1=0x0ecb;
		AX1=I1;
		AF=PASS AX1;			/*V msw pos ?		*/
		IF GE JUMP __restore_state;

__flip_sign:	AY1=0x8000;
                AR=SR1 XOR AY1;         	/*Flip sign of IEEE word*/
		SR1=AR;
		JUMP __restore_state;

.ENDMOD;

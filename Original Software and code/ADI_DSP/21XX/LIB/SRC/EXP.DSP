/*
    This C callable subroutine produces exp of its floating point input.

    DSP Applications Library.		    Alexander Soto

    Created on 10/15/93

    #include <math.h>				<Header>
    double exp (double v);			<Prototype>
*/

.MODULE/IMAGE	EXP_Module;

#include	"lib_glob.h";
#include	"ifp_glob.h";


.var exp_data[33];
.init exp_data:		0x0007,			/*BigX	(exp)		*/
			0x7f00,			/*BigX	(msw)		*/
			0x0000,			/*BigX	(lsw)		*/
			0x0007,			/*SmallX (exp)		*/
			0xa800,			/*SmallX (msw)		*/
			0x0000,			/*SmallX (lsw)		*/
			0x0001,			/*1/ln(2.0) (exp)	*/
			0x5c55,			/*1/ln(2.0) (msw)	*/
			0x1d94,			/*1/ln(2.0) (lsw)	*/
			0x0000,			/*C1 (exp)		*/
			0x58c0,			/*C1 (msw)		*/
			0x0000,			/*C1 (lsw)		*/
			0xfff4,			/*C2 (exp)		*/
			0x90bf,			/*C2 (msw)		*/
			0xbe8f,			/*C2 (lsw)		*/
			0xfff9,			/*P1 (exp)		*/
			0x617d,			/*P1 (msw)		*/
			0xe4ba,			/*P1 (lsw)		*/
			0xfffe,			/*P0 (exp)		*/
			0x7fff,			/*P0 (msw)		*/
			0xffff,			/*P0 (lsw)		*/
			0xfff5,			/*Q2 (exp)		*/
			0x4def,			/*Q2 (msw)		*/
			0x09ca,			/*Q2 (lsw)		*/
			0xfffc,			/*Q1 (exp)		*/
			0x6db4,			/*Q1 (msw)		*/
			0xce83,			/*Q1 (lsw)		*/
			0x0000,			/*Q0 (exp)		*/
			0x4000,			/*Q0 (msw)		*/
			0x0000;			/*Q0 (lsw)		*/
.global exp_data;


.ENTRY		exp_, expf_;

exp_:
expf_:		MR1=TOPPCSTACK;
                CALL ___lib_save_small_frame;

		AY1=0x2edb;			/*msw of eps (~ 1e-10)	*/
		AY0=0x7fff;			/*sign bit		*/
                reads(MR1,I6,M5);		/*Fetch MSW of input V  */
                reads(MR0,I6,M5), AR=MR1 AND AY0; /*Fetch LSW of input V, do abs*/
		AR=AR-AY1;			/*Check for < eps, > -eps*/
		IF LE JUMP __output_one;	/*e**0 = 1 (or e**eps)	*/
		
		CALL ___lib_sf_libfp;           /*Change V into 3 word  */
		I1=^exp_data;
		AX0=mem(I1,M1);			/*Get BigX		*/
		MR1=mem(I1,M1);
		MR0=mem(I1,M1);
		AY0=AR;				/*Move V		*/
		MY1=SR1;
		MY0=SR0;
		M5=-1;
		DM(I4,M5)=AR;			/*Push V (or X)		*/
		DM(I4,M5)=SR1;
		DM(I4,M5)=SR0;
		CALL ___lib_libfpcp;		/*Compare V to BigX	*/
		AF=PASS AR, AR=AY0;
		IF LE JUMP __output_too_large;	/*Jump if BigX <= V	*/
		AX0=mem(I1,M1);			/*Get SmallX		*/
		MR1=mem(I1,M1);
		MR0=mem(I1,M1);
		CALL ___lib_libfpcp;		/*Compare V to SmallX	*/
		AF=PASS AR, AR=AY0;
		IF GE JUMP __output_too_small;	/*Jump if SmallX >= V	*/
		AX0=mem(I1,M1);			/*Get 1/ln(2.0)		*/
		MR1=mem(I1,M1);
		MR0=mem(I1,M1);
		CALL ___lib_libfpmult;		/*N=V/ln(2.0)		*/
		AX0=AR;
		MR1=SR1;
		MR0=SR0;
		CALL ___libr_3wdtoi;		/*Convert 3wd to int=N	*/
		I6=I4;
		M5=1;
		MODIFY(I6,M5);
		MR0=DM(I6,M5);			/*Pop X off stack	*/
		MR1=DM(I6,M5);
		AX0=DM(I6,M6);
		I4=I6;
		M5=-1;
		DM(I4,M5)=AR;			/*Push N		*/
		DM(I4,M5)=AX0;			/*Push V (or X)	back	*/
		DM(I4,M5)=MR1;
		DM(I4,M5)=MR0;
		CALL ___lib_ito3wd;		/*Convert int to 3wd=XN	*/
		
__compute_g:	AX0=mem(I1,M1);			/*Get C1		*/
		MR1=mem(I1,M1);
		MR0=mem(I1,M1);
		AY0=AR;				/*Move XN		*/
		MY1=SR1;
		MY0=SR0;
		CALL ___lib_libfpmult;		/*(XN*C1)		*/
		I6=I4;
		M5=1;
		MODIFY(I6,M5);
		MR0=DM(I6,M5);			/*Pop X off stack	*/
		MR1=DM(I6,M5);
		AX0=DM(I6,M6);
		I4=I6;
		M5=-1;
		DM(I4,M5)=AY0;			/*Push XN		*/
		DM(I4,M5)=MY1;
		DM(I4,M5)=MY0;
		AY0=AR;				/*Move XN*C1		*/
		MY1=SR1;
		MY0=SR0;
		CALL ___lib_libfpsub;		/*X-(XN*C1)		*/
		I6=I4;
		M5=1;
		MODIFY(I6,M5);
		MY0=DM(I6,M5);			/*Pop XN off stack	*/
		MY1=DM(I6,M5);
		AY0=DM(I6,M6);
		I4=I6;
		M5=-1;
		DM(I4,M5)=AR;			/*Push X-(XN*C1)	*/
		DM(I4,M5)=SR1;
		DM(I4,M5)=SR0;
		AX0=mem(I1,M1);			/*Get C2		*/
		MR1=mem(I1,M1);
		MR0=mem(I1,M1);
		CALL ___lib_libfpmult;		/*(XN*C2)		*/
		I6=I4;
		M5=1;
		MODIFY(I6,M5);
		MR0=DM(I6,M5);			/*Pop X-(XN*C1)		*/
		MR1=DM(I6,M5);
		AX0=DM(I6,M6);
		I4=I6;
		AY0=AR;				/*Move XN*C2		*/
		MY1=SR1;
		MY0=SR0;
		CALL ___lib_libfpsub;		/*g=X-(XN*C1)-(XN*C2)	*/
		
__compute_R:	AY0=AR;				/*Move g		*/
		MY1=SR1;
		MY0=SR0;
		AX0=AR;
		MR1=SR1;
		MR0=SR0;
		CALL ___lib_libfpmult;		/*z=g*g			*/
		M5=-1;
		DM(I4,M5)=AR;			/*Push z		*/
		DM(I4,M5)=SR1;
		DM(I4,M5)=SR0;
		DM(I4,M5)=AY0;			/*Push g (level 2)	*/
		DM(I4,M5)=MY1;
		DM(I4,M5)=MY0;
		AY0=AR;				/*Move z		*/
		MY1=SR1;
		MY0=SR0;
		AX0=mem(I1,M1);			/*Get P1		*/
		MR1=mem(I1,M1);
		MR0=mem(I1,M1);
		CALL ___lib_libfpmult;		/*p1*z			*/
		AY0=AR;				/*Move p1*z		*/
		MY1=SR1;
		MY0=SR0;
		AX0=mem(I1,M1);			/*Get P0		*/
		MR1=mem(I1,M1);
		MR0=mem(I1,M1);
		CALL ___lib_libfpadd;		/*p1*z+p0		*/
		AX0=AR;				/*Move p1*z+p0		*/
		MR1=SR1;
		MR0=SR0;
		I6=I4;
		M5=1;
		MODIFY(I6,M5);
		MY0=DM(I6,M5);			/*Pop g	(level 1)	*/
		MY1=DM(I6,M5);
		AY0=DM(I6,M5);
		CALL ___lib_libfpmult;		/*g*P(z)=(p1*z+p0)*g	*/
		MY0=DM(I6,M5);			/*Pop z	(level 0)	*/
		MY1=DM(I6,M5);
		AY0=DM(I6,M6);
		I4=I6;
		M5=-1;
		DM(I4,M5)=AR;			/*Push g*P(z) (level 1)	*/
		DM(I4,M5)=SR1;
		DM(I4,M5)=SR0;
		AX0=mem(I1,M1);			/*Get q2		*/
		MR1=mem(I1,M1);
		MR0=mem(I1,M1);
		CALL ___lib_libfpmult;		/*q2*z			*/
		DM(I4,M5)=AY0;			/*Push z (level 2)	*/
		DM(I4,M5)=MY1;
		DM(I4,M5)=MY0;
		AY0=AR;				/*Move q2*z		*/
		MY1=SR1;
		MY0=SR0;
		AX0=mem(I1,M1);			/*Get q1		*/
		MR1=mem(I1,M1);
		MR0=mem(I1,M1);
		CALL ___lib_libfpadd;		/*q2*z+q1		*/
		I6=I4;
		M5=1;
		MODIFY(I6,M5);
		MR0=DM(I6,M5);			/*Pop z	(level 1)	*/
		MR1=DM(I6,M5);
		AX0=DM(I6,M5);
		AY0=AR;				/*Move q2*z+q1		*/
		MY1=SR1;
		MY0=SR0;
		CALL ___lib_libfpmult;		/*(q2*z+q1)*z		*/
		AX0=mem(I1,M1);			/*Get q0		*/
		MR1=mem(I1,M1);
		MR0=mem(I1,M1);
		AY0=AR;				/*Move (q2*z+q1)*z	*/
		MY1=SR1;
		MY0=SR0;
		CALL ___lib_libfpadd;		/*Q(z)=(q2*z+q1)*z+q0	*/
		AX0=AR;
		MR1=SR1;
		MR0=SR0;
		MY0=DM(I6,M5);			/*Pop g*P(z) (level 0)	*/
		MY1=DM(I6,M5);
		AY0=DM(I6,M6);
		I4=I6;
		CALL ___lib_libfpsub;		/*D=Q(z)-g*P(z)		*/
		M5=-1;
		DM(I4,M5)=AY0;			/*Push g*P(z) (level 1)	*/
		DM(I4,M5)=MY1;
		DM(I4,M5)=MY0;

		AY0=AR;				/*Move D		*/
		MY1=SR1;
		MY0=SR0;
		AX0=0x0001;			/*Load 1 in 3 wd	*/
		MR1=0x4000;
		MR0=0x0000;
		CALL ___lib_libfpdiv;		/*R0=1/D		*/
		AX0=AY0;			/*Move D to X		*/
                MR1=MY1;
                MR0=MY0;
		AY0=AR;				/*Move R0 to Y		*/
		MY1=SR1;
		MY0=SR0;
                CALL ___lib_libfpmult;          /*D'=D*R0               */
                I6=I4;
                M5=1;
                MODIFY(I6,M5);
                MR0=DM(I6,M5);                  /*Pop g*P(z) (level 0)	*/
                MR1=DM(I6,M5);
                AX0=DM(I6,M6);
                I4=I6;
                M5=-1;
                DM(I4,M5)=AR;                   /*push D' (level 1)     */
                DM(I4,M5)=SR1;
                DM(I4,M5)=SR0;
                CALL ___lib_libfpmult;          /*N*R0                  */
                I6=I4;
                M5=1;
                MODIFY(I6,M5);
                MY0=DM(I6,M5);                  /*Pop D' (level 0)      */
                MY1=DM(I6,M5);
                AY0=DM(I6,M6);
                I4=I6;
                M5=-1;
                DM(I4,M5)=AR;                   /*push N*R0 (level 1)   */
                DM(I4,M5)=SR1;
                DM(I4,M5)=SR0;
                AX0=0x0002;                     /*Load 2 in 3 wd format */
                MR1=0x4000;
                MR0=0x0000;
                CALL ___lib_libfpsub;           /*R1=2-D'               */
                AX0=AY0;                        /*Move D'               */
                MR1=MY1;
                MR0=MY0;
                AY0=AR;                         /*Move R1               */
                MY1=SR1;
                MY0=SR0;
                CALL ___lib_libfpmult;          /*D'=D'*R1              */
                I6=I4;
		M5=1;
                MODIFY(I6,M5);
                MR0=DM(I6,M5);                  /*Pop N*R0(level 0)     */
                MR1=DM(I6,M5);
                AX0=DM(I6,M6);
                I4=I6;
                M5=-1;
                DM(I4,M5)=AR;                   /*push D'=D'*R1 (level 1)*/
                DM(I4,M5)=SR1;
                DM(I4,M5)=SR0;
                CALL ___lib_libfpmult;          /*N*R1*R0               */
                I6=I4;
                M5=1;
                MODIFY(I6,M5);
                MY0=DM(I6,M5);                  /*Pop D' (level 0)      */
                MY1=DM(I6,M5);
                AY0=DM(I6,M6);
                I4=I6;
                M5=-1;
                DM(I4,M5)=AR;                   /*push N*R1*R0 (level 1)*/
                DM(I4,M5)=SR1;
                DM(I4,M5)=SR0;
                AX0=0x0002;                     /*Load 2 in 3 wd format */
                MR1=0x4000;
                MR0=0x0000;
                CALL ___lib_libfpsub;           /*R2=2-D'               */
                AX0=AY0;                        /*Move D'               */
                MR1=MY1;
                MR0=MY0;
                AY0=AR;                         /*Move R2=2-D'          */
                MY1=SR1;
                MY0=SR0;
                CALL ___lib_libfpmult;          /*D'=D'*R2              */
                I6=I4;
                M5=1;
                MODIFY(I6,M5);
                MR0=DM(I6,M5);                  /*Pop N*R1*R0 (level 0) */
                MR1=DM(I6,M5);
                AX0=DM(I6,M6);
                I4=I6;
                M5=-1;
                DM(I4,M5)=AR;                   /*push D'=D'*R2 (level 1)*/
                DM(I4,M5)=SR1;
                DM(I4,M5)=SR0;
                CALL ___lib_libfpmult;          /*N*R0*R1*R2            */
                I6=I4;
                M5=1;
                MODIFY(I6,M5);
                MY0=DM(I6,M5);                  /*Pop D'=D'*R2 (level 0)*/
		MY1=DM(I6,M5);
                AY0=DM(I6,M6);
                I4=I6;
                M5=-1;
                DM(I4,M5)=AR;                   /*push N*R0*R1*R2 (level 1)*/
                DM(I4,M5)=SR1;
                DM(I4,M5)=SR0;
                AX0=0x0002;                     /*Load 2 in 3 wd format */
                MR1=0x4000;
                MR0=0x0000;
                CALL ___lib_libfpsub;           /*R3=2-D'               */
                AY0=AR;                         /*Move R3               */
                MY1=SR1;
                MY0=SR0;
                I6=I4;
                M5=1;
                MODIFY(I6,M5);
                MR0=DM(I6,M5);                  /*pop N*R0*R1*R2 (level 0)*/
                MR1=DM(I6,M5);
                AX0=DM(I6,M6);
                I4=I6;
                CALL ___lib_libfpmult;          /*N*R0*R1*R2*R3		*/
		AY0=AR;
                MY1=SR1;
                MY0=SR0;
		AX0=0x0000;                     /*Load .5 in 3 wd format*/
                MR1=0x4000;
                MR0=0x0000;
		CALL ___lib_libfpadd;		/*R(g)=.5+(g*P(z)/(Q(z)-g*P(z)))*/
		AY0=AR;				/*Put answer in X regs	*/
                MY1=SR1;
                MY0=SR0;
		I6=I4;
                M5=1;
                MODIFY(I6,M5);
                AR=DM(I6,M5);			/*pop N			*/
		I4=I6;
		AF=AY0+1;			/*R(g)*s^(N+1) N=N+1	*/
		AR=AR+AF;			/*scalb instruction (exp+N+1)*/

__three_to_two: AX0=AR;				/*Answer in Y regs	*/
		MR1=MY1;
		MR0=MY0;
                CALL ___lib_libfp_sf;           /*Transform back to IEEE*/

__restore_state: JUMP ___lib_restore_small_frame;

__output_too_large:
__output_too_small: SR=LSHIFT AR BY -100 (LO);	/*Load SR with a zero   */
		JUMP __restore_state;

__output_one:	SR1=0x3f80;			/*Return one		*/
		SR0=0;
		JUMP __restore_state;

.ENDMOD;

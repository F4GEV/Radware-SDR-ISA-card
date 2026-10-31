/*
    This C callable subroutine produces log of its floating point input.

    DSP Applications Library.		    Alexander Soto

    Created on 10/14/93

    #include <math.h>				<Header>
    double log (double v);			<Prototype>
*/

.MODULE/IMAGE	LOG_Module;

#include	"lib_glob.h";
#include	"mth_glob.h";
#include	"ifp_glob.h";

#define F_EXP_BIAS 127

.var log_data[33];
.init log_data:		0x0000,			/*C0 (exp) 		*/
			0x5a82,			/*C0 (msw)		*/
			0x7999,			/*C0 (lsw)		*/
			0x0003,			/*B0 (exp) 		*/
			0xa6bc,			/*B0 (msw) 		*/
			0xeee1,			/*B0 (lsw) 		*/
			0xfffa,			/*A1 (exp)		*/
			0x6f6b,			/*A1 (msw)		*/
			0x44f2,			/*A1 (lsw)		*/
			0xffff,			/*A0 (exp)		*/
			0x88fb,			/*A0 (msw)		*/
			0xe7c1,			/*A0 (lsw)		*/
			0xfff4,			/*C2 (exp)		*/
			0x90bf,			/*C2 (msw)		*/
			0xbe8f,			/*C2 (lsw)		*/
			0x0000,			/*C1 (exp)		*/
			0x58c0,			/*C1 (msw)		*/
			0x0000;			/*C1 (lsw)		*/
.global log_data;


.ENTRY		log_, logf_, log10_, log10f_;

log10_:
log10f_:	AR=0xffff;
		SR1=0x6f2d;
		SR0=0xec54;
		JUMP __log_core;

log_:
logf_:		AR=PASS 0;
		SR=LSHIFT AR BY -100 (LO);	/*Load SR with a zero   */
		
__log_core:	MR1=TOPPCSTACK;
                CALL ___lib_save_small_frame;

                reads(MR1,I6,M5);		/*Fetch MSW of input V  */
                reads(MR0,I6,M5), AF=PASS MR1;	/*Fetch LSW of input V  */
		IF LE JUMP __input_error;

		M5=-1;
		DM(I4,M5)=AR;			/*Push value to mult	*/
		DM(I4,M5)=SR1;			/*for log vs log10	*/
		DM(I4,M5)=SR0;

		AY1=0x7f80;			/*Get exponent		*/
                AR=MR1 AND AY1;                 /*Mask out all but exp  */
                SR=LSHIFT AR BY -7 (LO);        /*Exp now in LSBs       */
                AR=F_EXP_BIAS;			/*Load exp bias (not -1)*/
                AF=PASS AR;
                AR=SR0-AF;                      /*Compute exp for save  */
		I1=^log_data;
		AF=PASS AR;
		AR=AF+1;			/*Increment exponent	*/
		AR=-AR, AX1=AR;			/*Negate exp, but save old*/
		AY0=AR;				/*Negated exp		*/
		CALL ___lib_sf_libfp;		/*Change V into 3 word	*/
		AR=AR+AY0, AX0=mem(I1,M1);	/*Add to exp (scalb)	*/
		AY0=AR;
		MY1=SR1;
		MY0=SR0;
                MR1=mem(I1,M1);
                MR0=mem(I1,M1);
		CALL ___lib_libfpcp;            /*Compare C0 to f	*/
		M3=AR;
		AX0=AY0;			/*Move f to the X reg	*/
		MR1=MY1;
		MR0=MY0;
		AY0=0x0000;    	             	/*Load .5 constant	*/
                MY1=0x4000;
                MY0=0x0000;
		DM(I4,M5)=AX0;			/*Push f (level 1)	*/
		DM(I4,M5)=MR1;
		DM(I4,M5)=MR0;
		CALL ___lib_libfpsub;		/*znum=f-.5		*/
		AX0=AR;
		MR1=SR1;
		MR0=SR0;
		DM(I4,M5)=AX0;			/*Push f-.5 (level 2)	*/
		DM(I4,M5)=MR1;
		DM(I4,M5)=MR0;
		CALL ___lib_libfpmult;		/*znum*.5 [(f-.5)*.5]	*/
		AY1=AR;
		AR=M3;
		AF=PASS AR, AR=AY1;
		IF LE JUMP __adjust_z;		/*If C0 <= f		*/
		AX0=AR;
		MR1=SR1;
		MR0=SR0, AF=PASS AX1;		/*Load N		*/
		AR=AF-1;			/*N=N-1			*/
		AX1=AR;
		CALL ___lib_libfpadd;		/*zden=znum*.5+.5	*/
		I6=I4;
		M5=1;
		MODIFY(I6,M5);			/*Pop f-.5 and f, swap	*/
		MR0=DM(I6,M5);			/*Pop f-.5		*/
		MR1=DM(I6,M5);
		AX0=DM(I6,M5);
		M5=2;
		MODIFY(I6,M5);			/*pop f			*/
		I4=I6;
		M5=-1;
		DM(I4,M5)=AX0;			/*push f-.5 (level 1)	*/
		DM(I4,M5)=MR1;
		DM(I4,M5)=MR0;
		JUMP __compute_r;

__adjust_z:	I6=I4;
		M5=1;
		MODIFY(I6,M5);
		MR0=DM(I6,M5);			/*Pop f-.5 (level 1)	*/
		MR1=DM(I6,M5);
		AX0=DM(I6,M5);
		I4=I6;
		CALL ___lib_libfpsub;		/*znum=f-.5-.5		*/
		MR0=DM(I6,M5);			/*Pop f	(level 0)	*/
		MR1=DM(I6,M5);
		AX0=DM(I6,M6);
		I4=I6;
		M5=-1;
		DM(I4,M5)=AR;			/*push f-.5-.5	(level 1)*/
		DM(I4,M5)=SR1;
		DM(I4,M5)=SR0;
		CALL ___lib_libfpmult;		/*f*.5			*/
		AX0=AR;
		MR1=SR1;
		MR0=SR0;
		CALL ___lib_libfpadd;		/*zden=f*.5+.5		*/

__compute_r:	AY0=AR;				/*Move D to Y		*/
		MY1=SR1;
		MY0=SR0;
		AX0=0x0001;			/*Load 1 (3 wd)		*/
		MR1=0x4000;
		MR0=0x0000;
		CALL ___lib_libfpdiv;		/*R0=1/D (recips)	*/
		AX0=AY0;			/*Move D to X		*/
		MR1=MY1;
		MR0=MY0;
		AY0=AR;				/*Move R0 to Y		*/
		MY1=SR1;
		MY0=SR0;
		CALL ___lib_libfpmult;		/*D'=D*R0		*/
		I6=I4;
		M5=1;
		MODIFY(I6,M5);
		MR0=DM(I6,M5);			/*Pop N	(level 0)	*/
		MR1=DM(I6,M5);
		AX0=DM(I6,M6);
		I4=I6;
		M5=-1;
		DM(I4,M5)=AR;			/*push D' (level 1)	*/
		DM(I4,M5)=SR1;
		DM(I4,M5)=SR0;
		CALL ___lib_libfpmult;		/*N*R0			*/
		I6=I4;
		M5=1;
		MODIFY(I6,M5);
		MY0=DM(I6,M5);			/*Pop D' (level 0)	*/
		MY1=DM(I6,M5);
		AY0=DM(I6,M6);
		I4=I6;
		M5=-1;
		DM(I4,M5)=AR;			/*push N*R0 (level 1)	*/
		DM(I4,M5)=SR1;
		DM(I4,M5)=SR0;
		AX0=0x0002;			/*Load 2 in 3 wd format	*/
		MR1=0x4000;
		MR0=0x0000;
		CALL ___lib_libfpsub;		/*R1=2-D'		*/
		AX0=AY0;			/*Move D'		*/
		MR1=MY1;
		MR0=MY0;
		AY0=AR;				/*Move R1		*/
		MY1=SR1;
		MY0=SR0;
		CALL ___lib_libfpmult;		/*D'=D'*R1		*/
		I6=I4;
		M5=1;
		MODIFY(I6,M5);
		MR0=DM(I6,M5);			/*Pop N*R0(level 0)	*/
		MR1=DM(I6,M5);
		AX0=DM(I6,M6);
		I4=I6;
		M5=-1;
		DM(I4,M5)=AR;			/*push D'=D'*R1 (level 1)*/
		DM(I4,M5)=SR1;
		DM(I4,M5)=SR0;			
		CALL ___lib_libfpmult;		/*N*R1*R0		*/
		I6=I4;
		M5=1;
		MODIFY(I6,M5);
		MY0=DM(I6,M5);			/*Pop D' (level 0)	*/
		MY1=DM(I6,M5);
		AY0=DM(I6,M6);
		I4=I6;
		M5=-1;
		DM(I4,M5)=AR;			/*push N*R1*R0 (level 1)*/
		DM(I4,M5)=SR1;
		DM(I4,M5)=SR0;
		AX0=0x0002;			/*Load 2 in 3 wd format	*/
		MR1=0x4000;
		MR0=0x0000;
		CALL ___lib_libfpsub;		/*R2=2-D'		*/
		AX0=AY0;			/*Move D'		*/
		MR1=MY1;
		MR0=MY0;
		AY0=AR;				/*Move R2=2-D'		*/
		MY1=SR1;
		MY0=SR0;
		CALL ___lib_libfpmult;		/*D'=D'*R2		*/
		I6=I4;
		M5=1;
		MODIFY(I6,M5);
		MR0=DM(I6,M5);			/*Pop N*R1*R0 (level 0)	*/
		MR1=DM(I6,M5);
		AX0=DM(I6,M6);
		I4=I6;
		M5=-1;
		DM(I4,M5)=AR;			/*push D'=D'*R2 (level 1)*/
		DM(I4,M5)=SR1;
		DM(I4,M5)=SR0;
		CALL ___lib_libfpmult;		/*N*R0*R1*R2		*/
		I6=I4;
		M5=1;
		MODIFY(I6,M5);
		MY0=DM(I6,M5);			/*Pop D'=D'*R2 (level 0)*/
		MY1=DM(I6,M5);
		AY0=DM(I6,M6);
		I4=I6;
		M5=-1;
		DM(I4,M5)=AR;			/*push N*R0*R1*R2 (level 1)*/
		DM(I4,M5)=SR1;
		DM(I4,M5)=SR0;
		AX0=0x0002;			/*Load 2 in 3 wd format	*/
		MR1=0x4000;
		MR0=0x0000;
		CALL ___lib_libfpsub;		/*R3=2-D'		*/
		AY0=AR;				/*Move R3		*/
		MY1=SR1;
		MY0=SR0;
		I6=I4;
		M5=1;
		MODIFY(I6,M5);
		MR0=DM(I6,M5);			/*pop N*R0*R1*R2	*/
		MR1=DM(I6,M5);
		AX0=DM(I6,M6);
		I4=I6;
		CALL ___lib_libfpmult;		/*z=N*R0*R1*R2*R3	*/
		AY0=AR;				/*Move z		*/
		MY1=SR1;
		MY0=SR0;
		M5=-1;
		DM(I4,M5)=AR;			/*push z (level 1)	*/
		DM(I4,M5)=SR1;
		DM(I4,M5)=SR0;
		AX0=AY0;
		MR1=MY1;
		MR0=MY0;
		CALL ___lib_libfpmult;		/*w=z^2			*/
		DM(I4,M5)=AR;			/*push w (level 2)	*/
		DM(I4,M5)=SR1;
		DM(I4,M5)=SR0;
		AX0=mem(I1,M1);                 /*Get B0		*/
                MR1=mem(I1,M1);
                MR0=mem(I1,M1);
		AY0=AR;				/*Move w		*/
		MY1=SR1;
		MY0=SR0;
		CALL ___lib_libfpadd;		/*B(w)=w+b0		*/
		DM(I4,M5)=AR;			/*push B(w) (level 3)	*/
		DM(I4,M5)=SR1;
		DM(I4,M5)=SR0;
		AX0=mem(I1,M1);                 /*Get a1		*/
                MR1=mem(I1,M1);
                MR0=mem(I1,M1);
		CALL ___lib_libfpmult;		/*w*a1			*/
		AY0=AR;				/*Move w*a1		*/
		MY1=SR1;
		MY0=SR0;
		AX0=mem(I1,M1);                 /*Get a0		*/
                MR1=mem(I1,M1);
                MR0=mem(I1,M1);
		CALL ___lib_libfpadd;		/*A(w)=w*a1+a0		*/
		I6=I4;
		M5=1;
		MODIFY(I6,M5);
		MY0=DM(I6,M5);			/*pop B(w) (level 2) 	*/
		MY1=DM(I6,M5);
		AY0=DM(I6,M6);
		I4=I6;
		M5=-1;
		DM(I4,M5)=AR;			/*push A(w) (level 3)	*/
		DM(I4,M5)=SR1;
		DM(I4,M5)=SR0;
		AX0=0x0001;			/*Load 1 (3 wd)		*/
		MR1=0x4000;
		MR0=0x0000;
		CALL ___lib_libfpdiv;		/*R0=1/B(w) [recips]	*/
		AX0=AY0;			/*Move D [B(w)] to X	*/
		MR1=MY1;
		MR0=MY0;
		AY0=AR;				/*Move R0 to Y		*/
		MY1=SR1;
		MY0=SR0;
		CALL ___lib_libfpmult;		/*D'=D*R0		*/
		I6=I4;
		M5=1;
		MODIFY(I6,M5);
		MR0=DM(I6,M5);			/*Pop A(w) or N	(level 2)*/
		MR1=DM(I6,M5);
		AX0=DM(I6,M6);
		I4=I6;
		M5=-1;
		DM(I4,M5)=AR;			/*push D' (level 3)	*/
		DM(I4,M5)=SR1;
		DM(I4,M5)=SR0;
		CALL ___lib_libfpmult;		/*N*R0			*/
		I6=I4;
		M5=1;
		MODIFY(I6,M5);
		MY0=DM(I6,M5);			/*Pop D' (level 2)	*/
		MY1=DM(I6,M5);
		AY0=DM(I6,M6);
		I4=I6;
		M5=-1;
		DM(I4,M5)=AR;			/*push N*R0 (level 3)	*/
		DM(I4,M5)=SR1;
		DM(I4,M5)=SR0;
		AX0=0x0002;			/*Load 2 in 3 wd format	*/
		MR1=0x4000;
		MR0=0x0000;
		CALL ___lib_libfpsub;		/*R1=2-D'		*/
		AX0=AY0;			/*Move D'		*/
		MR1=MY1;
		MR0=MY0;
		AY0=AR;				/*Move R1		*/
		MY1=SR1;
		MY0=SR0;
		CALL ___lib_libfpmult;		/*D'=D'*R1		*/
		I6=I4;
		M5=1;
		MODIFY(I6,M5);
		MR0=DM(I6,M5);			/*Pop N*R0(level 2)	*/
		MR1=DM(I6,M5);
		AX0=DM(I6,M6);
		I4=I6;
		M5=-1;
		DM(I4,M5)=AR;			/*push D'=D'*R1 (level 3)*/
		DM(I4,M5)=SR1;
		DM(I4,M5)=SR0;			
		CALL ___lib_libfpmult;		/*N*R1*R0		*/
		I6=I4;
		M5=1;
		MODIFY(I6,M5);
		MY0=DM(I6,M5);			/*Pop D' (level 2)	*/
		MY1=DM(I6,M5);
		AY0=DM(I6,M6);
		I4=I6;
		M5=-1;
		DM(I4,M5)=AR;			/*push N*R1*R0 (level 3)*/
		DM(I4,M5)=SR1;
		DM(I4,M5)=SR0;
		AX0=0x0002;			/*Load 2 in 3 wd format	*/
		MR1=0x4000;
		MR0=0x0000;
		CALL ___lib_libfpsub;		/*R2=2-D'		*/
		AX0=AY0;			/*Move D'		*/
		MR1=MY1;
		MR0=MY0;
		AY0=AR;				/*Move R2=2-D'		*/
		MY1=SR1;
		MY0=SR0;
		CALL ___lib_libfpmult;		/*D'=D'*R2		*/
		I6=I4;
		M5=1;
		MODIFY(I6,M5);
		MR0=DM(I6,M5);			/*Pop N*R1*R0 (level 2)	*/
		MR1=DM(I6,M5);
		AX0=DM(I6,M6);
		I4=I6;
		M5=-1;
		DM(I4,M5)=AR;			/*push D'=D'*R2 (level 3)*/
		DM(I4,M5)=SR1;
		DM(I4,M5)=SR0;
		CALL ___lib_libfpmult;		/*N*R0*R1*R2		*/
		I6=I4;
		M5=1;
		MODIFY(I6,M5);
		MY0=DM(I6,M5);			/*Pop D'=D'*R2 (level 2)*/
		MY1=DM(I6,M5);
		AY0=DM(I6,M6);
		I4=I6;
		M5=-1;
		DM(I4,M5)=AR;			/*push N*R0*R1*R2 (level 3)*/
		DM(I4,M5)=SR1;
		DM(I4,M5)=SR0;
		AX0=0x0002;			/*Load 2 in 3 wd format	*/
		MR1=0x4000;
		MR0=0x0000;
		CALL ___lib_libfpsub;		/*R3=2-D'		*/
		AY0=AR;				/*Move R3		*/
		MY1=SR1;
		MY0=SR0;
		I6=I4;
		M5=1;
		MODIFY(I6,M5);
		MR0=DM(I6,M5);			/*pop N*R0*R1*R2 (level 2)*/
		MR1=DM(I6,M5);
		AX0=DM(I6,M6);
		I4=I6;
		CALL ___lib_libfpmult;		/*z=N*R0*R1*R2*R3	*/
		I6=I4;
		M5=1;
		MODIFY(I6,M5);
		MR0=DM(I6,M5);			/*pop w (level 1)	*/
		MR1=DM(I6,M5);
		AX0=DM(I6,M6);
		I4=I6;
		AY0=AR;				/*Move r		*/
		MY1=SR1;
		MY0=SR0;
		CALL ___lib_libfpmult;		/*r(z^2)		*/

__compute_R: 	AX0=AR;				/*Move r(z^2)		*/
		MR1=SR1;
		MR0=SR0;
		I6=I4;
		M5=1;
		MODIFY(I6,M5);
		MY0=DM(I6,M5);			/*pop z (level 0)	*/
		MY1=DM(I6,M5);
		AY0=DM(I6,M6);
		I4=I6;
		CALL ___lib_libfpmult;		/*z*r(z^2)		*/
		AX0=AR;				/*Move z*r(z^2)		*/
		MR1=SR1;
		MR0=SR0;
		CALL ___lib_libfpadd;		/*R(z)=z+z*r(z^2)	*/
		M5=-1;
		DM(I4,M5)=AR;			/*push R(z) (level 1)	*/
		DM(I4,M5)=SR1;
		DM(I4,M5)=SR0, AR=PASS AX1;	/*Load N		*/
		CALL ___lib_ito3wd;		/*Change N to XN	*/
		AX0=mem(I1,M1);                 /*Get C2		*/
                MR1=mem(I1,M1);
                MR0=mem(I1,M1);
		AY0=AR;				/*Move XN		*/
		MY1=SR1;
		MY0=SR0;
		CALL ___lib_libfpmult;		/*XN*C2			*/
		I6=I4;
		M5=1;
		MODIFY(I6,M5);
		MR0=DM(I6,M5);			/*pop R(z) (level 0)	*/
		MR1=DM(I6,M5);
		AX0=DM(I6,M6);
		I4=I6;
		M5=-1;
		DM(I4,M5)=AY0;			/*push XN (level 1)	*/
		DM(I4,M5)=MY1;
		DM(I4,M5)=MY0;
		AY0=AR;				/*Move XN*C2		*/
		MY1=SR1;
		MY0=SR0;
		CALL ___lib_libfpadd;		/*XN*C2+R(z)		*/
		I6=I4;
		M5=1;
		MODIFY(I6,M5);
		MY0=DM(I6,M5);			/*pop XN (level 0)	*/
		MY1=DM(I6,M5);
		AY0=DM(I6,M6);
		I4=I6;
		M5=-1;
		DM(I4,M5)=AR;			/*push XN*C2+R(z) (level 1)*/
		DM(I4,M5)=SR1;
		DM(I4,M5)=SR0;
		AX0=mem(I1,M1);                 /*Get C1		*/
                MR1=mem(I1,M1);
                MR0=mem(I1,M1);
		CALL ___lib_libfpmult;		/*XN*C1			*/
		AY0=AR;				/*Move XN*C2		*/
		MY1=SR1;
		MY0=SR0;
		I6=I4;
		M5=1;
		MODIFY(I6,M5);
		MR0=DM(I6,M5);			/*pop XN*C2+R(z) (level 0)*/
		MR1=DM(I6,M5);
		AX0=DM(I6,M6);
		I4=I6;
		CALL ___lib_libfpadd;		/*ln(x)			*/
		I6=I4;
		M5=1;
		MODIFY(I6,M5);
		MR0=DM(I6,M5);			/*pop mult. factor	*/
		MR1=DM(I6,M5);
		AX0=DM(I6,M6), AF=PASS MR1;	/*log or log10 ?	*/
		I4=I6;
		IF EQ JUMP __three_to_two;
		AY0=AR;				/*Move ln(x)		*/
		MY1=SR1;
		MY0=SR0;
		CALL ___lib_libfpmult;		/*Compute Log base N	*/
		
__three_to_two: AX0=AR;
                MR1=SR1;
                MR0=SR0;
                call ___lib_libfp_sf;           /*Transform back to IEEE*/

__restore_state: JUMP ___lib_restore_small_frame;

__input_error:	SR=LSHIFT AR BY -100 (LO);	/*Load SR with a zero   */
                SI=EDOM;			/*Set errno=EDOM        */
                CALL ___lib_set_errno;		/*Set global errno      */
                JUMP __restore_state;

.ENDMOD;

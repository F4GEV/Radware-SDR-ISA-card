/*
    This C callable subroutine produces sin/cos of its floating point input.

    DSP Applications Library.		    Alexander Soto

    Created on 9/27/93

    #include <math.h>				<Header>
    double sin(double v);			<Prototype>
    double cos(double v);			<Prototype>
*/

.MODULE/IMAGE	__SINCOS_Module;

#include	"lib_glob.h";
#include	"mth_glob.h";
#include	"ifp_glob.h";



.var sine_data[33];
.init sine_data:	0xffff,			/* 1/PI	(exp)		*/
			0x517c,			/* 1/PI	(msw)		*/
			0xc1b7,			/* 1/PI	(lsw)		*/
			0x0002,			/* C1 fixed pt (exp)	*/
			0x6488,			/* C1	(msw)		*/
			0x0000,			/* C1	(lsw)		*/
			0xfff0,			/* C2 fixed pt (exp)	*/
			0xb544,			/* C2	(msw)		*/
			0x42d2,			/* C2	(lsw)		*/
			0xffec,			/* eps fixed pt (exp)	*/
			0x7fff,			/* eps	(msw)		*/
			0xffff,			/* eps	(lsw)		*/
			0xffd8,			/* R7 fixed pt (exp)	*/
			0x9844,			/* R7 (msw)		*/
			0x6382,			/* R7 (lsw)		*/
			0xffe0,			/* R6 fixed pt (exp)	*/
			0x5839,			/* R6 (msw)		*/
			0x555e,			/* R6 (lsw)		*/
			0xffe7,			/* R5 fixed pt (exp)	*/
			0x9467,			/* R5 (msw)		*/
			0x2d3a,			/* R5(lsw)		*/
			0xffee,			/* R4 fixed pt (exp)	*/
			0x5c77,			/* R4 (msw)		*/
			0x8df7,			/* R4 (lsw)		*/
			0xfff4,			/* R3 fixed pt (exp)	*/
			0x97f9,			/* R3 (msw)		*/
			0x7f9a,			/* R3 (lsw)		*/
			0xfffa,			/* R2 fixed pt (exp)	*/
 			0x4444,			/* R2 (msw)		*/
			0x4444,			/* R2 (lsw)		*/
			0xfffe,			/* R1 fixed pt (exp)	*/
			0xaaaa,			/* R1 (msw)		*/
			0xaaab;			/* R1 (lsw)		*/
.global sine_data;


.ENTRY		sin_, cos_;

cos_:		AY1=1;
		JUMP __sincos_core;

sin_:		AY1=0;

__sincos_core:  MR1=TOPPCSTACK;
                CALL ___lib_save_small_frame;

		reads(MR1,I6,M5), AR=PASS 1;	/*Fetch MSW of input V  */
		reads(MR0,I6,M5),AF=PASS MR1;	/*Fetch LSW of input V  */
		M5=-1;
		IF LT AR=PASS -1;		/*Set sign to negative	*/
		AX1=AR;
		AY0=AY1;			/*Move flag to safe reg	*/
		CALL ___lib_sf_libfp;		/*Change V into 3 word	*/
		CALL ___lib_3wd_abs;		/*Get Y=ABS(V)		*/
		DM(I4,M5)=AR;			/*Push Y		*/
		DM(I4,M5)=SR1;
		DM(I4,M5)=SR0;	
		AF=PASS AY0;			/*sin or cos path?	*/
		IF NE JUMP __cos_path;
		DM(I4,M5)=0x8000;		/*Push a 0(3 wd) to add	*/
		DM(I4,M5)=0;			/*later (no cos adj)	*/
		DM(I4,M5)=0;			/*(Ay0=0 for sin)	*/
		JUMP __check_ymax;

__cos_path:	AX1=1;				/*Reset sign flag for cos*/
		AX0=AR;				/*Move V into other regs*/
		MR1=SR1;
		MR0=SR0;
		AY0=1;				/*PI/2 in 3 wd format	*/
		MY1=0x6487;
		MY0=0xed00;
		CALL ___lib_libfpadd;		/*|V| + PI/2 = Y	*/
		DM(I4,M5)=0;			/*Push a .5 (cos adj)	*/
		DM(I4,M5)=0x4000;
		DM(I4,M5)=0;
		
__check_ymax:	I6=^sine_data;			/*Set ptr to data	*/
		M5=1;
		AY0=AR;				/*Y			*/
		MY1=SR1;
		MY0=SR0;
		AX0=0x12;			/*YMAX			*/
		MR1=0x6487;
		MR0=0xe000;
		CALL ___lib_libfpcp;		/*YMAX > Y ?		*/
		AF=PASS AR, AR=AY0;
		IF LE JUMP __input_error;	/*If Y > YMAX, error	*/
		AX0=mem(I6,M5);			/*Begin loading sin data*/
		MR1=mem(I6,M5);			/*This is 1/PI in 3 wd	*/
		MR0=mem(i6,M5);
		CALL ___lib_libfpmult;		/*Y * 1/PI		*/
		AX0=AR;
		MR1=SR1;
		MR0=SR0;
		CALL ___libr_3wdtoi;		/*Convert 3wd to int	*/
		SI=AR;				/*INTRND(Y/PI)=N	*/
		SR=LSHIFT SI BY -1 (HI);	/*SI holds N		*/
		AF=PASS SR0, AR=AX1;		/*Downshift LSB of N into MSB of SR0*/
		IF LT AR=-AR;			/* If N is odd, neg sign*/
		AX1=AR;				/*Need to set SGN=-SGN*/
		AR=SI;
		CALL ___lib_ito3wd;		/*Convert int to 3wd=XN	*/
		I1=I4;
		MODIFY(I1,M1);
		MR0=DM(I1,M1);			/*Pop cos adj (0 or .5)	*/
		MR1=DM(I1,M1);
		AX0=DM(I1,M2);
		I4=I1;
		AY0=AR;				/*Move XN		*/
		MY1=SR1;
		MY0=SR0;
		CALL ___lib_libfpsub;		/*XN (adjusted) (0/.5 - XN)*/
		CALL ___lib_3wd_neg;		/*Negate		*/
		M5=1;
		AY0=AR;				/*Move XN (adj)		*/
		MY1=SR1;
		MY0=SR0;
		AX0=mem(I6,M5);			/*Begin loading C1	*/
		MR1=mem(I6,M5);
		MR0=mem(i6,M5);
		CALL ___lib_libfpmult;		/*XN*C1			*/
		I1=I4;
		MODIFY(I1,M1);
		MR0=DM(I1,M1);			/*Pop |Y| (or |X|)	*/
		MR1=DM(I1,M1);
		AX0=DM(I1,M2);
		I4=I1;
		M5=-1;
		DM(I4,M5)=AY0;			/*Push XN (1)		*/
		DM(I4,M5)=MY1;
		DM(I4,M5)=MY0;
		AY0=AR;				/*Move (XN*C1)		*/
		MY1=SR1;
		MY0=SR0;
		CALL ___lib_libfpsub;		/*|X|-XN*C1		*/
		I1=I4;
		MODIFY(I1,M1);
		MY0=DM(I1,M1);			/*Pop XN (0)		*/
		MY1=DM(I1,M1);
		AY0=DM(I1,M2);
		I4=I1;
		M5=-1;
		DM(I4,M5)=AR;			/*Push |X|-XN*C1 (1)	*/
		DM(I4,M5)=SR1;
		DM(I4,M5)=SR0;
		M5=1;
		AX0=mem(I6,M5);			/*Begin loading C2	*/
		MR1=mem(I6,M5);
		MR0=mem(i6,M5);
		CALL ___lib_libfpmult;		/*(XN*C2)		*/
		AY0=AR;
		MY1=SR1;
		MY0=SR0;
		I1=I4;
		MODIFY(I1,M1);
		MR0=DM(I1,M1);			/*Pop |X|-XN*C1	(0)	*/
		MR1=DM(I1,M1);
		AX0=DM(I1,M2);
		CALL ___lib_libfpsub;		/*f=(|X|-XN*C1)-(XN*C2)	*/
		I4=I1;
		M5=-1;
		DM(I4,M5)=AR;			/*Push f (no abs)	*/
		DM(I4,M5)=SR1;
		DM(I4,M5)=SR0;
		CALL ___lib_3wd_abs;		/*abs(f)		*/
		M5=1;
		AX0=mem(I6,M5);			/*Begin loading eps	*/
		MR1=mem(I6,M5);
		MR0=mem(i6,M5);
		AY0=AR;				/*Move |f|		*/
		MY1=SR1;
		MY0=SR0;
		CALL ___lib_libfpcp;		/*Compare |f| to eps	*/
		AF=PASS AR, AR=AY0;		/*Restore exp of |f|	*/
		IF LE JUMP __cont_computing;
		I1=I4;
		MODIFY(I1,M1);
		SR0=DM(I1,M1);			/*Pop f			*/
		SR1=DM(I1,M1);
		AR=DM(I1,M2);
		JUMP __attach_sign;		/*Jump if eps > |f|	*/

__cont_computing: AX0=AY0;
		MR1=MY1;
		MR0=MY0;
		CALL ___lib_libfpmult;		/*g=f*f			*/
		M5=-1;
		DM(I4,M5)=AR;			/*Push g (level 2)	*/
		DM(I4,M5)=SR1;
		DM(I4,M5)=SR0;
		M5=1;
		AX0=mem(I6,M5);			/*Begin loading R7	*/
		MR1=mem(I6,M5);
		MR0=mem(i6,M5);
		AY0=AR;				/*Move current sum	*/
		MY1=SR1;
		MY0=SR0;
		CNTR=6;
		DO __compute_poly UNTIL CE;
		  CALL ___lib_libfpmult;	/*Compute sum*g		*/
		  AX0=mem(I6,M5);		/*Load next R data	*/
		  MR1=mem(I6,M5);
		  MR0=mem(i6,M5);
		  AY0=AR;			/*Move current sum	*/
		  MY1=SR1;
		  MY0=SR0;
		  CALL ___lib_libfpadd;		/*sum=sum+next R	*/
		  AY0=AR;			/*Move current sum	*/
		  MY1=SR1;
		  MY0=SR0;
		  I1=I4;
		  MODIFY(I1,M1);
		  MR0=DM(I1,M1);		/*Get g	(don't pop)	*/
		  MR1=DM(I1,M1);
__compute_poly:   AX0=DM(I1,M1);

		CALL ___lib_libfpmult;		/*R=sum*g		*/
		AX0=AR;				/*Move R		*/
		MR1=SR1;
		MR0=SR0;
		MY0=DM(I1,M1);			/*Pop f	(level 0)	*/
		MY1=DM(I1,M1);
		AY0=DM(I1,M2);
		I4=I1;				/*Reset local stack	*/
		CALL ___lib_libfpmult;		/*Compute f*R		*/
		AX0=AR;				/*Move f*R		*/
		MR1=SR1;
		MR0=SR0;
		CALL ___lib_libfpadd;		/*Compute f+f*R		*/

__attach_sign:  AF=PASS AX1;
		IF GE JUMP __three_to_two;
		CALL ___lib_3wd_neg;		/*Negate		*/

__three_to_two: AX0=AR;
                MR1=SR1;
                MR0=SR0;
                call ___lib_libfp_sf;           /*Transform back to IEEE*/

__restore_state: JUMP ___lib_restore_small_frame;

__input_error:	SR=LSHIFT AR BY -100 (LO);	/*Load SR with a zero   */
                SI=ERANGE;			/*Set errno=EDOM        */
                CALL ___lib_set_errno;		/*Set global errno      */
                JUMP __restore_state;

.ENDMOD;

/*
    	This internal library function computes the Decimation in Time (DIT)
	Fast Fourier Transform (FFT) of its fixed point input.
	The integer value of the block exponent is returned
	by xfft.  It is not user callable directly from C.  Rather it
	is called by the routines fft1024, fft256, etc.

	The C Runtime Library
	
	Craig Smilovitz
	Development Tools Engineering

    	Created on 7/20/93

	(There is no #include file here because this is not user callable)
	int __fftcore(int in_real[], int in_imag[], int out_real[], int out_imag[]);
*/

.MODULE/IMAGE		__DIT_FFT_Routine;

#include	"lib_glob.h";
#include  	"fft_glob.h";

.external 	___lib_save_frame_no_i2i3;	
.external       ___lib_restore_frame_no_i2i3;

.ENTRY		__fftcore;

__fftcore:	MR1=TOPPCSTACK;
		MR0=fft_locals;
		CALL ___lib_save_frame_no_i2i3;

		DIS M_MODE;		/* ###### Fractional mode ####### */

		M7=-1;
		AX0=AR;			/* AX0 points to real input*/
		I7=out_real;
					/* AY1 points to imag input */
		MODIFY(I7,M4);               /* temp pointer to out_real */
		reads(AX1,I6,M5);
		writes(AX1,I7,M7);  	/* store real output */
		reads(AX1,I6,M5);
		writes(AX1,I7,M7);	/* store imag output */

scramble:       MR1=0;
		I7=blk_exponent;	/*Need to set blk_exponent here!*/
		modify(I7,M4);
		writes(MR1,I7,M7);	/* because bfp_adj is called after scramble*/
		SB=-2;		/*Check for two-bit growth space*/
		I5=AY1;			/*Point to imaginary data buffer*/
		I6=AX0;
		I7=out_real;	/*I6-->sequentially ordered data*/
		MODIFY(I7,M4);
		reads(MR1,I7,M7);	/*Get address to reverse (out_real)*/
		CALL brev_address;	/*Generate the bit reverse address*/
		I0=AR;
		reads(MR1,I7,M7); /*Get imaginary bit reversed address (out_imag)*/
		CALL brev_address;
		I1=AR;
		I7=mod_value;
		MODIFY(I7,M4);
		reads(SI,I7,M5);   	/*M0=mod_value, ie modifier for reversing N bits*/
		M0=SI;
		reads(SI,I7,M7);	/*CNTR = N */
		CNTR=SI;
       		ENA BIT_REV, ENA M_MODE;    	/*Enable bit-reversed outputs on DAG1*/
        	DO brev UNTIL CE;	/* Loop N times */
		   AR=DM(I5,M5);		/*Read imaginary data*/
		   SB=EXPADJ AR, DM(I1,M0)=AR;	/*Write data in bit-rev locs*/
		   AR=DM(I6,M5);		/*Read sequential data*/
brev:		   SB=EXPADJ AR, DM(I0,M0)=AR;	/*Write data in bit-rev locs*/
        	L0=0;
		L1=0;
		DIS BIT_REV, DIS M_MODE;	/*Disable bit-reverse*/
		CALL bfp_adj;		/*Check for sufficient growth space*/

		AX1=0;
		I7=nover4;
		MODIFY(I7,M4);
		reads(AX1,I7,M5);	/* get nover4 */
		writes(AX1,I7,M5);	/* store groups */
		AX1=2;
		writes(AX1,I7,M7);	/*bflys_per_group = 2 */
		I7=node_space;
		MODIFY(I7,M4);
		writes(AX1,I7,M5); 	/* node_space     = 2 */

fft_strt:       M0=0;

/*
------------------------STAGE 1-------------------------
                                                        */
		reads(SI,I7,M5); /* out_imaginary */
		I5=SI;
		I6=I5;
		MODIFY(I6,M5);
                reads(SI,I7,M5); /* out_real */
		I0=SI;
		I1=I0;
		MODIFY(I1,M1);
                m2 = 2;

                ax0 = dm(i0,m0);
                ay0 = dm(i1,m0);
		reads(SI,I7,M6); /* nover2 */
		CNTR=SI;
                do group_lp until ce;
                  ar=ax0+ay0, ax1=dm(i5,m6);
                  sb=expadj ar, dm(i0,m2)=ar;
                  ar=ax0-ay0, AY1=DM(I6,M6);
                  sb=expadj ar, DM(I1,M2)=AR;
                  ar=ax1+ay1, AX0=DM(I0,M0);
                  sb=expadj ar, dm(i5,m5)=ar;
		  modify(i5,m5);
                  ar=ax1-ay1, AY0=DM(I1,M0);
                  sb=expadj ar, dm(i6,m5)=ar;      
group_lp:	  modify(i6,m5);
                call bfp_adj;

/*-------------------------------------------------------------------*/

	I7=nover2;
	MODIFY(I7,M4);
	reads(SI,I7,M6);  /*Set circular buffer length to nover2 */
	L6=SI;
	L5=L6;
	I7=log2Nminus2;
	modify(I7,M4);
	reads(SI,I7,M6);  /*Initialize stage counter to log2Nminus2 */
	CNTR=SI;
        DO stage_loop UNTIL CE; 	/*Compute all stages in FFT*/
		I7=groups;
		MODIFY(I7,M4);
		reads(SI,I7,M7);	/*M6=groups ie. twiddle factor modifier*/
                M6=SI;
		I7=node_space;
		MODIFY(I7,M4);
		reads(SI,I7,M7); 	/*M2=node space modifier*/
                M2=SI;
		I7=out_real;
		MODIFY(I7,M4);
		reads(SI,I7,M7);	/*I0=out_real -->x0 in 1st grp of stage*/
		I0=SI;
                reads(SI,I7,M7);	/*I7=out_imag -->y0 in 1st grp of stage*/
		I7=SI;
		sr0=i7;
                I1=I0;
                MODIFY(I1,M2);          /*I1 -->y0 of 1st grp in stage*/
	        ay0=m2;
		ar=sr0+ay0;
		i7=ar;
		CNTR=M6;		/*Loop once per group*/
                DO group_loop UNTIL CE;
		   L6=0;
		   I6=bflys_per_group;
		   MODIFY(I6,M4);
		   reads(SI,I6,M5);
		   CNTR=SI;	       /*CNTR = butterfly counter*/
		   reads(SI,I6,M5);
		   I5=SI;		/*I5=twiddle_imag --> (-S) of W0*/
		   reads(SI,I6,M5);
		   I6=SI;		/*I6=twiddle_real --> C of W0*/
		   L6=L5;		/* restore circular buffer on I6 */
                   MX0=DM(I1,M0),MY0=PM(I6,M6);		/*MY0=C,MX0=x1 */
                   MY1=PM(I5,M6);
		   MX1=DM(I7,M5);         /*MY1=-S,MX1=y1*/
		   modify(i7,m7);         /* Substitute dm(i7,0)*/
		   sr1=i7;
		   i7=sr0;
                   DO bfly_loop UNTIL CE;
                      MR=MX0*MY1(SS),AX0=DM(I0,M0); 	/*MR=x1(-S),AX0=x0*/
		      				 /* sr0&sr1 are used as*/
		      				/*temporary storage  */
                      MR=MR+MX1*MY0(RND),AX1=DM(I7,M5);	/*MR=(y1(C)+x1(-S)),AX1=y0*/
                      AY1=MR1,MR=MX0*MY0(SS);		/*AY1=y1(C)+x1(-S),MR=x1(C)*/
                      MR=MR-MX1*MY1(RND),MY1=PM(I5,M6);       /*MR=x1(C)-y1(-S)*/
                      AY0=MR1,AR=AX1-AY1;             	/*AY0=x1(C)-y1(-S),*/
                                      			/*AR=y0-[y1(C)+x1(-S)]*/
		      i7=sr1;           /* -------- die here ------- */
                      SB=EXPADJ AR,DM(I7,M5)=AR;   	/*Check for bit growth,*/
		      sr1=i7;	                       	/*y1=y0-[y1(C)+x1(-S)]*/
                      AR=AX0-AY0,MX1=DM(I7,M5);
                                           		/*AR=x0-[x1(C)-y1(-S)],*/
                                           		/*MX1=next y1,MY1=next (-S)*/
                      SB=EXPADJ AR,DM(I1,M1)=AR;   	/*Check for bit growth,*/
                                           		/*x1=x0-[x1(C)-y1(-S)]*/
                      AR=AX0+AY0,MX0=DM(I1,M0),MY0=PM(I6,M6);
                                           		/*AR=x0+[x1(C)-y1(-S)],*/
                                           		/*MX0=next x1,MY0=next C*/
                      SB=EXPADJ AR,DM(I0,M1)=AR;   	/*Check for bit growth,*/
                                           		/*x0=x0+[x1(C)-y1(-S)]*/
                      AR=AX1+AY1;             		/*AR=y0+[y1(C)+x1(-S)]*/
		   					/* sr0&sr1 are used as*/
		      i7=sr0;				/*temporary storage  */
                      SB=EXPADJ AR,DM(I7,M5)=AR;   	/*Check for growth,*/
bfly_loop:	      sr0=i7;
                                                	/*y0=y0+[y1(C)+x1(-S)]*/
                  MODIFY(I0,M2);  /*I0 -->1st x0 in next group*/
                  MODIFY(I1,M2);  /*I1 -->1st x1 in next group*/
		  ay0=m2;				/*modify i7 by m2*/
		  ar=sr1+ay0;
		  sr1=ar, ar=sr0+ay0;
		  sr0=ar;	
group_loop:       i7=sr1;
            CALL bfp_adj;   		/*Compensate for bit growth*/
	    I7=groups;
	    MODIFY(I7,M4);
	    M7=0;
            reads(SI,I7,M7);	     /* (note m7 is 0 here) */
            SR=ASHIFT SI BY -1(LO); /*groups / 2*/
            writes(SR0,I7,M5);	   /*groups=groups/2*/
            reads(SI,I7,M7);	   /* get bflys_per_group */
            SR=ASHIFT SI BY 1(LO);
	    writes(SR0,I7,M7);	   /* bflys_per_group = bflys_per_group 2 */
	    I7=node_space;
	    MODIFY(I7,M4);
	    writes(SR0,I7,M7);     /*node_space = bflys_per_group= */
stage_loop: M7=-1;		   /* restore M7 to normal value */


/*-----------------------  LAST STAGE  --------------------------*/
	    I7=node_space;
	    MODIFY(I7,M4);
            reads(SI,I7,M7);	/* m2 = node_space */
	    M2=SI;
            reads(SI,I7,M7);  	/* i6 = twiddle_real */
	    I6=SI;
            reads(SI,I7,M7);	/* i5 = twiddle_imag */
	    I5=SI;
	    I7=nover2;
	    MODIFY(I7,M4);
	    reads(AX0,I7,M7);	/* AX0 gets nover2 */
            reads(AY0,I7,M7);	/* I0  gets out_real */
	    I0=AY0;
	    AR=AX0+AY0, reads(AY0,I7,M7);
	    I1=AR;
	    sr0=AY0;		/* SR0 gets out_imag */
            cntr=AX0;		/* CNTR gets nover2); */
            M6=1;
            AR=AX0+AY0, MY0=PM(I6,M6),MX0=DM(I1,M0); /*MY0=C,MX0=x1 */
	    I7=AR;
            MY1=PM(I5,M6); MX1=DM(I7,M5);            /*MY1=-S,MX1=y1*/
	    modify(i7,m7);		/*substitute dm(i7,0)*/
	    sr1=i7;
            DO bfly_lp UNTIL CE;
                      MR=MX0*MY1(SS),AX0=DM(I0,M0);        /*MR=x1(-S),AX0=x0*/
		      i7=sr0;
                      MR=MR+MX1*MY0(RND),AX1=DM(I7,M5);
                                                    /*MR=(y1(C)+x1(-S)),AX1=y0*/
		      modify(i7,m7);		/*substitute dm(i7,0)*/
                      sr0=i7;
		      AY1=MR1,MR=MX0*MY0(SS);      /*AY1=y1(C)+x1(-S),MR=x1(C)*/
                      MR=MR-MX1*MY1(RND);                  /*MR=x1(C)-y1(-S)*/
                      AY0=MR1,AR=AX1-AY1;                  /*AY0=x1(C)-y1(-S),*/
                                                        /*AR=y0-[y1(C)+x1(-S)]*/

		      i7=sr1;
                      SB=EXPADJ AR,DM(I7,M5)=AR;        /*Check for bit growth,*/
                                                        /*y1=y0-[y1(C)+x1(-S)]*/
                      AR=AX0-AY0,MX1=DM(I7,M5);
		      modify(i7,m7);		/*substitute dm(i7,0)*/
		      MY1=PM(I5,M6);
                                                        /*AR=x0-[x1(C)-y1(-S)],*/
                                                    /*MX1=next y1,MY1=next (-S)*/
                      SB=EXPADJ AR,DM(I1,M1)=AR;    /*Check for bit growth,*/
                                                    /*x1=x0-[x1(C)-y1(-S)]*/
                      AR=AX0+AY0,MX0=DM(I1,M0),MY0=PM(I6,M6);
                                                    /*AR=x0+[x1(C)-y1(-S)],*/
                                                    /*MX0=next x1,MY0=next C*/
                      SB=EXPADJ AR,DM(I0,M1)=AR;    /*Check for bit growth,*/
                                                    /*x0=x0+[x1(C)-y1(-S)]*/
                      AR=AX1+AY1;                   /*AR=y0+[y1(C)+x1(-S)]*/
	              sr1=i7;
		      i7=sr0;
		      SB=EXPADJ AR,DM(I7,M5)=AR;    /*Check for bit growth,*/
bfly_lp:  		sr0=i7;

		I7=blk_exponent;
		MODIFY(I7,M4);
		reads(AR,I7,M6);	    /*Block exponent returned*/

restore_state:
		ENA M_MODE;	/* ###### End Fractional mode ####### */
		L6=0;
		L5=0;
		L1=0;
		L0=0;
		M6=0;
		M2=0;
		MR0=fft_locals;
		JUMP ___lib_restore_frame_no_i2i3;

bfp_adj:	AY0=-2;
		AX0=SB;
		SB=-2;				/*Prepare for next iteration*/
		AR=AY0-AX0;			/*Check for SB=-2*/
		IF EQ RTS;			/*If SB=-2, no bit growth, return*/
		I7=Nminus1;
		MODIFY(I7,M4);
		SI=DM(I7,M7);			/*CNTR=Nminus1 */
		CNTR=SI;
		I7=out_real;
		MODIFY(I7,M4);
		SI=DM(I7,M7);			/*I0=read pointer*/
		I0=SI;
		I1=I0;				/*I1=write pointer*/
		ax1=DM(I7,M7);			/*ax1=imaginary write pointer*/
		I7=ax1;				/*I7=imaginary read pointer*/
		SE=AR;				/*Prepare for shift*/
		SI=DM(I7,M5);			/*Read first sample*/
		ay1=I7;
		DO shift_loop UNTIL CE;		/*Shift block of data*/
		   SR=ASHIFT SI (HI), SI=DM(I0,M1); /*SR1=shifted data,SI=next value*/
		   i7=ax1;
		   DM(I7,M5)=SR1;		    /*output imaginary value*/ 
		   ax1=i7;
		   i7=ay1;
		   SR=ASHIFT SI (HI), SI=DM(I7,M5);
		   ay1=i7;
shift_loop:	   DM(I1,M1)=SR1;		    /*Unshifted data=shifted data*/
		SR=ASHIFT SI (HI), SI=DM(I0,M1);    /*Shift last data word*/
		i7=ax1;
		DM(I7,M5)=SR1, SR=ASHIFT SI (HI);
		I7=blk_exponent;
		modify(i7,m4);
		AY0=DM(i7,m6);   	/*Update block exponent and*/
		DM(I1,M1)=SR1, AR=AY0-AR;	/*store last shifted sample*/
		I7=blk_exponent;
		modify(i7,m4);
		DM(i7,m6)=AR;
		SB = -2;
		RTS;

brev_address:        /* note that code above counts on brev_address not changing I7 */
		AR = PASS 0;			/*Zero out AR for reverse*/
		AY1 = H#8000;			/*Used to test bit*/
		MY0 = H#4000;			/*Load MY0 with .5*/

		CNTR = 14;			/*Reverse 14 address bits*/
		DO reverse UNTIL CE;
		   AY0=AR, MR=MR1*MY0 (SS);	/*MSB of MR0 is next bit*/
		   AF=MR0+AY1;			/*Sets AC if bit is on*/
reverse:	   AR=AR+AY0+C; 		/*Carry set if bit set*/

		RTS;
.ENDMOD;



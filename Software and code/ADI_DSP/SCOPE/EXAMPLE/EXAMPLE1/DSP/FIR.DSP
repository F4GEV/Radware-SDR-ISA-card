{***************************************************************************
//
//      FIR.DSP
//
//      Copyright (c) 1994 Analog Devices, Inc.
//
//-----------------------------------------------------------------------
//
//      Module:    fir.dsp     
//      Purpose:   This program implements a single precision Direct Form FIR 
//                 filter structure. The filter coefficients used in this 
//                 program are for a lowpass filter of length 81 taps with 
//                 passband from 0 to .125fs and stopband from .15fs to .5fs, 
//                 where fs is the sampling rate in Hertz. It is designed 
//                 via Parks-McClellan algorithm. The coefficients are stored 
//                 the file FIRCOEFF.DAT.
/**************************************************************************}

.MODULE/RAM               FIR_LPF;

#define            taps 81
.VAR/CIRC          data[taps];
.VAR/DM/RAM        i0_pnt;
.VAR/PM/CIRC       fir_coeff[taps];
.INIT              fir_coeff: <fircoeff.dat>;

.external RegSave;
.external RegRestore;

.ENTRY   initFilter_; 
.ENTRY   sFilter_;

initFilter_:
         call RegSave;                 {save registers}
         I0=^data;   M0=1; l0=taps;
         CNTR=taps;
         DO zero UNTIL CE;
zero:    DM(I0,M0)=0;                  {clear the filter delay line buffer}
         dm(i0_pnt) = i0;
         call RegRestore;              { restore registers }
         RTS;


sFilter_: 
         DIS M_MODE; 
         call RegSave;                   { save registers }
         I0= dm(i0_pnt);   M0=1; L0=taps;

         I4= ^fir_coeff; M4=1; L4=taps;  
         DM(I0,M0)= ar;              {store sample in data buffer }
         CNTR=taps-1;
fir_df:  MR=0, MX0=DM(I0,M0), MY0=PM(I4,M4);
         DO conv UNTIL CE;
conv:       MR=MR+MX0*MY0(SS), MX0=DM(I0,M0), MY0=PM(I4,M4);
         MR=MR+MX0*MY0(RND);
         IF MV SAT MR;
         ar = mr1;
         dm(i0_pnt) = i0;
         call RegRestore;                { restore registers }
         ENA M_MODE;
         RTS;


.ENDMOD;


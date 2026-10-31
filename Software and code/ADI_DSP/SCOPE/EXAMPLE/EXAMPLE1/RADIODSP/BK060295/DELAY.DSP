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
#define            delay_taps 40
.VAR/CIRC          data[taps];
.VAR/CIRC          delay_data[delay_taps];
.VAR/DM/RAM        reg_l0, reg_l4, reg_i4, reg_m4, i0_pnt,delay_pnt;
.VAR/PM/CIRC       fir_coeff[taps];
.INIT              fir_coeff: <fircoeff.dat>;

.ENTRY   initFilter_; 
.ENTRY   sFilter_;
.ENTRY   delayLine_;

initFilter_:
         dm(reg_l0) = l0;                  {save registers}
         I0=^data;   M0=1; l0=taps;
         CNTR=taps;
         DO zero UNTIL CE;
zero:    DM(I0,M0)=0;                  {clear the filter delay line buffer}
         dm(i0_pnt) = i0;
         I0=^delay_data;   M0=1; l0=delay_taps;
         CNTR=delay_taps;
         DO zero_delay UNTIL CE;
zero_delay: DM(I0,M0)=0;  
         dm(delay_pnt) = i0;
         l0 = dm(reg_l0);
         RTS;

delayLine_: 
         DIS M_MODE; 
         dm(reg_l0) = l0;                  
         dm(reg_l4) = l4;                  
         dm(reg_i4) = i4;    
         dm(reg_m4) = m4;
         I0 = dm(delay_pnt); M0 = 1; L0 = delay_taps;
         AY0 = dm(I0,M0);
         I0 = dm(delay_pnt);
         dm(I0,M0) = AR;
         dm(delay_pnt) = I0;
         AR = AY0;
         l0 = dm(reg_l0);
         l4 = dm(reg_l4);   
         i4 = dm(reg_i4);   
         m4 = dm(reg_m4);   
         ENA M_MODE;
         RTS;          

sFilter_: 
         DIS M_MODE; 
         dm(reg_l0) = l0;                  
         dm(reg_l4) = l4;                  
         dm(reg_i4) = i4;    
         dm(reg_m4) = m4;
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
         l0 = dm(reg_l0);
         l4 = dm(reg_l4);   
         i4 = dm(reg_i4);   
         m4 = dm(reg_m4);   
         ENA M_MODE;
         RTS;


.ENDMOD;


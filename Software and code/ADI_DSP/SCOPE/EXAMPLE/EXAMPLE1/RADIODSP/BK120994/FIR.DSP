
/***************************************************************************
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
/**************************************************************************/

.MODULE/RAM               FIR_LPF;

#define            taps 81
#define            delay_taps 40
.VAR/CIRC          data[taps];
.VAR/CIRC          usb_data[taps];
.VAR/CIRC          lsb_data[taps];
.VAR/CIRC          delay_data[delay_taps];
.VAR/DM/RAM        reg_l0, reg_l4, reg_i4, reg_m4, i0_pnt,usb_pnt,lsb_pnt,delay_pnt;
.VAR/DM/RAM        which_filter;
.VAR/PM/CIRC       fir_coeff[taps];
.INIT              fir_coeff: <fircoeff.dat>;
.VAR/PM/CIRC       coeff_3to30[taps];
.INIT              coeff_3to30: <3to30.fir>;
.VAR/PM/CIRC       coeff_9to11[taps];
.INIT              coeff_9to11: <9to11.fir>;
.VAR/PM/CIRC       coeff_4to22[taps];
.INIT              coeff_4to22: <4to22.fir>;
.VAR/PM/CIRC       coeff_8to12[taps];
.INIT              coeff_8to12: <8to12.fir>;
.VAR/PM/CIRC       coeff_9to10[taps];
.INIT              coeff_9to10: <9to10.fir>;

.external RegSave;
.external RegRestore;

.ENTRY   initFilter_;
.ENTRY   sFilter_;
.ENTRY   UFilter_;
.ENTRY   LFilter_;
.ENTRY   delayLine_;
.ENTRY   SelectFilter_;

initFilter_:
         call RegSave;
         I0=^data;       M0=1; l0=taps;
         I1=^usb_data;   M1=1; l1=taps;
         I2=^lsb_data;   M2=1; l2=taps;
         CNTR=taps;
         DO zero UNTIL CE;
         DM(I0,M0)=0;
         DM(I1,M1)=0;
zero:    DM(I2,M2)=0;                  {clear the filter delay line buffer}
         dm(i0_pnt) = i0;
         dm(usb_pnt) = i1;
         dm(lsb_pnt) = i2;
         I0=^delay_data;   M0=1; l0=delay_taps;
         CNTR=delay_taps;
         DO zero_delay UNTIL CE;
zero_delay: DM(I0,M0)=0;
         dm(delay_pnt) = i0;
         i0 = 4;
         dm(which_filter) = i0;
         call RegRestore;
         RTS;

delayLine_:
         call RegSave;
         DIS M_MODE;
         I0 = dm(delay_pnt); M0 = 1; L0 = delay_taps;
         AY0 = dm(I0,M0);
         I0 = dm(delay_pnt);
         dm(I0,M0) = AR;
         dm(delay_pnt) = I0;
         AR = AY0;
         call RegRestore;
         ENA M_MODE;
         RTS;

sFilter_:
         call RegSave;
         DIS M_MODE;
         I0= dm(i0_pnt);   M0=1; L0=taps;

         I4= ^fir_coeff; M4=1; L4=taps;
         call Filter;
         dm(i0_pnt) = i0;
         ENA M_MODE;
         call RegRestore;
         RTS;

SelectFilter_: dm(which_filter) = AR;
               rts;

UFilter_: call RegSave;
          AY0 = dm(which_filter);
          AX0 = 5;
          AF = AX0-AY0;
          if EQ jump no_filter;
          DIS M_MODE;
          I0= dm(usb_pnt);   M0=1; L0=taps;
          call ChooseFilter;
          call Filter;
          dm(usb_pnt) = i0;
          ENA M_MODE;
no_filter: call RegRestore;
          RTS;

LFilter_: call RegSave;
          AY0 = dm(which_filter);
          AX0 = 5;
          AF = AX0-AY0;
          if EQ jump no_filter;
          DIS M_MODE;
          I0= dm(lsb_pnt);   M0=1; L0=taps;
          call ChooseFilter;
          call Filter;
          dm(lsb_pnt) = i0;
          ENA M_MODE;
          call RegRestore;
          RTS;

ChooseFilter: AY0 = dm(which_filter);
              AF = AY0-1;
              if LT jump filt9to10;
              if EQ jump filt9to11;
              AX0 = 3;
              AF = AY0-AX0;
              if LT jump filt8to12;
              if EQ jump filt4to22;
              I4= ^coeff_3to30;    M4=1; L4=taps;
              rts;
filt8to12:    I4= ^coeff_8to12;    M4=1; L4=taps;
              rts;
filt4to22:    I4= ^coeff_4to22;    M4=1; L4=taps;
              rts;
filt9to11:    I4= ^coeff_9to11;    M4=1; L4=taps;
              rts;
filt9to10:    I4= ^coeff_9to10;    M4=1; L4=taps;
              rts;

Filter:  DM(I0,M0)= ar;              {store sample in data buffer }
         CNTR=taps-1;
fir_df:  MR=0, MX0=DM(I0,M0), MY0=PM(I4,M4);
         DO conv UNTIL CE;
conv:       MR=MR+MX0*MY0(SS), MX0=DM(I0,M0), MY0=PM(I4,M4);
         MR=MR+MX0*MY0(RND);
         IF MV SAT MR;
         ar = mr1;
         rts;

.ENDMOD;


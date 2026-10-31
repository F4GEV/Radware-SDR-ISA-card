/******************************************************************************
//
//      GEN.DSP
//
//      Copyright (c) 1993 Analog Devices, Inc.
//
//-----------------------------------------------------------------------------
//
//      Module:    gen.dsp
//      Purpose:   Module used to generate signal waveforms on DSP
//
/*****************************************************************************/

#define taps 81
.MODULE/RAM/seg = int_pm     Signal_Gen;

.ENTRY Filter;
.ENTRY fir_df;
.ENTRY fir_cnt;

Filter:  CNTR=taps-1;
fir_cnt:  DM(I0,M0)= ar;              {store sample in data buffer }
fir_df:  MR=0, MX0=DM(I0,M0), MY0=PM(I4,M4);
         DO conv UNTIL CE;
conv:       MR=MR+MX0*MY0(SS), MX0=DM(I0,M0), MY0=PM(I4,M4);
         MR=MR+MX0*MY0(RND);
         IF MV SAT MR;
         ar = mr1;
         rts;

.ENDMOD;


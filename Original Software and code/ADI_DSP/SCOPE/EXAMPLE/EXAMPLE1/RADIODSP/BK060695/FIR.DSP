
.MODULE/RAM               FIR_LPF;

#define            taps 81
#define            delay_taps 40
#define            fm_taps 4
#define            cw_taps 5
#define            rtty_taps 21
#define            rtty_max 1000
#define            rtty_min -1000
.VAR/CIRC          data[taps];
.VAR/CIRC          usb_data[121/*taps*/];
.VAR/CIRC          lsb_data[121/*taps*/];
.VAR/CIRC          delay_data[delay_taps];
.VAR/CIRC          delay2_data[delay_taps];
.VAR/CIRC          delay3_data[delay_taps];
.VAR/CIRC          delay4_data[delay_taps];
.VAR/CIRC          fm_data[fm_taps];
.VAR/CIRC          fm2_data[fm_taps];
.VAR/CIRC          hz_60_data_usb[taps];
.VAR/CIRC          hz_60_data_lsb[taps];
.VAR/CIRC          hilbert_data[taps];
.VAR/CIRC          hilbert3_data[taps];
.VAR/CIRC          rtty_data[rtty_taps];
.VAR/CIRC          cw_data[cw_taps];
.VAR/DM/RAM        reg_l0, reg_l4, reg_i4, reg_m4, i0_pnt,hilbert3_pnt,hilbert_pnt;
.VAR/DM/RAM        usb_pnt,lsb_pnt,delay_pnt,delay2_pnt,delay3_pnt,delay4_pnt;
.VAR/DM/RAM        fm_pnt,fm2_pnt,hz_60_pnt_usb,hz_60_pnt_lsb,rtty_pnt,cw_pnt;
.VAR/DM/RAM        highpass_pnt;
.VAR/DM/RAM        which_filter,FM_OFF_MAX;
.VAR/DM/RAM        FM3_Last,FM2_Last;
.VAR/DM/RAM        Delta_Last,Delta_Last2;
.VAR/DM/RAM        rtty_usb,rtty_lsb,cw_time,cw_detected;
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
.VAR/PM/CIRC/seg = int_pm       coeff_9to10[taps];
.INIT              coeff_9to10:<highpass.fir>; /*<9to10.fir>;*/
.VAR/PM/CIRC/seg = int_pm       coeff_fm16[taps];
.INIT              coeff_fm16:<fm16.fir>;
.VAR/PM/CIRC/seg = int_pm       coeff_highpass[121];
.INIT              coeff_highpass:<high4.fir>;
.VAR/PM/CIRC/seg = int_pm       coeff_fm[fm_taps];
.INIT              coeff_fm:<dif.fir>;
.VAR/PM/CIRC       coeff_60_hz[taps];
.INIT              coeff_60_hz: <60hz.fir>;

.external RegSave;
.external RegRestore;
.external RegSave2;
.external RegRestore2;
.external Filter;
.external fir_df;
.external fir_cnt;
.external Offset_1_pnt;
.external Offset_2_pnt;
.external Offset_Array_1;
.external Offset_Array_2;

.ENTRY   Highpass_Both_;
.ENTRY   initFilter_;
.ENTRY   sFilter_;
.ENTRY   UFilter_;
.ENTRY   LFilter_;
.ENTRY   delayLine_;
.ENTRY   SelectFilter_;
.ENTRY   FM_Filter_;
.ENTRY   FM_Filter2_;
.ENTRY   FM_Filter8;
.ENTRY   FM_Filter9;
.ENTRY   Filter_60_Hz_;
.ENTRY   Double_Filter_60_Hz_;
.ENTRY   RTTY_;
.ENTRY   RTTY_Block_;
.ENTRY   Remove_Noise_;
.ENTRY   Remove_Noise2_;
.ENTRY   Remove_Noise3_;
.ENTRY   Hilbert_And_Delay_;
.ENTRY   Hilbert_And_Delay2_;
.ENTRY   Hilbert_And_Delay3_;
.ENTRY   Xmit_LSB_H_And_D_;
.ENTRY   Xmit_USB_H_And_D_;
.ENTRY   Make_SSB_;
.ENTRY   Filter_Both_;
.ENTRY   Filter_Both_And_Copy_;
.ENTRY   Filter_First_;
.ENTRY   Remove_USB_;
.ENTRY   Remove_LSB_;
.ENTRY   CW_Block_;

initFilter_:
         call RegSave;
         I0=^data;           M0=1; l0=taps;
         I1=^usb_data;       M1=1; l1=taps;
         I2=^lsb_data;       M2=1; l2=taps;
         I3=^hz_60_data_lsb; M3=1; l3=taps;
         CNTR=taps;
         DO zero UNTIL CE;
         DM(I0,M0)=0;
         DM(I1,M1)=0;
         DM(I2,M2)=0;
zero:    DM(I3,M3)=0;                  {clear the filter delay line buffer}
         dm(i0_pnt) = i0;
         dm(usb_pnt) = i1;
         dm(lsb_pnt) = i2;
         dm(hz_60_pnt_lsb) = i3;
         I0=^hz_60_data_usb; M0=1; l0=taps;
         I1=^hilbert_data; l1 = taps;
         I2=^hilbert3_data; l2 = taps;
         CNTR=taps;
         DO zero_usb UNTIL CE;
         DM(I0,M0)=0;
         DM(I1,M0)=0;
zero_usb: DM(I2,M0)=0;
         dm(hz_60_pnt_usb) = i0;
         dm(hilbert_pnt) = i1;
         dm(hilbert3_pnt) = i2;
         I0=^delay_data;   M0=1; l0=delay_taps;
         I1=^delay2_data;        l1=delay_taps;
         I2=^delay3_data;        l2=delay_taps;
         I3=^delay4_data;        l3=delay_taps;
         CNTR=delay_taps;
         DO zero_delay UNTIL CE;
         DM(I0,M0)=0;
         DM(I1,M0)=0;
         DM(I2,M0)=0;
zero_delay: DM(I3,M0)=0;
         dm(delay_pnt) = i0;
         dm(delay2_pnt) = i1;
         dm(delay3_pnt) = i2;
         dm(delay4_pnt) = i3;
         I0=^fm_data;   M0=1; l0=fm_taps;
         I1=^fm2_data;  l1 = fm_taps;
         CNTR=fm_taps;
         DO zero_fm UNTIL CE;
         DM(I0,M0)=0;
zero_fm: DM(I1,M0)=0;
         dm(fm_pnt) = i0;
         dm(fm2_pnt) = i1;
         I0=^rtty_data;   M0=1; l0=rtty_taps;
         CNTR=rtty_taps;
         DO zero_rtty UNTIL CE;
zero_rtty: DM(I0,M0)=0;
         dm(rtty_pnt) = i0;
         I0=^cw_data;   M0=1; l0=cw_taps;
         CNTR=cw_taps;
         DO zero_cw UNTIL CE;
zero_cw: DM(I0,M0)=0;
         dm(cw_pnt) = i0;
         AX0 = 0;
         dm(cw_time) = AX0;
         dm(cw_detected) = AX0;
/*         I0 = ^usb_data2;
         DM(usb_pnt2) = I0;
         I0 = ^lsb_data2;
         DM(lsb_pnt2) = I0;
*/         i0 = 4;
         dm(which_filter) = i0;
         I0 = 0;
         DM(FM2_Last) = I0;
         i0 = ^Offset_Array_1;
         DM(Offset_1_pnt) = i0;
         i0 = ^Offset_Array_2;
         DM(Offset_2_pnt) = i0;
         call RegRestore;
         RTS;

delayLine_: call RegSave;
            DIS M_MODE;
            call delayLine;
            ENA M_MODE;
            call RegRestore;
            rts;

delayLine: I0 = dm(delay_pnt); M0 = 1; L0 = delay_taps;
         MR0 = dm(I0,M0);
         I0 = dm(delay_pnt);
         dm(I0,M0) = AR;
         dm(delay_pnt) = I0;
         SR = ASHIFT MR0 BY -1 (HI);  /* divide by 2 */
         AR = SR1;
         RTS;

delayLinex2: I0 = dm(delay_pnt); M0 = 1; L0 = delay_taps;
             MR0 = dm(I0,M0);
             I0 = dm(delay_pnt);
             dm(I0,M0) = AR;
             dm(delay_pnt) = I0;
             RTS;

delayLine2: I0 = dm(delay2_pnt); M0 = 1; L0 = delay_taps;
            MR0 = dm(I0,M0);
            I0 = dm(delay2_pnt);
            dm(I0,M0) = AR;
            dm(delay2_pnt) = I0;
            SR = ASHIFT MR0 BY -1 (HI);  /* divide by 2 */
            AR = SR1;
            RTS;

delayLine3: I0 = dm(delay3_pnt); M0 = 1; L0 = delay_taps;
            MR0 = dm(I0,M0);
            I0 = dm(delay3_pnt);
            dm(I0,M0) = AR;
            dm(delay3_pnt) = I0;
            SR = ASHIFT MR0 BY -1 (HI);  /* divide by 2 */
            AR = SR1;
            RTS;

delayLine4: I0 = dm(delay4_pnt); M0 = 1; L0 = delay_taps;
            MR0 = dm(I0,M0);
            I0 = dm(delay4_pnt);
            dm(I0,M0) = AR;
            dm(delay4_pnt) = I0;
            SR = ASHIFT MR0 BY -1 (HI);  /* divide by 2 */
            AR = SR1;
            RTS;

FM_Filter_:
         call RegSave2;
         DIS M_MODE;
         I0= dm(fm_pnt);   M0=1; L0=fm_taps;
         I4= ^coeff_fm; M4=1; L4=fm_taps;
         DM(I0,M0)= ar;
         CNTR=fm_taps-1;
         call fir_df;
         dm(fm_pnt) = i0;
         ENA M_MODE;
         call RegRestore2;
         RTS;

FM_Filter2_:
         call RegSave;
         DIS M_MODE;
         I0= dm(fm2_pnt);   M0=1; L0=fm_taps;
         I4= ^coeff_fm; M4=1; L4=fm_taps;
         DM(I0,M0)= ar;
         CNTR=fm_taps-1;
         call fir_df;
         dm(fm2_pnt) = i0;
         ENA M_MODE;
         call RegRestore;
         RTS;

FM_Filter7: AY1 = dm(FM3_Last);
            dm(FM3_Last) = AR;
            AR = AR-AY1;
            AY1 = DM(FM2_Last);
            AX0 = AR;
            AR = AR-AY1;
            AR = ABS AR;
            AY1 = 0x5000;
            AR = AR-AY1;
            if LT jump FM_Filt7_End;
            AX1 = AR;
            AR = -AX0;
            MR1 = AR;
            AR = AR-AY1;
            AF = ABS AR;
            AR = AX1-AF;
            if LT jump FM_Filt7_End;
            AX0 = MR1;
FM_Filt7_End: AR = AX0;
            DM(FM2_Last) = AR;
            rts;

FM_Filter8: AY1 = dm(FM3_Last);
            dm(FM3_Last) = AR;
            AR = AR-AY1;
            AY1 = DM(FM2_Last);
            AX0 = AR;
            AR = AR XOR AY1;
            AY1 = 0xC000;
            AR = AR AND AY1;
            AR = AR-AY1;
            if NE jump FM_Filt_End;
            AR = ABS AX0;
            AY1 = 0x4000;
            AR = AR-AY1;
            if LT jump FM_Filt_End;
            AR = PASS AX0;
            if LT jump FM_F_High;
            AX0 = 0x8000;
            jump FM_Filt_End;
FM_F_High:  AX0 = 0x7FFF;
FM_Filt_End: AR = AX0;
            DM(FM2_Last) = AR;
            rts;

FM_Filter9: AY1 = dm(FM3_Last);
            dm(FM3_Last) = AR;
            AR = AR-AY1;
            rts;

Filter_60_Hz_: call RegSave;
              DIS M_MODE;
              I1 = AR;
              L1 = 0;
              M1 = 1;
              M2 = 0;
              CNTR = 128;
              AR = dm(I1,M2);
              do loop_60_hz until CE;
              call Filter_60_Hz_USB;
              dm(I1,M1) = AR;
              AR = dm(I1,M2);
              call Filter_60_Hz_LSB;
              dm(I1,M1) = AR;
loop_60_hz:   AR = dm(I1,M2);
              ENA M_MODE;
              call RegRestore;
              rts;

Double_Filter_60_Hz_: call RegSave;
                      DIS M_MODE;
                      I1 = AR;
                      L1 = 0;
                      M1 = 2;
                      M2 = 0;
                      CNTR = 128;
                      AR = dm(I1,M2);
                      do d_loop_60_hz until CE;
                      call Filter_60_Hz_USB;
                      call Filter_60_Hz_LSB;
                      dm(I1,M1) = AR;
d_loop_60_hz:         AR = dm(I1,M2);
                      ENA M_MODE;
                      call RegRestore;
                      rts;

Filter_60_Hz_USB: I0= dm(hz_60_pnt_usb);   M0=1;   L0=taps;
                  I4= ^coeff_60_hz;        M4=1;   L4=taps;
                  call Filter;
                  dm(hz_60_pnt_usb) = i0;
                  RTS;

Filter_60_Hz_LSB: I0= dm(hz_60_pnt_lsb);   M0=1;   L0=taps;
                  I4= ^coeff_60_hz;        M4=1;   L4=taps;
                  call Filter;
                  dm(hz_60_pnt_lsb) = i0;
                  RTS;

sFilter_: call RegSave;
          DIS M_Mode;
          call sFilter;
          ENA M_Mode;
          call RegRestore;
          rts;

sFilter: I0= dm(i0_pnt);   M0=1; L0=taps;
         I4= ^fir_coeff; M4=1; L4=taps;
         call Filter;
         dm(i0_pnt) = i0;
         SR = ASHIFT AR BY -1 (HI);  /* divide by 2 */
         AR = SR1;
         RTS;

sFilterx2: I0= dm(i0_pnt);   M0=1; L0=taps;
           I4= ^fir_coeff; M4=1; L4=taps;
           call Filter;
           dm(i0_pnt) = i0;
           RTS;

Hilbert2: I0= dm(hilbert_pnt);   M0=1; L0=taps;
          I4= ^fir_coeff; M4=1; L4=taps;
          call Filter;
          dm(hilbert_pnt) = i0;
          SR = ASHIFT AR BY -1 (HI);  /* divide by 2 */
          AR = SR1;
          RTS;

Hilbert3: I0= dm(hilbert3_pnt);   M0=1; L0=taps;
          I4= ^fir_coeff; M4=1; L4=taps;
          call Filter;
          dm(hilbert3_pnt) = i0;
          SR = ASHIFT AR BY -1 (HI);  /* divide by 2 */
          AR = SR1;
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

Remove_Noise3_: call RegSave2;
                I2 = AR;
                I3 = AR;
                L2 = 0;
                L3 = 0;
                M2 = 1;
                DIS M_MODE;
                CNTR = 128;
                do Remove_Loop3 until CE;
                AR = DM(I2,M2);
                SR = ASHIFT AR BY -1 (HI);
                AX1 = SR1;
                call delayLine;
                AY0 = AR;
                AR = DM(I2,M2);
                SR = ASHIFT AR BY -1 (HI);
                AY1 = SR1;
                call sFilter;
                AR = AR+AY0;
                DM(I3,M2) = AR;
                AR = AX1+AY1;
                call delayLine2;
                AY0 = AR;
                AR = AX1-AY1;
                call Hilbert2;
                AR = AR-AY0;
Remove_Loop3:   DM(I3,M2) = AR;
                ENA M_MODE;
                call RegRestore2;
                rts;


Remove_Noise2_: call RegSave2;
                I2 = AR;
                I3 = AR;
                L2 = 0;
                L3 = 0;
                M2 = 1;
                CNTR = 128;
                DIS M_MODE;
                do Remove_Loop2 until CE;
                AR = DM(I2,M2);
                SR = ASHIFT AR BY -1 (HI);
                AX1 = SR1;
                AR = DM(I2,M2);
                SR = ASHIFT AR BY -1 (HI);
                AY1 = SR1;
                AR = AX1+AY1;
                call delayLine2;
                SR = ASHIFT AR BY -1 (HI);
                AX0 = SR1;
                AR = AX1-AY1;
       DM(I3,M2) = AR;
       AR = AX1+AY1;
/*                call Hilbert2;
                SR = ASHIFT AR BY -1 (HI);
                AY0 = SR1;
                AR = AX0+AY0;
                AY0 = AR;
                AR = AX1;
                call delayLine3;
                AR = AR-AY0;
                DM(I3,M2) = AR;
                AR = AY1;
                call delayLine4;
                AR = AR-AY0;
*/Remove_Loop2:   DM(I3,M2) = AR;
                ENA M_MODE;
                call RegRestore2;
                rts;

Remove_Noise_: call RegSave2;
               I2 = AR;
               I3 = AR;
               L2 = 0;
               L3 = 0;
               M2 = 1;
               DIS M_MODE;
               CNTR = 128;
               do Remove_Loop until CE;
               AR = DM(I2,M2);
               SR = ASHIFT AR BY -1 (HI);
               AX1 = SR1;
               AR = DM(I2,M2);
               SR = ASHIFT AR BY -1 (HI);
               AY1 = SR1;
               AR = AX1-AY1;
               call delayLine2;
               DM(I3,M2) = AR;
               AR = AY1;
               call Hilbert2;
               AY1 = AR;
               AR = AX1;
               call Hilbert3;
               AR = AR-AY1;
Remove_Loop:   DM(I3,M2) = AR;
               ENA M_MODE;
               Call RegRestore2;
               rts;

Hilbert_And_Delay_: call RegSave2;
                    I2 = AR;
                    I3 = AY1;
                    L2 = 0;
                    L3 = 0;
                    M2 = 1;
                    AR = DM(I2,M2);
                    CNTR = 128;
                    DIS M_MODE;
                    do T_and_D_loop until CE;
                    call sFilter;       /* Hilbert transform */
                    AY0 = AR;
                    AR = DM(I2,M2);
                    call delayLine;
                    AX0 = AR;
                    AR = AY0-AX0;
                    DM(I3,M2) = AR;
                    AR = AX0+AY0;
                    DM(I3,M2) = AR;
T_and_D_loop:       AR = DM(I2,M2);
                    ENA M_MODE;
                    call RegRestore2;
                    rts;

Xmit_LSB_H_And_D_: call RegSave2;
                   I2 = AR;
                   I3 = AY1;
                   L2 = 0;
                   L3 = 0;
                   M2 = 1;
                   AR = DM(I2,M2);
                   CNTR = 128;
                   DIS M_MODE;
                   do Xmit_LSB_loop until CE;
                   call sFilterx2;       /* Hilbert transform */
                   DM(I3,M2) = AR;
                   AR = DM(I2,M2);
                   call delayLinex2;
                   DM(I3,M2) = AR;
Xmit_LSB_loop:     AR = DM(I2,M2);
                   ENA M_MODE;
                   call RegRestore2;
                   rts;

Xmit_USB_H_And_D_: call RegSave2;
                   I2 = AR;
                   I3 = AY1;
                   L2 = 0;
                   L3 = 0;
                   M2 = 1;
                   AR = DM(I2,M2);
                   CNTR = 128;
                   DIS M_MODE;
                   do Xmit_USB_loop until CE;
                   call delayLinex2;
                   DM(I3,M2) = AR;
                   AR = DM(I2,M2);
                   call sFilterx2;       /* Hilbert transform */
                   DM(I3,M2) = AR;
Xmit_USB_loop:     AR = DM(I2,M2);
                   ENA M_MODE;
                   call RegRestore2;
                   rts;

Hilbert_And_Delay2_: call RegSave2;
                     I2 = AR;
                     I3 = AY1;
                     L2 = 0;
                     L3 = 0;
                     M2 = 1;
                     M3 = 0;
                     AR = DM(I2,M3);
                     CNTR = 128;
                     DIS M_MODE;
                     do T_and_D_loop2 until CE;
                     call sFilter;
/*                     SR = ASHIFT AR by -1 (HO);
                     AY0 = SR1;
*/                   AY0 = AR;
                     AR = DM(I2,M2);
                     call delayLine2;
/*                     SR = ASHIFT AR by -1 (HI);
                     AY1 = SR1;
*/                   AY1 = AR;
                     AR = DM(I2,M3);
                     call delayLine;
/*                     SR = ASHIFT AR by -1 (HI);
                     AX0 = SR1;
*/                   AX0 = AR;
                     AR = DM(I2,M2);
                     call Hilbert2;
/*                     SR = ASHIFT AR by -1 (HI);
                     AX1 = SR1;
*/                   AX1 = AR;
                     AR = AY0-AX0;
                     DM(I3,M2) = AR;
                     AR = AX1+AY1;
                     DM(I3,M2) = AR;
T_and_D_loop2:       AR = DM(I2,M3);
                     ENA M_MODE;
                     call RegRestore2;
                     rts;

Hilbert_And_Delay3_: call RegSave2;
                     I2 = AR;
                     I3 = AY1;
                     L2 = 0;
                     L3 = 0;
                     M2 = 1;
                     M3 = 0;
                     AR = DM(I2,M3);
                     CNTR = 128;
                     DIS M_MODE;
                     do T_and_D_loop3 until CE;
                     call sFilter;
/*                     SR = ASHIFT AR by -1 (HO);
                     AY0 = SR1;
*/                   AY0 = AR;
                     AR = DM(I2,M2);
                     call delayLine2;
                     DIS M_MODE;
/*                     SR = ASHIFT AR by -1 (HI);
                     AY1 = SR1;
*/                   AY1 = AR;
                     AR = DM(I2,M3);
                     DIS M_MODE;
                     call delayLine;
/*                     SR = ASHIFT AR by -1 (HI);
                     AX0 = SR1;
*/                   AX0 = AR;
                     AR = DM(I2,M2);
                     call Hilbert2;
/*                     SR = ASHIFT AR by -1 (HI);
                     AX1 = SR1;
*/                   AX1 = AR;
                     AR = AY0-AX0;
                     call delayLine3;
                     DM(I3,M2) = AR;
                     AR = AX1+AY1;
                     call Hilbert3;
                     DM(I3,M2) = AR;
T_and_D_loop3:       AR = DM(I2,M3);
                     ENA M_MODE;
                     call RegRestore2;
                     rts;

Filter_Both_And_Copy_:   call RegSave2;
                         DIS M_MODE;
                         I2 = AR;
                         M2 = 0;
                         M3 = 1;
                         L2 = 0;
                         CNTR = 128;
                         AR = DM(I2,M2);
                         do First_Loop2 until CE;
                         call Filter_FM;
                         call Filter_60_Hz_USB;
                         DM(I2,M3) = AR;
                         DM(I2,M3) = AR;
First_Loop2:              AR = DM(I2,M2);
                          ENA M_MODE;
                         call RegRestore2;
                         rts;

Filter_FM: /*call RegSave;
           */I0= dm(usb_pnt);   M0=1; L0=41;
           I4= ^coeff_fm16;    M4=1; L4=41;
           CNTR = 40; /* 41-1 */
           call fir_cnt;
           dm(usb_pnt) = i0;
          /* call RegRestore;
          */ RTS;

/*
Filter_Both_:    call RegSave2;
                 I2 = AR;
                 M2 = 1;
                 M3 = 0;
                 L2 = 0;
                 CNTR = 128;
                 AR = DM(I2,M3);
                 DIS M_MODE;
                 do Both_Loop until CE;
                 call UFilter_;
                 DM(I2,M2) = AR;
                 AR = DM(I2,M3);
                 call LFilter_;
                 DM(I2,M3) = AR;
Both_Loop:       AR = DM(I2,M3);
                 ENA M_MODE;
                 call RegRestore2;
                 rts;
*/

Filter_Both_:    call RegSave2;
                 I0 = AR;
                 M0 = 1;
                 M1 = 0;
                 L0 = 0;
                 CNTR = 128;
                 AR = DM(I0,M1);
                 do Both_Loop until CE;
                 call UFilter_;
                 DM(I0,M0) = AR;
                 AR = DM(I0,M1);
                 call LFilter_;
                 DM(I0,M0) = AR;
Both_Loop:       AR = DM(I0,M1);
                 call RegRestore2;
                 rts;

Highpass_Both_:  call RegSave2;
                 I2 = AR;
                 M2 = 1;
                 M1 = 0;
                 M0 = 1;
                 M4 = 1;
                 L4 = 121; /*taps;*/
                 L2 = 0;
                 L0 = 121; /*taps;*/
                 CNTR = 128;
                 AR = DM(I2,M1);
                 DIS M_MODE;
                 do Highpass_Loop until CE;
                 AX0 = CNTR;
                 I4= ^coeff_highpass; /*   M4=1; L4=taps;*/
                 I0= dm(usb_pnt);   /* M0=1; L0=taps;*/
                 CNTR = 121;
/*                 call Filter;
*/               call fir_cnt;
                 dm(usb_pnt) = I0;
                 DM(I2,M2) = AR;
                 AR = DM(I2,M1);
                 I4= ^coeff_highpass;    /*M4=1; L4=taps;*/
                 I0= dm(lsb_pnt);   /*M0=1; L0=taps;*/
                 CNTR = 121;
/*                 call Filter;
*/               call fir_cnt;
                 dm(lsb_pnt) = I0;
                 DM(I2,M2) = AR;
                 CNTR = AX0;
Highpass_Loop:   AR = DM(I2,M1);
                 ENA M_MODE;
                 call RegRestore2;
                 rts;
Filter_First_:   call RegSave2;
                 I2 = AR;
                 M2 = 2;
                 M3 = 0;
                 L2 = 0;
                 CNTR = 128;
                 AR = DM(I2,M3);
                 DIS M_MODE;
                 do First_Loop until CE;
                 call UFilter_;
                 DM(I2,M2) = AR;
First_Loop:      AR = DM(I2,M3);
                 ENA M_MODE;
                 call RegRestore2;
                 rts;

Remove_USB_:     call RegSave;
                 I0 = AR;
                 I1 = AR;
                 M0 = 1;
                 M1 = 2;
                 L0 = 0;
                 L1 = 0;
                 CNTR = 128;
                 AR = DM(I1,M0);    /* increment I1 */
                 AR = DM(I1,M1);
                 do Remove_USB_Loop until CE;
                 DM(I0,M1) = AR;
Remove_USB_Loop: AR = DM(I1,M1);
                 call RegRestore;
                 rts; 

Remove_LSB_:     call RegSave;
                 I0 = AR;
                 M0 = 1;
                 L0 = 0;
                 CNTR = 128;
                 AR = DM(I0,M0);
                 do Remove_LSB_Loop until CE;
                 DM(I0,M0) = AR;
Remove_LSB_Loop: AR = DM(I0,M0);
                 call RegRestore;
                 rts;

Make_SSB_:       call RegSave;     /* this subroutine extracts A Sideband from hilbert & delay */
                                   /* call it with hilbert&delay +1 to extract LSB */
                 I0 = AR;
                 I1 = AY1;
                 M0 = 2;
                 M1 = 1;
                 L0 = 0;
                 L1 = 0;
                 CNTR = 128;
                 AR = DM(I0,M0);
                 do Make_SSB_Loop until CE;
                 DM(I1,M1) = AR;
Make_SSB_Loop:   AR = DM(I0,M0);
                 call RegRestore;
                 rts;

CW_Block_:     call RegSave;
               DIS M_MODE;
               MY1 = 512;   /* 512 * 128 = 65536*/
               I0 = AY1;
               I1 = AR;
               AX0 = AR;
               L0 = 0;
               L1 = 0;
               M0 = 1;
               M1 = 0;
               MR = 0, MX1 = DM(I0,M0);
               CNTR = 128;
               do CW_Block_USB UNTIL CE;
CW_Block_USB:  MR=MR+MX1*MY1(SS),MX1 = DM(I0,M0);
               SR0 = MR1;  /* result / 65536 */
               I0 = AR;
               CNTR = 128;
               AX1 = 150;
               AY1 = dm(cw_detected);
               AX0 = dm(cw_time);
               do CW_Block_loop UNTIL CE;
               AY0 = 1;
               AR = AX0+AY0;
               AX0 = AR;
               AY0 = DM(I0,M1);
               AR = AY0-SR0;
               AF = ABS AR;
               AR = 0;
               AF = AF-AX1;
               if not AC jump CW_no_tone;
               AR = 1000;
CW_no_tone:    CALL cw_fir;
               DM(I0,M0) = AR;
               AF = AY1-AR;
               if eq jump CW_Block_loop;
               DM(I1,M0) = AY1;
               DM(I1,M0) = AX0;
               AX0 = 0;                  /* cw_time is in AX0*/
               AY1 = AR;
CW_Block_loop: nop;
               dm(cw_time) = AY1;
               dm(cw_detected) = AX0;
               I0 = AX0;
               AR = 0;
               dm(I1,M0) = AR;
               AX0 = 1000;
               DM(I0,M0) = AR;
               DM(I0,M0) = AX0;
               DM(I0,M0) = AR;
               DM(I0,M0) = AX0;
               ENA M_MODE;
               call RegRestore;
               rts;

           /* CW_FIR requires M_Mode disabled */
cw_fir:      I2 = dm(cw_pnt); M2 = 1; L2 = cw_taps;
             dm(I2,M2) = AR;
             dm(cw_pnt) = I2;
             cntr = cw_taps;
             MY0 = 1;
             MR0 = 0;
             mx0 = dm(I2,M2);
             do cw_fir_loop until CE;
cw_fir_loop: MR = MR+MX0*MY0(SS),mx0 = dm(I2,M2);
             AR = PASS MR0;
             if eq jump end_cw_fir;
             AR = 1000;
end_cw_fir:  RTS;

RTTY_Block_: call RegSave;
             MY1 = 512;   /* 512 * 128 = 65536*/
             I0 = AR;
             L0 = 0;
             M0 = 1;
             M1 = 0;
             MR = 0, MX1 = DM(I0,M0);
             CNTR = 128;
             do Block_USB UNTIL CE;
Block_USB:   MR=MR+MX1*MY1(SS),MX1 = DM(I0,M0);
             SR0 = MR1;  /* result / 65536 */
             I0 = AY1;
             CNTR = 128;
             MR = 0, MX1 = DM(I0,M0);
             do Block_LSB UNTIL CE;
Block_LSB:   MR=MR+MX1*MY1(SS),MX1 = DM(I0,M0);
             SR1 = MR1;
             CNTR = 128;
             I0 = AR;
             I1 = AY1;
             L1 = 0;
             do Block_loop UNTIL CE;
             AY0 = DM(I0,M1);
             AX0 = 0;
             AR = AY0-SR0, AY1 = DM(I1,M1);
             DM(I1,M0) = AR;
             AF = ABS AR;
             AR = AY1-SR1;
             AR = ABS AR;
             AR = AF-AR;
             if AC jump Block_UL;
             if EQ jump Block_none;
             AX0 = 100;
             jump Block_none;
Block_UL:    AX0 = -100;
Block_none:  AR = AX0;
             call rtty_fir;
Block_loop:  DM(I0,M0) = AR;
             call RegRestore;
             rts;

RTTY_:    call RegSave;
/*          call UFilter_;   this UFilter has been changed to not save registers
          AX1 = AR;
          AR = AY1;
          call LFilter_;
          AY1 = AR;
          AR = AX1;
*/          AY0 = dm(rtty_usb);
          AX1 = dm(rtty_lsb);
          dm(rtty_usb) = AR;
          dm(rtty_lsb) = AY1;
          AR = AR-AY0;
          AF = ABS AR;
          AR = AY1-AX1;
          AR = ABS AR;
          AR = AR - AF;
          if ac jump lsb_rtty;
          if eq jump no_rtty;
          AR = 100;

rtty_cfir: call rtty_fir;
          call RegRestore;
          rts;

lsb_rtty: AR = -100;
          jump rtty_cfir;
no_rtty:  AR = 0;
          jump rtty_cfir;

rtty_fir: DIS M_MODE;
          I2= dm(rtty_pnt);   M2=1; L2=rtty_taps;
          DM(I2,M2)= ar;              {store sample in data buffer }
          CNTR=rtty_taps;
          MY0 = 1;
          MR=0, MX0=DM(I2,M2);
          DO rtty_loop UNTIL CE;
rtty_loop: MR=MR+MX0*MY0(SS), MX0=DM(I2,M2);
          ar = mr0;
          dm(rtty_pnt) = I2;

          ENA M_MODE;
          AR = PASS AR; 
          if pos jump max_hi;
          if neg jump max_low;
rtty_end: rts;

max_hi:   AR = rtty_max;
          jump rtty_end;

max_low:  AR = rtty_min;
          jump rtty_end;


.ENDMOD;


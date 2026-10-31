/*   Volume.dsp  */
/*   Version 0.1 slow AGC prototype */
/*   Version 0.2 faster AGC prototype 6/30/94 */

.MODULE/RAM        VOLUMEMOD;

#define PIO_I2C_CLOCK_HIGH 0x0202
#define PIO_I2C_CLOCK_LOW 0xfffd
#define PIO_I2C_DATA_HIGH 0x0101
#define PIO_I2C_DATA_LOW 0xfffe
#define MICROSECOND_DELAY_COUNT 16
#define I2CDELAY 40
#define BUFFER_LENGTH_SLOW_AGC 4

.VAR/DM/RAM true_volume;
.VAR/DM/RAM volume;
.VAR/DM/RAM reg_l0;
.VAR/DM/RAM largest;
.VAR/DM/RAM error;
.VAR/DM/RAM PIO_reg_data;
.VAR/DM/RAM OFFSET;
.VAR/DM/RAM wanted_volume;
.VAR/CIRC slow_agc_values[BUFFER_LENGTH_SLOW_AGC];
.VAR/DM/RAM slow_agc_pnt;

.external PIO_;
.external RegSave;
.external RegRestore;

.ENTRY ChangeVolume_;
.ENTRY RestoreVolume_;
.ENTRY GetVolume_;
.ENTRY GetError_;
.ENTRY ClearError_;
.ENTRY ClearOffset_;
.ENTRY MultiplyByVolume_;
.ENTRY AGC_;
.ENTRY NewAttn_;
.ENTRY SetTone_;
.ENTRY SlowAGC_;
.ENTRY InitAGC_;
.ENTRY SSBAGC_;
.ENTRY FreqCal_;

/* New Attn returns new value in AR for Attenuators in 1848 based on AGC Volume */
/* NewAttn_ has never been tested due to noise from 1848 change! */

InitAGC_:      call RegSave;
               I0 = ^slow_agc_values; M0 = 1; l0 = BUFFER_LENGTH_SLOW_AGC;
               CNTR = BUFFER_LENGTH_SLOW_AGC;
               AR = 0;
               do initialize until CE;
initialize:    DM(I0,M0) = AR;
               dm(slow_agc_pnt) = I0;
               call RegSave;
               rts;

NewAttn_:      call RegSave;
               AX0 = dm(volume);
               MR0 = AR;
               AY1 = H#00F0;
               AR = MR0 AND AY1;
               MX1 = AR;
               AY1 = H#000F;
               AR = MR0 AND AY1;
               AY1 = H#100;
               AY0 = 1;
               AF = AX0-AY1;
               if AC jump decrease_attn;
               AY1= H#1000;
               AF = AX0-AY1;
               if AC jump return;
               AR=AR+AY0;
               AY1 = 10;
               AF = AR-AY1;
               if AC jump return;
               AR=AR-AY0;
               jump return;
decrease_attn: AR=AR-AY0;
               if NE jump return;
               AR=AR+AY0;
return:        AY0 = MX1;
               AR = AR+AY0;
               call RegRestore;
               rts;

ChangeVolume_: dm(volume) = AR;
               dm(true_volume) = AR;
               dm(wanted_volume) = AR;
               ar = 0;
               dm(OFFSET) = AR;
               RTS;

RestoreVolume_: AR = 0;
                dm(OFFSET) = AR;
                AR = dm(true_volume);
                dm(volume) = AR;
                dm(wanted_volume) = AR;
                rts;

GetVolume_: AR = dm(largest);
            RTS;

ClearOffset_: AR = 0;
              dm(OFFSET) = AR;
              RTS;

ClearError_: AR = 0;
             dm(error) = AR;
             RTS;

GetError_: AR = dm(error);
           rts;

MultiplyByVolume_: call RegSave;
                   MY1=dm(volume);
                   AY0= DM(OFFSET);
                   AR = AR-AY0;
                   MR=AR*MY1(SS);
                   SR=LSHIFT MR0 BY -10 (LO);
                   SR=SR OR ASHIFT MR1 BY -10 (HI);
                   AF=PASS SR1;
                   IF GT JUMP MaxSine;
                   AF = AF+1;
                   IF LT JUMP MinSine;
                   AR=SR0;
mod_volume:        AX0 = AR;
             /*      AX1 = dm(volume);
                   AY1 = dm(wanted_volume);
                   AR = AX1 - AY1;
                   if eq jump end_volume;
                   if ac jump inc_volume;
                   AY0 = 250;
                   AR = AR-AY0;
                   if ac jump eq_volume;
                   AR = AX1-AY0;
                   dm(volume) = AR;
                   jump end_volume;
eq_volume:         AR = dm(wanted_volume);
                   dm(volume) = AR;
                   jump end_volume;
inc_volume:    jump eq_volume;    AY0 = 250;
                   AR = AR+AY0;
                   if ac jump eq_volume;
                   AR = AX1+AY0;
                   dm(volume) = AR;
*/end_volume:        AR = AX0;
                   call RegRestore;
                   RTS;
MinSine:           AR=H#8000;
                   dm(error)=AR;
                   jump mod_volume;
MaxSine:           AR=H#7FFF;
                   dm(error)=AR;
                   jump mod_volume;

SlowAGC_:   call RegSave;      /* slow agc will keep volume at its lowest*/
            call AGC_;
            I0 = dm(slow_agc_pnt);M0 = 1;L0 = BUFFER_LENGTH_SLOW_AGC;
            AY0 = dm(wanted_volume);
            dm(I0,M0) = AY0;
            dm(slow_agc_pnt) = I0;
            CNTR = BUFFER_LENGTH_SLOW_AGC;
            do slow_agc until CE;
            AX0 = dm(I0,M0);
            AR = AX0-AY0;
            if ac jump slow_agc;
            AY0 = AX0;
slow_agc:   NOP;
            dm(wanted_volume) = AY0;
            dm(volume) = AY0;
            call RegRestore;
            rts;

SSBAGC_:    call RegSave;
            i0 = AR;
            l0 = 0;
            m0 = 1;
            CNTR = 128;
            AY0 = 0;
            AY1 = H#FFFF;
            AX0 = dm(i0,m0);
            do ssb_loop UNTIL CE;
            AX1 = H#8000;    /* make numbers run from 0x0000 to 0xffff unsigned */
            AF = PASS AX1;
            AR = AX0+AF;
            AF = AR - AY0;
            if NOT AC jump ssb_not_hi;
            AY0 = AR;
ssb_not_hi: AF = AR-AY1;
            if AC jump ssb_loop;
            AY1 = AR;
ssb_loop:   AX0 = dm(i0,m0);
            jump AGC_loop;
            
AGC_:       call RegSave;
            i0 = AR;
            l0 = 0;
            m0 = 2;
            CNTR = 128; /*was 128, only look at one sideband do to DC shifts */
            AY0 = 0;
            AY1 = H#FFFF;
            AX0 = dm(i0,m0);
            do AGC_loop UNTIL CE;
            AX1 = H#8000;    /* make numbers run from 0x0000 to 0xffff unsigned */
            AF = PASS AX1;
            AR = AX0+AF;
            AF = AR - AY0;
            if NOT AC jump not_high;
            AY0 = AR;
not_high:   AF = AR-AY1;
            if AC jump AGC_loop;
            AY1 = AR;
AGC_loop:   AX0 = dm(i0,m0);
            AX0 = AY0;
            AR = AX0-AY1;
            dm(largest) = AR;
            AX0 = AR;
            SR = LSHIFT AR BY -1 (LO);
            AF = SR0+AY1;
            AX1 = H#8000;
            AR = AX1+AF; 
            DM(OFFSET) = AR;             
            AY0 = H#F000;
            AF = AY0-AX0;
            if ac jump not_hi;
            AX0 = H#F000;
not_hi:     AY0 = H#1000;
            AF = AY0-AX0;
            if not ac jump no_squelch;
            AX0 = H#1000;
no_squelch:
            SI = dm(true_volume);
            SR = LSHIFT SI BY 13 (LO);
            AF = PASS SR1;
            AY0 = SR0;
/*          AY1 = H#0280;
            AF = pass AY1;
            AY0 = H#0000;
*/            CALL divide;
            dm(wanted_volume) = AY0;
/*            AX0 = dm(volume);
            AR = AX0 - AY0;
            if ac jump restore;
*/            dm(volume) = AY0;   /* remove volume for ramping */
restore:    call RegRestore;
            rts;

divide:     ASTAT = 0;
            DIVQ AX0;
            DIVQ AX0; DIVQ AX0; DIVQ AX0;
            DIVQ AX0; DIVQ AX0; DIVQ AX0;
            DIVQ AX0; DIVQ AX0; DIVQ AX0;
            DIVQ AX0; DIVQ AX0; DIVQ AX0;
            DIVQ AX0; DIVQ AX0; DIVQ AX0;
            RTS;

FreqCal_:   call RegSave;
            i0 = AR;
            l0 = 0;
            m0 = 1;
            CNTR = 128; /*was 128, only look at one sideband do to DC shifts */
            MR = 0;
            do FreqLoop UNTIL CE;
            MX0 = DM(I0,M0);
            MY0 = DM(I0,M0);
FreqLoop:   MR = MR+MX0*MY0 (SS);
            AR = MR0;
            call RegRestore;
            rts;

SetTone_: rts;  call RegSave;
            AX0 = 0xC0;
            AR = PASS AR;
            if EQ jump SetZero;
            AX0 = 0x00FF;
SetZero:    call set_MASTER;
            AY0 = 0x01C0;
            AR = AX0+AY0;
            AX0 = AY0;
            call set_MASTER;
            call RegRestore;
/*            ax0 = 0xFF00;
            ay0 = 0xFFFF;
TestTone:   dm(PIO_) = ax0;

            dm(PIO_) = ay0;
            jump TestTone;
*/            rts;
  
set_MASTER:
    call start_I2C;

    call send_I2C_one;      {send the module address}
    call send_I2C_zero;
    call send_I2C_zero;
    call send_I2C_zero;
    call send_I2C_zero;
    call send_I2C_zero;
    call send_I2C_one;
    call send_I2C_zero;     { send the write command }
    call send_I2C_zero;     { for FPGA send zero for acknowledge }

    ay0=0x8000;             { send the sub address }
    call send_I2C_bit;
    ay0=0x4000;
    call send_I2C_bit;
    ay0=0x2000;
    call send_I2C_bit;
    ay0=0x1000;
    call send_I2C_bit;
    ay0=0x0800;
    call send_I2C_bit;
    ay0=0x0400;
    call send_I2C_bit;
    ay0=0x0200;
    call send_I2C_bit;
    ay0=0x0100;
    call send_I2C_bit;
    call send_I2C_zero;     { for FPGA send zero for acknowledge }

    ay0=0x0080;             { send the data byte }
    call send_I2C_bit;
    ay0=0x0040;
    call send_I2C_bit;
    ay0=0x0020;
    call send_I2C_bit;
    ay0=0x0010;
    call send_I2C_bit;
    ay0=0x0008;
    call send_I2C_bit;
    ay0=0x0004;
    call send_I2C_bit;
    ay0=0x0002;
    call send_I2C_bit;
    ay0=0x0001;
    call send_I2C_bit;
    call send_I2C_zero;     { for FPGA send zero for acknowledge }

    call stop_I2C;
    rts;

start_I2C:              { send I2C start command }
    call data_high_I2C;
    call delay_I2C;
    call clock_high_I2C;
    call delay_I2C;
    call delay_I2C;
    call data_low_I2C;
    call delay_I2C;
    call delay_I2C;
    call clock_low_I2C;
    call delay_I2C;
    call data_high_I2C;
    rts;

send_I2C_bit:
    ar=ax0 and ay0;
    if ne jump send_I2C_one;
    if eq jump send_I2C_zero;

send_I2C_one:           { send I2C "one" data bit }
    call data_high_I2C;
    call delay_I2C;
    call clock_high_I2C;
    call delay_I2C;
    call delay_I2C;
    call clock_low_I2C;
    call delay_I2C;
    rts;

send_I2C_zero:          { send I2C "zero" data bit }
    call data_low_I2C;
    call delay_I2C;
    call clock_high_I2C;
    call delay_I2C;
    call delay_I2C;
    call clock_low_I2C;
    call delay_I2C;
    rts;

stop_I2C:               { send I2C stop command }
    call data_low_I2C;
    call delay_I2C;
    call clock_high_I2C;
    call delay_I2C;
    call delay_I2C;
    call data_high_I2C;
    call delay_I2C;
    rts;

clock_high_I2C:         { set I2C clock high }
    ax1=dm(PIO_reg_data);
    ay1=PIO_I2C_CLOCK_HIGH;
    ar=ax1 or ay1;
    dm(PIO_reg_data)=ar;
    dm(PIO_)=ar;
    rts;

clock_low_I2C:          { set I2C clock low }
    ax1=dm(PIO_reg_data);
    ay1=PIO_I2C_CLOCK_LOW;
    ar=ax1 and ay1;
    dm(PIO_)=ar;
    dm(PIO_reg_data)=ar;
    rts;

data_high_I2C:          { set I2C data high }
    ax1=dm(PIO_reg_data);
    ay1=PIO_I2C_DATA_HIGH;
    ar=ax1 or ay1;
    dm(PIO_reg_data)=ar;
    dm(PIO_)=ar;
    rts;

data_low_I2C:           { set I2C data low }
    ax1=dm(PIO_reg_data);
    ay1=PIO_I2C_DATA_LOW;
    ar=ax1 and ay1;
    dm(PIO_reg_data)=ar;
    dm(PIO_)=ar;
    rts;

delay_I2C:              { delay for 2.5 uSec }
    CNTR = I2CDELAY;
    DO I2C_delay UNTIL CE;
I2C_delay:
    nop;
    rts;
        {This routine delays N microseconds where N is passed in ax0}
delay_us_:
    cntr = ax0;
    do dw2 until CE;
    cntr = MICROSECOND_DELAY_COUNT;
    do dwait until CE;
dwait:
    nop;
dw2:
    nop;
    rts;

.ENDMOD;
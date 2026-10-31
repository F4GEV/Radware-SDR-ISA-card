/*   Volume.dsp  */
/*   Version 0.1 slow AGC prototype */
/*   Version 0.2 faster AGC prototype 6/30/94 */

.MODULE/RAM        VOLUMEMOD;

#define PIO_I2C_CLOCK_HIGH 0x0202
#define PIO_I2C_CLOCK_LOW 0xfffd
#define PIO_I2C_DATA_HIGH 0x0101
#define PIO_I2C_DATA_LOW 0xfffe
#define MICROSECOND_DELAY_COUNT 20
  /*  was 16 */
#define I2CDELAY 160
  /*  was 40 */
#define I2CDELAY2 320
#define BUFFER_LENGTH_SLOW_AGC 64

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

.VAR/DM/RAM/ABS=0x31c8 PIO_;
.external RegSave;
.external RegRestore;
.external UFilter_;
.external LFilter_;
.external FM_Filter_;

.ENTRY ChangeVolume_;
.ENTRY RestoreVolume_;
.ENTRY GetVolume_;
.ENTRY GetError_;
.ENTRY ClearError_;
.ENTRY ClearOffset_;
.ENTRY MultiplyByVolume_;
.ENTRY MultiplyAllByVolume_;
.ENTRY AGC_;

.ENTRY SetTone_;
.ENTRY SlowAGC_;
.ENTRY InitAGC_;
.ENTRY SSBAGC_;
.ENTRY FreqCal_;
.ENTRY ChangeIFGain_;
.ENTRY ChangeIFGain2_;
.ENTRY Multiply_;
.ENTRY FreqCal2_;
.ENTRY FreqCal3_;
.ENTRY FreqCal4_;
.ENTRY Pythagoras_;
.ENTRY Center_;
.ENTRY FM_;
.ENTRY CheckAnalog_;

ChangeIFGain2_: call RegSave;
                SR0 = IMASK;
                IMASK = H#00;
                SR1 = dm(H#3FFE);
                AX0 = H#7FFF;
                dm(H#3FFE) = AX0;
                MR0 = 0;
                MR1 = H#0100;
                DM(h#3440) = MR0;
                DM(h#3448) = AR;
                DM(h#3440) = MR1;
                DM(h#3448) = AR;
                IMASK = SR0;
                dm(H#3FFE) = SR1;
                call RegRestore;
                rts;


/*ChangeIFGain2_: call RegSave;
               I0 = 0x3440;
               I1 = 0x3448;
               M0 = 0;
               AY1 = 0;
               call Write_1848;
               call Write_1848;
               call RegRestore;
               rts;

*/ChangeIFGain_: call RegSave;
               SR0 = IMASK;
               IMASK = H#00;
/*               SR1 = DM(H#3FFE);
               MR0 = 5;
               DM(H#3FFE) = MR0;
*/               I0 = 0x3440;
               I1 = 0x3448;
               M0 = 0;
               AY1 = 0;
               call Write_1848; /* write AR into Left Input Control */
               call Write_1848; /* write AR into Right Input Control */
               AR = 0x9F9F;
               call Write_1848; /* write 9f into Left Aux #1 */
               call Write_1848; /* write 9f into Right Aux #1 */
               call Write_1848; /* write 9f into Left Aux #2 */
               call Write_1848; /* write 9f into Right Aux #2 */
               IMASK = SR0;
/*               DM(H#3FFE) = SR1;
*/               call RegRestore;
               rts;

Write_1848:    AY0 = AR;
               DM(I0,M0) = AY1;
               nop;
               AX0 = 0x0100;
               AR = AX0+AY1;
               AY1 = ar;
               AR = AY0;
               DM(I1,M0) = AR;
               nop;
               rts;

InitAGC_:      call RegSave;
               I0 = ^slow_agc_values; M0 = 1; l0 = BUFFER_LENGTH_SLOW_AGC;
               CNTR = BUFFER_LENGTH_SLOW_AGC;
               AR = 0xFFFF;
               do initialize until CE;
initialize:    DM(I0,M0) = AR;
               dm(slow_agc_pnt) = I0;
               call RegRestore;
               rts;

CheckAnalog_:  call RegSave;
               AX0 = dm(largest);
               AY0 = H#1000;
               AR = AX0-AY0;
               if ac jump check_up;
               AY0 = H#4000;
               AR = AX0+AY0;
               if ac jump check_down;
               AR = PASS 0;
check_end:     call RegRestore;
               rts;
check_down:    AR = -1;
               jump check_end;
check_up:      AR = 1;
               jump check_end;

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

MultiplyAllByVolume_ : call RegSave;
                       I0 = AR;
                       M0 = 1;
                       M1 = 0;
                       L0 = 0;
                       MY1 = dm(volume);
                       AY0 = DM(OFFSET);
                       CNTR = 256;
                       AR = DM(I0,M1);
                       do MultiplyAllLoop until CE;
                       AR = AR-AY0;
                       MR=AR*MY1(SS);
                       SR=LSHIFT MR0 BY -10 (LO);
                       SR=SR OR ASHIFT MR1 BY -10 (HI);
                       AF=PASS SR1;
                       IF GT JUMP MaxSineAll;
                       AF = AF+1;
                       IF LT JUMP MinSineAll;
                       AR=SR0;
ModVolumeAll:          DM(I0,M0) = AR;
MultiplyAllLoop:       AR = DM(I0,M1);
                       call RegRestore;
                       rts;

MinSineAll:        AR=H#8000;
                   dm(error)=AR;
                   jump ModVolumeAll;
MaxSineAll:        AR=H#7FFF;
                   dm(error)=AR;
                   jump ModVolumeAll;

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
/*            call AGC_;
*/            I0 = dm(slow_agc_pnt);M0 = 1;L0 = BUFFER_LENGTH_SLOW_AGC;
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
            m0 = 2;
            CNTR = 128;
            MR0 = 0;
            MR1 = H#FFFF;
            AX0 = dm(i0,m0);
            AY1 = H#8000;    /* make numbers run from 0x0000 to 0xffff unsigned */
            do ssb_loop UNTIL CE;
            AF = AX0+AY1;
            AR = AF - MR0;
            if NOT AC jump ssb_not_hi;
            AR = PASS AF;
            MR0 = AR;
ssb_not_hi: AR = AF-MR1;
            if AC jump ssb_loop;
            AR = PASS AF;
            MR1 = AR;
ssb_loop:   AX0 = dm(i0,m0);
            AY0 = MR0;
            AR = AY0-MR1;
            dm(largest) = AR;
            AY1 = MR1;
            jump AF_BIG;   /* enter AGC loop at end of largest detection cycle */

AGC_:       call RegSave;
            i0 = AR;
            l0 = 0;
            m0 = 1;
            CNTR = 128; /*was 128, only look at one sideband due to DC shifts */
            MR0 = 0;
            MR1 = H#FFFF;
            SR0 = 0;
            SR1 = H#FFFF;
            AX0 = dm(i0,m0);
            AY1 = H#8000;    /* make numbers run from 0x0000 to 0xffff unsigned */
            do AGC_loop2 UNTIL CE;
            AF = AX0+AY1;
            AR = AF - MR0;
            if NOT AC jump not_high;
            AR = PASS AF;
            MR0 = AR;
not_high:   AR = AF-MR1;
            if AC jump AGC_loop;
            AR = PASS AF;
            MR1 = AR;
AGC_loop:   AX0 = dm(i0,m0);
            AF = AX0+AY1;
            AR = AF - SR0;
            if NOT AC jump not_high2;
            AR = PASS AF;
            SR0 = AR;
not_high2:  AR = AF-SR1;
            if AC jump AGC_loop2;
            AR = PASS AF;
            SR1 = AR;
AGC_loop2:  AX0 = dm(i0,m0);
            AY0 = SR0;
            AF = AY0-SR1;
            AY0 = MR0;
            AR = AY0-MR1;
            AX0 = AR;
            AR = AF-AR;
            if NOT AC jump AR_BIG;
            AR = PASS AF;
            dm(largest) = AR;
            AY1 = SR1;
            jump AF_BIG;
AR_BIG:     AR = AX0;
            dm(largest) = AR;
            AY1 = MR1;
AF_BIG:     AX0 = AR;
            SR = LSHIFT AR BY -1 (LO);
            AF = SR0+AY1;
            AX1 = H#8000;
            AR = AX1+AF;
            DM(OFFSET) = AR;
            AY0 = H#E800;
            AF = AY0-AX0;
            if ac jump not_hi;
            AX0 = H#E800;
not_hi:     AY0 = H#0800;
            AF = AY0-AX0;
            if not ac jump no_squelch;
            AX0 = H#0800;
no_squelch: SI = AX0;
            SR = LSHIFT SI BY -1 (LO);
            AX0 = SR0;
            SI = dm(true_volume);
            SR = LSHIFT SI BY 10 (LO);
            AF = PASS SR1;
            AY0 = SR0;
            CALL divide;
            dm(wanted_volume) = AY0;
            dm(volume) = AY0;   /* remove volume for ramping */
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

sdivide:    DIVS AF,AX0;
            DIVQ AX0; DIVQ AX0; DIVQ AX0;
            DIVQ AX0; DIVQ AX0; DIVQ AX0;
            DIVQ AX0; DIVQ AX0; DIVQ AX0;
            DIVQ AX0; DIVQ AX0; DIVQ AX0;
            DIVQ AX0; DIVQ AX0; DIVQ AX0;
            RTS;

Pythagoras_: call RegSave;
             MX0 = AR;
             MY0 = AR;
             MR = MX0*MY0 (SS);
             MX0 = AY1;
             MY0 = AY1;
             MR = MR+MX0*MY0 (SS);
             call sqrt;
/*             AR = AY1;
             if lt jump Pythagoras2;
*/             AR = SR1;
             call RegRestore;
             rts;
Pythagoras2: AY0 = 0;
             AR = AY0 - SR1;
             call RegRestore;
             rts;

Multiply_:  call RegSave;
            MX0 = AR;
            MY0 = AY1;
            MR = MX0*MY0(SS);
            SR = LSHIFT MR0 BY 8 (LO);
            SR = SR OR LSHIFT MR1 BY 8 (HI);
            AR = SR1;
            call RegRestore;
            rts;

FreqCal_:   call RegSave;   /* Costas Loop */
            i0 = AR;
            l0 = 0;
            m0 = 1;
            CNTR = 128;
            MR = 0;
            MY0 = DM(I0,M0);
            do FreqLoop UNTIL CE;
            MX0 = DM(I0,M0);
FreqLoop:   MR = MR+MX0*MY0(SS), MY0 = DM(I0,M0);
            AR = MR1;
            call RegRestore;
            rts;

/*FreqCal4_:  call RegSave;
            i0 = AR;
            l0 = 0;
            m0 = 1;
            i1 = AY1;
            l1 = 0;
            AX0 = 10;     /*Set AX0 to an absurd quadrant */
/*            CNTR = 128;
            AY1 = 0;
            do FreqLoop4 UNTIL CE;
            MY0 = DM(I0,M0);
            MX0 = DM(I1,M0);
            call quadrant;
            AY0 = AR;
            AF = AX0 - AY0;
            AR = AF-1;
            if eq jump Cal4Down;
            AR = AF+1;
            if ne jump FreqLoop4;
            AR = AY1+1;
            AY1 = AR;
            jump FreqLoop4;
Cal4Down:   AR = AY1-1;
            AY1 = AR;
FreqLoop4:   AX0 = AY0;
            AR = AY1;
            call RegRestore;
            rts;

quadrant:   AR= MX0;
            if pos jump quad12;
            ar=MY0;
            if pos jump quad4;
            ar = 3;
            rts;
quad4:      ar = 4;
            rts;
quad12:     ar = MY0;
            if pos jump quad1;
            ar = 2;
            rts;
quad1:      ar = 1;
            rts;
*/

FreqCal4_:  call RegSave;   /* Costas Loop */
            i0 = AR;
            l0 = 0;
            m0 = 1;
            i1 = AY1;
            l1 = 0;
            CNTR = 128;
            MR = 0;
            do FreqLoop4 UNTIL CE;
            AY0 = DM(I0,M0);
            AX1 = DM(I1,M0);
            AR = AX1-AY0;
            MX0 = AR;
            AR = AX1+AY0;
            MY0 = AR;
FreqLoop4:  MR = MR+MX0*MY0(SS);
            AR = MR1;
            AR = -AR;
/*            MX0 = 0x8000;
            MY0 = 0x8000;
            MR = MR+MX0*MY0(UU);
            MR = MR+MX0*MY0(UU);
            AF = PASS MR2;
            if eq jump dontmodify4;
            ar = 0;
*/dontmodify4: call RegRestore;
            rts;

FreqCal3_:  call RegSave;   /* Costas Loop */
            i0 = AR;
            l0 = 0;
            m0 = 1;
            i1 = AY1;
            l1 = 0;
            CNTR = 128;
            MR = 0;
            MY0 = DM(I0,M0);
            do FreqLoop3 UNTIL CE;
            MX0 = DM(I1,M0);
FreqLoop3:  MR = MR+MX0*MY0(SS), MY0 = DM(I0,M0);
            AR = MR1;
            MX0 = 0x8000;
            MY0 = 0x8000;
            MR = MR+MX0*MY0(UU);
            MR = MR+MX0*MY0(UU);
            AF = PASS MR2;
            if eq jump dontmodify;
            ar = 0;
/*            AY1 = H#0008;
            if LT jump Freql2;
            AF = AR - AY1;
            if GT jump dontmodify;
            AR = 1;
*/dontmodify: call RegRestore;
            rts;
/*Freql2:     AF = AR + AY1;
            if LE jump dontmodify;
            ar = -1;
            jump dontmodify;
*/
FreqCal2_:  call RegSave;
            AX1 = dm(volume);
            call SSBAGC_;
            AX0 = dm(largest);
            AR = AY1;
            call SSBAGC_;
            AY0 = dm(largest);
            dm(volume) = AX1;
            AR = AX0-AY0;
            call RegRestore;
            rts;

SetTone_:   call RegSave;
            MR1 = AR;
            AY0 = 0xC0;
            AR = AR+AY0;
            AX0 = AR;  
            call set_MASTER;
            AY0 = 0x01C0;
            AR = MR1+AY0;
            AX0 = AR;
            call set_MASTER;
            call RegRestore;
            rts;
  
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
    call delay_I2C2;
/*    call delay_I2C;
*/    call data_low_I2C;
    call delay_I2C2;
/*    call delay_I2C;
*/    call clock_low_I2C;

    call delay_I2C;
    call data_high_I2C;
  call delay_I2C;
    rts;

send_I2C_bit:
    ar=ax0 and ay0;
    if ne jump send_I2C_one;
    if eq jump send_I2C_zero;

send_I2C_one:           { send I2C "one" data bit }
    call data_high_I2C;
    call delay_I2C;
    call clock_high_I2C;
/*    call delay_I2C;
*/    call delay_I2C2;
    call clock_low_I2C;
    call delay_I2C;
    rts;

send_I2C_zero:          { send I2C "zero" data bit }
    call data_low_I2C;
    call delay_I2C;
    call clock_high_I2C;
/*    call delay_I2C;
*/    call delay_I2C2;
    call clock_low_I2C;
    call delay_I2C;
    rts;

stop_I2C:               { send I2C stop command }
    call data_low_I2C;
    call delay_I2C;
    call clock_high_I2C;
/*    call delay_I2C;
*/    call delay_I2C2;
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
  nop;
I2C_delay:
    nop;
    rts;

delay_I2C2:              { delay for 2.5 uSec }
    CNTR = I2CDELAY2;
    DO I2C_delay2 UNTIL CE;
  nop;
  nop;
  nop;
  nop;
I2C_delay2:
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

{
        Square Root
                y = sqrt(x)

        Calling Parameters
                MR1 = MSW of x
                MR0 = LSW of x
                M3 = 1
                L3 = 0

        Return Values
                SR1 = y in 8.8 UNSIGNED format

        Altered Registers
                AX0,AY0,AY1,AR,MY0,MY1,MX0,MF,MR,SE,SR,I3

        Computation Time
                75 cycles (maximum)
}

{.CONST  base=H#0D49, sqrt2=H#5A82;}
#define base 0x0D49
#define sqrt2 0x5A82


.VAR/DM sqrt_coeff[5];

.INIT   sqrt_coeff : H#5D1D, H#A9ED, H#46D6, H#DDAA, H#072D;

.ENTRY  sqrt;

sqrt:   I3=^sqrt_coeff;                       {Pointer to coeff. buffer}
        SE=EXP MR1 (HI);                      {Check for redundant bits}
        SE=EXP MR0 (LO);
        AX0=SE, SR=NORM MR1 (HI);             {Remove redundant bits}
        SR=SR OR NORM MR0 (LO);
        MY0=SR1, AR=PASS SR1;
        IF EQ RTS;
        MR=0;
        MR1=base;                             {Load constant value}
        MF=AR*MY0 (RND), MX0=DM(I3,M3);       {MF = x**2}
        MR=MR+MX0*MY0 (SS), MX0=DM(I3,M3);    {MR = base + C1*x}
        CNTR=4;
        DO approx UNTIL CE;
                MR=MR+MX0*MF (SS), MX0=DM(I3,M3);
approx:         MF=AR*MF (RND);
        AY0=15;
        MY0=MR1, AR=AX0+AY0;                 {SE + 15 = 0?}
        IF NE JUMP scale;                    {No, compute sqrt(s)}
        SR=ASHIFT MR1 BY -6 (HI);
        RTS;
scale:  MR=0;
        MR1=sqrt2;                           {Load 1/sqrt(2)}
        MY1=MR1, AR=ABS AR;
        AY0=AR;
        AR=AY0-1;
        IF EQ JUMP pwr_ok;
        CNTR=AR;                             {Compute (1/sqrt(2))^(SE+15)}
        DO compute UNTIL CE;
compute:  MR=MR1*MY1 (RND);
pwr_ok: IF NEG JUMP frac;
        AY1=H#0080;                          {Load a 1 in 9.23 format}
        AY0=0;                               {Compute reciprocal of MR}
        DIVS AY1, MR1;
        DIVQ MR1; DIVQ MR1; DIVQ MR1;
        DIVQ MR1; DIVQ MR1; DIVQ MR1;
        DIVQ MR1; DIVQ MR1; DIVQ MR1;
        DIVQ MR1; DIVQ MR1; DIVQ MR1;
        DIVQ MR1; DIVQ MR1; DIVQ MR1;
        MX0=AY0;
        MR=0;
        MR0=H#2000;
        MR=MR+MX0*MY0 (US);
        SR=ASHIFT MR1 BY 2 (HI);
        SR=SR OR LSHIFT MR0 BY 2 (LO);
        RTS;
frac:   MR=MR1*MY0 (RND);
        SR=ASHIFT MR1 BY -6 (HI);
        RTS;

Center_:       call RegSave;
               MX1 = AR;
/*               i0 = AR;
               l0 = 0;
               m0 = 1;
               CNTR = 128;
               AY0 = 0;
               AY1 = H#FFFF;
               AX0 = dm(i0,m0);
               do center_loop UNTIL CE;
               AX1 = H#8000;
               AF = PASS AX1;
               AR = AX0+AF;
               AF = AR-AY0;
               if NOT AC jump center_not_hi;
               AY0 = AR;
center_not_hi: AF = AR-AY1;
               if AC jump center_loop;
               AY1 = AR;
center_loop:   AX0 = dm(i0,m0);
               AX0 = AY0;
               AR = AX0-AY1;
               AX0 = AR;
               SR = LSHIFT AR BY -1(LO);
               AF = SR0+AY1;
               AX1 = H#8000;
               AR = AX1+AF;
               MX0 = AR;
               AY0 = H#0000;
               AY1 = H#0004;
               AF = PASS AY1;
               call divide;  /* AX0 was set earlier */
/*               MY0 = AY0;
               i0 = MX1;
               m1 = 0;
               CNTR = 128;
               AX0 = dm(i0,m1);
               do center_2 UNTIL CE;
               AR = AX0-AF;
               MR = AR * MY0 (SU);
               SR=LSHIFT MR0 BY -10 (LO);
               SR=SR OR ASHIFT MR1 BY -10 (HI);
               dm(i0,m0) = SR0;
center_2:      AX0 = dm(i0,m1);

*/               i0 = MX1;
               l0 = 0;
               m0 = 1;
               CNTR = 128;
               AY0 = 0;
               AY1 = H#FFFF;
               AX0 = dm(i0,m0);
               do center2_loop UNTIL CE;
               AX1 = H#8000;
               AF = PASS AX1;
               AR = AX0+AF;
               AF = AR-AY0;
               if NOT AC jump center2_not_hi;
               AY0 = AR;
center2_not_hi: AF = AR-AY1;
               if AC jump center2_loop;
               AY1 = AR;
center2_loop:  AX0 = dm(i0,m0);
               AX0 = AY0;
               AR = AX0-AY1;
               AX1 = AR;
               SR = LSHIFT AR BY -1(LO);
               AF = SR0+AY1;
               AF = AF-1;
/*               AX1 = H#8000;
               AR = AX1+AF;
               MX0 = AR;
*/               i0 = MX1;
               m1 = 0;
               CNTR = 128;
               AX0 = dm(i0,m1);
               MY0 = 0;
               do center2_2 UNTIL CE;
               AR = AX0-AF;
               dm(i0,m0) = AR;
center2_2:     AX0 = dm(i0,m1);
               AY0 = H#8000;
               AR = AX1-AY0;
               call RegRestore;
               rts;


FM_:           call RegSave;
               AX0 = AR;
               AX1 = AY1;
               AR = ABS AX1;
               SR = LSHIFT AR BY 8 (LO);
               AF = ABS AX0;
               AX1 = SR1;
               AR = AX1-AF;
               if POS jump xtosmall;
               SI = AY0;
               SR = ASHIFT SI BY -7 (HI);
               AF = PASS SR1;
               AY0 = SR0;
               call sdivide;
               SI = AY0;
               SR = ASHIFT SI BY -8(HI);
               MR0 = SR0;
               MR1 = SR1;
               M3 = 1;
               L3 = 0;
               call arctan;
               call RegRestore;
               rts;
xtosmall:      AR = PASS AY0;
               if POS jump plus90;
               ar = H#8000;
               call RegRestore;
               rts;
plus90:        ar = H#7FFF;
               call RegRestore;
               rts;

{
        Arctangent Approximation
                y = Arctan(x)

        Calling Parameters
                MR1 = Integer part of x
                MR0 = Fractional part of x
                M3 = 1
                L3 = 0

        Return Values
                AR = Arctan(x) in 1.15 scaled format
                (-0.5 for -90 degrees, 0.5 for 90 degrees)

        Altered Registers
                AX0,AX1,AY0,AY1,AR,AF,MY0,MY1,MX1,MF,MR,SR,SI

        Computation Time
                58 cycles (maximum)
}

.VAR/DM/RAM     atn_coeff[5];

.INIT   atn_coeff : H#28BD, H#006D, H#EF3E, H#08C6, H#FED4;

.ENTRY  arctan;

arctan: I3 = ^atn_coeff;            {Point to coefficients}
        AY0=0;
        AX1=MR1;
        AR=PASS MR1;
        IF GE JUMP posi;            {Check for positive input}
        AR=-MR0;                    {Make negative number positive}
        MR0=AR;
        AR=AY0-MR1+C-1;
        MR1=AR;
posi:   SR=LSHIFT MR0 BY -1 (LO);   {Produce 1.15 value in SR0}
        AR=SR0;
        AY1=MR1;
        AF=PASS MR1;
        IF EQ JUMP noinv;           {If input < 1, no need to invert}
        SE=EXP MR1 (HI);            {Invert input}
        SR=NORM MR1 (HI);
        SR=SR OR NORM MR0 (LO);
        AX0=SR1;
        SI=H#0001;
        SR=NORM SI (HI);
        AY1=SR1;
        AY0=SR0;
        DIVS AY1,AX0;
        DIVQ AX0; DIVQ AX0; DIVQ AX0;
        DIVQ AX0; DIVQ AX0; DIVQ AX0;
        DIVQ AX0; DIVQ AX0; DIVQ AX0;
        DIVQ AX0; DIVQ AX0; DIVQ AX0;
        DIVQ AX0; DIVQ AX0; DIVQ AX0;
        AR=AY0;
noinv:  MY0=AR;
        MF=AR*MY0 (RND), MY1=DM(I3,M3);
        MR=AR*MY1 (SS), MX1=DM(I3,M3);
        CNTR=3;
        DO approximate UNTIL CE;
                MR=MR+MX1*MF (SS), MX1=DM(I3,M3);
approximate:    MF=AR*MF (RND);
        MR=MR+MX1*MF (SS);
        AR=MR1;
        AY0=H#4000;
        AF=PASS AY1;
        IF NE AR=AY0-MR1;
        AF=PASS AX1;
        IF LT AR=-AR;
        RTS;

.ENDMOD;


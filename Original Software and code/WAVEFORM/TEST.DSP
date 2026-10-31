{
***************************************************************************
* Analog Dev. ADSP-2101 Filter Realization Copyright (C) by HYPERCEPTION, INC.
***************************************************************************
*
*       Filter Generated from File: TEST.iir
*
*       Filter Order: M=10;  Number of Sections: N=(M+1)/2=5
*
*       Direct Form II cascade of 2nd order sections:
*
*                N
*             _______              -1        -2
*               | |  [ A0i +  A1i z  +  A2i z  ]
*       H(z)= K | |   -------------------------- 
*               i=1                -1        -2
*                    [  1  +  B1i z  +  B2i z  ]
*
*        K
*  x(n) -->- + ----.---- + -->-- + ----.---- + - .... - + ----.---- + -> y(n)
*           *|  z-1|     |*     *|  z-1|     |*        *|  z-1|     |*
*          | |_____v_____| |   | |_____v_____| |      | |_____v_____| |
*          |  -B11 | A11   |   |  -B12 | A12   |      |  -B1i | A1i   |
*          |       |       |   |       |       |      |       |       |
*          |    z-1|       |   |    z-1|       |      |    z-1|       |
*          |_______v_______|   |_______v_______|      |_______v_______|
*             -B21   A21          -B22   A22             -B2i   A2i
*
}
{*
*        The following main module example may be copied to a separate
*        filename (e.g. Main.dsp) and uncommented to provide a test of
*        the filter.  A typical example of assembly and  link commands
*        needed to produce executable filter test code would then be:
*
*        dsppa Main  <cr>
*        dsppa !!!!
*        dsppl Main !!!!
*
*        NOTE: The Linker expects an Architecture Description File (.ACH)
*              which is created by the System Builder.
*}
{
.MODULE   Main;
.CONST    Length = 996;
.VAR/DM   Out_Data[Length];
.ENTRY    Pause;
.EXTERNAL Filt;


            I3 = ^Out_Data;
            M3 = 1;
            L3 = 0;


            CNTR = Length;

            MY1 = 8192;

            DO next_sample UNTIL CE;

               CALL Filt;
               DM(I3,M3) = MR1
next_sample:   MY1 = 0;

Pause:      NOP;
            JUMP Pause;

.ENDMOD;

}

.MODULE IIR_Filt;             { Q15-Notation (scaling) is in effect }
.CONST  Stages = 5;           { number of 2nd order stages in filter }
.VAR/PM Delay[10];             { stages*2 delay elements required }
.VAR/DM Coefficients[21];

{  NOTE: The user may wish to store the following coefficients to an
         external .DAT file in hexadecimal format. Keep in mind that
         these source code coefficients have been quantized.       }

{  Coefficients stored as: (const K) B1i, B2i, A1i, A2i  }

.INIT   Coefficients: 333, -15020, 32255, -24124, 32767,
                      15020, 32255, 24124, 32767,
                      -11036, 30365, -18111, 32767,
                      11036, 30365, 18111, 32767,
                      0, 28352, 0, -32767;

.INIT   Delay: 0,0,0,0,0,0,0,0,0,0;



.ENTRY  Filt;              { Input is in MY1 register, output }
                           { is in MR1 register }

Filt:       M1 = 1;        { Remember to restore these registers }
            M5 = 1;        { if necessary at end of routine }
            M6 = 0;
            M7 = -1;
            L1 = 0;
            L5 = 0;

            CNTR = Stages;
            I1 = ^Coefficients;        { I1 points to filter coefficients }
            I5 = ^Delay;               { I5 points to delay elements }

            MR1 = MY1;                 { Accumulator = input }
            MR0 = 0;

            MY0 = DM(I1,M1);    { my0 = filter constant K }
            MR = MR1*MY0 (ss);  { mr = k*input }

            DO sections UNTIL CE;

               MX0 = DM(I1,M1),        MY0 = PM(I5,M5);
             { mx0 = b1i               my0 = delay z-1 }

               MR = MR - MX0*MY0 (ss), MX0 = DM(I1,M1),    MY1 = PM(I5,M7);
             { mr = xinput - b1i*(z-1) mx0 = b2i           my1 = delay z-2 }

               MR = MR - MX0*MY1 (ss), MX0 = DM(I1,M1);
             { mr = mr - b2i*(z-2)     mx0 = a1i }


               PM(I5,M5) = MR1;
             { update delay z-1 }

               MR = MR + MX0*MY0 (ss), MX0 = DM(I1,M1),    MY1 = PM(I5,M6);
             { mr = mr + a1i*delay z-1 mx0 = a2i           my1 = delay z-2 }

sections:      PM(I5,M5) = MY0,        MR = MR + MX0*MY1 (ss);
             { delay z-2 = delay z-1   mr = mr + a2i*delay z-2 }

 { following commented section should only be used if filter }
 { is very high order, and tends to become unstable (blow-up)}

             { SR = ASHIFT MR1 (HI); }
             { MR1 = SR1;            { Scale down next stage input }
             { SR = SR OR LSHIFT MR0 (LO); }
 { sections:   MR0 = SR0; }


            RTS;

.ENDMOD;

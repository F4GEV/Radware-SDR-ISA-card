/******************************************************************************
//
//      SIN.DSP
//
//      Copyright (c) 1994 Analog Devices, Inc.
//
//-----------------------------------------------------------------------------
//
//      Module:    sin.dsp
//      Purpose:   Generates sine wave data using a series approximation.
//                 sin(x) = 3.140625x + 0.02026367x**2 - 5.325196x**3
//                          + 0.5446778x**4 + 1.800293x**5
//
/*****************************************************************************/


.MODULE/RAM  Sin_Approximation;


.EXTERNAL sin;
.external RegSave;
.external RegRestore;

		{ these are the parameters for the 3 sin terms }
.VAR/DM/RAM     freq1_,freq2_,freq3_;
.VAR/DM/RAM     phi1_,phi2_,phi3_;
.VAR/DM/RAM     gain1_,gain2_,gain3_;
.VAR/DM/RAM     delta_count_;

.VAR/DM/RAM     sin_place;
.VAR/CIRC       sin_data[8];
.INIT sin_data:  0,0x3150,0x4000,0x3150,0,0xce50,0xc000,0xce50;
.GLOBAL         freq1_,freq2_,freq3_;
.GLOBAL         phi1_,phi2_,phi3_;
.GLOBAL         gain1_,gain2_,gain3_;

.VAR/DM I0_HOLD,L0_HOLD;

{Scratch Registers: AF, AR, AY1, I1, I6, M3, M5, MF, MR0, MR1,
		    MR2, MY1, PX, SB, SE, SI, SR0, SR1}


.ENTRY  genSignal_;
.ENTRY  initSignal_;
.ENTRY FillSin_;

initSignal_:
	AR = 0;
	DM(gain1_) = AR;                {initialize all gain variables to zero}
	DM(gain2_) = AR;
	DM(gain3_) = AR;
	DM(phi1_) = AR;                 {initialize all phase variables to zero}
	DM(phi2_) = AR;
	DM(phi3_) = AR;
	DM(delta_count_) = AR;          {initialize delta_count_ to zero}
        AR = ^sin_data;
        DM(sin_place) = AR;
	RTS;

genSignal_: call RegSave;
            I0 = DM(sin_place);
            M0 = 1;
            L0 = 8;
            AR = DM(I0,M0);
            DM(sin_place) = I0;
            call RegRestore;
            rts;

FillSin_: call RegSave;
           DIS M_MODE;
           M2 = 1;
           I0 = AR;
           L0 = 0;
           CNTR = 128;
           AR = DM(phi1_);
           do Fill_Sine UNTIL CE;
           L2 = 0;
           CALL sin;
           MY1 = DM(gain1_);
           MR = AR * MY1 (SS);
           DM(I0,M2) = MR1;
           DM(I0,M2) = MR1;
           AR = DM(phi1_);
           AY1 = DM(freq1_);
           AR = AR + AY1;
Fill_Sine: DM(phi1_) = AR;
           ENA M_MODE;
           call RegRestore;
           rts;
{
	Sine Approximation
		Y = Sin(x)

	Calling Parameters
		AR = x in scaled 1.15 format 
		M2 = 1
		L2 = 0

	Return Values
		AR = y in 1.15 format

	Altered Registers
		AY0,AF,AR,MY1,MX1,MF,MR,SR,I2

	Computation Time
		25 cycles

}

.VAR/DM sin_coeff[5];
.VAR/DM mr_hold[3];
.VAR/DM sr1_hold;
.VAR/DM ireg_hold[1];
.VAR/DM ax0_hold;
.VAR/DM mx1_hold;

.INIT   sin_coeff : H#3240, H#0053, H#AACC, H#08B7, H#1CCE;

.ENTRY  sin;

sin:    
	DM(mr_hold)=MR2;                   { store contents of accumulator }
	DM(mr_hold+1)=MR1;
	DM(mr_hold+2)=MR0;
	DM(sr1_hold)=SR1;                  { store contents registers }
	DM(ireg_hold)=I2;
	DM(ax0_hold) = AX0;     
	DM(mx1_hold) = MX1;
	AX0 = AR;
	I2=^sin_coeff;                      {pointer to coeff. buffer}
	l2 = 0;
	AY1=H#4000;
	AR=AX0, AF=AX0 AND AY1;             {Check 2nd or 4th quad.}
	IF NE AR=-AX0;                      {If yes, negate input}
	AY1=H#7FFF;
	AR=AR AND AY1;                      {Remove sign bit}
	MY1=AR;
	MF=AR*MY1 (RND), MX1=DM(I2,M1);     {MF = x**2}
	MR=MX1*MY1 (SS), MX1=DM(I2,M1);     {MR = C1*x}

	CNTR=3;
	DO approx UNTIL CE;

		MR=MR+MX1*MF (SS);
		MF=AR*MF (RND), MX1=DM(I2,M1);
		MR=MR+MX1*MF (SS);
		MF=AR*MF (RND), MX1=DM(I2,M1);
		MR=MR+MX1*MF (SS);
		MF=AR*MF (RND), MX1=DM(I2,M1);
approx:  
	MR=MR+MX1*MF (SS);
	SR=ASHIFT MR1 BY 3 (HI);
	SR=SR OR LSHIFT MR0 BY 3 (LO);      {Convert to 1.15 format}
	AR=PASS SR1;
	IF LT AR=PASS AY1;                  {Saturate if needed}
	AF=PASS AX0;
	IF LT AR=-AR;                       {Negate output if needed}
	
	MR0=DM(mr_hold+2);                  { restore accumulator }
	MR1=DM(mr_hold+1);
	MR2=DM(mr_hold);
	SR1=DM(sr1_hold);                   { restore saved registers }
	I2=DM(ireg_hold);       
	AX0 = DM(ax0_hold);     
	MX1 = DM(mx1_hold);
	RTS;                               
     
.ENDMOD;


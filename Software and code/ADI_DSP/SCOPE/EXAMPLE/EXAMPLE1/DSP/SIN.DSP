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


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


.MODULE/RAM     Signal_Gen;

.EXTERNAL       sin;
.external       RegSave;
.external       RegRestore;
.external freq1_,phi1_,gain1_;

.ENTRY  genSignal_;
.ENTRY  initSignal_;

initSignal_:
	AR = 0;
	DM(gain1_) = AR;                {initialize all gain variables to zero}
	DM(gain2_) = AR;
	DM(gain3_) = AR;
	DM(chirp_gain_) = AR;
	DM(ccitt_gain_) = AR;    dalg
	DM(phi1_) = AR;                 {initialize all phase variables to zero}
	DM(phi2_) = AR;
	DM(phi3_) = AR;
	DM(chirp_phi_) = AR;
	DM(delta_count_) = AR;          {initialize delta_count_ to zero}
	RTS;

genSignal_:
	DIS M_MODE;                       {disable integer mode}
        call RegSave;                     { save register }
	MR = 0;                           {zero accumulator}
					  {calculate sin1 waveform}
	SR1 = DM(gain1_);                 {load SR1 with gain1_}
	AF = PASS SR1;                    {pass contents of SR1 thru ALU and store in AF}
	IF EQ JUMP sin_2;                 {jump if ALU result is zero}
		AR = DM(phi1_);           {load AR with phi1_}
		CALL sin;                 {call sin subroutine}
		MY1 = AR;                 {MY1 = sin(DM(phi1_))}
		AR = DM(phi1_);           {load AR with phi1_}
		AY1 = DM(freq1_);         {load AY1 with freq1_(note freq1 is converted}
					  {from hertz to phase increment in alg.c}
		AR = AR + AY1;            {add freq1_ (phase inc) to current phase}
		DM(phi1_) = AR;           {store new phase in phi1_}
		MR = MR + SR1 * MY1 (SS); {MR = MR + gain1_ * MY1}

sin_2:                                    {calculate sin2 waveform}

genEnd:
	AR = MR1;                          {load return value in AR}
	call RegRestore;                   { restore registers }
	ENA M_MODE;                        {enable integer mode}
	RTS;

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

.ENDMOD;


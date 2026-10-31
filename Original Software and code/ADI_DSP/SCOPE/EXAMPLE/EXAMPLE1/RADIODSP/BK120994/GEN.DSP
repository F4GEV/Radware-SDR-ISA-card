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

		{ these are the parameters for the 3 sin terms }
.VAR/DM/RAM     freq1_,freq2_,freq3_; 
.VAR/DM/RAM     phi1_,phi2_,phi3_;
.VAR/DM/RAM     gain1_,gain2_,gain3_;
.VAR/DM/RAM     delta_count_;

.GLOBAL         freq1_,freq2_,freq3_; 
.GLOBAL         phi1_,phi2_,phi3_;
.GLOBAL         gain1_,gain2_,gain3_;

.VAR/DM I0_HOLD,L0_HOLD;

{Scratch Registers: AF, AR, AY1, I1, I6, M3, M5, MF, MR0, MR1, 
		    MR2, MY1, PX, SB, SE, SI, SR0, SR1}        
			    

.ENTRY  genSignal_;
.ENTRY  initSignal_;

initSignal_:                         
	AR = 0;
	DM(gain1_) = AR;                {initialize all gain variables to zero}
	DM(gain2_) = AR;
	DM(gain3_) = AR;
	DM(phi1_) = AR;                 {initialize all phase variables to zero}
	DM(phi2_) = AR;
	DM(phi3_) = AR;
	DM(delta_count_) = AR;          {initialize delta_count_ to zero}
	RTS;                          

genSignal_:                   
						   
	DM(I0_HOLD) = I0;                 {save I0 register}                    
	DM(L0_HOLD) = L0;                 {save L0 register}
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
	
	SR1 = DM(gain2_);
	AF = PASS SR1;
	IF EQ JUMP sin_3;
		AR = DM(phi2_);
		CALL sin;
		MY1 = AR;
		AR = DM(phi2_);
		AY1 = DM(freq2_);
		AR = AR + AY1;
		DM(phi2_) = AR;
		MR = MR + SR1 * MY1 (SS);
		
sin_3:                                     {calculate sin3 waveform}
	
	SR1 = DM(gain3_);
	AF = PASS SR1;
	IF EQ JUMP genEnd;
		AR = DM(phi3_);
		CALL sin;
		MY1 = AR;
		AR = DM(phi3_);
		AY1 = DM(freq3_);
		AR = AR + AY1;
		DM(phi3_) = AR;
		MR = MR + SR1 * MY1 (SS);

genEnd:        
	AR = MR1;                          {load return value in AR}
	I0 = DM(I0_HOLD);                  {restore I0}
	L0 = DM(L0_HOLD);                  {restore L0}
	RTS;                              
 
.ENDMOD;


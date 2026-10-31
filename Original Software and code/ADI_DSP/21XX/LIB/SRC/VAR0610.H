{_______________________________________________________________________________
	VAR0610.H
			Analog Devices INC. DSP Division
			One Technology Way, Norwood MA, 02062
			DSP Applications: (617) 461-3672

        This files holds the variable declarations for the encoder and
	decoder (GSM specification 06.10) and comfort noise functions in the 
	encoder (GSM spec. 06.12) and Discontinuous Transmission functions
	in the encoder (GSM spec. 06.11).  This file is used when the 
	assembler directive "-Dalias" is NOT specified.
_______________________________________________________________________________}

{These variables must be saved between calls:

	_Variables_	_Section_	_Used_for:______________________________


     (these variables are used in the encoder)	
	z1,L_z2_h,L_z2_l  4.2.2		offset compensation
	mp		  4.2.3		pre-emphasis 
	table_mac[8]	  4.2.7		lar encoding
	table_a[8]	  4.2.7		lar encoding
	old_larpp[8]
	u[8]		  4.2.10	short term analysis filtering
	dp[120]		  4.2.11,18	previous short term residual signal
	table_dlb[4]	  4.2.11	ltp gain (bc) encoding
	h[11]		  4.2.13	RPE weighting filter
	table_nrfac[8]	  4.2.15	RPE sample encoding
	cni_wait	  cni		count of consecutive VAD inactivity
	oldlar_pntr	  cni		active buffer location
	oldxmax_pntr	  cni		active buffer location
	oldlar_buffer[32] cni		values stored for cni
	oldxmax_buffer[16]cni		values stored for cni
	mean_lar[8]       cni           values stored for DTX
	mean_xmaxc        cni           value stored for DTX
	speech_count      cni           count of speech frames between sid frams

     (these variables are used in both the encoder and decoder)
	table_mic[8]	  4.2.7,8	lar encode/decode
	table_b[8]	  4.2.7,8	lar encode/decode
	table_inva[8]	  4.2.8		lar decoding
	table_qlb[4]	  4.2.12	ltp gain (bc) decoding
	table_fac[8]	  4.2.16	RPE sample decoding

     (these variables are used in the decoder)
	nrp		  4.3.2		long term synthesis filtering
	drp[160]	  4.3.2		reconstructed short term residual signal
	v[9]		  4.3.4		short term synthesis filtering
	msr		  4.3.5		de-emphasis
	seed_lsw,seed_msw cni		initial seed for random number generator
}

.CONST		window_length=160, sub_window_length=40;

	{The variables below are used by the encoder}

.VAR/PM/RAM	h[11];			{Constant Table, init at powerup}

.VAR/DM/RAM	L_ACF[18];			{9 long (32-bit) words}
.VAR/PM/RAM	dp[120];
.VAR/DM/RAM	dpp[40];

.VAR/DM/RAM	r[8]; 
.VAR/DM/RAM	k[8];
.VAR/DM/RAM	acf[9];
.VAR/DM/RAM	p[9];

.VAR/DM/RAM	LAR[8];
						{Must be declared in sequence}
.VAR/DM/RAM	u[8], old_LARpp[8], z1, L_z2_h, L_z2_l, mp;

.VAR/DM/RAM	scaleauto, scal;		{Used in pre-emphasis save}
.VAR/DM/RAM	speech_in, xmit_buffer;

	{The variables below are used by the decoder}

.VAR/DM/RAM	nrp;
.VAR/DM/RAM	msr,                            {Must be declared in sequence}
		old_LARrpp[8],
		v[9];
.VAR/DM/RAM	drp[window_length];
.VAR/DM/RAM	recv_buffer, speech_out;
	
	{The variables below are used by both the encoder and decoder}

.VAR/DM/RAM	rp[8];

.VAR/DM/RAM	wt[50];
.VAR/DM/RAM	LARp[8];
.VAR/DM/RAM	LARpp[8];

.VAR/DM/RAM	LARc[8];			{Window data}
.VAR/DM/RAM	Nc, bc, Mc, xmaxc, xMc[13];	{Must be declared in sequence}

.VAR/DM/RAM	ep[40];

.VAR/PM/RAM	table_a[8];		{Constant Table, init at powerup}
.VAR/PM/RAM	table_b[8];		{Constant Table, init at powerup}	
.VAR/PM/RAM	table_mic[8];		{Constant Table, init at powerup}
.VAR/PM/RAM	table_mac[8];		{Constant Table, init at powerup}
.VAR/PM/RAM	table_inva[8];		{Constant Table, init at powerup}
.VAR/PM/RAM	table_dlb[4];		{Constant Table, init at powerup}
.VAR/PM/RAM	table_qlb[4];		{Constant Table, init at powerup}
.VAR/PM/RAM	table_nrfac[8];		{Constant Table, init at powerup}
.VAR/PM/RAM	table_fac[8];		{Constant Table, init at powerup}

{The variables below are used by the encoder for comfort noise insertion}

.VAR/DM/RAM	cni_wait,
		mean_xmaxc;
.VAR/DM/RAM	cni_flag;
.VAR/DM/RAM	oldlar_buffer[32];
.VAR/DM/RAM	oldlar_pntr;
.VAR/DM/RAM	mean_lar[8];
.VAR/DM/RAM	mean_larc[8];
.VAR/DM/RAM	oldxmax_buffer[16];
.VAR/DM/RAM	oldxmax_pntr;
.VAR/DM/RAM	speech_count;
.VAR/DM/RAM	sp_flag;

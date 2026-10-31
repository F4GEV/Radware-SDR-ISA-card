/*********** saves and restores all registers except AR  ********/

.module/ram   RegUtil;

#define OFFSET_AX0    0
#define OFFSET_AX1    1
#define OFFSET_AY0    2 
#define OFFSET_AY1    3 
#define OFFSET_AR     4 
#define OFFSET_AF     5 
#define OFFSET_ASTAT  6 
#define OFFSET_MX0    7 
#define OFFSET_MX1    8
#define OFFSET_MY0    9
#define OFFSET_MY1   10
#define OFFSET_MR0   11
#define OFFSET_MR1   12
#define OFFSET_MR2   13
#define OFFSET_MF    14
#define OFFSET_MSTAT 15
#define OFFSET_SI    16
#define OFFSET_SE    17
#define OFFSET_SR0   18
#define OFFSET_SR1   19
#define OFFSET_SB    20
#define OFFSET_PX    21
#define OFFSET_I0    22
#define OFFSET_I1    23
#define OFFSET_I2    24
#define OFFSET_I3    25
#define OFFSET_M0    26
#define OFFSET_M1    27
#define OFFSET_M2    28
#define OFFSET_M3    29
#define OFFSET_L0    30
#define OFFSET_L1    31
#define OFFSET_L2    32
#define OFFSET_L3    33
#define OFFSET_I4    34
#define OFFSET_I5    35
#define OFFSET_I6    36
#define OFFSET_I7    37
#define OFFSET_M4    38
#define OFFSET_M5    39
#define OFFSET_M6    40
#define OFFSET_M7    41
#define OFFSET_L4    42
#define OFFSET_L5    43
#define OFFSET_L6    44
#define OFFSET_L7    45
#define OFFSET_SSTAT 46
#define OFFSET_IMASK 47
#define OFFSET_ICNTL 48
#define OFFSET_CNTR  49
#define OFFSET_RX0   50
#define OFFSET_TX0   51
#define OFFSET_RX1   52
#define OFFSET_TX1   53
#define OFFSET_TOS   54
#define OFFSET_PC    55
#define OFFSET_MAX   56

.VAR RegArea[OFFSET_MAX];

.ENTRY RegSave;
.ENTRY RegRestore;


RegSave:
		dm (RegArea + OFFSET_I0) = i0;
		dm (RegArea + OFFSET_I1) = i1;    
		dm (RegArea + OFFSET_I2) = i2;  
		dm (RegArea + OFFSET_I3) = i3;   
		dm (RegArea + OFFSET_I4) = i4;  
		dm (RegArea + OFFSET_I5) = i5;  
		dm (RegArea + OFFSET_I6) = i6;  
		dm (RegArea + OFFSET_I7) = i7;  
		dm (RegArea + OFFSET_M0) = m0;
		dm (RegArea + OFFSET_M1) = m1;    
		dm (RegArea + OFFSET_M2) = m2;  
		dm (RegArea + OFFSET_M3) = m3;   
		dm (RegArea + OFFSET_M4) = m4;  
		dm (RegArea + OFFSET_M5) = m5;  
		dm (RegArea + OFFSET_M6) = m6;  
		dm (RegArea + OFFSET_M7) = m7;  
		dm (RegArea + OFFSET_L0) = l0;
		dm (RegArea + OFFSET_L1) = l1;    
		dm (RegArea + OFFSET_L2) = l2;  
		dm (RegArea + OFFSET_L3) = l3;   
		dm (RegArea + OFFSET_L4) = l4;  
		dm (RegArea + OFFSET_L5) = l5;  
		dm (RegArea + OFFSET_L6) = l6;  
		dm (RegArea + OFFSET_L7) = l7;  
		dm (RegArea + OFFSET_AX0) = AX0;  
		dm (RegArea + OFFSET_AX1) = AX1;  
		dm (RegArea + OFFSET_MX0) = MX0;  
		dm (RegArea + OFFSET_MX1) = MX1;  
		dm (RegArea + OFFSET_AY0) = AY0;  
		dm (RegArea + OFFSET_AY1) = AY1;  
		dm (RegArea + OFFSET_MY0) = MY0;  
		dm (RegArea + OFFSET_MY1) = MY1;  
		dm (RegArea + OFFSET_SI) = SI;  
		dm (RegArea + OFFSET_SE) = SE;  
		dm (RegArea + OFFSET_MR0) = MR0;  
		dm (RegArea + OFFSET_MR1) = MR1;  
		dm (RegArea + OFFSET_MR2) = MR2;  
		dm (RegArea + OFFSET_SR0) = SR0;  
		dm (RegArea + OFFSET_SR1) = SR1;  
                rts;



RegRestore:
		i0 = dm (RegArea + OFFSET_I0);
		i1 = dm (RegArea + OFFSET_I1);
		i2 = dm (RegArea + OFFSET_I2);
		i3 = dm (RegArea + OFFSET_I3);
		i4 = dm (RegArea + OFFSET_I4);
		i5 = dm (RegArea + OFFSET_I5);
		i6 = dm (RegArea + OFFSET_I6);
		i7 = dm (RegArea + OFFSET_I7);
		m0 = dm (RegArea + OFFSET_M0);
		m1 = dm (RegArea + OFFSET_M1);
		m2 = dm (RegArea + OFFSET_M2);
		m3 = dm (RegArea + OFFSET_M3);
		m4 = dm (RegArea + OFFSET_M4);
		m5 = dm (RegArea + OFFSET_M5);
		m6 = dm (RegArea + OFFSET_M6);
		m7 = dm (RegArea + OFFSET_M7);
		l0 = dm (RegArea + OFFSET_L0);
		l1 = dm (RegArea + OFFSET_L1);
		l2 = dm (RegArea + OFFSET_L2);
		l3 = dm (RegArea + OFFSET_L3);
		l4 = dm (RegArea + OFFSET_L4);
		l5 = dm (RegArea + OFFSET_L5);
		l6 = dm (RegArea + OFFSET_L6);
		l7 = dm (RegArea + OFFSET_L7);
		ax0 = dm (RegArea + OFFSET_AX0);
		ax1 = dm (RegArea + OFFSET_AX1);
		mx0 = dm (RegArea + OFFSET_MX0);
		mx1 = dm (RegArea + OFFSET_MX1);
		ay0 = dm (RegArea + OFFSET_AY0);
		ay1 = dm (RegArea + OFFSET_AY1);
		my0 = dm (RegArea + OFFSET_MY0);
		my1 = dm (RegArea + OFFSET_MY1);
		si = dm (RegArea + OFFSET_SI);
		se = dm (RegArea + OFFSET_SE);
		mr0 = dm (RegArea + OFFSET_MR0);
		mr1 = dm (RegArea + OFFSET_MR1);
		mr2 = dm (RegArea + OFFSET_MR2);
		sr0 = dm (RegArea + OFFSET_SR0);
		sr1 = dm (RegArea + OFFSET_SR1);
                rts;

.endmod;


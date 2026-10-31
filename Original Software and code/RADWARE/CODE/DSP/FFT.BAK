{***************************************************************************
//
//      FFT.DSP
//
//      Copyright (c) 1994 Analog Devices, Inc.
//
//-----------------------------------------------------------------------------
//
//      Module:    fft.dsp
//      Purpose:   This module calculates a 256 point complex FFT, radix-4
//                 algorithm. It also implements a logarithm subroutine.
//
/*************************************************************************}

.Module/ram      FFT;
#define          N 256
#define          Nx2 512
#define          Nov2 128
#define          Nov4 64
#define          N3ov8 96
.VAR/PM/CIRC     cos_table[N];
.INIT            cos_table:<cos256.dat>;        {N cosine values}
.VAR/PM/CIRC     window[N];        
.INIT            window:<hanning.dat>;

.VAR/DM          M3_SPACE;      {memory space used to store M3 values when }
				{M3 is loaded with its alternate value     }
.VAR/DM          BFY_COUNT;

.external        Input_Position;              {alg.c variables used here}
.external        USB_BUFFER_;
.external        LSB_BUFFER_;
.external        LogOn_;
.external        HannOn_;
.external        RegSave;
.external        RegRestore;

.VAR/DM/abs=0x3800   output[Nx2];       {output array holds results in order}
.VAR/DM/RAM   inplace[Nx2];             {inplace array contains original input}
					{and also hold intermediate results}
.ENTRY  dofft_;
.ENTRY  dousbfft_;
.ENTRY  dolsbfft_;

dofft_:         DIS M_MODE;             {select fractional mode}
                call RegSave;           { save registers }
		call get_data;          {call subroutine to get buffer of data}
		call fft;               {call fft subroutine}
		call magnitude_squared; {call subroutine to calculate magnitude}
					{  squared of spectrum}
                call RegRestore;        { restore registers }
		ar = ^results;          {return value is starting address of results buffer}
		ENA M_MODE;             {select integer mode}
 		RTS;

dousbfft_:      DIS M_MODE;             {select fractional mode}
                call RegSave;           { save registers }
		i4 = ^USB_BUFFER_;
		call get_data2;         {call subroutine to get buffer of data}
		call fft;               {call fft subroutine}
		call magnitude_squared; {call subroutine to calculate magnitude}
					{  squared of spectrum}
                call RegRestore;        { restore registers }
		ar = ^results;          {return value is starting address of results buffer}
		ENA M_MODE;             {select integer mode}
 		RTS;

dolsbfft_:      DIS M_MODE;             {select fractional mode}
                call RegSave;           { save registers }
		i4 = ^LSB_BUFFER_;
         	call get_data2;         {call subroutine to get buffer of data}
		call fft;               {call fft subroutine}
		call magnitude_squared; {call subroutine to calculate magnitude}
					{  squared of spectrum}
                call RegRestore;        { restore registers }
		ar = ^results;          {return value is starting address of results buffer}
		ENA M_MODE;             {select integer mode}
 		RTS;

{############################################################################}

get_data:       i4 = dm(Input_Position); {and imaginary part with zero     }
get_data2:	i0 = ^inplace;                {set up pointers to copy data into   }
		m0 = 1;                 {  inplace array filling the real part}
		l0 = 0;                 {  with samples from lastInputBuffer }
		m4 = 1;
		l4 = 0;
		i6 = ^window;           {set up pointers for windowing buffer}
		m6 = 1;
		l6 = %window;
		cntr = 256;             {set counter to length of lastInputBuffer}
		ax0 = dm(HannOn_);      {check if windowing on}
		ar = pass ax0;
		if eq jump no_window;
		ax0 = 0;
		do wfloop until ce;     {multiply lastInputBuffer samples by} 
		   mx0 = dm(i4,m4);     {  window coefficients}
		   my0 = pm(i6,m6);
		   mr = mx0 * my0(ss);
		   sr = ashift mr1 by -3 (hi); {provide enough guard bits to}
		   dm(i0,m0) = sr1;            { prevent input saturation}
wfloop:            dm(i0,m0) = ax0;
		RTS;
						 
no_window:      my0 = 0x7fff;                 {no windowing case}
		do floop until ce;
		   mx0 = dm(i4,m4);
		   mr = mx0 * my0(ss);
		   sr = ashift mr1 by -3 (hi); 
		   dm(i0,m0) = sr1;
floop:             dm(i0,m0) = ax0;
                RTS;

{###########################################################################
 ##                    Magnitude squared Routines                         ##
 ###########################################################################
}
.var/dm   results[Nov2];      
magnitude_squared:
	
	i4=^results;
	m4=1;
	i0=^output;             { start at begining of positive spectrum } 
	m0=1;                   { skip over imag for now }
	cntr=Nov2;
			      
square: 
	mx0=dm(i0,m0);
	my0=mx0;
	mr=mx0*my0(ss),mx1=dm(i0,m0);
	my1=mx1;
	mr=mr+mx1*my1(rnd);
	if mv sat mr; 
	 sr1 = mr1;                {test LogOn flag if LogOn = 1 then}
	ax0 = dm(LogOn_);          {call log subroutine else skip it}
	ar = pass ax0;                       
				 
     if eq jump no_log;
     call log;
       
no_log: dm(i4,m4)=sr1;
	if not ce jump square;
	rts;


{##########################################################################
 ##                         Logarithm Routine                            ##
 ##                                                                      ##
 ## This code is taken from chapter 4 of applications handbook volume 1. ##
 ##########################################################################
}
#define  log_2 0x2688

.VAR/pm log_coeffs[5];

.INIT   log_coeffs : H#6F1500, H#C98700, H#1FC300, H#F13500, H#037D00;

	

log:  
	M5=1; L5=0;
	I5=^log_coeffs;                     {Common log start here}
	MY1=log_2;
compute:        SE=EXP MR1 (HI);            {Check for redundant bits}
	SE=EXP MR0 (LO);
	AY0=SE;
	AR=AY0-1;
	AX0=15;
	SE=AR, AR=AX0+AY0;
	SR=NORM MR1 (HI);                   {Remove redundant bits}
	SR=SR OR NORM MR0 (LO);
	MR=AR*MY1 (SS);                     {(SE+14) * log(2)}
	MY1=MR1;                            {Shift left 16 bits}
	MR1=MR0;
	MR2=MY1;
	MR0=0;
	AY0=H#8000;
	AR=SR1-AY0;
	MY1=AR;
	MF=AR*MY1 (RND), MX1=pm(I5,M5);     {MF = x**2}
	MR=MR+MX1*MY1 (SS), MX1=PM(i5,M5);  {MR = C1*x}
   
	CNTR=3;
approx:      MR=MR+MX1*MF (SS);
	     MF=AR*MF (RND), MX1=PM(i5,M5);
	if not ce jump approx;

	MR=MR+MX1*MF (RND);
	SR=ASHIFT MR2 BY 12 (HI);           {Shift to correct format}
	SR=SR OR LSHIFT MR1 BY 12 (LO);
	ax0 = sr1;   
	ar = pass ax0; 
	if lt jump error ; 
    
	RTS;
error:  sr1 = 0x0000;
	RTS;


{#######################################################################
 ##                 Fast Fourier Transform Routine                    ##
 #######################################################################
}

fft: M4=-Nov4;          {-N/4 = -90 degrees for sine}
     L0=0;
     L1=0;
     L2=0;
     L3=0;
     L5=%cos_table;
     L6=%cos_table;
     L7=%cos_table;
     SE=0;

{_____________________________Stage 1_______________________________}

stage1: I0=^inplace;                   {in       ->Xa,Xc}
	I1=^inplace+Nov2;              {in+N/2   ->Xb,Xd}
	I2=^inplace+1;                 {in+1     ->Ya,Yc}
	I3=^inplace+Nov2+1;            {in+N/2+1 ->Yb,Yd}
	I5=^cos_table;
	I6=^cos_table;
	I7=^cos_table;
	M0=N;               {N,    skip forward to dual node}
	M1=-N;              {-N,   skip back to primary node}
	M2=-N+2;            {-N+2, skip to next butterfly}

	M3=-2;              {because we have modified the middle branches }
			    {of bfly, pointers for I0 have to be treated a}
			    {little more carefully.  This requires a more }
			    {complex pointer manipulation.  M3 is used in }
			    {doing this                                   }

	M5=Nov4+1;          {N/4 + groups/stage*1, Cb Sb offset}

	M6=Nov4+2;          {N/4 + groups/stage*2, Cc Sc offset}
	M7=Nov4+3;          {N/4 + groups/stage*3, Cd Sc offset}
	AX0=DM(I0,M0);                        {get first Xa}
	AY0=DM(I0,M1);                        {get first Xc}
	AR=AX0-AY0, AX1=DM(I2,M0);            {Xa-Xc,get first Ya}
	SR=LSHIFT AR(LO), AY1=DM(I2,M1);      {SR1=Xa-Xc,get first Yc}
	CNTR=Nov4;                            {Bfly/group, stage one}
	


  {This radix_4 bfly algorithm is a modified one.  It has its middle 2  }
  {Branches reversed.  This alteration, done to every stage, results in }
  {a bit_reversed output instead of a digit reversed output             }


stg1bfy:         AR=AX0+AY0, AX0=DM(I1,M0);
			   {AR=xa+xc, AX0=xb }
		 MR0=AR, AR=AX1+AY1;
			   {MR0=xa+xc, AR=ya+yc}
		 MR1=AR, AR=AX1-AY1;
			   {MR1=ya+yc, AR=ya-yc}
		 SR=SR OR LSHIFT AR (HI), AY0=DM(I1,M1);
			   {SR1=ya-yc, AY0=xd}
		 AF=AX0+AY0, AX1=DM(I3,M0);
			   {AF=xb+xd, AX1=yb}
		 AR=MR0+AF, AY1=DM(I3,M1);
			   {AR=xa+xb+xc+xd, AY1=yd}
		 DM(I0,M0)=AR, AR=MR0-AF;
			   {output x'a=(xa+xb+xc+xd), AR=xa+xc-xb-xd}
		 AF=AX1+AY1, MX0=AR;
			   {AF=yb+yd, MX0=xa+xc-xb-xd}
		 AR=MR1+AF, MY0=PM(I6,M4);
			   {AR=ya+yc+yb+yd, MY0=(Cc)}
		 DM(I2,M0)=AR, AR=MR1-AF;
			   {output y'a, AR=ya+yc-yb-yd}
		 MR=MX0*MY0(SS), MY1=PM(I6,M6);
			   {MR=(xa+xc-xb-xd)(Cc), MY1=(Sc)}

		 MR=MR+AR*MY1(RND),SI=DM(I0,M2);    
			   {MR=(xa-xb+xc-xd)(Cc)+(ya-yb+yc-yd)(Sc)}
			   {si here is a dummy to perform a modify(I0,M2)}
		 DM(I1,M0)=MR1, MR=AR*MY0(SS);
			   {output x'c to  positionx'b}
			   {MR=(ya+yc-yb-yd)(Cc)}
		 MR=MR-MX0*MY1(RND), MY0=PM(I5,M4);
			   {MR=(ya+yc-yb-yd)(Cc)-(xa+xc-xb-xd)(Sc), MY0=(Cb)}
		 DM(I3,M0)=MR1, AR=AX0-AY0;
			   {output y'c=to position y'b}
			   {AR=xb-xd}
		 AY0=AR, AF=AX1-AY1;
			   {AY0=xb-xd, AF=yb-yd}
		 AR=SR0-AF, MY1=PM(I5,M5);
			   {AR=xa-xc-(yb-yd), MY1=(Sb)}
		 MX0=AR, AR=SR0+AF;
			   {MX0=xa-xc-yb+yd, AR=xa-xc+yb-yd}
		 SR0=AR, AR=SR1+AY0;
			   {SR0=xa-xc+yb-yd, AR=ya-yc+xb-xd}
		 MX1=AR, AR=SR1-AY0;
			   {MX1=ya-yc+xb-xd, AR=ya-yc-(xb-xd)}
		 MR=SR0*MY0(SS),AX0=DM(I0,M0);
			   {MR=(xa-xc+yb-yd)(Cb), AX0= xa of next bfly}
		 MR=MR+AR*MY1(RND),AY0=DM(I0,M3);
			   {MR=(xa-xc+yb-yd)(Cb)+ (ya-yc-xb+xd)(Sb)}
			   {AY0= xc of next bfly}
		 DM(I0,M2)=MR1, MR=AR*MY0(SS);
			   {output x'b=to position x'c}
			   {MR=ya-yc-xb+xd)(Cb)}
		 MR=MR-SR0*MY1(RND), MY0=PM(I7,M4);
			   {MR=(ya-yc-xb+xd)(Cb)-(xa-xc+yb-yd)(Sb)}
			   {MY0=(Cd)}
		 DM(I2,M2)=MR1, MR=MX0*MY0(SS);
			   {ouput y'b= to position y'c}
			   {MR=(xa-yb-xc+yd)(Cd)}
		 MY1=PM(I7,M7), AR=AX0-AY0;
			    {MY1=(Sd), AR=xa-xc }  
		 MR=MR+MX1*MY1(RND), AX1=DM(I2,M0);
			   {MR=(xa-yb-xc+yd)(Cd)+(ya+xb-yc-xd)(sd)}
			   {AX1=ya of next bfly}
		 DM(I1,M2)=MR1, MR=MX1*MY0(SS);
			   {output x'd=(xa-yb-xc+yd)(Cd)+(ya+xb-yc-xd)(sd)}
			   {MR=(ya+yb-yc-yd)(Cd)}
		 MR=MR-MX0*MY1(RND), AY1=DM(I2,M1);
			   {MR=(ya+yb-yc-yd)(Cd)-(xa-xc-yb+yd)Sd}
			   {yc of next bfly}
		 DM(I3,M2)=MR1, SR=LSHIFT AR(LO);
			   {output y'd=(ya+xb-yc-xd)Cd-(xa-xc-yb+yd)Sd}
			   {SR0=ya-yc of next bfly}

if not ce jump stg1bfy;       
{_____________________________Stage 2_______________________________}

	   
stage2:     I0=^inplace;       {in       -> Xa,Xc}
	    I1=^inplace+32;    {in+N/8   -> Xb,Xd}
	    I2=^inplace+1;     {in+1     -> Ya,Yc}
	    I3=^inplace+33;    {in+N/8+1 -> Yb,Yd}
	    M0=Nov4;           {N/4,    skip forward to dual node}
	    M1=-Nov4;          {-N/4,   skip back to primary node}
	    M2=-Nov4+2;        {-N/4+2, skip to next butterfly}
	    
	    M3=96;             {N*3/8,  skip to next group}
	    DM(M3_SPACE) = M3; {m3_space is temporary storage space needed}       
			       {because M3 is used in 2 contexts and will}
			       {alternate in value}
	   
	    M5=Nov4+4;         {N/4 +groups/stage*1, Cb Sb offset}
	    M6=Nov4+8;         {N/4 +groups/stage*2, Cc Sc offset}
	    M7=Nov4+12;        {N/4 +groups/stage*3, Cd Sd offset}
	    
	    SI = 16;            {Bfy/group, save counter for inner loop}
	    DM(BFY_COUNT) = SI; {SI is used as a temporary storage dummy}     

	    CNTR=4;            {groups/stage}
	    CALL mid_stg;      {do stage 2}    
	 
{_____________________________Stage 3_______________________________}

stage3:  I0=^inplace;           {in         -> Xa,Xc}
	    I1=^inplace+8;      {in+N/32    -> Xb,Xd}
	    I2=^inplace+1;      {in+1       -> Ya,Yc}
	    I3=^inplace+9;      {in+N/32+1  -> Yb,Yd}
	    M0=16;              {N/16,    skip forward to dual node}
	    M1=-16;             {-N/16,   skip back to primary node}
	    M2=-14;             {-N/16+2, skip to next butterfly}
 
	    M3=24;              {N*3/32,  skip to next group}
	    DM(M3_SPACE) = M3;  {m3_space is temporary storage space needed}       
				{because M3 is used in 2 contexts and will}
				{alternate in value}
	   
	    M5=Nov4+16;         {N/4 +groups/stage*1, Cb Sb offset}
	    M6=Nov4+32;         {N/4 +groups/stage*2, Cc Sc offset}
	    M7=Nov4+48;         {N/4 +groups/stage*3, Cd Sd offset}
			
	    SI = 4;             {Bfy/group, save counter for inner loop}
	    DM(BFY_COUNT) = SI; {SI is a dummy temporary register}
     
 
	    CNTR=16;            {groups/stage}
	    CALL mid_stg;       {do stage 3}
	   

{_________________Last Stage__No Multiplies__________________________}

laststage:  I4=^inplace;       {in         ->Xa,Xc}
	    I5=^inplace+2;     {in+N/512   ->Xb,Xd}
	    I6=^inplace+1;     {in+1       ->Ya,Yc}
	    I7=^inplace+3;     {in+N/512+1 ->Yb,Yd}
	    M4=4;              {N/256,  skip forward to dual node}

	    M0=h#0040;         {this magic modify value is used to         }
			       {perform a bit reverse as the final results }
			       { are written out.  Because of interleave   }
			       {this is not as simple as explained in      }
			       {1410 Handbook.  Derivation of these values }
			       {is in the Apps. note of this code          }

	    I0=h#0007;         {these base address values are derived      }
	    I2=h#2007;         {for an output at address 3800. Output      }
	    I1=h#0027;         {address can be modified by modifying m0,   }
	    I3=h#2027;         {and I0..I3                                 }

	    L4=0;              {this last stage has no twiddles and        }
	    L5=0;              {because it is where the DAG#1 bit_         }
	    L6=0;              {reverses the output, the I's M's & L's     }
	    L7=0;              {have been reassigned and re_initialized    }
       
	    AX0=DM(I4,M4);      {first Xa} 
	    AY0=DM(I4,M4);      {first Xc}
	    CNTR=Nov4;          {groups/stage}
	    ENA BIT_REV;        {all data accesses using I0..I3 are}
				{bit-reversed  }
      
  
  {This radix_4 bfly algorithm is a modified one.  It has it's middle 2 }
  {Branches reversed This alteration, done to every stage, results in   }
  {a bit_reversed output instead of a digit reversed output             }

laststgbfy:
		 AR=AX0-AY0, AX1=DM(I6,M4);       {AR=xa-xc, AX1=ya}
		 SR=LSHIFT AR(LO), AY1=DM(I6,M4); {SR0=xa-xc, AY1=yc}
		 AR=AX0+AY0, AX0=DM(I5,M4);       {AR=xa+xc, AX0=xb}
		 MR0=AR, AR=AX1+AY1;              {MR0=xa+xc, AR=ya+yc}
		 MR1=AR, AR=AX1-AY1;              {MR1=ya+yc, AR=ya-yc}
		 SR=SR OR LSHIFT AR (HI), AY0=DM(I5,M4);
						  {SR1=ya-yc, AY0 xd}
		 AF=AX0+AY0, AX1=DM(I7,M4);       {AF=xb+xd, AX1=yb}
		 AR=MR0+AF, AY1=DM(I7,M4);        {AR=xa+xc+xb+xd, AY1=yd}
		 DM(I0,M0)=AR, AR=MR0-AF;         {output x'a=xa+xc+xb+xd}
						  {AR=xa+xc-(xb+xd)}
		 DM(I1,M0)=AR, AF=AX1+AY1;        {output x'c to position x'b}
						  {AF=yb+yd}
		 AR=MR1+AF;                       {AR=ya+yb+yc+yd}
		 DM(I2,M0)=AR, AR=MR1-AF;         {output y'a=ya+yc+yb+yd}
						  {AR=ya+yc-(yb+yd)}
		 DM(I3,M0)=AR, AR=AX0-AY0;        {output y'c to position y'b}
						  {AR=xb-xd}
		 AX0=DM(I4,M4);                   {AX0=xa of next group}
		 AF=AX1-AY1, AY1=AR;              {AF=yb-yd, AY1=xb-xd}
		 AR=SR0+AF, AY0=DM(I4,M4);        {AR=xa-xc+yb-yd}
						  {AY0=xc of next group}
		 DM(I0,M0)=AR, AR=SR0-AF;         {output x'b to position x'c}
						  {AR=xa-xc-(yb-yd)}
		 DM(I1,M0)=AR, AR=SR1-AY1;        {output x'd=xa-xc-(yb-yd)}
						  {AR=ya-yc+(xb-xd)}
		 DM(I2,M0)=AR, AR=SR1+AY1;        {output y'b to position y'c}
						  {AR=ya-yc-(xb-xd)}
		 DM(I3,M0)=AR;                    {output y'd=ya-yc-(xb-xd)}
     
if not ce jump laststgbfy;
  
		 DIS  BIT_REV;                    {shut-off bit reverse}
		     
		 RTS;                             {exit from FFT subroutine} 


{_____________________Subroutine for middle stages___________________}

mid_stg:    
	      I5=^cos_table;
	      I6=^cos_table;
	      I7=^cos_table;
	      AX0=DM(I0,M0);                        {get first Xa}
	      AY0=DM(I0,M1);                        {get first Xc}
	      AR=AX0-AY0, AX1=DM(I2,M0);            {Xa-Xc,get first Ya}
	      SR=LSHIFT AR (LO), AY1=DM(I2,M1);     {SR1=Xa-Xc,get first Yc}
	    
	      CNTR = DM(BFY_COUNT);                 {butterflies/group }

	      M3=-2;               {M3 is loaded with the special value}
				   {required for pointer manipulation }
				   {as explained previously}
	 


  {This radix_4 bfly algorithm is a modified one.  It has it's middle 2  }
  {Branches reversed.  This alteration, done to every stage, results in  }
  {a bit reversed output instead of a digit reversed outputs             }

  midbfy:
		AR=AX0+AY0, AX0=DM(I1,M0);
			   {AR=xa+xc, AX0=xb }
		 MR0=AR, AR=AX1+AY1;
			   {MR0=xa+xc, AR=ya+yc}
		 MR1=AR, AR=AX1-AY1;
			   {MR1=ya+yc, AR=ya-yc}
		 SR=SR OR LSHIFT AR (HI), AY0=DM(I1,M1);
			   {SR1=ya-yc, AY0=xd}
		 AF=AX0+AY0, AX1=DM(I3,M0);
			   {AF=xb+xd, AX1=yb}
		 AR=MR0+AF, AY1=DM(I3,M1);
			   {AR=xa+xb+xc+xd, AY1=yd}
		 DM(I0,M0)=AR, AR=MR0-AF;
			   {output x'a=(xa+xb+xc+xd), AR=xa+xc-xb-xd}
		 AF=AX1+AY1, MX0=AR;
			   {AF=yb+yd, MX0=xa+xc-xb-xd}
		 AR=MR1+AF, MY0=PM(I6,M4);
			    {AR=ya+yc+yb+yd, MY0=(Cc)}
		 DM(I2,M0)=AR, AR=MR1-AF;
			   {output y'a, AR=ya+yc-yb-yd}
		 MR=MX0*MY0(SS), MY1=PM(I6,M6);
			   {MR=(xa+xc-xb-xd)(Cc), MY1=(Sc)}

		 MR=MR+AR*MY1(RND), SI=DM(I0,M2);
			   {MR=(xa-xb+xc-xd)(Cc)+(ya-yb+yc-yd)(Sc)}
			   {SI is a dummy here to cause a modify(I0,M2)}   
		 DM(I1,M0)=MR1, MR=AR*MY0(SS);
			   {output x'c to position x'b}
			   {MR=(ya+yc-yb-yd)(Cc)}
		 MR=MR-MX0*MY1(RND), MY0=PM(I5,M4);
			   {MR=(ya+yc-yb-yd)(Cc)-(xa+xc-xb-xd)(Sc), MY0=(Cb)}
		 DM(I3,M0)=MR1, AR=AX0-AY0;
			   {output y'c to position y'b}
			   {AR=xb-xd}
		 AY0=AR, AF=AX1-AY1;
			   {AY0=xb-xd, AF=yb-yd}
		 AR=SR0-AF, MY1=PM(I5,M5);
			   {AR=xa-xc-(yb-yd), MY1=(Sb)}
		 MX0=AR, AR=SR0+AF;
			   {MX0=xa-xc-yb+yd, AR=xa-xc+yb-yd}
		 SR0=AR, AR=SR1+AY0;
			   {SR0=xa-xc+yb-yd, AR=ya-yc+xb-xd}
		 MX1=AR, AR=SR1-AY0;
			   {MX1=ya-yc+xb-xd, AR=ya-yc-(xb-xd)}
	    
		 MR=SR0*MY0(SS),  AX0=DM(I0,M0);
			   {MR=(xa-xc+yb-yd)(Cb), AX0= xa of next bfly}
		 MR=MR+AR*MY1(RND),  AY0=DM(I0,M3);
			   {MR=(xa-xc+yb-yd)(Cb)+ (ya-yc-xb+xd)(Sb)}
			   {AY0= xc of next bfly}

		 DM(I0,M2)=MR1, MR=AR*MY0(SS);
			   {ouput x'b to position x'c}
			   {MR=ya-yc-xb+xd)(Cb)}
		 MR=MR-SR0*MY1(RND), MY0=PM(I7,M4);
			   {MR=(ya-yc-xb+xd)(Cb)-(xa-xc+yb-yd)(Sb)}
			   {MY0=(Cd)}
		 DM(I2,M2)=MR1, MR=MX0*MY0(SS);
			   {ouput y'b to position y'c}
			   {MR=(xa-yb-xc+yd)(Cd)}
		 MY1=PM(I7,M7), AR=AX0-AY0;
			    {MY1=(Sd), AR=xa-xc }  
		 MR=MR+MX1*MY1(RND), AX1=DM(I2,M0);
			   {MR=(xa-yb-xc+yd)(Cd)+(ya+xb-yc-xd)(sd)}
			   {AX1=ya of next bfly}
		 DM(I1,M2)=MR1, MR=MX1*MY0(SS);
			   {output x'd=(xa-yb-xc+yd)(Cd)+(ya+xb-yc-xd)(sd)}
			   {MR=(ya+yb-yc-yd)(Cd)}
		 MR=MR-MX0*MY1(RND), AY1=DM(I2,M1);
			   {MR=(ya+yb-yc-yd)(Cd)-(xa-xc-yb+yd)Sd}
			   {yc of next bfly}
		 DM(I3,M2)=MR1, SR=LSHIFT AR(LO);
			   {output y'd=(ya+xb-yc-xd)Cd-(xa-xc-yb+yd)Sd}
			   {SR0=ya-yc of next bfly}

	if not ce jump midbfy;
	
	   M3 = DM(M3_SPACE);     {modifier M3 is loaded with skip to next}
				  {group_count and is used in the next four}
				  {instructions}
	   

	   MODIFY (I0,M3);
	   MODIFY (I1,M3);                { point to next group }
	   MODIFY (I2,M3);                { of butterflies }
	   MODIFY (I3,M3);
if not ce jump mid_stg;
	RTS;         {return to middle stage calling code}

{__________________________END OF FFT.DSP_______________________________}
.endmod;

	 

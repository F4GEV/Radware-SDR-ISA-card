.MODULE/RAM/BOOT=1 FIR_ROUTINE;

.CONST TAPS=15;
.EXTRY FIR_START;

.EXTERNAL DATA_BUFFER, COEFFICIENT;



FIR_START:   CNTR = 14;
             SI = RX0;
             DM(IO,MO) = SI;
             MR=0, MY0=PM(I4,M4), MX0=DM(I0,M0); 


             DO CONVOLUTION UNTIL CE;
CONVOLUTION: MR=MR+MX0*MY0(SS),MY0=PM(I4,M4),MX0=DM(I0,M0);


             MR=MR+MX0*MY0(RND);
             IF MV SAT MR;
             TX0 = MR1;
             RTS;
.ENDMOD;
 
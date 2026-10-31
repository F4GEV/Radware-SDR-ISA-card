		DM(UNIQUE(PX))=PX;
		DM(UNIQUE(SB))=SB;
		DM(UNIQUE(L0))=L0;
		
		DM(UNIQUE(I1))=I1;
		DM(UNIQUE(L1))=L1;

		DM(UNIQUE(M2))=M2;
                DM(UNIQUE(M3))=M3;

		DM(UNIQUE(M5))=M5;
		DM(UNIQUE(L5))=L5;

		DM(UNIQUE(I6))=I6;
                DM(UNIQUE(M6))=M6;
		DM(UNIQUE(L6))=L6;

		M5=-1;
		put(AR,M5), AR=PASS AF;
		put(AR,M5);
		put(AY1,M5);
		put(MR2,M5);
		put(MR1,M5);
		put(MR0,M5); 
		MR0 = 0x8000; 
		MR  = MR0 * MF (UU);
		put(MR1,M5);
		put(MY1,M5);
		put(SR1,M5);
		put(SR0,M5);
		put(SE,M5);
		put(SI,M5);

		L0=0;
		L1=0;
		L5=0;
		L6=0;

                M2=0;
                M6=0;

                DIS BIT_REV, DIS AR_SAT, DIS AV_LATCH, ENA M_MODE;

		M5=DM(UNIQUE(next_handler));
		I6=DM(UNIQUE(handler));
		DM(UNIQUE(handler))=M5;

		AR=INTERRUPT_NAME;
		CALL (I6);

		I6=I4;			/* Point to open stack space	*/
		M5=1;			/* Used to increment up stack	*/
		MODIFY(I6,M5);		/* Point to first saved value	*/

		get(SI,M5);
		get(SE,M5);
		get(SR0,M5);
		get(SR1,M5);
		get(MY1,M5);
		get(MR1,M5);
		get(MR0,M5), MF=MR;
		get(MR1,M5);
		get(MR2,M5);
		get(AY1,M5);
		get(AR,M5);
		get(AR,M5), AF=PASS AR;

                I4 = I6;               /* restore stack pointer */
		M5 = -1;
                MODIFY(I4,M5);

		L0=DM(UNIQUE(L0));
		
		I1=DM(UNIQUE(I1));
		L1=DM(UNIQUE(L1));

		M2=DM(UNIQUE(M2));
                M3=DM(UNIQUE(M3));

		M5=DM(UNIQUE(M5));
		L5=DM(UNIQUE(L5));

		I6=DM(UNIQUE(I6));
                M6=DM(UNIQUE(M6));
		L6=DM(UNIQUE(L6));

		SB=DM(UNIQUE(SB));
		PX=DM(UNIQUE(PX));
		RTI;

.VAR/DM/RAM	UNIQUE(L0);
.VAR/DM/RAM	UNIQUE(I1);
.VAR/DM/RAM	UNIQUE(L1);
.VAR/DM/RAM     UNIQUE(M2);
.VAR/DM/RAM	UNIQUE(M3);
.VAR/DM/RAM	UNIQUE(M5);
.VAR/DM/RAM	UNIQUE(L5);
.VAR/DM/RAM	UNIQUE(I6);
.VAR/DM/RAM     UNIQUE(M6);
.VAR/DM/RAM	UNIQUE(L6);
.VAR/DM/RAM	UNIQUE(SB);
.VAR/DM/RAM	UNIQUE(PX);

.VAR/DM/RAM	UNIQUE(next_handler);
.VAR/DM/RAM	UNIQUE(handler);

.GLOBAL		UNIQUE(next_handler), UNIQUE(handler);


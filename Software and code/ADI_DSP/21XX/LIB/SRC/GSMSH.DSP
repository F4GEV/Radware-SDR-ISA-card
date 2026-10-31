/*
    	This shell sets up to processor for a call to an old-style
	assembly routine that encodes the speech buffer based on 
	GSM recommendations.

	The scale value allows for downshifting the input from the codec.
	
	Gordon A. Sterling		(617) 461 - 3076
	Development Tools Engineering

    	Created on    8/1/93

	int vad(void *speech, int scale);
*/

.MODULE/RAM	__Voice_Activity_Detector__;

#include 	"lib_glob.h";

.ENTRY		vad_;

.VAR/DM/RAM	__I4_save, __M4_save;
.VAR/DM/RAM	__vad_coeff_buffer[100];	/* Needs to be > 76	*/
.VAR/DM/RAM	__vad_scaled_speech[160];	/* Copy of input window	*/

.EXTERNAL	vad, dmr_encode;

vad_:		MR1=TOPPCSTACK;
		MR0=-1;
		JUMP ___lib_save_frame;

		DM(__I4_save)=I4;
		DM(__M4_save)=M4;

		DIS M_MODE;

		I0=^__vad_scaled_speech;/* Point to scratch buffer	*/
		I1=^__vad_coeff_buffer;	/* Point to coeffs buffer	*/
		I2=AR;			/* Point to speech buffer	*/
		SE=AY1;			/* Used for scaling speech buf	*/
				
		M0=0;			/* Setup M-registers for DMR	*/
		M1=1;
		M2=-1;
		M3=2;
		
		M4=0;
		M5=1;
		M6=-1;

		I3=I0;			/* Point to buffer to fill	*/
		CNTR=160;		/* Loop over entire window	*/
		DO __scale UNTIL CE;	/* Downshift into proper format	*/
		   SI=DM(I2,M1);	/* Read speech sample		*/
		   SR=ASHIFT SI (HI);	/* Shift sample as specified	*/
__scale:	   DM(I3,M1)=SR1;	/* Store shifted sample		*/
		
		CALL dmr_encode;

		I4=DM(__I4_save);
		M4=DM(__M4_save);
		
		ENA M_MODE;		/* Re-enable for C-environment	*/

		L0=0; L1=0; L2=0; L3=0;	/* Restore all L-regs to zero	*/
		L4=0; L5=0; L6=0; L7=0;	

		M2=0; M1=1; M6=0;	/* Restore required M-regs for C*/
		
		AR=DM(vad);		/* Read Voice Activity Flag	*/

__restore_state:MR0=-1;
		JUMP ___lib_restore_frame;

.ENDMOD;






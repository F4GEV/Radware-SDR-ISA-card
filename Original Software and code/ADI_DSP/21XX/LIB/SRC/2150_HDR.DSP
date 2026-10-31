.MODULE/ABS=0		ADSP2150_Runtime_Header;

.ENTRY		___lib_prog_term;
.EXTERNAL	___lib_setup_everything;
.EXTERNAL   	main_;

.EXTERNAL	___lib_int2_ctrl, ___lib_sp0x_ctrl, ___lib_sp0r_ctrl;
.EXTERNAL	___lib_int1_ctrl, ___lib_int0_ctrl, ___lib_tmri_ctrl;
.EXTERNAL	___lib_hipw_ctrl, ___lib_hipr_ctrl, ___lib_dacx_ctrl,___lib_adcr_ctrl;
.EXTERNAL       ___lib_pwdi_ctrl;

__Reset_vector:		CALL ___lib_setup_everything;
			CALL main_;			{Begin C program}
___lib_prog_term:	JUMP ___lib_prog_term;
			NOP;
__Interrupt2:		JUMP ___lib_int2_ctrl;NOP;NOP;NOP;
__Hip_write:		JUMP ___lib_hipw_ctrl;NOP;NOP;NOP;
__Hip_read:		JUMP ___lib_hipr_ctrl;NOP;NOP;NOP;
__Sport0_trans:		JUMP ___lib_sp0x_ctrl;NOP;NOP;NOP;
__Sport0_recv:		JUMP ___lib_sp0r_ctrl;NOP;NOP;NOP;
__Dac_trans:   		JUMP ___lib_dacx_ctrl;NOP;NOP;NOP;
__Adc_recv:   		JUMP ___lib_adcr_ctrl;NOP;NOP;NOP;
__Interrupt1:		JUMP ___lib_int1_ctrl;NOP;NOP;NOP;
__Interrupt0:		JUMP ___lib_int0_ctrl;NOP;NOP;NOP;
__Timer_interrupt:	JUMP ___lib_tmri_ctrl;NOP;NOP;NOP;
__Powerdown_interrupt:	JUMP ___lib_pwdi_ctrl;NOP;NOP;NOP;
.ENDMOD;






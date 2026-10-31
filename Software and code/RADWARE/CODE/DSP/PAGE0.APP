! this file contains int program memory subroutines/tables only

.MODULE/RAM/ABS=0 page0;

#include 1 <1848.h>
 



#const      MCE = 0x4000,             

 

#const         L_Input_Sel = 0x0,             
#const         R_Input_Sel = 0x100, 
#const        L_Aux1In_Sel = 0x200,             
#const        R_Aux1In_Sel = 0x300, 
#const        L_Aux2In_Sel = 0x400,             
#const        R_Aux2In_Sel = 0x500, 
#const        L_Output_Sel = 0x600,             
#const        R_Output_Sel = 0x700, 
#const    ClkDtaFormat_Sel = 0x800,             
#const      IntfConfig_Sel = 0x900,             
#const         PinCtrl_Sel = 0xa00,             
#const        TestInit_Sel = 0xb00,             
#const        MiscInfo_Sel = 0xc00,             
#const          DigMix_Sel = 0xd00,             
#const      Hi_BaseCnt_Sel = 0xe00,             
#const      Lo_BaseCnt_Sel = 0xf00,             

#const    ClkDtaFormat_Sel_MCE = 0x4800, 
 







 
#const      LineSel = 0x0,              
#const      Aux1Sel = 0x4000,              
#const       MicSel = 0x8000,              
#const       DACSel = 0xc000,              
#const    MicPreAmp = 0x2000,              
#const      GainSet = 0x100,              

#const     L_Input_Cfg = 0x8f00,   
#const     R_Input_Cfg = 0x8f00,   


  
#const     AuxMute = 0x8000,             
#const     AuxAttn = 0x100,             

#const     L_Aux1In_Cfg = 0x100, 
#const     R_Aux1In_Cfg = 0x100, 
#const     L_Aux2In_Cfg = 0x8000, 
#const     R_Aux2In_Cfg = 0x8000, 

#const    L_InputMUTE = 0, 
#const    R_InputMUTE = 0, 
#const     L_Aux1InMUTE = 0x8100, 
#const     R_Aux1InMUTE = 0x8100, 
#const     L_Aux2InMUTE = 0x10000, 
#const     R_Aux2InMUTE = 0x10000, 

  
#const     OutMute = 0x8000,             
#const     OutAttn = 0x100,             

#const     L_Output_Cfg = 0, 
#const     R_Output_Cfg = 0, 

#const     L_OutputMUTE = 0x8000, 
#const     R_OutputMUTE = 0x8000, 

  
#const     Unsigned8 = 0x0, 
#const         ULaw8 = 0x2000, 
#const      Signed16 = 0x4000,     
#const         ALaw8 = 0x6000, 
#const        Stereo = 0x1000, 
                
#const      COM8 = 0x0,            
#const     COM16 = 0x200,            
#const     COM27 = 0x400,            
#const     COM32 = 0x600,            
#const     COM48 = 0xc00,            
#const      COM9 = 0xe00,            
                 
#const      AUD5 = 0x100,            
#const     AUD11 = 0x300,            
#const     AUD18 = 0x500,            
#const     AUD22 = 0x700,            
#const     AUD37 = 0x900,            
#const     AUD44 = 0xb00,            
#const     AUD33 = 0xd00,            
#const      AUD6 = 0xf00,            

#const      ClkDtaFormat_Cfg = 0x5000, 
#const      Sample_8_KHz = 0x5000, 
#const      Sample_16_KHz = 0x5200, 


  
#const           PEN = 0x100,             
#const           CEN = 0x200,             
#const     SingleDMA = 0x400,             
#const          ACAL = 0x800,             
#const          PPIO = 0x4000,             
#const          CPIO = 0x8000,             

#const    IntfConfig_Cfg = 0, 

  
#const     XCTL1 = 0x8000, 
#const     XCTL0 = 0x4000, 
#const       IEN = 0x200, 

#const     PinCtrl_Cfg = 0, 


  
#const       CaptureOverrun = 0x8000, 
#const     PlaybackUnderrun = 0x4000, 
#const    AutoCALInProgress = 0x2000, 
#const          DataRequest = 0x1000, 
#const     R_Overrange = 0xc00, 
#const     L_Overrange = 0x300, 

 
#const     MixEnable = 0x100,             
#const       MixAttn = 0x400,             

         
#const     DigMixMUTE = 0x0,             

#const     Hi_BaseCnt_Cfg = 0x0,            ;Upper Base Count for DMA
#const     Lo_BaseCnt_Cfg = 0x0,            ;Lower Base Count for DMA

#const     PSS_WRITE_EMPTY = 0x8000, 
#const       PSS_READ_FULL = 0x4000, 
#const       PSS_DMA_READY = 0x2000, 
#const           PSS_FLAG1 = 0x1000, 
#const           PSS_FLAG0 = 0x800, 


#endinclude 6 "page0.dsp"

.external PSS_data_reg;
.external PSS_control_reg;
.external PSS_status_reg;
.external PSS_dma_reg;
.external ram_bank_reg;
.external ext_mem_latch;

.external SP_address_reg;
.external SP_data_reg;
.external SP_status_reg;
.external SP_pio_reg;
.external SP_dmal_reg;
.external SP_dmar_reg;
.external SP_enable_reg;

.external MUSIC0_reg;
.external MUSIC_dacl;
.external MUSIC1_reg;
.external MUSIC_dacr;
.external MUSIC_status_reg;
.external MUSIC_pc_status_reg;
.external MUSIC_divisor_reg;

.external SB_data_reg;
.external SB_status_reg;
.external SB_control_reg;

.external MIDI_data_reg;
.external MIDI_command_reg;
.external MIDI_status_reg;
.external MIDI_control_reg;
.external MIDI_uart;

.external UART_data_reg;
.external UART_status_reg;
.external UART_control_reg;

.external IRQ_status_reg;
.external IRQ_enable_reg;
 
.external PSS_config_reg;
.external WSS_config_reg;
.external SB_config_reg;
.external MIDI_config_reg;
.external CD_config_reg;

.external main_;
.external out_message;
.external out_message_number;
.external out_message_length;
.external out_message_overflow;
.external out_message_pointer;
.external important_message_number;
.external important_message_length;
.external important_message;
.external important_message_overflow;
.external message_number;
.external message_length;
.external message;
.external message_pointer;
.external TalkOn_;
.external USB_BUFFER_;
.external LSB_BUFFER_;
.external input_data;
.external output_data;
.external data_ready;
.external play_voice;
.external DitLength;
.external SendingMorse;
.external AnalogAGC;
.external AGC_On_;
.external AGC_Up;
.external AGC_Down;
.external fft_time;
.external LogOn_;
.external HannOn_;
.external mode;
.external Morse;
.external MorsePosition;
.external PlotUSBS;
.external PlotLSBS;
.external PlotUSBT;
.external PlotLSBT;
.external CurrentIFGain;
.external NewSetTone;
.external squelch;
.external ibm_data_ready;
.external old_i0;
.external NewIFGain;
.external old_i1;
.external old_l0;
.external mute;
.external Mute_Length;
.external old_m0;
.external data_from_ibm;
.external input_mute;
.external Sample_Rate;
.external sp_input_place;
.external New_Sample_Rate;
.external sp_output_place;
.external UART_config_reg;
.external winwave;

.var/dm/ram old_l1;

start: jump main_; nop; nop; nop;       
       jump Sound_Port_Int; nop; nop; nop;              
       rti; nop; nop; nop;              
       rti; nop; nop; nop;              
       rti; nop; nop; nop;              
       rti; nop; nop; nop;              
       rti; nop; nop; nop;              

Sound_Port_Int: ENA SEC_REG;
                dm(old_i0) = i0;
                dm(old_i1) = i1;
                dm(old_m0) = m0;
                dm(old_l0) = l0;
                dm(old_l1) = l1;
                ax0 = dm(New_Sample_Rate);
                ay0 = dm(Sample_Rate);
                ar = ax0-ay0;
                if eq jump same_sample_rate;
 




 
 







same_sample_rate:  ar = dm(NewIFGain);
                ar = pass ar;
                if lt jump get_sample;
                dm(CurrentIFGain) = ar;
                ax0 = -1;
                dm(NewIFGain) = ax0;
                ax0 = 0;
                dm(AnalogAGC) = ax0;
                ax0 = dm(Mute_Length);
                dm(mute) = ax0;
                my0 = 0x101;
                mr = ar*my0 (UU);
                mx0 = 0x8080;          
                ar = dm(winwave);
                ar = pass ar;
                if eq jump not_winwave;
                mx0 = 0x4040;
                my0 = 1;
                mr = mr+mx0*my0 (UU);
not_winwave:    my0 = dm(TalkOn_);
                mr = mr+mx0*my0 (UU);
                ax0 = 0x0;
                dm(SP_address_reg) = ax0;
                dm(SP_data_reg) = mr0;
                ax0 = 0x100;
                dm(SP_address_reg) = ax0;
                nop;
                dm(SP_data_reg) = mr0;
                ax1 = 0x9f9f;
                ax0 = 0x200;
                dm(SP_address_reg) = ax0;
                nop;
                dm(SP_data_reg) = ax1;
                ax0 = 0x300;
                dm(SP_address_reg) = ax0;
                nop;
                dm(SP_data_reg) = ax1;
                ax0 = 0x400;
                dm(SP_address_reg) = ax0;
                nop;
                dm(SP_data_reg) = ax1;
                ax0 = 0x500;
                dm(SP_address_reg) = ax0;
                nop;
                dm(SP_data_reg) = ax1;

get_sample:     ax0 = dm(IRQ_status_reg);
                ay0 = 0x2000;
                ar = ax0 and ay0;
                if eq jump check_ibm;
                i0 = dm(sp_input_place);
                i1 = dm(sp_output_place);
                m0 = 1;
                l0 = 0;
                l1 = 0;
                ax0 = dm(SP_dmal_reg);
                ax1 = dm(SP_dmar_reg);
                ay1 = dm(input_mute);
                ar = pass ay1;
                if eq jump no_input_mute;
                ax0 = 0;
                ax1 = 0;
                ar = ay1-1;
                dm(input_mute) = ar;
no_input_mute:  dm(i0,m0) = ax0;
                dm(i0,m0) = ax1;
do_mute:        ax0 = dm(i1,m0);
                ax1 = dm(i1,m0);
                ay0 = dm(mute);
                ar = pass ay0;
                if eq jump either_mute;
                ax0 = 0;
                ax1 = 0;
                ar = ay0-1;
                dm(mute) = ar;

either_mute:    dm(SP_dmal_reg)=ax0;
                dm(SP_dmar_reg)=ax1;
                ay0 = ^input_data+256;
                ax0 = i0;
                ar = ax0-ay0;
                if eq jump sp_switch_to_2nd_bank;
                ay0 = ^input_data+512;
                ar = ax0-ay0;
                if eq jump sp_switch_to_1st_bank;
end_switch:     dm(sp_input_place) = i0;
                dm(sp_output_place) = i1;
check_ibm:      ar = dm(ibm_data_ready);
                ar = pass ar;
                if ne jump write_ibm;
                ax0 = dm(PSS_status_reg);
                ay0 = 0x4000;
                ar = ax0 and ay0;
                if EQ jump write_ibm;
                ax0 = dm(PSS_data_reg);
                if eq jump write_ibm;
                dm(data_from_ibm) = ax0;
                ax0 = 1;
                dm(ibm_data_ready) = ax0;
write_ibm:      ax1 = dm(out_message_number);
                ar = pass ax1;
                if eq jump end_int;
                ax1=dm(PSS_status_reg);
                ay0=0x8000;
                ar=ax1 and ay0;
                if eq jump end_int;
                ax1 = dm(out_message_pointer);
                i0 = ax1;
                m0 = 1;
                l0 = 0;
                ax0 = dm(I0,M0);
                ay0 = ^out_message_number;
                ar = ax1-ay0;
                if ne jump not_out_number;
                ay0 = ^out_message_length;
                jump out_char_int;
not_out_number: ay0 = ^out_message_length;
                ar = ax1-ay0;
                if ne jump not_out_length;
                ay0 = ^out_message;
                jump out_char_int;
not_out_length: ay0 = i0;
out_char_int:   dm(PSS_data_reg) = ax0;
                dm(out_message_pointer) = ay0;
                ar = pass ax0;
                if ne jump not_zero_len;
                ay0 = ^out_message_length;
                ar = ax1 - ay0;
                if eq jump erase_out_msg;
not_zero_len:   ay0 = ^out_message-1;
                ax0 = dm(out_message_length);
                ar = ax0+ay0;
                ay0 = ar;
                ar = ax1-ay0;
                if ne jump end_int;
erase_out_msg:  ax0 = ^out_message_number;
                dm(out_message_pointer) = ax0;
                ax0 = dm(important_message_number);
                ar = pass ax0;
                if ne jump copy_important;
                ax0 = 0;
                dm(out_message_length) = ax0;
                dm(out_message_number) = ax0;
end_int:        i0 = dm(old_i0);
                i1 = dm(old_i1);
                m0 = dm(old_m0);
                l0 = dm(old_l0);
                l1 = dm(old_l1);
                DIS SEC_REG;
                rti;

copy_important:  ax0 = dm(important_message_length);
                 dm(out_message_length) = ax0;
                 ar = pass ax0;
                 if eq jump imp_length_zero;
                 cntr = ax0;
                 i0 = ^important_message;
                 i1 = ^out_message;
                 m0 = 1;
                 l0 = 0;
                 l1 = 0;
                 do copy_loop until ce;
                 ar = dm(I0,M0);
copy_loop:       dm(I1,M0) = ar;
imp_length_zero: ax0 = ^out_message_number;
                 dm(out_message_pointer) = ax0;
                 ar = dm(important_message_number);
                 dm(out_message_number) = ar;
                 ax0 = 0;
                 dm(important_message_number) = ax0;
                 dm(important_message_length) = ax0;
                 jump end_int;

sp_switch_to_2nd_bank: ax0 = 1;
                       dm(data_ready) = ax0;
                       jump end_switch;

sp_switch_to_1st_bank: ax0 = 2;
                       dm(data_ready) = ax0;
                       i0 = ^input_data;
                       i1 = ^output_data;
                       jump end_switch;

.ENDMOD;

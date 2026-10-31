# 1 "radware.dsp"
     


# 1 "d:\radware\code\dsp\1848.h" 1
 

.CONST MCE =   0x4000;            

 

.CONST L_Input_Sel      = 0x0000;            
.CONST R_Input_Sel      = 0x0100;
.CONST L_Aux1In_Sel     = 0x0200;            
.CONST R_Aux1In_Sel     = 0x0300;
.CONST L_Aux2In_Sel     = 0x0400;            
.CONST R_Aux2In_Sel     = 0x0500;
.CONST L_Output_Sel     = 0x0600;            
.CONST R_Output_Sel     = 0x0700;
.CONST ClkDtaFormat_Sel = 0x0800;            
.CONST IntfConfig_Sel   = 0x0900;            
.CONST PinCtrl_Sel      = 0x0A00;            
.CONST TestInit_Sel     = 0x0B00;            
.CONST MiscInfo_Sel     = 0x0C00;            
.CONST DigMix_Sel       = 0x0D00;            
.CONST Hi_BaseCnt_Sel   = 0x0E00;            
.CONST Lo_BaseCnt_Sel   = 0x0F00;            

.CONST ClkDtaFormat_Sel_MCE = 0x4800;
 
 
.CONST LineSel   = 0x0000;             
.CONST Aux1Sel   = 0x4000;             
.CONST MicSel    = 0x8000;             
.CONST DACSel    = 0xC000;             
.CONST MicPreAmp = 0x2000;             
.CONST GainSet   = 0x0100;             

.CONST L_Input_Cfg =  MicSel+GainSet+GainSet;
.CONST R_Input_Cfg =  MicSel+GainSet+GainSet;


  
.CONST AuxMute =  0x8000;            
.CONST AuxAttn =  0x0100;            

.CONST L_Aux1In_Cfg =  AuxAttn;
.CONST R_Aux1In_Cfg =  AuxAttn;
.CONST L_Aux2In_Cfg =  AuxMute;
.CONST R_Aux2In_Cfg =  AuxMute;

.CONST L_Aux1InMUTE =  L_Aux1In_Cfg+AuxMute;
.CONST R_Aux1InMUTE =  R_Aux1In_Cfg+AuxMute;
.CONST L_Aux2InMUTE =  L_Aux2In_Cfg+AuxMute;
.CONST R_Aux2InMUTE =  R_Aux2In_Cfg+AuxMute;

  
.CONST OutMute =  0x8000;            
.CONST OutAttn =  0x0100;            

.CONST L_Output_Cfg =  0;
.CONST R_Output_Cfg =  0;

.CONST L_OutputMUTE =  L_Output_Cfg+OutMute;
.CONST R_OutputMUTE =  R_Output_Cfg+OutMute;

  
.CONST Unsigned8 =  0x0000;
.CONST ULaw8     =  0x2000;
.CONST Signed16  =  0x4000;    
.CONST ALaw8     =  0x6000;
.CONST Stereo    =  0x1000;
                
.CONST COM8  =  0x0000;           
.CONST COM16 =  0x0200;           
.CONST COM27 =  0x0400;           
.CONST COM32 =  0x0600;           
.CONST COM48 =  0x0C00;           
.CONST COM9  =  0x0E00;           
                 
.CONST AUD5  =  0x0100;           
.CONST AUD11 =  0x0300;           
.CONST AUD18 =  0x0500;           
.CONST AUD22 =  0x0700;           
.CONST AUD37 =  0x0900;           
.CONST AUD44 =  0x0B00;           
.CONST AUD33 =  0x0D00;           
.CONST AUD6  =  0x0F00;           

.CONST ClkDtaFormat_Cfg  =  Signed16+Stereo+COM8;


  
.CONST PEN       =  0x0100;            
.CONST CEN       =  0x0200;            
.CONST SingleDMA =  0x0400;            
.CONST ACAL      =  0x0800;            
.CONST PPIO      =  0x4000;            
.CONST CPIO      =  0x8000;            

.CONST IntfConfig_Cfg = 0;

  
.CONST XCTL1 =  0x8000;
.CONST XCTL0 =  0x4000;
.CONST IEN   =  0x0200;

.CONST PinCtrl_Cfg =  0;


  
.CONST CaptureOverrun    = 0x8000;
.CONST PlaybackUnderrun  = 0x4000;
.CONST AutoCALInProgress = 0x2000;
.CONST DataRequest       = 0x1000;
.CONST R_Overrange =  0x0C00;
.CONST L_Overrange =  0x0300;

 
.CONST MixEnable =  0x0100;            
.CONST MixAttn   =  0x0400;            

         
.CONST DigMixMUTE =  0x0000;            

.CONST Hi_BaseCnt_Cfg =  0x0000;           ;Upper Base Count for DMA
.CONST Lo_BaseCnt_Cfg =  0x0000;           ;Lower Base Count for DMA

# 4 "radware.dsp" 2


.MODULE/RAM main_routine;

.PORT PSS_data_reg;
.PORT PSS_control_reg;
.PORT PSS_status_reg;
.PORT PSS_dma_reg;
.PORT ram_bank_reg;
.PORT ext_mem_latch;

.PORT SP_address_reg;
.PORT SP_data_reg;
.PORT SP_status_reg;
.PORT SP_pio_reg;
.PORT SP_dmal_reg;
.PORT SP_dmar_reg;
.PORT SP_enable_reg;

.PORT MUSIC0_reg;
.PORT MUSIC_dacl;
.PORT MUSIC1_reg;
.PORT MUSIC_dacr;
.PORT MUSIC_status_reg;
.PORT MUSIC_pc_status_reg;
.PORT MUSIC_divisor_reg;

.PORT SB_data_reg;
.PORT SB_status_reg;
.PORT SB_control_reg;

.PORT MIDI_data_reg;
.PORT MIDI_command_reg;
.PORT MIDI_status_reg;
.PORT MIDI_control_reg;
.PORT MIDI_uart;

.PORT UART_data_reg;
.PORT UART_status_reg;
.PORT UART_control_reg;

.PORT IRQ_status_reg;
.PORT IRQ_enable_reg;
.PORT PIO_reg;
.PORT PSS_config_reg;
.PORT WSS_config_reg;
.PORT SB_config_reg;
.PORT MIDI_config_reg;
.PORT CD_config_reg;
.PORT UART_config_reg;

.var  PIO_reg_data;

     jump main_; nop; nop; nop;       
     rti; nop; nop; nop;              
     rti; nop; nop; nop;               
     rti; nop; nop; nop;              
     rti; nop; nop; nop;              
     rti; nop; nop; nop;              
     rti; nop; nop; nop;              

 

main_:
     ax0 = dm(PSS_data_reg);      
       
     ICNTL=0;
     
     l0=0;l1=0;l2=0;l3=0;      
     l4=0;l5=0;l6=0;l7=0;      
     m0=0;m1=1;m2=0;m3=-1;     
     m5=1;m6=0;m7=-1;     
     
     ax0=0x3012;
     dm(0x3ffe) = ax0;      
     ax0=0x0418;            
     dm(0x3fff) = ax0;     
     
     ax1 = 0x0001 ;           
     call put_word_;
                    
     call SP_init;               

 




















     call SP_dma_start;      ! Start the 1848 going
     
     imask=0;
     ax0=0x2000;             ! Turn on 1848 interrupt
     dm(IRQ_enable_reg)=ax0;
     
     ax1=0x0000;
     ay1=0x100;

loop_:      
     dm(SP_dmal_reg)=ax1;
     dm(SP_dmar_reg)=ax1;
     ar = ax1+ay1;    
     ax1 = ar;
rwait_dac:  
     ax0=dm(IRQ_status_reg);
     ay0=0x2000;
     ar=ax0 and ay0;
     if eq jump rwait_dac;
     jump loop_;

put_word_:
     ax0=dm(PSS_status_reg);
     ay0=0x8000;
     ar=ax0 and ay0;
     if eq jump put_word_;
     dm(PSS_data_reg) = ax1;
     rts;
     
 








     



    
SP_init:
     ax0=1;
     dm(SP_enable_reg)=ax0;   

  

     imask=0;
     ay0=0x2000;
     dm(IRQ_enable_reg)=ay0;

     ax0=10000;
     af=pass ax0;

dma_detect_loop:
     ax0=dm(IRQ_status_reg);
     ay0=0x2000;
     ar=ax0 and ay0;
     if eq jump dec_counter;

     ax0=0x4900;
     dm(SP_address_reg)=ax0;
     ax0=0;
     dm(SP_data_reg)=ax0;   ! PEN and CEN off
     
     dm(SP_dmal_reg)=ax0;   ! Clear the ASIC interrupt
     dm(SP_dmar_reg)=ax0;
     jump mute_1848;

dec_counter:
     af=af-1;
     if gt jump dma_detect_loop;
     
mute_1848:
     ax0=0;
     dm(IRQ_enable_reg)=ax0;

     i0=^SP_address_reg;
     i1=^SP_data_reg;
     m0=0;
 
     call SP_ini_wait;        
     
     dm(i0,m0) =L_Input_Sel+MCE;     
                                  
     dm(i1,m0) =L_InputMUTE;
     dm(i0,m0) =R_Input_Sel+MCE;
     dm(i1,m0) =R_InputMUTE;
     dm(i0,m0) =L_Aux1In_Sel+MCE;
     dm(i1,m0) =L_Aux1InMUTE;
     dm(i0,m0) =R_Aux1In_Sel+MCE;
     dm(i1,m0) =R_Aux1InMUTE;
     dm(i0,m0) =L_Aux2In_Sel+MCE;
     dm(i1,m0) =L_Aux2InMUTE;
     dm(i0,m0) =R_Aux2In_Sel+MCE;
     dm(i1,m0) =R_Aux2InMUTE;
     dm(i0,m0) =L_Output_Sel+MCE;
     dm(i1,m0) =L_OutputMUTE;
     dm(i0,m0) =R_Output_Sel+MCE;
     dm(i1,m0) =R_OutputMUTE;
     dm(i0,m0) =DigMix_Sel+MCE;
     dm(i1,m0) =DigMixMUTE;
     
     dm(i0,m0) =Hi_BaseCnt_Sel+MCE;
     dm(i1,m0) =Hi_BaseCnt_Cfg;
     dm(i0,m0) =Lo_BaseCnt_Sel+MCE;
     dm(i1,m0) =Lo_BaseCnt_Cfg;
     
     dm(i0,m0) =IntfConfig_Sel+MCE;     
     ax0=dm(i1,m0) ;
     ay0=ACAL;
     ar=ax0 and ay0;
     if ne jump autocal_already_set;
     
     dm(i0,m0) =IntfConfig_Sel+MCE;     
     dm(i1,m0) =ACAL;
     
autocal_already_set:
     dm(i0,m0) =ClkDtaFormat_Sel+MCE;   
     dm(i1,m0) =ClkDtaFormat_Cfg;      
     call SP_autocalibrate;          
                                     
     
     dm(i0,m0) =IntfConfig_Sel+MCE;     
     dm(i1,m0) =0;
     call SP_autocalibrate;          
 
     dm(i0,m0) =PinCtrl_Sel;            
     dm(i1,m0) =PinCtrl_Cfg;
 
     dm(i0,m0) =L_Aux1In_Sel;
     dm(i1,m0) =L_Aux1In_Cfg;
     dm(i0,m0) =R_Aux1In_Sel;
     dm(i1,m0) =R_Aux1In_Cfg;
     dm(i0,m0) =L_Aux2In_Sel;
     dm(i1,m0) =L_Aux2In_Cfg;
     dm(i0,m0) =R_Aux2In_Sel;
     dm(i1,m0) =R_Aux2In_Cfg;
     dm(i0,m0) =L_Output_Sel;
     dm(i1,m0) =L_Output_Cfg;
     dm(i0,m0) =R_Output_Sel;
     dm(i1,m0) =R_Output_Cfg;
     dm(i0,m0) =L_Input_Sel;
     dm(i1,m0) =L_Input_Cfg;
     dm(i0,m0) =R_Input_Sel;
     dm(i1,m0) =R_Input_Cfg;

     ax0=0x600;
     dm(SP_address_reg)=ax0;
     ax0=dm(SP_data_reg);
     dm(SB_data_reg)=ax0;
     rts;

SP_autocalibrate:
     call SP_ini_wait;
     
     dm(i0,m0) =TestInit_Sel;
     
     ay0=AutoCALInProgress;
autocal_w1:                          
     ax0=dm(i1,m0) ;
     ar=ax0 and ay0;
     if EQ jump autocal_w1;
 
autocal_w2:                          
     ax0=dm(i1,m0) ;
     ar=ax0 and ay0;
     if NE jump autocal_w2;
     rts;    

SP_ini_wait:
    ay0=0x8000;       
iwxx:
    ax0=dm(SP_address_reg);
    ar=ax0 and ay0;
    if NE jump iwxx;
    rts;

SP_dma_start:
    ax0=IntfConfig_Sel;
    dm(SP_address_reg)=ax0;
    ax0=0x0300;           ! Turn on PEN and CEN
    dm(SP_data_reg)=ax0;  ! MCE not required for these two bits
    rts;

SP_dma_stop:
    ax0=IntfConfig_Sel;
    dm(SP_address_reg)=ax0;
    ax0=0x0000;           ! Turn off PEN and CEN
    dm(SP_data_reg)=ax0;  ! MCE not required for these two bits
    rts;



.ENDMOD;



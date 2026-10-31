{*****************************************************************************
* 1848 equates
*****************************************************************************}

.CONST MCE =   0x4000;           {Mode Change Bit in Index Addr Reg.}

{******* Index Register Set Ups to Select Indirectly Addressed Regs ******}

.CONST L_Input_Sel      = 0x0000;           {Left Input Control}
.CONST R_Input_Sel      = 0x0100;
.CONST L_Aux1In_Sel     = 0x0200;           {Left Aux#1 Input Control}
.CONST R_Aux1In_Sel     = 0x0300;
.CONST L_Aux2In_Sel     = 0x0400;           {Left Aux#2 Input Control}
.CONST R_Aux2In_Sel     = 0x0500;
.CONST L_Output_Sel     = 0x0600;           {Left Output Control}
.CONST R_Output_Sel     = 0x0700;
.CONST ClkDtaFormat_Sel = 0x0800;           {Clock and Data Format}
.CONST IntfConfig_Sel   = 0x0900;           {Interface Configuration}
.CONST PinCtrl_Sel      = 0x0A00;           {Pin Control}
.CONST TestInit_Sel     = 0x0B00;           {Test and Initialization}
.CONST MiscInfo_Sel     = 0x0C00;           {Misc. Information}
.CONST DigMix_Sel       = 0x0D00;           {Digital Mix}
.CONST Hi_BaseCnt_Sel   = 0x0E00;           {Upper Base Count for DMA}
.CONST Lo_BaseCnt_Sel   = 0x0F00;           {Lower Base Count for DMA}

.CONST ClkDtaFormat_Sel_MCE = 0x4800;
{
**********************************************************************
****** These are the configuration values for the Parallel Codec *****
*
*  The attempt here is to permit easy access to all features without
*  requiring the programmer to comprehend the device data sheet
***********************************************************************
}
{*** Left and Right Channel Input Source/Gain Selection ***}
.CONST LineSel   = 0x0000;            {LEFT LINE is DAA}
.CONST Aux1Sel   = 0x4000;            {this is line level input}
.CONST MicSel    = 0x8000;            {microphone input (electret OK)}
.CONST DACSel    = 0xC000;            {selects Post-Mixed DAC Output}
.CONST MicPreAmp = 0x2000;            {set = enable +20dB gain Mic Pre-Amp}
.CONST GainSet   = 0x0100;            {sets gain in +1.5dB steps (0-15 steps)}

.CONST L_Input_Cfg =  MicSel+GainSet+GainSet;
.CONST R_Input_Cfg =  MicSel+GainSet+GainSet;


 {*** Left and Right Aux #1/Aux #2 Input configuration ***}
.CONST AuxMute =  0x8000;           {set = mute enable}
.CONST AuxAttn =  0x0100;           {sets attn in -1.5dB steps (0-31 steps)}

.CONST L_Aux1In_Cfg =  AuxAttn;
.CONST R_Aux1In_Cfg =  AuxAttn;
.CONST L_Aux2In_Cfg =  AuxMute;
.CONST R_Aux2In_Cfg =  AuxMute;

.CONST L_Aux1InMUTE =  L_Aux1In_Cfg+AuxMute;
.CONST R_Aux1InMUTE =  R_Aux1In_Cfg+AuxMute;
.CONST L_Aux2InMUTE =  L_Aux2In_Cfg+AuxMute;
.CONST R_Aux2InMUTE =  R_Aux2In_Cfg+AuxMute;

 {*** Left and Right Output Control ***}
.CONST OutMute =  0x8000;           {set = mute enable}
.CONST OutAttn =  0x0100;           {attn in -1.5dB steps (0-63 steps)}

.CONST L_Output_Cfg =  0;
.CONST R_Output_Cfg =  0;

.CONST L_OutputMUTE =  L_Output_Cfg+OutMute;
.CONST R_OutputMUTE =  R_Output_Cfg+OutMute;

 {*** Clock and Data Format Configuration ***}
.CONST Unsigned8 =  0x0000;
.CONST ULaw8     =  0x2000;
.CONST Signed16  =  0x4000;   {Always use Signed16 + Stereo}
.CONST ALaw8     =  0x6000;
.CONST Stereo    =  0x1000;
               {  Comm sample rates - assume 24.576 MHz Osc. on XTAL1 }
.CONST COM8  =  0x0000;          { 8.0     kHz }
.CONST COM16 =  0x0200;          {16.0     kHz }
.CONST COM27 =  0x0400;          {27.42857 kHz }
.CONST COM32 =  0x0600;          {32.0     kHz }
.CONST COM48 =  0x0C00;          {48.0     kHz }
.CONST COM9  =  0x0E00;          { 9.6     kHz }
                { Audio (CD) sample rates - assume 16.9344 MHz Osc. on XTAL2}
.CONST AUD5  =  0x0100;          { 5.5125 kHz  }
.CONST AUD11 =  0x0300;          {11.025  kHz  }
.CONST AUD18 =  0x0500;          {18.9    kHz  }
.CONST AUD22 =  0x0700;          {22.05   kHz  }
.CONST AUD37 =  0x0900;          {37.8    kHz  }
.CONST AUD44 =  0x0B00;          {44.1    kHz  }
.CONST AUD33 =  0x0D00;          {33.075  kHz  }
.CONST AUD6  =  0x0F00;          { 6.615  kHz  }

.CONST ClkDtaFormat_Cfg  =  Signed16+Stereo+COM8;


 {*** Interface Configuration Register Set up ***}
.CONST PEN       =  0x0100;           {set = playback enabled}
.CONST CEN       =  0x0200;           {set = capture  enabled}
.CONST SingleDMA =  0x0400;           {set = single ch. DMA mode}
.CONST ACAL      =  0x0800;           {autocal}
.CONST PPIO      =  0x4000;           {playback via PIO only}
.CONST CPIO      =  0x8000;           {capture  via PIO only}

.CONST IntfConfig_Cfg = 0;

 {*** Pin Control Register}
.CONST XCTL1 =  0x8000;
.CONST XCTL0 =  0x4000;
.CONST IEN   =  0x0200;

.CONST PinCtrl_Cfg =  0;


 {*** Test and Initialization Register(Read-Only Status) ***}
.CONST CaptureOverrun    = 0x8000;
.CONST PlaybackUnderrun  = 0x4000;
.CONST AutoCALInProgress = 0x2000;
.CONST DataRequest       = 0x1000;
.CONST R_Overrange =  0x0C00;
.CONST L_Overrange =  0x0300;

{ *** Digital Mix Control Configuraton *** }
.CONST MixEnable =  0x0100;           {0 == Mute, 1 == enable}
.CONST MixAttn   =  0x0400;           { attn -1.5dB steps (0-63 steps) }

{ DigMix_Cfg         equ  (63*MixAttn) }       {max attn, mix disabled }
.CONST DigMixMUTE =  0x0000;           {Mute the Digital Mix feature }

.CONST Hi_BaseCnt_Cfg =  0x0000;           ;Upper Base Count for DMA
.CONST Lo_BaseCnt_Cfg =  0x0000;           ;Lower Base Count for DMA


/* RADWARE library header

  This headers has been documented to help you see how to use each function.
It is unfortunate, but this version will only work for C++. This is because
I wrote the standard RADWARE program in C++, and as you can see by the
quantity of classes, find it to be quite useful. I will make the full DLL as
doon as I can, and I have been working on that. This library is actually a
direct result of such work. It has come along automatically, as sort of the
first pass.
  The standard RADWARE program that comes with your card is now 10,175 lines
of code. (not including headers, RC, sym,... files) This code is distributed
over 23 files. As a result, when a variable or classhas been named for a long
time, and would now make sense to have a different name, I haven't touched
it. The following is a list of classes that may be confusing in this manner
   You'd Expect                 Really Called
   VCOA_Control                 Frequency_Control
   VCOB_Control                 TX_Frequency_Control
   Analog_Gain_Control          IF_Gain_Control
   RX_ANTENNA_POWER_Control     ANTENNA_POWER_Control
   TX_ANTENNA_POWER_Control     TXANTENNA_POWER_Control
  To run a program thst has been compiled with this library, you must have
already installed the standard RADWARE program. This is due to the fact that
this library looks in the DSPMGR.INI file for the location of RADIO.LD
program which is then loaded into the sound card. */

#ifndef __DSP1LIB__H
#define __DSP1LIB__H

_CLASSDEF(Radio_Control);  /* This is the root class for all classes that
                              follow. By using these classes, you can
                              create a control routine that changes whichever
                              control was passed to it as a class. A word
                              about the Mute & Restore Controls are in order
                              here. They are in the base class because they
                              are needed in the Volume control. In all other
                              class, they perform no function.*/

class Radio_Control

{
public:
  virtual char *Get_Name(void);
  virtual char *Get_Error_Message(void);
  virtual long Get_Value(void);
  virtual int Set_Value(long);
  virtual long Get_Min(void);
  virtual long Get_Max(void);
  virtual void Mute(void);
  virtual void Restore(void);
};

_CLASSDEF(Int_Control);              /* This class was added to encapsulate
                                        an integer. The class does not
                                        modify anything in the radio. It's
                                        sole function (For me, at least) Is
                                        to allow my routine for placing knobs
                                        on the screen to access a variable
                                        instead of a radio function. It is
                                        included here for completeness, and
                                        because I think some of you might
                                        find it useful. */

class Int_Control : public Radio_Control

{
  int Min,Max,Value;

public:
  Int_Control(int min,int max);
  virtual char *Get_Name(void);
  virtual char *Get_Error_Message(void);
  virtual long Get_Value(void);
  virtual int Set_Value(long);
  virtual long Get_Min(void);
  virtual long Get_Max(void);
  virtual void Mute(void);
  virtual void Restore(void);
};

_CLASSDEF(SQUELCH_Control);

class SQUELCH_Control : public Radio_Control

{
public:
  virtual char *Get_Name(void);
  virtual char *Get_Error_Message(void);
  virtual long Get_Value(void);
  virtual int Set_Value(long);           /* If this function returns 0, one
                                            likely cause is that the frequency
                                            is not inside valid amateur bands. */
  virtual long Get_Min(void);
  virtual long Get_Max(void);
  virtual void Mute(void);
  virtual void Restore(void);
};

_CLASSDEF(PTT_Control);

class PTT_Control : public Radio_Control

{
public:
  virtual char *Get_Name(void);
  virtual char *Get_Error_Message(void);
  virtual long Get_Value(void);
  virtual int Set_Value(long);           /* If this function returns 0, one
                                            likely cause is that the frequency
                                            is not inside valid amateur bands. */
  virtual long Get_Min(void);
  virtual long Get_Max(void);
  virtual void Mute(void);
  virtual void Restore(void);
};

_CLASSDEF(Monitor_Control);    /* Monitor control is the feedback to the
                                  speakers when in Transmit. These controls
                                  can be used while in Receive, but you will
                                  notice no change until you enter Transmit.
                                  If used in Transmit, change will take place
                                  immediately, of course.*/

class Monitor_Control : public Radio_Control

{
public:
  virtual char *Get_Name(void);
  virtual char *Get_Error_Message(void);
  virtual long Get_Value(void);
  virtual int Set_Value(long);
  virtual long Get_Min(void);
  virtual long Get_Max(void);
  virtual void Mute(void);
  virtual void Restore(void);
};

_CLASSDEF(Split_Control);         /* This controls which VCO the library
                                     looks at when deciding to change
                                     frequency. These VCOs are accessed via
                                     Frequency_Control and
                                     TX_Frequency_Control. */

class Split_Control : public Radio_Control

{
public:
  virtual char *Get_Name(void);
  virtual char *Get_Error_Message(void);
  virtual long Get_Value(void);
  virtual int Set_Value(long);
  virtual long Get_Min(void);
  virtual long Get_Max(void);
  virtual void Mute(void);
  virtual void Restore(void);
};

_CLASSDEF(Compression_Control);

class Compression_Control : public Radio_Control

{
public:
  virtual char *Get_Name(void);
  virtual char *Get_Error_Message(void);
  virtual long Get_Value(void);
  virtual int Set_Value(long);
  virtual long Get_Min(void);
  virtual long Get_Max(void);
  virtual void Mute(void);
  virtual void Restore(void);
};

_CLASSDEF(Compression_Level_Control);

class Compression_Level_Control : public Radio_Control

{
public:
  virtual char *Get_Name(void);
  virtual char *Get_Error_Message(void);
  virtual long Get_Value(void);
  virtual int Set_Value(long);
  virtual long Get_Min(void);
  virtual long Get_Max(void);
  virtual void Mute(void);
  virtual void Restore(void);
};

_CLASSDEF(Mic_Level_Control);

class Mic_Level_Control: public Radio_Control

{
public:
  virtual char *Get_Name(void);
  virtual char *Get_Error_Message(void);
  virtual long Get_Value(void);
  virtual int Set_Value(long);
  virtual long Get_Min(void);
  virtual long Get_Max(void);
  virtual void Mute(void);
  virtual void Restore(void);
};

_CLASSDEF(Volume_Control);     /* The Volume control is done inside the
                                  ADSP 2115. Although I litterally have
                                  65536 choices, most of them are pointless
                                  to have. (Who wants to go from x63.998 to
                                  63.999?) This allowed me to choose
                                  multipliers for my steps that followed a
                                  logrithimic scale rather than the liner
                                  model. */

class Volume_Control: public Radio_Control

{
public:
  virtual char *Get_Name(void);
  virtual char *Get_Error_Message(void);
  virtual long Get_Value(void);
  virtual int Set_Value(long);
  virtual long Get_Min(void);
  virtual long Get_Max(void);
  virtual void Mute(void);
  virtual void Restore(void);
};

_CLASSDEF(AGC_Control);

class AGC_Control: public Radio_Control

{
public:
  virtual char *Get_Name(void);
  virtual char *Get_Error_Message(void);
  virtual long Get_Value(void);
  virtual int Set_Value(long);
  virtual long Get_Min(void);
  virtual long Get_Max(void);
  virtual void Mute(void);
  virtual void Restore(void);
};

_CLASSDEF(IF_GAIN_Control);         /*This is called Analog Gain in the
                                      RADWARE program. The RADWARE program
                                      started out with this called IF Gain
                                      as well, but when it was pointed out
                                      that there isn't an IF because it is
                                      a direct conversion reciever, we felt
                                      it was better to change it over to
                                      what it really is. The Analog Gain
                                      control changes the Gain on the AD1848
                                      (This device includes your DAC, ADC,
                                      some amplifiers, an anti-aliasing
                                      filter, and some other stuff just to
                                      fill it out.) As such, this is the
                                      gain after it has been downconverted,
                                      but before it has been digitized. */
                                    /*NOTE: This function has 2 different
                                      functions depending on whether the card
                                      is in transmit or reciever. During
                                      transmit, the input to the 1848 moves
                                      to the front of the reciever & is then
                                      amplifing/attenuating the mic input
                                      dirrectly before digitization. As such,
                                      to facilitate different values being
                                      given to both functions, when this
                                      class is called, only the gain of the
                                      current PTT state is affected. To
                                      help you in saving the current settings
                                      to disk, Set_TX_IF_Gain() &
                                      Get_TX_IF_Gain() are available.
                                      See Below  */


class IF_GAIN_Control: public Radio_Control

{
public:
  virtual char *Get_Name(void);
  virtual char *Get_Error_Message(void);
  virtual long Get_Value(void);
  virtual int Set_Value(long);
  virtual long Get_Min(void);
  virtual long Get_Max(void);
  virtual void Mute(void);
  virtual void Restore(void);
};

_CLASSDEF(ANTENNA_POWER_Control);    /* This is the RX version */

class ANTENNA_POWER_Control: public Radio_Control

{
public:
  virtual char *Get_Name(void);
  virtual char *Get_Error_Message(void);
  virtual long Get_Value(void);
  virtual int Set_Value(long);
  virtual long Get_Min(void);
  virtual long Get_Max(void);
  virtual void Mute(void);
  virtual void Restore(void);
};

_CLASSDEF(TXANTENNA_POWER_Control);

class TXANTENNA_POWER_Control: public Radio_Control

{
public:
  virtual char *Get_Name(void);
  virtual char *Get_Error_Message(void);
  virtual long Get_Value(void);
  virtual int Set_Value(long);
  virtual long Get_Min(void);
  virtual long Get_Max(void);
  virtual void Mute(void);
  virtual void Restore(void);
};

_CLASSDEF(Frequency_Control);  /* This is VCO A. This class also serves as the
                                  baises for any other control that is found
                                  in a resizable numbered window. This class
                                  will NOT allow you the change frequency
                                  while you are in Transmit. */

class Frequency_Control: public Radio_Control

{
public:
  virtual char *Get_Name(void);
  virtual char *Get_Error_Message(void);
  virtual long Get_Value(void);
  virtual int Set_Value(long);
  virtual long Get_Min(void);
  virtual long Get_Max(void);
  virtual void Mute(void);
  virtual void Restore(void);
};

_CLASSDEF(AM_Frequency_Offset_Control); /* Even though the AM Offset is not
                                           shown in a standard frequency type
                                           window, in the early days it was.
                                           As a result, it is still using
                                           Frequency_Control as its base.
                                           This should not affect anything,
                                           and is just in here as a historical
                                           reference. */

class AM_Frequency_Offset_Control: public Frequency_Control

{
public:
  virtual char *Get_Name(void);
  virtual char *Get_Error_Message(void);
  virtual long Get_Value(void);
  virtual int Set_Value(long);
  virtual long Get_Min(void);
  virtual long Get_Max(void);
  virtual void Mute(void);
  virtual void Restore(void);
};

_CLASSDEF(TX_Frequency_Control);     /* this is VCO B. Again, this is a class
                                        that has seen a name change between
                                        first implementation and the current
                                        version, */

class TX_Frequency_Control: public Frequency_Control

{
public:
  virtual char *Get_Name(void);
  virtual char *Get_Error_Message(void);
  virtual long Get_Value(void);
  virtual int Set_Value(long);
  virtual long Get_Min(void);
  virtual long Get_Max(void);
  virtual void Mute(void);
  virtual void Restore(void);
};

_CLASSDEF(Max_Frequency_Control);    /* This function controls what
                                        Frequency_Control &
                                        TX_Frequency_Control will accept as
                                        an input. */

class Max_Frequency_Control: public Frequency_Control

{
public:
  virtual char *Get_Name(void);
  virtual char *Get_Error_Message(void);
  virtual long Get_Value(void);
  virtual int Set_Value(long);
  virtual long Get_Min(void);
  virtual long Get_Max(void);
  virtual void Mute(void);
  virtual void Restore(void);
};

_CLASSDEF(Min_Frequency_Control);    /* This function controls what
                                        Frequency_Control &
                                        TX_Frequency_Control will accept as
                                        an input. */

class Min_Frequency_Control: public Frequency_Control

{
public:
  virtual char *Get_Name(void);
  virtual char *Get_Error_Message(void);
  virtual long Get_Value(void);
  virtual int Set_Value(long);
  virtual long Get_Min(void);
  virtual long Get_Max(void);
  virtual void Mute(void);
  virtual void Restore(void);
};

_CLASSDEF(I_ADJUST_Control);  /* Allows amplification/attenuation I (of I/Q)
                                 to allow calibrate of the two. Unless
                                 extreme accuracy is required, this class
                                 is no longer neccessary as improvements to
                                 the production methods have eliminated the
                                 vast discrepencies we where seeing in the
                                 I/Q amplifiers. Still, if you have use for
                                 it, here it is. (This is a calculation
                                 done inside the DSP to correct for the
                                 offset, not an actual hardware level change*/

class I_ADJUST_Control : public Frequency_Control

{
  int Min,Max,Value;

public:
  virtual char *Get_Name(void);
  virtual char *Get_Error_Message(void);
  virtual long Get_Value(void);
  virtual int Set_Value(long);
  virtual long Get_Min(void);
  virtual long Get_Max(void);
  virtual void Mute(void);
  virtual void Restore(void);
};

_CLASSDEF(Calibrate_Control);      /* For calibrating the DDS to the computer
                                      crystal. Standard clock crystal on the
                                      bus is 14.31818 */

class Calibrate_Control: public Frequency_Control

{
public:
  virtual char *Get_Name(void);
  virtual char *Get_Error_Message(void);
  virtual long Get_Value(void);
  virtual int Set_Value(long);
  virtual long Get_Min(void);
  virtual long Get_Max(void);
  virtual void Mute(void);
  virtual void Restore(void);
};

_CLASSDEF(FILTER_Control);       /* Many different filters are expected soon.
                                    If enough are created, this control many
                                    have to be modified. */

class FILTER_Control: public Radio_Control

{
public:
  virtual char *Get_Name(void);
  virtual char *Get_Error_Message(void);
  virtual long Get_Value(void);
  virtual int Set_Value(long);
  virtual long Get_Min(void);
  virtual long Get_Max(void);
  virtual void Mute(void);
  virtual void Restore(void);
};

_CLASSDEF(MIXER_GAIN_Control);          /* Mixer gain is implemented on the
                                           RADWARE board. This is a hardware
                                           control */

class MIXER_GAIN_Control: public Radio_Control

{
public:
  virtual char *Get_Name(void);
  virtual char *Get_Error_Message(void);
  virtual long Get_Value(void);
  virtual int Set_Value(long);
  virtual long Get_Min(void);
  virtual long Get_Max(void);
  virtual void Mute(void);
  virtual void Restore(void);
};

_CLASSDEF(MODE_Control);      /* different types of modes are expected to
                                 become available in the near future */

class MODE_Control: public Radio_Control

{
public:
  virtual char *Get_Name(void);
  virtual char *Get_Error_Message(void);
  virtual long Get_Value(void);
  virtual int Set_Value(long);
  virtual long Get_Min(void);
  virtual long Get_Max(void);
  virtual void Mute(void);
  virtual void Restore(void);
};

void get_1848();
int get_agc();
int get_filter();
int set_tx_analog_gain(int value);
int get_tx_analog_gain();
void Set_Morse(int how);
int Valid_Xmit(int display_error);
long GetNumDevs(void);
char convert_from_baudot(int which);
void save_voice_memory_start(char *label,char *filename,int which);
void save_voice_memory_end();
void save_voice_memory();
int save_voice_memory_open();
void start_play();
void end_play();
void end_play_and_null();
UINT send_play_message(int *string);

#endif // __DSP1LIB_H

/*********************** Tables of settings ***************************
There is no code beyond this point

AGC Settings
  0 = OFF
  1 = ON
  2 = FAST

Filter Settings
  0 = 50 Hz
  1 = 200 Hz
  2 = 400 Hz
  3 = 1800 Hz
  4 = 2700 Hz
  5 = 16000 Hz
*/

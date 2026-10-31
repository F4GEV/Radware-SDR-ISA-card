#include <string.h>

/* RADWARE library header

  This headers has been documented to help you see how to use each function.
It is unfortunate, but this version will only work for C++. This is because
I wrote the standard RADWARE program in C++, and as you can see by the
quantity of classes, find it to be quite useful.
  The standard RADWARE program that comes with your card is now 10,175 lines
of code. (not including headers, RC, sym,... files) This code is distributed
over 23 files. As a result, when a variable or class has been named for a long
time, and would now make sense to have a different name, I haven't touched
it. The following is a list of classes that may be confusing in this manner
   You'd Expect                 Really Called
   VCOA_Control                 Frequency_Control
   VCOB_Control                 TX_Frequency_Control
   Analog_Gain_Control          IF_Gain_Control
   RX_ANTENNA_POWER_Control     ANTENNA_POWER_Control
   TX_ANTENNA_POWER_Control     TXANTENNA_POWER_Control
  To run a program that has been compiled with this library, you must have
the file RADWARE.LD in the active directory. Please note that you must call
LOAD_DSP before any other function or the DSP will not be able to recieve 
data. It is this function that loads RADWARE.LD into the dsp card.*/

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

_CLASSDEF(Custom_Filter_Min_Control);   /* This sets the lower edge of the
                                           custom filter  */

class Custom_Filter_Min_Control : public Radio_Control

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

_CLASSDEF(Custom_Filter_Max_Control);   /* This sets the upper edge of the
                                           custom filter  */

class Custom_Filter_Max_Control : public Radio_Control

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

_CLASSDEF(TX_I_ADJUST_Control);  /* Allows amplification/attenuation I (of I/Q)
												to allow calibrate of the two. Unless
												extreme accuracy is required, this class
												is no longer neccessary as improvements to
												the production methods have eliminated the
												vast discrepencies we where seeing in the
												I/Q amplifiers. Still, if you have use for
												it, here it is. (This is a calculation
												done inside the DSP to correct for the
												offset, not an actual hardware level change*/

class TX_I_ADJUST_Control : public Frequency_Control

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

extern int lsb_string[];
extern int usb_string[];
extern int lsb_time_string[];
extern int usb_time_string[];
extern int smeter_string[];
extern int frequency_calibrate_string[];
extern int analog_string[];
extern int packet_string[];
extern int get_1848_string[];

#define GET_1848_STRING_LENGTH 32

int DSPGetMsg();  /* this routine polls the DSP card to see if there are
                       any messages.
                     Return:   Message
                       0         No message
                       101       usb spectrum analyzer updated
                                   new data will be in usb_string.
                                   256 bytes, byte 0 is 0 to 1/4000 hz,
                                   byte 1 is  1/4000 to 2/4000 hz, etc.
                       102       lsb spectrum analyzer updated
                                   new data will be in lsb_string.
                                   see above
                       103       usb time analyzer updated
                                   new data will be in usb_time_string
                                   256 bytes of data.
                       104       lsb time analyzer updated
                                   new data will be in lsb_time_string
                                   256 bytes of data.
                       105       smeter updated
                                   new data will be in smeter_string[0]
                       112       frequency calibration data updated
                                   new data is in frequency_calibrate_string[0].
                                   the only true meaning of this value tends to
                                   be its sign.
                       113       AGC change requested/notified
                                   new data in in analog_string.
                                   analog_string[0] is the current volume
                                     multiplier to create the correct level.
                                     (This setting is the finest available,
                                     and is automatically calculated by the
                                     dsp card.)
                                   analog_string[1] requests that the PC
                                     either increase, decrease or hold the
                                     mixer gain. You must follow this request
                                     if you wish normal AGC operation.
                                     (0 is hold, -1 is decrease, 2 is increase.)
                                     the DSP does NOT know if the limit has
                                     been reached.
                                   analog_string[2] notifies the program of
                                     the current IF Gain level. The DSP has
                                     direct control over this function and
                                     as such, just relays what it has done.
                                 Note: in transmit, these function will
                                   relate to the compression level instead of
                                   the analog gain level.
                       116       new data from data mode (Not Operational)
                       120       diagnostics data
                                 this is really just a personal tool,
                                 but is described here to keep this file
                                 complete.
                                   data is in get_1848_string.
                                     [0]    1848 left input control
                                     [1]    1848 right input control
                                     [2]    1848 left aux #1 input control
                                     [3]    1848 right aux #1 input control
                                     [4]    1848 left aux #2 input control
                                     [5]    1848 right aux #2 input control
                                     [6]    1848 left DAC control
                                     [7]    1848 right DAC control
                                     [8]    1848 clock and data format register
                                     [9]    1848 interface configuration register
                                     [10]   1848 pin control register
                                     [11]   1848 test and initialization register
                                     [12]   1848 Miscellaneous control
                                     [13]   1848 digital mix register
                                     [14]   1848 upper base control
                                     [15]   1848 lower base control
                                     [16]   Volume  (after calculations)
                                     [17]   AGC
                                     [18]   Noise Level
                                     [19]   Squelch (after calculations)
                                     [20]   I adjust
                                     [21]   PTT (0 = recv, 1 = xmit)
                  */

int get_agc();
int get_filter();
int set_tx_analog_gain(int value);
int get_tx_analog_gain();
void set_port_on_off(int value);    /* Set whether or not the xmit
				       module is operational & linked
				       to the parallel port. */
int get_port_on_off();
int Valid_Xmit(int display_error);  /* Test if the active frequency
				       is acceptable to transmit on.
				       display_error controls whether
				       or not the user is notified
				       if frequency is unacceptable */
void Simulation_Mode();   /* causes this library not to talk to
			     the DSP sound card. */

long Load_DSP();  /* loads RADWARE.LD into the sound card. This must be
			 done before any other calls to this library. The
			 exception to this is you may call Simulation Mode
			 first if you dont want to talk to the sound card.
			 HWND is the handle of a window that you want to
			 recieve all messages about the radio events.
          Load_DSP return -2 if RADWARE.LD not found,
          anything else is ok.*/

void Set_Frequency_Wrap(int value);     /* Sets whether or not the frequency
					   will wrap arround when a band edge
					   is reached.
					   value     result
					     0       no wrap
					     1       wrap arround  */

int Get_Frequency_Wrap();     /* returns current frequency wrap value */

void Set_CallSign(char *);    /* Tells the dll the current callsign.
				 If this routine is not called or an
				 invalid callsign is entered, the dll
				 will not allow transmit. */

void Get_CallSign(char *);    /* Retrieves the current callsign */

int Get_Sound_Card();         /* returns the current address of the
				 DSP card (Sound Card). It will
				 either be 0x220 or 0x240 */

int Is_Frequency_Amateur(long frequency);  /* return 1 is frequency is
					      in an amateur band, else
					      returns 0 */  
void Set_DSP_Card(int which); /* 0 sets library for dsp 100,
                                 1 sets library for dsp 1D */

                                 void set_winwave(int value);  /* 0 sets the dsp card to default
                                 1 sets the dsp card to the winwave card */

int get_winwave();            /* return value of dsp card (see above)*/
#endif // __DSP1LIB_H

/*********************** Tables of settings ***************************
There is no code beyond this point

AGC Settings
  0 = OFF
  1 = ON
  2 = FAST

Analog Gain
  0..15

Compression
  0 Off
  1 On

Compression Level
  0..63

Filter Settings
  0 = 50 Hz
  1 = 200 Hz
  2 = 400 Hz
  3 = 1800 Hz
  4 = 2700 Hz
  5 = 16000 Hz

Mic Gain
  0..63

Mixer Gain
  0  -10 DB Attn
  1  0 DB
  2  10 DB  Amp

Mode
  0 AM
  1 LSB
  2 USB
  3 ISB

Monitor
  0..36 in 1.5 DB steps (Exception: 0 is mute)

PTT
  0 OFF
  1 ON

RX DC Antenna Power
  0 OFF
  1 ON

Split
  0 VCO A
  1 VCO B
  2 A/B
  3 B/A

TX DC Antenna Power
  0 OFF
  1 ON

Volume
  0..63
*/

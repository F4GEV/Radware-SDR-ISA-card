#include "thisCls.h"
#include <stdio.h>

extern int get_1848_string[];
extern int ptt_via_joystick;
extern int old_key;
extern int key_config;
extern int save_on_exit;
extern int Sound_Card;
extern int Game_Port_Address;
extern int leading_zeros,frequency_wrap,stations_reset_bands;
extern char CallSign[];
extern int dds_control_address,dds_freq_address,preselector_address;

TDIAGNOSTICDlg::TDIAGNOSTICDlg(TWindow *AParent,LPSTR AName)
				  :TDialog(AParent,"Diagnostics")

{
  int count;

  SetCaption(AName);
  for(count=0;count<22;count++)
    Static_1848[count] = new TStatic(this,102+count,20);
}

TDIAGNOSTICDlg::~TDIAGNOSTICDlg()

{
}

void TDIAGNOSTICDlg::Request_Update()

{
  get_1848();
}

void TDIAGNOSTICDlg::Recieved_1848()

{
  char string[20];
  int count;
  unsigned value;

  for(count=0;count<16;count++)
  {
    value = get_1848_string[count];
    value /= 256;
    sprintf(string,"%02X",value);
    Static_1848[count]->SetText(string);
  }
  for(count=16;count<22;count++)
  {
    value = get_1848_string[count];
    sprintf(string,"%04X",value);
    Static_1848[count]->SetText(string);
  }
}

DEFINE_RESPONSE_TABLE1(TDIAGNOSTICDlg,TDialog)
  EV_COMMAND(101,Request_Update),
END_RESPONSE_TABLE;




#include "thisCls.h"
#include <stdio.h>

unsigned int dip_switch[8] = {0x200,0x100,0x80,0,0x8,0x40,0x20,0x10};

extern int ptt_via_joystick;
extern int old_key;
extern int key_config;
extern int save_on_exit;
extern int Sound_Card;
extern int Game_Port_Address;
extern int leading_zeros,stations_reset_bands;
extern int dds_control_address,dds_freq_address,preselector_address;
extern PI_ADJUST_Control i_adjust_control;
extern PTX_I_ADJUST_Control tx_i_adjust_control;

TCONFIGUREDlg::TCONFIGUREDlg(TWindow *AParent,LPSTR AName)
				  :TDialog(AParent,"CONFIGURE")

{
  int count;

  SetCaption(AName);
  Leading_Zeros = new TCheckBox(this,101);
  Frequency_Wrap = new TCheckBox(this,102);
  Stations_Reset_Bands = new TCheckBox(this,103);
  for(count=0;count<8;count++)
    Scroll[7-count] = new TScrollBar(this,104+count);
  Address = new AEdit(this,112);
  callsign = new TEdit(this,113,19);
  I_Adjust = new AEdit(this,114);
  PTT_Joystick = new TRadioButton(this,116,NULL);
  Sound_220 = new TRadioButton(this,117,NULL);
  Sound_240 = new TRadioButton(this,118,NULL);
  No_Key = new TRadioButton(this,119,NULL);
  One_Key = new TRadioButton(this,120,NULL);
  Two_Keys = new TRadioButton(this,121,NULL);
  TX_I_Adjust = new AEdit(this,123);
  Save_On_Exit = new TRadioButton(this,124,NULL);
  Transmit_Filters = new TRadioButton(this,125,NULL);
  ignore_address = 0;
  PTI_ADJUST = NULL;
  PTTX_I_ADJUST = NULL;
}

TCONFIGUREDlg::~TCONFIGUREDlg()

{
}

void TCONFIGUREDlg::SetupWindow()

{
  int count;
  char string[80];

  TDialog::SetupWindow();
  if (leading_zeros)
	 Leading_Zeros->Check();
  if (Get_Frequency_Wrap())
	 Frequency_Wrap->Check();
  if (stations_reset_bands)
	 Stations_Reset_Bands->Check();
  for(count=0;count<8;count++)
  {
	 Scroll[count]->SetRange(0,1);
	 if (dip_switch[count] == 0)
		Scroll[count]->SetPosition(0);
	 else
		if (dds_freq_address&dip_switch[count])
		  Scroll[count]->SetPosition(1);
		else
		  Scroll[count]->SetPosition(0);
  }
  DIP_SWITCH();
  Get_CallSign(string);
  callsign->SetText(string);
  if (ptt_via_joystick)
	 PTT_Joystick->SetCheck(1);
  else
	 PTT_Joystick->SetCheck(0);
  if (save_on_exit)
	 Save_On_Exit->SetCheck(1);
  else
	 Save_On_Exit->SetCheck(0);
  if (Sound_Card == 0x240)
  {
	 Sound_220->SetCheck(0);
    Sound_240->SetCheck(1);
  }
  else
  {
    Sound_220->SetCheck(1);
    Sound_240->SetCheck(0);
  }
  I_Adjust->Set_Value(i_adjust_control->Get_Value());
  TX_I_Adjust->Set_Value(tx_i_adjust_control->Get_Value());
  No_Key->SetCheck(0);
  One_Key->SetCheck(0);
  Two_Keys->SetCheck(0);
  switch(key_config)
  {
	 case 1  : One_Key->SetCheck(1);
				  break;
	 case 2  : Two_Keys->SetCheck(1);
				  break;
	 default : No_Key->SetCheck(1);
				  break;
  }
  if (get_port_on_off())
    Transmit_Filters->SetCheck(1);
  else
    Transmit_Filters->SetCheck(0);
}

void TCONFIGUREDlg::DIP_SWITCH()

{
  int count,address;
  char string[80];

  address = 0;
  for(count=0;count<8;count++)
  {
    if (Scroll[count]->GetPosition()==1)
      address+=dip_switch[count];
  }
  if (Scroll[3]->GetPosition() == 1)
    Scroll[3]->SetPosition(0);
  ignore_address = 1;
  sprintf(string,"%X",address);
  Address->SetText(string);
}

void TCONFIGUREDlg::EvHScroll(UINT scrollCode,UINT thumbPos,HWND hWndCtl)

{
  TDialog::EvHScroll(scrollCode,thumbPos,hWndCtl);
  DIP_SWITCH();
}

void TCONFIGUREDlg::ADDRESS()

{
  int count;
  unsigned int address;
  char string[80];

  if (ignore_address)
  {
    ignore_address = 0;
    return;
  }
  Address->GetText(string,79);
  if (string[0] == 0)
    address = 0;
  else
    sscanf(string,"%X",&address);
  for(count=0;count<8;count++)
  {
    Scroll[count]->SetRange(0,1);
    if (dip_switch[count] == 0)
      Scroll[count]->SetPosition(0);
    else
      if (address&dip_switch[count])
		  Scroll[count]->SetPosition(1);
      else
        Scroll[count]->SetPosition(0);
  }
  if (address>0x3f8)
  {
    address/=16;
    if (address>0x3f8)
      address/=16;
    sprintf(string,"%X",0x3f8-address);
    Address->SetText(string);
  }
}

void TCONFIGUREDlg::I_ADJUST_WINDOW()

{
  if (PTI_ADJUST==NULL)
  {
	 PTI_ADJUST = new TFreq_Window2(this,200,"RX Sideband Adjust",i_adjust_control,
											  55,14,1,0);
	 PTI_ADJUST->Create();
  }
}

void TCONFIGUREDlg::TX_I_ADJUST_WINDOW()

{
  if (PTTX_I_ADJUST==NULL)
  {
	 PTTX_I_ADJUST = new TFreq_Window2(this,201,"TX Sideband Adjust",tx_i_adjust_control,
											  55,15,1,0);
	 PTTX_I_ADJUST->Create();
  }
}

void TCONFIGUREDlg::OK()

{
  int count,address;
  char string[80];

  if (Leading_Zeros->GetCheck()==BF_UNCHECKED)
	 leading_zeros = 0;
  else
	 leading_zeros = 1;
  if (Frequency_Wrap->GetCheck()==BF_UNCHECKED)
	 Set_Frequency_Wrap(0);
  else
	 Set_Frequency_Wrap(1);
  if (Stations_Reset_Bands->GetCheck()==BF_UNCHECKED)
	 stations_reset_bands = 0;
  else
	 stations_reset_bands = 1;
  if (PTT_Joystick->GetCheck()==BF_CHECKED)
	 ptt_via_joystick = 1;
  else
	 ptt_via_joystick = 0;
  if (Save_On_Exit->GetCheck()==BF_CHECKED)
	 save_on_exit = 1;
  else
	 save_on_exit = 0;
  if (Sound_220->GetCheck()==BF_CHECKED)
	 Sound_Card = 0x220;
  else
	 Sound_Card = 0x240;
  old_key = 0;
  outport(Game_Port_Address,inport(Sound_Card+0x10)|0x0400);
  callsign->GetText(string,19);
  Set_CallSign(string);
  i_adjust_control->Set_Value(I_Adjust->Get_Value_Long());
  tx_i_adjust_control->Set_Value(TX_I_Adjust->Get_Value_Long());
  address = 0;
  for(count=0;count<8;count++)
  {
	 if (Scroll[count]->GetPosition()!=0)
		address+=dip_switch[count];
  }
  dds_freq_address = address;
  preselector_address = address+2;
  dds_control_address = address+4;
  if (One_Key->GetCheck()==BF_CHECKED)
	 key_config = 1;
  else
	 if (Two_Keys->GetCheck()==BF_CHECKED)
		key_config = 2;
	 else
		key_config = 0;
  if (Transmit_Filters->GetCheck()==BF_CHECKED)
    set_port_on_off(1);
  else
    set_port_on_off(0);
  CloseWindow(IDOK);
}

void TCONFIGUREDlg::UPDATE_I_ADJUST()

{
  PTI_ADJUST->Check_For_Repaint();
  I_Adjust->Set_Value(i_adjust_control->Get_Value());
}

void TCONFIGUREDlg::UPDATE_TX_I_ADJUST()

{
  PTTX_I_ADJUST->Check_For_Repaint();
  TX_I_Adjust->Set_Value(tx_i_adjust_control->Get_Value());
}

DEFINE_RESPONSE_TABLE1(TCONFIGUREDlg,TDialog)
  EV_BN_CLICKED(199,OK),
  EV_WM_HSCROLL,
  EV_EN_CHANGE(112,ADDRESS),
  EV_BN_CLICKED(115,I_ADJUST_WINDOW),
  EV_COMMAND(200,UPDATE_I_ADJUST),
  EV_BN_CLICKED(122,TX_I_ADJUST_WINDOW),
  EV_COMMAND(201,UPDATE_TX_I_ADJUST),
END_RESPONSE_TABLE;




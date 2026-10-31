#include "thisCls.h"
#include <stdio.h>

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
extern TCallSign *PT_CallSign;
extern short int delete_class[];

TCONFIGUREDlg::TCONFIGUREDlg(TWindow *AParent,LPSTR AName)
				  :TDialog(AParent,"CONFIGURE")

{
  int count;

  SetCaption(AName);
  Leading_Zeros = new TCheckBox(this,101);
  Frequency_Wrap = new TCheckBox(this,102);
  Stations_Reset_Bands = new TCheckBox(this,103);
  callsign = new TEdit(this,113,19);
  I_Adjust = new AEdit(this,114);
  PTT_Joystick = new TRadioButton(this,116,NULL);
  TX_I_Adjust = new AEdit(this,123);
  Save_On_Exit = new TRadioButton(this,124,NULL);
  Transmit_Filters = new TRadioButton(this,125,NULL);
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
  I_Adjust->Set_Value(i_adjust_control->Get_Value());
  TX_I_Adjust->Set_Value(tx_i_adjust_control->Get_Value());
  if (get_port_on_off())
	 Transmit_Filters->SetCheck(1);
  else
    Transmit_Filters->SetCheck(0);
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
  old_key = 0;
  outport(Game_Port_Address,inport(Sound_Card+0x10)|0x0400);
  callsign->GetText(string,19);
  if (PT_CallSign && delete_class[18] == 0)
    PT_CallSign->Update_CallSign(string);
  else
    Set_CallSign(string);
  i_adjust_control->Set_Value(I_Adjust->Get_Value_Long());
  tx_i_adjust_control->Set_Value(TX_I_Adjust->Get_Value_Long());
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
  EV_BN_CLICKED(115,I_ADJUST_WINDOW),
  EV_COMMAND(200,UPDATE_I_ADJUST),
  EV_BN_CLICKED(122,TX_I_ADJUST_WINDOW),
  EV_COMMAND(201,UPDATE_TX_I_ADJUST),
END_RESPONSE_TABLE;




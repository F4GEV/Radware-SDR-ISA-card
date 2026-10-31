
     /********************************************************************
      *                                                                  *
      *   Source File: ENTER_FR.cpp                                      *
      *   Description: Dialog implementation for TEMP                    *
      *   Date:        Tue Jun 08 09:30:36 1993                          *
      *                                                                  *
      ********************************************************************/

#include "thisCls.h"
#include <stdio.h>

TENTER_FREQUENCYDlg::TENTER_FREQUENCYDlg(TWindow *AParent,
                LPSTR AName,PFrequency_Control control)
               :TDialog(AParent,"ENTER_FREQUENCY")

{
  Control = control;
  SetCaption(AName);
  Edit117 = new TEdit(this, 117, 20);
//  MHz_Button = new TButton(this,115);
}

TENTER_FREQUENCYDlg::~TENTER_FREQUENCYDlg()

{
   delete Edit117;
//   delete MHz_Button;
}

void TENTER_FREQUENCYDlg::RTENTER_FREQUENCY102()

{
  Edit117->Insert("0");
//  MHz_Button->SendMessage(WM_ACTIVATE,TRUE,NULL);
  Edit117->SendMessage(WM_ACTIVATE,TRUE,NULL);
}

void TENTER_FREQUENCYDlg::RTENTER_FREQUENCY103()

{
  Edit117->Insert("1");
  Edit117->SendMessage(WM_ACTIVATE,TRUE,NULL);
}

void TENTER_FREQUENCYDlg::RTENTER_FREQUENCY104()

{
  Edit117->Insert("2");
  Edit117->SendMessage(WM_ACTIVATE,TRUE,NULL);
}

void TENTER_FREQUENCYDlg::RTENTER_FREQUENCY105()

{
  Edit117->Insert("3");
  Edit117->SendMessage(WM_ACTIVATE,TRUE,NULL);
}

void TENTER_FREQUENCYDlg::RTENTER_FREQUENCY106()

{
  Edit117->Insert("4");
  Edit117->SendMessage(WM_ACTIVATE,TRUE,NULL);
}

void TENTER_FREQUENCYDlg::RTENTER_FREQUENCY107()

{
  Edit117->Insert("5");
  Edit117->SendMessage(WM_ACTIVATE,TRUE,NULL);
}

void TENTER_FREQUENCYDlg::RTENTER_FREQUENCY108()

{
  Edit117->Insert("6");
  Edit117->SendMessage(WM_ACTIVATE,TRUE,NULL);
}

void TENTER_FREQUENCYDlg::RTENTER_FREQUENCY109()

{
  Edit117->Insert("7");
  Edit117->SendMessage(WM_ACTIVATE,TRUE,NULL);
}

void TENTER_FREQUENCYDlg::RTENTER_FREQUENCY110()

{
  Edit117->Insert("8");
  Edit117->SendMessage(WM_ACTIVATE,TRUE,NULL);
}

void TENTER_FREQUENCYDlg::RTENTER_FREQUENCY111()

{
  Edit117->Insert("9");
  Edit117->SendMessage(WM_ACTIVATE,TRUE,NULL);
}

void TENTER_FREQUENCYDlg::RTENTER_FREQUENCY112()

{
  Edit117->Insert(".");
  Edit117->SendMessage(WM_ACTIVATE,TRUE,NULL);
}

void TENTER_FREQUENCYDlg::Hz()

{
  long value;
  char Text_Field[20];

  Edit117->GetText(Text_Field,20);
  value = atof(Text_Field);
  if (Control->Set_Value(value))
    CloseWindow();
  else
  {
    MessageBox(Control->Get_Error_Message(),"Error",MB_OK | MB_ICONHAND);
    ::SendMessage(Edit117->HWindow,WM_ACTIVATE,TRUE,NULL);
  }
}

void TENTER_FREQUENCYDlg::KHz()

{
  double value;
  char Text_Field[20];

  Edit117->GetText(Text_Field,20);
  value = atof(Text_Field);
  value *=1000;
  if (Control->Set_Value(value))
	 CloseWindow();
  else
  {
    MessageBox(Control->Get_Error_Message(),"Error",MB_OK | MB_ICONHAND);
    ::SendMessage(Edit117->HWindow,WM_ACTIVATE,TRUE,NULL);
  }
}

void TENTER_FREQUENCYDlg::MHz()

{
  double value;
  char Text_Field[20];

  Edit117->GetText(Text_Field,20);
  value = atof(Text_Field);
  value *= 1000000.0;
  if (Control->Set_Value(value))
    CloseWindow();
  else
  {
    MessageBox(Control->Get_Error_Message(),"Error",MB_OK | MB_ICONHAND);
    ::SendMessage(Edit117->HWindow,WM_ACTIVATE,TRUE,NULL);
  }
}

void TENTER_FREQUENCYDlg::Close()

{
  CloseWindow();
}

void TENTER_FREQUENCYDlg::Check_Edit()

{
  char Text_Field[20];
  char last_char;

  Edit117->GetText(Text_Field,20);
  if (strlen(Text_Field)>0)
  {
    last_char = Text_Field[strlen(Text_Field)-1];
    if (isalpha(last_char))
    {
      Text_Field[strlen(Text_Field)-1]=0;
      Edit117->SetText(Text_Field);
      switch (last_char)
      {
        case 'm' :
        case 'M' : PostMessage(WM_COMMAND,115,NULL);
                   break;
        case 'k' :
        case 'K' : PostMessage(WM_COMMAND,114,NULL);
                   break;
        case 'h' :
        case 'H' : PostMessage(WM_COMMAND,113,NULL);
                   break;
        case 'c' :
        case 'C' : Close();
                   break;
      }
    }
  }
}

DEFINE_RESPONSE_TABLE1(TENTER_FREQUENCYDlg,TDialog)
  EV_BN_CLICKED(102,RTENTER_FREQUENCY102),
  EV_BN_CLICKED(103,RTENTER_FREQUENCY103),
  EV_BN_CLICKED(104,RTENTER_FREQUENCY104),
  EV_BN_CLICKED(105,RTENTER_FREQUENCY105),
  EV_BN_CLICKED(106,RTENTER_FREQUENCY106),
  EV_BN_CLICKED(107,RTENTER_FREQUENCY107),
  EV_BN_CLICKED(108,RTENTER_FREQUENCY108),
  EV_BN_CLICKED(109,RTENTER_FREQUENCY109),
  EV_BN_CLICKED(110,RTENTER_FREQUENCY110),
  EV_BN_CLICKED(111,RTENTER_FREQUENCY111),
  EV_BN_CLICKED(112,RTENTER_FREQUENCY112),
  EV_BN_CLICKED(113,Hz),
  EV_BN_CLICKED(114,KHz),
  EV_BN_CLICKED(115,MHz),
  EV_BN_CLICKED(116,Close),
  EV_EN_CHANGE(117,Check_Edit),
END_RESPONSE_TABLE;


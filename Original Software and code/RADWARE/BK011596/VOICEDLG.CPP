#include "thisCls.h"
#include <stdio.h>

extern struct voice_memory voice_memories[];

TVOICEDlg::TVOICEDlg(TWindow *AParent, int which)
               :TDialog(AParent,"VOICE")

{
  char AName[80];

  Which = which;
  sprintf(AName,"Voice Memory %d",which);
  SetCaption(AName);
  Mic_Record = new TRadioButton(this,111,NULL);
  Air_Record = new TRadioButton(this,112,NULL);
  Speaker_Play = new TRadioButton(this,103,NULL);
  Air_Play = new TRadioButton(this,104,NULL);
  Label_Edit = new TEdit(this, 105, 5);
  Filename_Edit = new TEdit(this, 107, 79);
}

TVOICEDlg::~TVOICEDlg()

{
  delete Label_Edit;
  delete Filename_Edit;
  delete Mic_Record;
  delete Air_Record;
  delete Speaker_Play;
  delete Air_Play;
}

void TVOICEDlg::SetupWindow()

{
  TDialog::SetupWindow();
  if (!voice_memories[Which].record)
    Mic_Record->Check();
  else
    Air_Record->Check();
  if (!voice_memories[Which].play)
    Speaker_Play->Check();
  else
    Air_Play->Check();
  Label_Edit->SetText(voice_memories[Which].label);
  Filename_Edit->SetText(voice_memories[Which].filename);
}

void TVOICEDlg::Respond_Mic_Record()

{
  CheckRadioButton(111,112,111);
}

void TVOICEDlg::Respond_Air_Record()

{
  CheckRadioButton(111,112,112);
}

void TVOICEDlg::Respond_Speaker_Play()

{
  CheckRadioButton(103,104,103);
}

void TVOICEDlg::Respond_Air_Play()

{
  CheckRadioButton(103,104,104);
}

void TVOICEDlg::Respond_Record()

{
  char label[5],filename[80];

  Label_Edit->GetText(label,4);
  Filename_Edit->GetText(filename,79);
  if (label[0]!=0 && filename[0]!=0)
  {
    if (save_voice_memory_start(filename,Which))
    {
  		strcpy(voice_memories[Which].label,label);
		strcpy(voice_memories[Which].filename,filename);
    }
    MessageBox("Recording, press OK to end.","Record",MB_OK/* | MB_SYSTEMMODAL*/);
    save_voice_memory_end();
  }
}

void TVOICEDlg::OK()

{
  if (Mic_Record->GetCheck()==BF_UNCHECKED)
	 voice_memories[Which].record = 1;
  else
	 voice_memories[Which].record = 0;
  if (Speaker_Play->GetCheck()==BF_UNCHECKED)
	 voice_memories[Which].play = 1;
  else
	 voice_memories[Which].play = 0;
  Label_Edit->GetText(voice_memories[Which].label,5);
  Filename_Edit->GetText(voice_memories[Which].filename,79);
  CloseWindow();
}

DEFINE_RESPONSE_TABLE1(TVOICEDlg,TDialog)
  EV_BN_CLICKED(101,OK),
  EV_BN_CLICKED(111,Respond_Mic_Record),
  EV_BN_CLICKED(112,Respond_Air_Record),
  EV_BN_CLICKED(103,Respond_Speaker_Play),
  EV_BN_CLICKED(104,Respond_Air_Play),
  EV_BN_CLICKED(106,Respond_Record),
END_RESPONSE_TABLE;


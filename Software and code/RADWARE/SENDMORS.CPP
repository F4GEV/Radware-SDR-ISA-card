#include <thiscls.h>
#include <stdio.h>
#define MAX_CHARACTORS 44
#define TO_SEND_BUFFER_LENGTH 80

void dly();

char morse_dit_dahs[MAX_CHARACTORS][9]={{1,2,0},{2,1,1,1,0},{2,1,2,1,0},{2,1,1,0},{1,0},
			{1,1,2,1,0},{2,2,1,0},{1,1,1,1,0},{1,1,0},{1,2,2,2,0},{2,1,2,0},
			{1,2,1,1,0},{2,2,0},{2,1,0},{2,2,2,0},{1,2,2,1,0},{2,2,1,2,0},
			{1,2,1,0},{1,1,1,0},{2,0},{1,1,2,0},{1,1,1,2,0},{1,2,2,0},
			{2,1,1,2,0},{2,1,2,2,0},{2,2,1,1},{1,2,2,2,2,0},{1,1,2,2,2,0},
			{1,1,1,2,2,0},{1,1,1,1,2,0},{1,1,1,1,1,0},{2,1,1,1,1,0},
			{2,2,1,1,1,0},{2,2,2,1,1,0},{2,2,2,2,1,0},{2,2,2,2,2,0},
			{1,2,1,2,1,2,0},{2,2,1,1,2,2,0},{1,1,2,2,1,1,0},{2,1,1,2,1,0},
			{2,1,1,1,2,0},{1,2,1,2,1,0},{1,1,1,1,1,1,1,1,0},{1,1,1,2,1,2}};

char morse_charactors[MAX_CHARACTORS+1]="ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890.,?/=+-*";

extern int starting_places[][2];
extern int delete_class[];
extern char morse_button_names[][6];
extern char morse_button_macros[][41];

extern TCONTROLChld *PTPTT;
extern PPTT_Control ptt_control;
extern TBitmap *sheet_bitmap[];
extern TBitmap *tune_bitmap[];

TSEND_MORSEChld::TSEND_MORSEChld(TWindow *AParent,int id,LPSTR Title,int child_number,int w,int h):
                                 AWindow(AParent,Title,child_number)

{
  int x,y,count;

  Child_Number = child_number;
  x = starting_places[Child_Number][0];
  y = starting_places[Child_Number][1];
  if (y<0)
    y = -y;
  stream = NULL;
  Attr.X = x;
  Attr.Y = y;
  Attr.W = w;
  Attr.H = h;
  ID = id;
  Attr.Style = WS_POPUP | WS_VISIBLE | WS_CAPTION |WS_MINIMIZEBOX|WS_SYSMENU;
  char_being_sent = new TStatic(this,-1,"",3,3,15,20,1);
  char_to_send = new TEdit(this,102,"",18,-1,w-98,26,TO_SEND_BUFFER_LENGTH);
  new TStatic(this,-1,"WPM:",w-78,5,40,18,4);
  WPM = new AEdit(this,103,"12",w-36,1,32,22);
//  new ABitmapButton(this,105,"",w-41,25,20,24,sheet_bitmap[0],sheet_bitmap[1]);
  new ABitmapButton(this,107,"",w-21,25,20,24,tune_bitmap[0],tune_bitmap[1]);
  new AButton(this,108,"CQ",0,25,50,24,FALSE);
  for(count=0;count<6;count++)
	 Macro_Buttons[count] = new AButton(this,110+count,morse_button_names[count],count*50+49,25,50,24,FALSE);
  morse_off = 1;
  tuning = 0;
}

void TSEND_MORSEChld::SetupWindow()

{
  AWindow::SetupWindow();
  Change_WPM();
}

TSEND_MORSEChld::~TSEND_MORSEChld()

{
  Attr.Y *= -1;
  if (stream!=NULL)
  {
	 fclose(stream);
    stream = NULL;
  }
  if (Attr.Y == 0)
    Attr.Y = -1;
  starting_places[Child_Number][0] = Attr.X;
  starting_places[Child_Number][1] = Attr.Y;
  delete_class[Child_Number] = 1;
}

void TSEND_MORSEChld::EvActivate(uint active,bool b,HWND c)

{
  AWindow::EvActivate(active,b,c);
//  PostMessage(WM_COMMAND,201,NULL);
}

void TSEND_MORSEChld::Edit_Check()

{
  char string[TO_SEND_BUFFER_LENGTH+1];

  char_to_send->GetText(string,TO_SEND_BUFFER_LENGTH);
  if (string[0] != 0 && !Valid_Xmit(0))
  {
    char_to_send->SetText("");
    return;
  }
  if (string[strlen(string)-1] == '`')
  {
    string[strlen(string)-1] = 'C';
    PostMessage(WM_COMMAND,104,NULL);
    char_to_send->SetText(string);
  }
  if (morse_off)
  {
	 if (Valid_Xmit(0))
		if (PTPTT == NULL || PTPTT->Attr.H<0)
		  ptt_control->Set_Value(1);
		else
		  PTPTT->Set_Value(1);
	 morse_off = 0;
	 dly();
	 dly();
	 Send_Next_Char();
  }
}

void TSEND_MORSEChld::Send_Next_Char()

{
  char message[30],edit_string[TO_SEND_BUFFER_LENGTH+1];
  int count,charactor;
  char sent[2]=" ";

  if (tuning)
  {
    Add_Tune_Char();
	 return;
  }
  for(count=0;count<30;count++)
    message[count] = 0;
  do
  {
    char_to_send->GetText(edit_string,TO_SEND_BUFFER_LENGTH);
    edit_string[0] = toupper(edit_string[0]);
    for(charactor=0;charactor<MAX_CHARACTORS;charactor++)
      if (edit_string[0] == morse_charactors[charactor])
      {
        count = 0;
        while (morse_dit_dahs[charactor][count]!=0)
        {
          message[count*4] = 9+morse_dit_dahs[charactor][count]*2;
          message[count*4+2] = 9;
          count++;
        }
        message[count*4] = 0;
		  message[count*4+2] = 0;
        break;
		}
    if (edit_string[0] == ' ')
    {
      message[0] = 8;
      charactor = -1;
    }
    if (strlen(edit_string)>0)
    {
      char_to_send->SetText(edit_string+1);
      char_to_send->SetSelection(strlen(edit_string)-1,strlen(edit_string)-1);
    }
  }
  while (charactor==MAX_CHARACTORS && strlen(edit_string)>1);
  if (charactor<MAX_CHARACTORS)
  {
    char_being_sent->GetText(sent,2);
    Last_Char = sent[0];
	 ::PostMessage(Parent->HWindow,WM_COMMAND,ID,NULL);
    sent[0] = edit_string[0];
	 char_being_sent->SetText(sent);
    message[count*4-2] = 7;
    DSPSendMsg(msg_MORSE_CHAR,(int *) message,28);
    for(count=0;count<28;count++)
      message[count]+='A';
  }
  else
  {
    char_being_sent->GetText(sent,2);
    Last_Char = sent[0];
    ::PostMessage(Parent->HWindow,WM_COMMAND,ID,NULL);
    char_being_sent->SetText("");
  }
  if (strlen(edit_string+1)<40 && stream != NULL)
    Read_File();
}

void TSEND_MORSEChld::End_Morse()

{
  char edit_string[TO_SEND_BUFFER_LENGTH+1];

  if (tuning)
  {
    Add_Tune_Char();
    return;
  }
  char_to_send->GetText(edit_string,TO_SEND_BUFFER_LENGTH);
  morse_off = 1;
  if (edit_string[0] != 0)
  {
    Send_Next_Char();
    morse_off = 0;
  }
  else
  {
	 char_being_sent->SetText("");
    if (PTPTT == NULL || PTPTT->Attr.H<0)
		ptt_control->Set_Value(0);
    else
      PTPTT->Set_Value(0);
  }
}

void TSEND_MORSEChld::Add_CallSign()

{
  char string[TO_SEND_BUFFER_LENGTH+1];
  char CallSign[40];

  char_to_send->GetText(string,TO_SEND_BUFFER_LENGTH);
  strcat(string,"Q CQ CQ DE ");
  Get_CallSign(CallSign);
  if (strcmp(CallSign,"UNKNOWN")!=0)
  {
	 strcat(string,CallSign);
	 strcat(string," AR K ");
  }
  char_to_send->SetText(string);
}

void TSEND_MORSEChld::Change_WPM()

{
  int time;
  char message[4];

  if (WPM->Get_Value()>1)
  {
    time = 9000/WPM->Get_Value();
    message[0] = time%256;
    message[1] = time/256;
    DSPSendMsg(msg_MORSE_SPEED,(int *) message,2);
  }
}

void TSEND_MORSEChld::Send_File()

{
  if (!tuning)
  {
    if (stream == NULL)
    {
      TFileOpenDialog::TData FilenameData(OFN_FILEMUSTEXIST | OFN_HIDEREADONLY|
								 OFN_PATHMUSTEXIST,
                         "Text Files (*.txt)|*.txt|All Files (*.*)|*.*|",0,"","*");
      if (TFileOpenDialog(this,FilenameData).Execute() == IDOK)
      {
//        if ((stream = fopen(FilenameData.FileName,"r")) == NULL)
//          return;
//        PostMessage(WM_COMMAND,106,NULL);
      }
    }
  }
}

void TSEND_MORSEChld::Read_File()

{
/*  char string[2];
  char message[TO_SEND_BUFFER_LENGTH+1];
  int count;

  char_to_send->GetText(message,TO_SEND_BUFFER_LENGTH);
  if (strlen(message)<40)
  {
    string[0] = toupper(fgetc(stream));
    for(count=0;count<MAX_CHARACTORS;count++)
    {
      if (morse_charactors[count]==string[0])
        break;
    }
    if (count<MAX_CHARACTORS)
    {
		string[1] = 0;
      strcat(message,string);
		char_to_send->SetText(message);
    }
    if (feof(stream))
    {
      fclose(stream);
      stream = NULL;
    }
    else
      PostMessage(WM_COMMAND,106,NULL);
  }
*/}

void TSEND_MORSEChld::Tune()

{
  if (tuning)
  {
	 tuning = 0;
    return;
  }
  tuning = 1;
  if (morse_off)
  {
    if (PTPTT == NULL || PTPTT->Attr.H<0)
      ptt_control->Set_Value(1);
    else
      PTPTT->Set_Value(1);
    dly();
    morse_off = 0;
  }
  Add_Tune_Char();
}

void TSEND_MORSEChld::Add_Tune_Char()

{
  char message[30];

  message[0] = 13;
  message[1] = 0;
  message[2] = 13;
  message[3] = 0;
  message[4] = 0;
  message[5] = 0;
  DSPSendMsg(msg_MORSE_CHAR,(int *) message,28);
}

void TSEND_MORSEChld::CQ_Button()

{
  char string[TO_SEND_BUFFER_LENGTH+1];
  int length;

  char_to_send->GetText(string,TO_SEND_BUFFER_LENGTH);
  length = strlen(string);
  string[length] = 'C';
  string[length+1] = 0;
  char_to_send->SetText(string);
  PostMessage(WM_COMMAND,104,NULL);
}

void TSEND_MORSEChld::Macro(WPARAM ID)

{
  char string[TO_SEND_BUFFER_LENGTH+1];
  int length;
  int which;

  which = ID-110;
  char_to_send->GetText(string,TO_SEND_BUFFER_LENGTH);
  length = strlen(string);
  string[length] = morse_button_macros[which][0];
  string[length+1] = 0;
  char_to_send->SetText(string);
  last_macro = which;
  if (morse_button_macros[which][0] != 0)
	 PostMessage(WM_COMMAND,204,NULL);
}

void TSEND_MORSEChld::Finish_Macro()

{
  char string[TO_SEND_BUFFER_LENGTH+1];

  char_to_send->GetText(string,TO_SEND_BUFFER_LENGTH);
  strcat(string,morse_button_macros[last_macro]+1);
  char_to_send->SetText(string);
}

void TSEND_MORSEChld::Change_Macro(WPARAM ID)

{
  int which;
  char name[30];
  char macro[41];

  which = ID-210;
  strcpy(name,morse_button_names[which]);
  if (TInputDialog(this,"Morse Macros","Please enter the new label for this macro",
						 name,5).Execute()==IDOK)
  {
	 strcpy(morse_button_names[which],name);
	 Macro_Buttons[which]->SetCaption(name);
	 strcpy(macro,morse_button_macros[which]);
	 if (TInputDialog(this,"Morse Macros","Please enter the new macro",
							macro,40).Execute()==IDOK)
		strcpy(morse_button_macros[which],macro);
  }
}

void TSEND_MORSEChld::Paint(TDC& DC, BOOL,TRect &)

{

  DC.MoveTo(0,24);
  DC.LineTo(Attr.W,24);
}
  
DEFINE_RESPONSE_TABLE1(TSEND_MORSEChld,AWindow)
  EV_EN_CHANGE(102,Edit_Check),
  EV_EN_CHANGE(103,Change_WPM),
  EV_COMMAND(104,Add_CallSign),
  EV_COMMAND(105,Send_File),
  EV_COMMAND(106,Read_File),
  EV_COMMAND(107,Tune),
  EV_COMMAND(108,CQ_Button),
  EV_COMMAND_AND_ID(110,Macro),
  EV_COMMAND_AND_ID(111,Macro),
  EV_COMMAND_AND_ID(112,Macro),
  EV_COMMAND_AND_ID(113,Macro),
  EV_COMMAND_AND_ID(114,Macro),
  EV_COMMAND_AND_ID(115,Macro),
  EV_COMMAND_AND_ID(210,Change_Macro),
  EV_COMMAND_AND_ID(211,Change_Macro),
  EV_COMMAND_AND_ID(212,Change_Macro),
  EV_COMMAND_AND_ID(213,Change_Macro),
  EV_COMMAND_AND_ID(214,Change_Macro),
  EV_COMMAND_AND_ID(215,Change_Macro),
  EV_COMMAND(201,Send_Next_Char),
  EV_COMMAND(202,End_Morse),
  EV_COMMAND(204,Finish_Macro),
END_RESPONSE_TABLE;

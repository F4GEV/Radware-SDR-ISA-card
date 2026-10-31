#include "thisCls.h"
#include <owl\inputdia.h>
#include <stdio.h>
#include <math.h>

//REGEN_VARIABLES

extern struct voice_memory voice_memories[];
extern int voice_string[];
extern int starting_places[][2];
extern int delete_class[];

static FILE *play_voice_stream = NULL;
//static FILE *test_stream = NULL;

TVoiceChld::TVoiceChld(AWindow *AParent, LPSTR AName,int Bank,int child_number)
			  :AWindow(AParent, AName, child_number)
{
	int count,x,y;

   Child_Number = child_number;
   x = starting_places[Child_Number][0];
   y = starting_places[Child_Number][1];
	Attr.Style = WS_POPUP | WS_VISIBLE | WS_CAPTION |WS_MINIMIZEBOX |
					 WS_SYSMENU;
	bank = Bank;
	if (x>=0)
	  Attr.X = x;
	else
	  Attr.X = -x;
	if (y>=0)
	  Attr.Y = y;
	else
	  Attr.Y = -y;
	Attr.W = 140;
	Attr.H = 101;
	Button[0] = new AButton(this,101,voice_memories[bank*9+0].label,0,0,46,28,0);
	Button[1] = new AButton(this,102,voice_memories[bank*9+1].label,46,0,47,28,0);
	Button[2] = new AButton(this,103,voice_memories[bank*9+2].label,93,0,46,28,0);
	Button[3] = new AButton(this,104,voice_memories[bank*9+3].label,0,27,46,28,0);
	Button[4] = new AButton(this,105,voice_memories[bank*9+4].label,46,27,47,28,0);
	Button[5] = new AButton(this,106,voice_memories[bank*9+5].label,93,27,46,28,0);
	Button[6] = new AButton(this,107,voice_memories[bank*9+6].label,0,54,46,28,0);
	Button[7] = new AButton(this,108,voice_memories[bank*9+7].label,46,54,47,28,0);
	Button[8] = new AButton(this,109,voice_memories[bank*9+8].label,93,54,46,28,0);
}

TVoiceChld::~TVoiceChld()

{
  Attr.Y *= -1;
  if (Attr.Y>=0)
	 Attr.Y = -1;
  starting_places[Child_Number][0] = Attr.X;
  starting_places[Child_Number][1] = Attr.Y;
  delete_class[Child_Number] = 1;
//  Remove_Old_Classes();
//  ::SendMessage(Parent->HWindow,WM_COMMAND,501,0);
}

void TVoiceChld::CleanupWindow()

{
  AWindow::CleanupWindow();
}

TVoiceChld::Where_Y()

{
  return(Attr.Y);
}

void TVoiceChld::EvActivate(uint active,bool b,HWND c)

{
  if (active != WA_INACTIVE)
    MoveWindowToTopOfList(Child_Number);
  AWindow::EvActivate(active,b,c);
}

void TVoiceChld::UpdateAllButtons()

{
  int count;

  for(count = 0;count<9;count++)
    Button[count]->SetCaption(voice_memories[count].label);
}

void TVoiceChld::Reply_Button(WPARAM ID)

{
  int which;

  which = bank*9+ID-101;
  play_voice_memory_start(which);
}

void TVoiceChld::Change_Button(WPARAM ID)

{
  int which;
  char label[30],filename[80];
  int count;

  which = bank*9+ID-201;

	if (save_voice_memory_open())
		save_voice_memory_end();
	else
	{
     if (play_voice_stream)
     {
       end_play();
       MessageBox("Playback aborted","Voice Memory",MB_OK);
       play_voice_stream = NULL;
     }
     else
     {
/*       strcpy(label,voice_memories[which].label);
		 if (TInputDialog(this,"Voice Memory Label","Please enter the new label for this channels",
		                  label,5).Execute()==IDOK)
       {
			strcpy(filename,voice_memories[which].filename);
			if (TInputDialog(this,"Voice Memory Filename","Please enter the filename ",
				filename,78).Execute()==IDOK)
			{
			  if (label[0]!=0 && filename[0]!=0)
			  {
			    save_voice_memory_start(label,filename,which);
			    Button[ID-201]->SetCaption(label);
	        }
			}
       }
*/       TVOICEDlg(this, which).Execute();
         Button[which]->SetCaption(voice_memories[which].label);
     }
	}
}

static int times_through = 0;
int message[256];
static int sine2;

void play_voice_memory_start(int which)

{
  int count;
  if (play_voice_stream)
  {
    end_play();
    MessageBox(NULL,"Playback aborted","Voice Memory",MB_OK);
    play_voice_stream = NULL;
  }
  else
  {
    if (save_voice_memory_open())
		save_voice_memory_end();
    else
    {
      play_voice_stream = fopen(voice_memories[which].filename,"rb");
//      test_stream = fopen("test","w");
	   if (play_voice_stream)
      {
//        play_voice_memory();
	     start_play();
//        play_voice_memory();
      }
      else
	     MessageBox(NULL,"Voice file not found","File Error",MB_OK | MB_ICONHAND);
    }
  }
}

int repeat = 0;

void play_voice_memory()

{
  int count;
  int high,low;
//  fprintf(test_stream,"%d,%d\n",voice_string[0],voice_string[1]);
  times_through++;
  if (play_voice_stream)
  {
    for(count=1;count<129;count++)
	 {
      high = fgetc(play_voice_stream);
      low = fgetc(play_voice_stream);
      message[count] = high*256+low;
      if (feof(play_voice_stream))
        break;
    }
    if (feof(play_voice_stream))
    {
      end_play();
      fclose(play_voice_stream);
//      fclose(test_stream);
      play_voice_stream = NULL;
    }
    else
    {
//      message[0] = times_through;
//      message[1] = times_through;
      send_play_message(message);
//      sine2 += 3;
    }
  }
  else
  {
    end_play/*_and_null*/();
  }
}


DEFINE_RESPONSE_TABLE1(TVoiceChld,AWindow)
  EV_COMMAND_AND_ID(101,Reply_Button),
  EV_COMMAND_AND_ID(102,Reply_Button),
  EV_COMMAND_AND_ID(103,Reply_Button),
  EV_COMMAND_AND_ID(104,Reply_Button),
  EV_COMMAND_AND_ID(105,Reply_Button),
  EV_COMMAND_AND_ID(106,Reply_Button),
  EV_COMMAND_AND_ID(107,Reply_Button),
  EV_COMMAND_AND_ID(108,Reply_Button),
  EV_COMMAND_AND_ID(109,Reply_Button),
  EV_COMMAND_AND_ID(201,Change_Button),
  EV_COMMAND_AND_ID(202,Change_Button),
  EV_COMMAND_AND_ID(203,Change_Button),
  EV_COMMAND_AND_ID(204,Change_Button),
  EV_COMMAND_AND_ID(205,Change_Button),
  EV_COMMAND_AND_ID(206,Change_Button),
  EV_COMMAND_AND_ID(207,Change_Button),
  EV_COMMAND_AND_ID(208,Change_Button),
  EV_COMMAND_AND_ID(209,Change_Button),
  EV_WM_ACTIVATE,
END_RESPONSE_TABLE;


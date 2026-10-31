#include "thisID.h"
#include "thisCls.h"
#include <stdio.h>
#include <math.h>

extern int starting_places[][2];
extern int delete_class[];
extern TSTATChld *PTStations[];
extern TCONTROLChld *PTMode,*PTFilter;
extern TFreq_Window2 *PTFreq;
extern TBANDSCANChld *PTBandScan;

extern PFrequency_Control frequency_control;
extern PMODE_Control mode_control;
extern PFILTER_Control filter_control;

struct single_station
{
  char name[16];
  long frequency;
  short mode;
  short band;
};

extern struct single_station quick_stations[];
extern int channels_selected[];

TALL_CHANNELSChld::TALL_CHANNELSChld(AWindow *AParent,int ID,LPSTR AName,int child_number)
		     :AWindow(AParent,AName,child_number)

{
  int x,y;
  char string[20];
  int x_column = 65;

  Child_Number = child_number;
  x = starting_places[Child_Number][0];
  y = starting_places[Child_Number][1];
  Attr.Style = WS_POPUP | WS_VISIBLE | WS_CAPTION |WS_MINIMIZEBOX|WS_SYSMENU;
  Attr.X = x;
  if (y<0)
    Attr.Y = -y;
  else
    Attr.Y = y;
  Attr.W = 600;
  Attr.H = 285;
  id = ID;
  for(x=0;x<9;x++)
  {
    sprintf(string,"band %d",x+1);
    new TStatic(this,-1,string,x*x_column+18,10,50,23);
    for(y=0;y<9;y++)
    {
      Selection[x*9+y] = new TCheckBox(this,100+x*9+y,"",10+x*x_column,30+y*25,15,23);
      Channel_Buttons[x*9+y] = new AButton(this,200+x*9+y,quick_stations[x*9+y].name,27+x*x_column,
                                           30+y*25,40,23,FALSE);
    }
  }
}

TALL_CHANNELSChld::~TALL_CHANNELSChld()

{
  int count;

  Attr.Y *= -1;
  if (Attr.Y>=0)
    Attr.Y = -1;
  Attr.H = Attr.W = -1;
  starting_places[Child_Number][0] = Attr.X;
  starting_places[Child_Number][1] = Attr.Y;
  delete_class[Child_Number] = 1;
}

void TALL_CHANNELSChld::SetupWindow()

{
  int count;

  AWindow::SetupWindow();
  for(count=0;count<81;count++)
    if (channels_selected[count] == 1)
      Selection[count]->Check();
    else
      Selection[count]->Uncheck();
}

void TALL_CHANNELSChld::EvActivate(uint active,bool b,HWND c)

{
  if (active != WA_INACTIVE)
    MoveWindowToTopOfList(Child_Number);
  AWindow::EvActivate(active,b,c);
}

void TALL_CHANNELSChld::ReturnFocus()

{
  ::SetFocus(Parent->HWindow);
}

LPSTR TALL_CHANNELSChld::GetClassName()

{
  return("ALL_CHANNELS");
}

void TALL_CHANNELSChld::GetWindowClass(WNDCLASS _FAR & AWndClass)

{
  AWindow::GetWindowClass(AWndClass);
}

TALL_CHANNELSChld::Where_Y()

{
  return(Attr.Y);
}

void TALL_CHANNELSChld::Get_Channel(WPARAM ID)

{
  char string[80];
  int which;

  which = ID;
  Recall_Channel(which-200);
}

void TALL_CHANNELSChld::Set_Channel(WPARAM ID)

{
  int which;
  char string[30];
  int bank;

  which = ID-300;

  strcpy(string,quick_stations[which].name);
  if (TInputDialog(this,"Memory Channels","Please enter the new label for this channel",
               string,5).Execute()==IDOK)
  {
    if (string[0]!=0)
    {
      quick_stations[which].frequency = frequency_control->Get_Value();
      quick_stations[which].mode = mode_control->Get_Value();
      quick_stations[which].band = filter_control->Get_Value();
      strcpy(quick_stations[which].name,string);
      Channel_Buttons[which]->SetCaption(quick_stations[which].name);
      bank = which/9;
      if (PTStations[bank]!=NULL && delete_class[which+31] == 0)
        PTStations[bank]->Update_Channel();
    }
  }
}

void TALL_CHANNELSChld::Get_Selected()

{
  int count;

  for(count=0;count<81;count++)
    if (Selection[count]->GetCheck()==BF_CHECKED)
      channels_selected[count] = 1;
    else
      channels_selected[count] = 0;
}

void TALL_CHANNELSChld::EvMove(TPoint &point)

{
  AWindow::EvMove(point);
  starting_places[Child_Number][0] = Attr.X;
  starting_places[Child_Number][1] = Attr.Y;
}

void TALL_CHANNELSChld::EvChar(UINT key, UINT repeatCount,UINT flags)

{
  if (key == 13)
  {
    ClipCursor(NULL);
    ShowCursor(0);
    Attr.H = -1;
    while (ShowCursor(1)<0)
      {}
  }
}

void TALL_CHANNELSChld::Update_Channel_Names()

{
  int count;

  for(count=0;count<81;count++)
    if (strcmp(Channel_Buttons[count]->Title,quick_stations[count].name)!=0)
      Channel_Buttons[count]->SetCaption(quick_stations[count].name);
}

DEFINE_RESPONSE_TABLE1(TALL_CHANNELSChld,AWindow)
  EV_WM_ACTIVATE,
  EV_WM_MOVE,
  EV_COMMAND(100,Get_Selected),
  EV_COMMAND(101,Get_Selected),
  EV_COMMAND(102,Get_Selected),
  EV_COMMAND(103,Get_Selected),
  EV_COMMAND(104,Get_Selected),
  EV_COMMAND(105,Get_Selected),
  EV_COMMAND(106,Get_Selected),
  EV_COMMAND(107,Get_Selected),
  EV_COMMAND(108,Get_Selected),
  EV_COMMAND(109,Get_Selected),
  EV_COMMAND(110,Get_Selected),
  EV_COMMAND(111,Get_Selected),
  EV_COMMAND(112,Get_Selected),
  EV_COMMAND(113,Get_Selected),
  EV_COMMAND(114,Get_Selected),
  EV_COMMAND(115,Get_Selected),
  EV_COMMAND(116,Get_Selected),
  EV_COMMAND(117,Get_Selected),
  EV_COMMAND(118,Get_Selected),
  EV_COMMAND(119,Get_Selected),
  EV_COMMAND(120,Get_Selected),
  EV_COMMAND(121,Get_Selected),
  EV_COMMAND(122,Get_Selected),
  EV_COMMAND(123,Get_Selected),
  EV_COMMAND(124,Get_Selected),
  EV_COMMAND(125,Get_Selected),
  EV_COMMAND(126,Get_Selected),
  EV_COMMAND(127,Get_Selected),
  EV_COMMAND(128,Get_Selected),
  EV_COMMAND(129,Get_Selected),
  EV_COMMAND(130,Get_Selected),
  EV_COMMAND(131,Get_Selected),
  EV_COMMAND(132,Get_Selected),
  EV_COMMAND(133,Get_Selected),
  EV_COMMAND(134,Get_Selected),
  EV_COMMAND(135,Get_Selected),
  EV_COMMAND(136,Get_Selected),
  EV_COMMAND(137,Get_Selected),
  EV_COMMAND(138,Get_Selected),
  EV_COMMAND(139,Get_Selected),
  EV_COMMAND(140,Get_Selected),
  EV_COMMAND(141,Get_Selected),
  EV_COMMAND(142,Get_Selected),
  EV_COMMAND(143,Get_Selected),
  EV_COMMAND(144,Get_Selected),
  EV_COMMAND(145,Get_Selected),
  EV_COMMAND(146,Get_Selected),
  EV_COMMAND(147,Get_Selected),
  EV_COMMAND(148,Get_Selected),
  EV_COMMAND(149,Get_Selected),
  EV_COMMAND(150,Get_Selected),
  EV_COMMAND(151,Get_Selected),
  EV_COMMAND(152,Get_Selected),
  EV_COMMAND(153,Get_Selected),
  EV_COMMAND(154,Get_Selected),
  EV_COMMAND(155,Get_Selected),
  EV_COMMAND(156,Get_Selected),
  EV_COMMAND(157,Get_Selected),
  EV_COMMAND(158,Get_Selected),
  EV_COMMAND(159,Get_Selected),
  EV_COMMAND(160,Get_Selected),
  EV_COMMAND(161,Get_Selected),
  EV_COMMAND(162,Get_Selected),
  EV_COMMAND(163,Get_Selected),
  EV_COMMAND(164,Get_Selected),
  EV_COMMAND(165,Get_Selected),
  EV_COMMAND(166,Get_Selected),
  EV_COMMAND(167,Get_Selected),
  EV_COMMAND(168,Get_Selected),
  EV_COMMAND(169,Get_Selected),
  EV_COMMAND(170,Get_Selected),
  EV_COMMAND(171,Get_Selected),
  EV_COMMAND(172,Get_Selected),
  EV_COMMAND(173,Get_Selected),
  EV_COMMAND(174,Get_Selected),
  EV_COMMAND(175,Get_Selected),
  EV_COMMAND(176,Get_Selected),
  EV_COMMAND(177,Get_Selected),
  EV_COMMAND(178,Get_Selected),
  EV_COMMAND(176,Get_Selected),
  EV_COMMAND(180,Get_Selected),
  EV_COMMAND_AND_ID(200,Get_Channel),
  EV_COMMAND_AND_ID(201,Get_Channel),
  EV_COMMAND_AND_ID(202,Get_Channel),
  EV_COMMAND_AND_ID(203,Get_Channel),
  EV_COMMAND_AND_ID(204,Get_Channel),
  EV_COMMAND_AND_ID(205,Get_Channel),
  EV_COMMAND_AND_ID(206,Get_Channel),
  EV_COMMAND_AND_ID(207,Get_Channel),
  EV_COMMAND_AND_ID(208,Get_Channel),
  EV_COMMAND_AND_ID(209,Get_Channel),
  EV_COMMAND_AND_ID(210,Get_Channel),
  EV_COMMAND_AND_ID(211,Get_Channel),
  EV_COMMAND_AND_ID(212,Get_Channel),
  EV_COMMAND_AND_ID(213,Get_Channel),
  EV_COMMAND_AND_ID(214,Get_Channel),
  EV_COMMAND_AND_ID(215,Get_Channel),
  EV_COMMAND_AND_ID(216,Get_Channel),
  EV_COMMAND_AND_ID(217,Get_Channel),
  EV_COMMAND_AND_ID(218,Get_Channel),
  EV_COMMAND_AND_ID(219,Get_Channel),
  EV_COMMAND_AND_ID(220,Get_Channel),
  EV_COMMAND_AND_ID(221,Get_Channel),
  EV_COMMAND_AND_ID(222,Get_Channel),
  EV_COMMAND_AND_ID(223,Get_Channel),
  EV_COMMAND_AND_ID(224,Get_Channel),
  EV_COMMAND_AND_ID(225,Get_Channel),
  EV_COMMAND_AND_ID(226,Get_Channel),
  EV_COMMAND_AND_ID(227,Get_Channel),
  EV_COMMAND_AND_ID(228,Get_Channel),
  EV_COMMAND_AND_ID(229,Get_Channel),
  EV_COMMAND_AND_ID(230,Get_Channel),
  EV_COMMAND_AND_ID(231,Get_Channel),
  EV_COMMAND_AND_ID(232,Get_Channel),
  EV_COMMAND_AND_ID(233,Get_Channel),
  EV_COMMAND_AND_ID(234,Get_Channel),
  EV_COMMAND_AND_ID(235,Get_Channel),
  EV_COMMAND_AND_ID(236,Get_Channel),
  EV_COMMAND_AND_ID(237,Get_Channel),
  EV_COMMAND_AND_ID(238,Get_Channel),
  EV_COMMAND_AND_ID(239,Get_Channel),
  EV_COMMAND_AND_ID(240,Get_Channel),
  EV_COMMAND_AND_ID(241,Get_Channel),
  EV_COMMAND_AND_ID(242,Get_Channel),
  EV_COMMAND_AND_ID(243,Get_Channel),
  EV_COMMAND_AND_ID(244,Get_Channel),
  EV_COMMAND_AND_ID(245,Get_Channel),
  EV_COMMAND_AND_ID(246,Get_Channel),
  EV_COMMAND_AND_ID(247,Get_Channel),
  EV_COMMAND_AND_ID(248,Get_Channel),
  EV_COMMAND_AND_ID(249,Get_Channel),
  EV_COMMAND_AND_ID(250,Get_Channel),
  EV_COMMAND_AND_ID(251,Get_Channel),
  EV_COMMAND_AND_ID(252,Get_Channel),
  EV_COMMAND_AND_ID(253,Get_Channel),
  EV_COMMAND_AND_ID(254,Get_Channel),
  EV_COMMAND_AND_ID(255,Get_Channel),
  EV_COMMAND_AND_ID(256,Get_Channel),
  EV_COMMAND_AND_ID(257,Get_Channel),
  EV_COMMAND_AND_ID(258,Get_Channel),
  EV_COMMAND_AND_ID(259,Get_Channel),
  EV_COMMAND_AND_ID(260,Get_Channel),
  EV_COMMAND_AND_ID(261,Get_Channel),
  EV_COMMAND_AND_ID(262,Get_Channel),
  EV_COMMAND_AND_ID(263,Get_Channel),
  EV_COMMAND_AND_ID(264,Get_Channel),
  EV_COMMAND_AND_ID(265,Get_Channel),
  EV_COMMAND_AND_ID(266,Get_Channel),
  EV_COMMAND_AND_ID(267,Get_Channel),
  EV_COMMAND_AND_ID(268,Get_Channel),
  EV_COMMAND_AND_ID(269,Get_Channel),
  EV_COMMAND_AND_ID(270,Get_Channel),
  EV_COMMAND_AND_ID(271,Get_Channel),
  EV_COMMAND_AND_ID(272,Get_Channel),
  EV_COMMAND_AND_ID(273,Get_Channel),
  EV_COMMAND_AND_ID(274,Get_Channel),
  EV_COMMAND_AND_ID(275,Get_Channel),
  EV_COMMAND_AND_ID(276,Get_Channel),
  EV_COMMAND_AND_ID(277,Get_Channel),
  EV_COMMAND_AND_ID(278,Get_Channel),
  EV_COMMAND_AND_ID(279,Get_Channel),
  EV_COMMAND_AND_ID(280,Get_Channel),
  EV_COMMAND_AND_ID(300,Set_Channel),
  EV_COMMAND_AND_ID(301,Set_Channel),
  EV_COMMAND_AND_ID(302,Set_Channel),
  EV_COMMAND_AND_ID(303,Set_Channel),
  EV_COMMAND_AND_ID(304,Set_Channel),
  EV_COMMAND_AND_ID(305,Set_Channel),
  EV_COMMAND_AND_ID(306,Set_Channel),
  EV_COMMAND_AND_ID(307,Set_Channel),
  EV_COMMAND_AND_ID(308,Set_Channel),
  EV_COMMAND_AND_ID(309,Set_Channel),
  EV_COMMAND_AND_ID(310,Set_Channel),
  EV_COMMAND_AND_ID(311,Set_Channel),
  EV_COMMAND_AND_ID(312,Set_Channel),
  EV_COMMAND_AND_ID(313,Set_Channel),
  EV_COMMAND_AND_ID(314,Set_Channel),
  EV_COMMAND_AND_ID(315,Set_Channel),
  EV_COMMAND_AND_ID(316,Set_Channel),
  EV_COMMAND_AND_ID(317,Set_Channel),
  EV_COMMAND_AND_ID(318,Set_Channel),
  EV_COMMAND_AND_ID(319,Set_Channel),
  EV_COMMAND_AND_ID(320,Set_Channel),
  EV_COMMAND_AND_ID(321,Set_Channel),
  EV_COMMAND_AND_ID(322,Set_Channel),
  EV_COMMAND_AND_ID(323,Set_Channel),
  EV_COMMAND_AND_ID(324,Set_Channel),
  EV_COMMAND_AND_ID(325,Set_Channel),
  EV_COMMAND_AND_ID(326,Set_Channel),
  EV_COMMAND_AND_ID(327,Set_Channel),
  EV_COMMAND_AND_ID(328,Set_Channel),
  EV_COMMAND_AND_ID(329,Set_Channel),
  EV_COMMAND_AND_ID(330,Set_Channel),
  EV_COMMAND_AND_ID(331,Set_Channel),
  EV_COMMAND_AND_ID(332,Set_Channel),
  EV_COMMAND_AND_ID(333,Set_Channel),
  EV_COMMAND_AND_ID(334,Set_Channel),
  EV_COMMAND_AND_ID(335,Set_Channel),
  EV_COMMAND_AND_ID(336,Set_Channel),
  EV_COMMAND_AND_ID(337,Set_Channel),
  EV_COMMAND_AND_ID(338,Set_Channel),
  EV_COMMAND_AND_ID(339,Set_Channel),
  EV_COMMAND_AND_ID(340,Set_Channel),
  EV_COMMAND_AND_ID(341,Set_Channel),
  EV_COMMAND_AND_ID(342,Set_Channel),
  EV_COMMAND_AND_ID(343,Set_Channel),
  EV_COMMAND_AND_ID(344,Set_Channel),
  EV_COMMAND_AND_ID(345,Set_Channel),
  EV_COMMAND_AND_ID(346,Set_Channel),
  EV_COMMAND_AND_ID(347,Set_Channel),
  EV_COMMAND_AND_ID(348,Set_Channel),
  EV_COMMAND_AND_ID(349,Set_Channel),
  EV_COMMAND_AND_ID(350,Set_Channel),
  EV_COMMAND_AND_ID(351,Set_Channel),
  EV_COMMAND_AND_ID(352,Set_Channel),
  EV_COMMAND_AND_ID(353,Set_Channel),
  EV_COMMAND_AND_ID(354,Set_Channel),
  EV_COMMAND_AND_ID(355,Set_Channel),
  EV_COMMAND_AND_ID(356,Set_Channel),
  EV_COMMAND_AND_ID(357,Set_Channel),
  EV_COMMAND_AND_ID(358,Set_Channel),
  EV_COMMAND_AND_ID(359,Set_Channel),
  EV_COMMAND_AND_ID(360,Set_Channel),
  EV_COMMAND_AND_ID(361,Set_Channel),
  EV_COMMAND_AND_ID(362,Set_Channel),
  EV_COMMAND_AND_ID(363,Set_Channel),
  EV_COMMAND_AND_ID(364,Set_Channel),
  EV_COMMAND_AND_ID(365,Set_Channel),
  EV_COMMAND_AND_ID(366,Set_Channel),
  EV_COMMAND_AND_ID(367,Set_Channel),
  EV_COMMAND_AND_ID(368,Set_Channel),
  EV_COMMAND_AND_ID(369,Set_Channel),
  EV_COMMAND_AND_ID(370,Set_Channel),
  EV_COMMAND_AND_ID(371,Set_Channel),
  EV_COMMAND_AND_ID(372,Set_Channel),
  EV_COMMAND_AND_ID(373,Set_Channel),
  EV_COMMAND_AND_ID(374,Set_Channel),
  EV_COMMAND_AND_ID(375,Set_Channel),
  EV_COMMAND_AND_ID(376,Set_Channel),
  EV_COMMAND_AND_ID(377,Set_Channel),
  EV_COMMAND_AND_ID(378,Set_Channel),
  EV_COMMAND_AND_ID(379,Set_Channel),
  EV_COMMAND_AND_ID(380,Set_Channel),
END_RESPONSE_TABLE;


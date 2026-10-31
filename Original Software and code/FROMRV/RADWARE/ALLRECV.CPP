//REGEN_FILEHEADING
//REGEN_FILEHEADING

     /********************************************************************
      *                                                                  *
      *   Source File: All_Frequencies.cpp                                          *
      *   Description: Dialog implementation for PRORADIO                *
      *   Date:        Tue Sep 15 12:37:33 1992                          *
      *                                                                  *
      ********************************************************************/

#include <owl\framewin.h>
#include <owl\edit.h>
#include <owl\listbox.h>
#include <owl\combobox.h>
#include <owl\scrollba.h>
#include <owl\dialog.h>
#include <bwcc.h>
#include <stdio.h>
#include "thisCls.h"
//#include "SWLIB.h"
#include <owl\radiobut.h>
#include <owl\button.h>
#include <math.h>;

extern int starting_places[][2];
extern short int delete_class[];

TAllRecieve::TAllRecieve(AWindow *AParent, LPSTR AName,PFrequency_Control vfo_a,
                         int child_number):AWindow(AParent, AName,child_number)
{
//   char string[21];
   int x,y;

   Child_Number = child_number;
   x = starting_places[Child_Number][0];
   y = starting_places[Child_Number][1];
   Attr.Style = WS_MINIMIZEBOX | WS_POPUP | WS_CLIPSIBLINGS | WS_VISIBLE |
                WS_CAPTION | WS_SYSMENU;
   SetCaption("Frequencies");
   if (x>=0)
     Attr.X = x;
   else
     Attr.X = -x;
   if (y>=0)
     Attr.Y = y;
   else
     Attr.Y = -y;
   Attr.W = 162;
   Attr.H = 172;
}

TAllRecieve::~TAllRecieve()

{
  Attr.H = -1;
}


DEFINE_RESPONSE_TABLE1(TAllRecieve,AWindow)
END_RESPONSE_TABLE;


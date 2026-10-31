
//REGEN_FILEHEADING
//REGEN_FILEHEADING


     /********************************************************************
      *                                                                  *
      *   Source File: FILTER.cpp                                        *
      *   Description: C++ Source file for FILTER application            *
      *   Date:        Thu Sep 16 14:58:28 1993                          *
      *                                                                  *
      ********************************************************************/

#include <owl.h>
#include <edit.h>
#include <listbox.h>
#include <combobox.h>
#include <scrollba.h>
#include <dialog.h>
#include <bwcc.h>
#include "FILTERID.h"
#include "FILTER.h"
#include "FILTECls.h"
#include <math.h>
#include <edit.h>
#include <stdio.h>
#include <checkbox.h>
#include <complex.h>

//REGEN_VARIABLES
//REGEN_VARIABLES

PTCheckBox TPlotFilter;
PTEdit THZ,TDEPTH,TDELAY;
int DEPTH;
double HZ,DELAY;
// Define application class derived from  TApplication
double FIR(double,int);
double IIR(double,int);
double INPUT(double);
double FIR_RESPONSE(int);
double IIR_RESPONSE(int);
double HIGH_PASS_RESPONSE(double);
double DECIMATE(double new_sample_rate,double sample_nu,
                double old_sample_rate);
double fftatk(int input,int output,int samples,int k,int inverse);

double WAVE_FORMS[1256][4];

class TFILTER : public TApplication
{
public:
  TFILTER(LPSTR AName, HANDLE hInstance, HANDLE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
    : TApplication(AName, hInstance, hPrevInstance, lpCmdLine, nCmdShow) {};
    virtual void InitMainWindow();
    //REGEN_APPCLASS
    //REGEN_APPCLASS
};

// Declare TMainWindow, a TWindow descendant
class TMainWindow : public TWindow
{
public:
   TMainWindow(PTWindowsObject AParent, LPSTR ATitle);
   ~TMainWindow();
   virtual void REPLOT(RTMessage Msg) = [CM_FIRST + IDM_REPLOT];
   virtual void PARAMATERS(RTMessage Msg) = [CM_FIRST + IDM_PARAMATERS];

   virtual void Paint(HDC DC, PAINTSTRUCT _FAR & PaintInfo);
   //REGEN_MAINCLASS
   //REGEN_MAINCLASS

};


/****************************************************
 * TMainWindow implementations: 
 ****************************************************/

// Define TMainWindow, a TWindow constructor
TMainWindow::TMainWindow(PTWindowsObject AParent, LPSTR ATitle)
                         : TWindow(AParent, ATitle)
{
   AssignMenu("FILTER");
   new TStatic(this,-1,"FREQ",350,15,50,25,5);
   THZ = new TEdit(this,4,"2",410,15,80,25,5,FALSE);
   new TStatic(this,-1,"DEPTH",200,15,50,25,5);
   TDEPTH = new TEdit(this,3,"10",260,15,80,25,5,FALSE);
   new TStatic(this,-1,"DELAY",50,15,50,25,5);
   TDELAY = new TEdit(this,2,"-8.5",110,15,50,25,7,FALSE);
   TPlotFilter = new TCheckBox(this,1,"Plot Filter",10,15,90,25,NULL);
   //REGEN_MAINCONSTRUCT
   //REGEN_MAINCONSTRUCT

}

// Define TMainWindow destructor
TMainWindow::~TMainWindow()
{
   //REGEN_MAINDESTRUCT
   //REGEN_MAINDESTRUCT

}

void TMainWindow::REPLOT(RTMessage)
{
  InvalidateRgn(HWindow,NULL,1);
  UpdateWindow(HWindow);
}

void TMainWindow::PARAMATERS(RTMessage)
{
   //REGEN_PARAMATERS_EXEC
   //REGEN_PARAMATERS_EXEC
}

void TMainWindow::Paint(HDC DC,PAINTSTRUCT _FAR &)

{
  HDC memdc;
  char string[25],s2[2]=" ";
  HRGN region;
  int count,count2;
  static LOGPEN lpBlue = {PS_SOLID,1,1,RGB(0,0,255)};
  static LOGPEN lpGreen = {PS_SOLID,1,1,RGB(0,255,0)};
  static LOGPEN lpRed = {PS_SOLID,1,1,RGB(255,0,0)};
  static LOGPEN lpPurple = {PS_SOLID,1,1,RGB(255,0,255)};
  HPEN Blue,Green,Red,Purple;

  THZ->GetText(string,4);
  HZ = atoi(string);
  TDEPTH->GetText(string,4);
  DEPTH = atoi(string);
  TDELAY->GetText(string,8);
  DELAY = atof(string);
  memdc = CreateCompatibleDC(DC);
  Blue = CreatePenIndirect(&lpBlue);
  Green = CreatePenIndirect(&lpGreen);
  Red = CreatePenIndirect(&lpRed);
  Purple = CreatePenIndirect(&lpPurple);
  for(count=0;count<200;count++)
    for(count2=0;count2<4;count2++)
      WAVE_FORMS[count][count2] = 0;
  MoveTo(DC,4,150);
  SelectObject(DC,Red);
  for(count=0;count<628;count++)
    LineTo(DC,count+5,150-100*FIR(FIR(INPUT(count-DELAY),0),1));
/*  MoveTo(DC,4,150);
  SelectObject(DC,Blue);
  for(count=0;count<628;count++)
    LineTo(DC,count+5,150-100*INPUT(count));
  SelectObject(DC,Green);
  for(count=0;count<10;count++)
  {
    MoveTo(DC,0,50+10*count);
    LineTo(DC,324,50+10*count);
  }
*/  SelectObject(DC,Purple);
  if (TPlotFilter->GetCheck())
  {
    MoveTo(DC,4,150);
    for(count=1;count<50;count++)
      LineTo(DC,count+5,150-100*HIGH_PASS_RESPONSE(count/2.0));
  }
  else
  {
    for(count=0;count<100;count++)
    {
      WAVE_FORMS[count][0]=INPUT(count/4.0);
      WAVE_FORMS[count][1]=0;
    }
    for(count=0;count<50;count++)
    {
      MoveTo(DC,count*628.0/50+5,150);
      LineTo(DC,count*628.0/50+5,150-20*fftatk(0,2,50,count,1));
    }
  }
  MoveTo(DC,4,150);
  SelectObject(DC,Blue);
  for(count=0;count<50;count++)
  {
    LineTo(DC,count*628.0/50+5,150-2*fftatk(2,0,50,count,1));
  }
  DeleteObject(Red);
  DeleteObject(Purple);
  DeleteObject(Blue);
  DeleteObject(Green);
  DeleteObject(DC);
}


/***************************************************
 * TFILTERApp method implementations:
 ***************************************************/

// Construct the TFILTER's MainWindow of type TMainWindow
void TFILTER::InitMainWindow()
{
   MainWindow = new TMainWindow(NULL, Name);
   //REGEN_MAINCREATE
   //REGEN_MAINCREATE
}

// Main program
int PASCAL WinMain(HANDLE hInstance,
                   HANDLE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
   HANDLE hBorLibrary;

   hBorLibrary = LoadLibrary("bwcc.dll");

   if(hBorLibrary <= 32)
      MessageBox(NULL, "Unable to load Borland Controls", "System Error", MB_OK | MB_ICONHAND);

   //REGEN_INIT
   //REGEN_INIT

   TFILTER FILTER ("Filter Design", hInstance, hPrevInstance,
      lpCmdLine, nCmdShow);

   FILTER.Run();

   if(hBorLibrary > 32)
      FreeLibrary(hBorLibrary);

   //REGEN_CLEANUP
   //REGEN_CLEANUP

   return FILTER.Status;
}

//REGEN_CUSTOMCODE
double INPUT(double x)

{
  return (sin(x/100.0*HZ)+sin(x/4.0))/2;
/*  if (sin(x*HZ/100.0)>0)
    return 1;
  else
    return -1;*/
}

double FIR(double input,int which)

{
  double answer=0;
  int count;
  int depth;

  answer = input;
  depth = DEPTH-which;
  for(count=0;count<depth-1;count++)
    answer += WAVE_FORMS[count][which];
  for(count=depth-1;count>0;count--)
    WAVE_FORMS[count][which]=WAVE_FORMS[count-1][which];
  WAVE_FORMS[0][which]=input;
  return(answer/depth);
}

double HIGH_PASS_RESPONSE(double freq)

{
  int count,count2;
  double answer = 0;
  double this_fir;
  int max_req;

  for(count=0;count<400;count++)
    for(count2=0;count2<5;count2++)
      WAVE_FORMS[count][count2] = 0;
  HZ = freq;
  max_req = 314/HZ;
  if (max_req<150)
    max_req = 150;
  for(count=0;count<628;count++)
    if (answer<(this_fir = INPUT(count)-FIR(FIR(INPUT(count-DELAY),0),1)) && count>100)
      answer = this_fir;
//  return(answer);
  return(1+20*log10(answer+.000000001)/100);
}

double IIR_RESPONSE(int freq)

{
  int count,count2;
  double answer = 0;
  double this_iir;
  int max_req;

  for(count=0;count<400;count++)
    for(count2=0;count2<5;count2++)
      WAVE_FORMS[count][count2] = 0;
  HZ = freq;
  max_req = 314/HZ;
  if (max_req<150)
    max_req = 150;
  for(count=0;count<628;count++)
    if (answer<(this_iir = FIR(INPUT(count),0)) && count>100)
      answer = this_iir;
//  return(answer);
  return(1+20*log10(answer+.000000001)/100);
}

double DECIMATE(double new_sample_rate,double sample_nu,
                double old_sample_rate)

{
  double sample[2];
  int where,offset;
  double answer;

  where = old_sample_rate*sample_nu/new_sample_rate;
  offset = (1.0*old_sample_rate*sample_nu/new_sample_rate-where)*100;
  sample[0] = INPUT(where);
  sample[1] = INPUT(where+1);
  answer = (sample[0]*(100-offset)+sample[1]*(offset))/100;
  return(answer);
}


double fftatk(int input,int output,int samples,int k,int inverse)

{
  int count;
  double answer = 0,answer2 = 0;
  double pi;
  complex c;

  pi = 3.141592653589793;
  int which_sample = 628/samples;
  for(count=0;count<samples;count++)
  {
//    if (inverse)
//      c = complex(cos(2*pi/samples),+sin(2*pi/samples));
//    else
      c = complex(cos(2*pi/samples),-sin(2*pi/samples));
    c = pow(c,count*k);
    answer += WAVE_FORMS[count*which_sample][input]*real(c);
    answer2 += WAVE_FORMS[count*which_sample][input+1]*real(c);
  }
  answer /= 10;
  answer2 /= 10;
  WAVE_FORMS[k][output]=answer;
  WAVE_FORMS[k][output]=answer2;
  return(abs(answer));
}
   
//REGEN_CUSTOMCODE

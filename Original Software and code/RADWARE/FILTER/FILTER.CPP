
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
PTEdit THZ,TDEPTH,TDELAY,TWEIGHTS[20];
//long weights[21]={46,1026,2330,624,-4885,-5938,7430,30020,41522,30020,7430,-5938,-4885,624,2330,1026,46,0,0,0,0};
long weights[21] = {3,2,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//long weights[31]={-42,-45,-25,37,114,154,100,-64,-278,-411,-317,92,773,1550,2166,2401,2166,1550,773,92,-317,-411,-278,
//                 -64,100,154,114,37,-25,-45,-42};
long total_weight=0;
int DEPTH;
double HZ,DELAY;
// Define application class derived from  TApplication
double FIR(double,int);
double IIR(double);
double INPUT(double);
double FIR_RESPONSE(int);
double IIR_RESPONSE(int);
double HIGH_PASS_RESPONSE(double);
double LOW_PASS_RESPONSE(double);
double LOW_PASS_RESPONSE2(double);
double DECIMATE(double new_sample_rate,double sample_nu,
                double old_sample_rate);
double fftatk(int input,int output,int samples,int k,int inverse);

double WAVE_FORMS[1256][4];
double IIR_DATA[5];
void find_weight();
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
   int count;
   char string[80];

   AssignMenu("FILTER");
   new TStatic(this,-1,"FREQ",350,15,50,25,5);
   THZ = new TEdit(this,4,"2",410,15,80,25,5,FALSE);
   new TStatic(this,-1,"DEPTH",200,15,50,25,5);
   TDEPTH = new TEdit(this,3,"21",260,15,80,25,5,FALSE);
   new TStatic(this,-1,"DELAY",50,15,50,25,5);
   TDELAY = new TEdit(this,2,"-8.5",110,15,50,25,7,FALSE);
   TPlotFilter = new TCheckBox(this,1,"Plot Filter",10,15,90,25,NULL);
   for(count = 0;count<10;count++)
   {
     sprintf(string,"Weight %d",count*2+1);
     new TStatic(this,-1,string,25,215+(count)*20,80,25,5);
     sprintf(string,"%d",weights[count*2]);
     TWEIGHTS[count*2] = new TEdit(this,count*2+5,string,110,215+count*20,50,25,5,FALSE);
     sprintf(string,"Weight %d",count*2+2);
     new TStatic(this,-1,string,325,215+(count)*20,80,25,5);
     sprintf(string,"%d",weights[count*2+1]);
     TWEIGHTS[count*2+1] = new TEdit(this,count*2+6,string,410,215+count*20,50,25,5,FALSE);
   }
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
  for(count=0;count<20;count++)
  {
    TWEIGHTS[count]->GetText(string,4);
    weights[count] = atof(string);
  }
  find_weight();
  memdc = CreateCompatibleDC(DC);
  Blue = CreatePenIndirect(&lpBlue);
  Green = CreatePenIndirect(&lpGreen);
  Red = CreatePenIndirect(&lpRed);
  Purple = CreatePenIndirect(&lpPurple);
  for(count=0;count<5;count++)
    IIR_DATA[count]=0;
  for(count=0;count<200;count++)
    for(count2=0;count2<4;count2++)
      WAVE_FORMS[count][count2] = 0;
  MoveTo(DC,4,150);
  SelectObject(DC,Red);
  for(count=0;count<628;count++)
    LineTo(DC,count+5,150-100*FIR(INPUT(count-DELAY),0));
  MoveTo(DC,4,150);
  SelectObject(DC,Blue);
  for(count=0;count<628;count++)
    LineTo(DC,count+5,150-100*INPUT(count));
  SelectObject(DC,Green);
  for(count=0;count<10;count++)
  {
    MoveTo(DC,0,50+10*count);
    LineTo(DC,324,50+10*count);
  }
  SelectObject(DC,Purple);
  if (TPlotFilter->GetCheck())
  {
    MoveTo(DC,4,150);
    for(count=1;count<310;count++)
      LineTo(DC,count+5,150-100*LOW_PASS_RESPONSE(count));
  }
/*  else
  {
    for(count=0;count<100;count++)
    {
      WAVE_FORMS[count][0]=INPUT(count/4.0);
      WAVE_FORMS[count][1]=0;
    }
    for(count=0;count<50;count++)
    {
      MoveTo(DC,count*628.0/50+5,150);
      LineTo(DC,count*628.0/50+5,150-200*fftatk(0,2,50,count,0));
    }
  }
  MoveTo(DC,4,150);
  SelectObject(DC,Blue);
  for(count=0;count<50;count++)
  {
    LineTo(DC,count*628.0/50+5,150-500*fftatk(2,0,50,count,1));
  }
*/  DeleteObject(Red);
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
  return (sin(x/100.0*HZ));//+sin(x/4.0))/2;
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

  answer = 0;
  depth = DEPTH-which;
  for(count=depth;count>0;count--)
    WAVE_FORMS[count][which]=WAVE_FORMS[count-1][which];
  WAVE_FORMS[0][which]=input;
  for(count=0;count<depth-1;count++)
    answer += WAVE_FORMS[count][which]*weights[count];
  return(answer/total_weight);
}

void find_weight()

{
  int count;

  total_weight = 0;
  for(count=0;count<DEPTH-1;count++)
    total_weight+=labs(weights[count]);
  printf("%ld wieght",total_weight);
}

double IIR(double input)

{
  double temp;

  temp = IIR_DATA[3];
  IIR_DATA[4]=IIR_DATA[1]+IIR_DATA[2]+IIR_DATA[3];
  IIR_DATA[3]=IIR_DATA[2]/2;
  IIR_DATA[2]=IIR_DATA[1]/2;
  IIR_DATA[1]=IIR_DATA[0]+IIR_DATA[3]+temp;
  IIR_DATA[0]=input;
  return(IIR_DATA[4]);
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
  if (max_req<50)
    max_req = 50;
  for(count=0;count<max_req;count++)
    if (answer<(this_fir = INPUT(count)-FIR(INPUT(count-DELAY),0)) && count>100)
      answer = this_fir;
//  return(answer);
  return(1+20*log10(answer+.000000001)/100);
}

double LOW_PASS_RESPONSE(double freq)

{
  int count,count2;
  double answer = 0;
  double this_fir;
  int max_req;

  for(count=0;count<400;count++)
    for(count2=0;count2<5;count2++)
      WAVE_FORMS[count][count2] = 0;
  HZ = freq;
  max_req = 628;
  if (max_req<30)
    max_req = 30;
  for(count=0;count<max_req;count++)
    if (answer<(this_fir = FIR(INPUT(count-DELAY),0)) && count>10)
      answer = this_fir;
//  return(answer);
  return(1+20*log10(answer+.000000001)/100);
}

double LOW_PASS_RESPONSE2(double freq)

{
  int count,count2;
  double answer = 0;
  double this_fir;
  int max_req;

  for(count=0;count<400;count++)
    for(count2=0;count2<5;count2++)
      WAVE_FORMS[count][count2] = 0;
  HZ = freq;
  max_req = 628;
  if (max_req<30)
    max_req = 30;
  for(count=0;count<max_req;count++)
    if (answer<(this_fir = FIR(FIR(INPUT(count-DELAY),0),1)) && count>10)
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
    if (inverse)
      c = complex(cos(2*pi/samples),+sin(2*pi/samples));
    else
      c = complex(cos(2*pi/samples),-sin(2*pi/samples));
    c = pow(c,count*k);
    answer += WAVE_FORMS[count*which_sample][input]*real(c);
    answer2 += WAVE_FORMS[count*which_sample][input+1]*imag(c);
  }
  answer /= 10;
  answer2 /= 1;
  WAVE_FORMS[k][output]=answer;
  WAVE_FORMS[k][output+1]=answer2;
  if (inverse)
    return(answer);
  return(fabs(answer));
}
   
//REGEN_CUSTOMCODE

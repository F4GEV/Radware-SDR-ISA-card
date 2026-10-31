#ifndef __TSEND_MORSE__H
#define __TSEND_MORSE__H

_CLASSDEF(TSEND_MORSEChld)

class TSEND_MORSEChld : public AWindow

{
  TEdit *char_to_send;
  AEdit *WPM;
  TStatic *char_being_sent;
  int morse_off;
  int ID;
  FILE *stream;
  int tuning;
  AButton *Special;
  AButton *Macro_Buttons[MAX_MORSE_BUTTONS];
  int Child_Number;
  int last_macro;

public:
  char Last_Char;
  TSEND_MORSEChld(TWindow *AParent,int id, LPSTR AName,int child_number,int w,int h);
  ~TSEND_MORSEChld();
  void EvActivate(uint active,bool b,HWND c);
  void Edit_Check();
  void Send_Next_Char();
  void Change_WPM();
  void Add_CallSign();
  void End_Morse();
  void Send_File();
  void Read_File();
  void Paint(TDC& DC, BOOL,TRect &);
  void Add_Tune_Char();
  void Tune();
  void CQ_Button();
  void Macro(WPARAM ID);
  void Change_Macro(WPARAM ID);
  void Finish_Macro();
  void SetupWindow();
  DECLARE_RESPONSE_TABLE(TSEND_MORSEChld);
};
#endif // __TSEND_MORSE__H

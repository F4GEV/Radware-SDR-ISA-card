#include "thisCls.h"
#include <stdio.h>

TAboutDlg::TAboutDlg(TWindow *AParent,LPSTR AName)
				  :TDialog(AParent,"ABOUT")

{
}

TAboutDlg::~TAboutDlg()

{
}

void TAboutDlg::Ordering()

{
  GetModule()->ExecDialog(new TDialog(this, "Ordering"));
}

DEFINE_RESPONSE_TABLE1(TAboutDlg,TDialog)
  EV_BN_CLICKED(101,Ordering),
END_RESPONSE_TABLE;




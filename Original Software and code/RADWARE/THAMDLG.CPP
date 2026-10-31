/*  Project logbook
    
    Copyright © 1994. All Rights Reserved.

    SUBSYSTEM:    logbook.apx Application
    FILE:         thamdlg.cpp
    AUTHOR:       


    OVERVIEW
    ========
    Source file for implementation of THamDlg (TDialog).      
*/

#include <owl\owlpch.h>
#pragma hdrstop

#include "thamdlg.h"


//{{THamDlg Implementation}}


THamDlg::THamDlg (TWindow* parent, TResId resId, TModule* module):
    TDialog(parent, resId, module)
{
    // INSERT>> Your constructor code here.

}


THamDlg::~THamDlg ()
{
    Destroy();

    // INSERT>> Your destructor code here.

}


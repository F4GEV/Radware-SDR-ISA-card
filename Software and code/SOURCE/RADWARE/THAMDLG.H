#if !defined(__thamdlg_h)              // Sentry, use file only if it's not already included.
#define __thamdlg_h

/*  Project logbook
    
    Copyright © 1994. All Rights Reserved.

    SUBSYSTEM:    logbook.apx Application
    FILE:         thamdlg.h
    AUTHOR:       


    OVERVIEW
    ========
    Class definition for THamDlg (TDialog).      
*/

#include <owl\owlpch.h>
#pragma hdrstop

#include <owl\dialog.h>

#include "logbkapp.rh"            // Definition of all resources.


//{{TDialog = THamDlg}}
class THamDlg : public TDialog {
public:
    THamDlg (TWindow* parent, TResId resId = IDD_HAMS, TModule* module = 0);
    virtual ~THamDlg ();
};    //{{THamDlg}}


#endif                                      // __thamdlg_h sentry.


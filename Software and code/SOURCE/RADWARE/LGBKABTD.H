#if !defined(__lgbkabtd_h)              // Sentry, use file only if it's not already included.
#define __lgbkabtd_h

/*  Project logbook
    
    Copyright © 1994. All Rights Reserved.

    SUBSYSTEM:    logbook.exe Application
    FILE:         lgbkabtd.h
    AUTHOR:       


    OVERVIEW
    ========
    Class definition for logbookAboutDlg (TDialog).      
*/


#include <owl\owlpch.h>
#pragma hdrstop

#include "logbkapp.rh"                  // Definition of all resources.


//{{TDialog = logbookAboutDlg}}
class logbookAboutDlg : public TDialog {
public:
    logbookAboutDlg (TWindow *parent, TResId resId = IDD_ABOUT, TModule *module = 0);
    virtual ~logbookAboutDlg ();

//{{logbookAboutDlgVIRTUAL_BEGIN}}
public:
    void SetupWindow ();
//{{logbookAboutDlgVIRTUAL_END}}
};    //{{logbookAboutDlg}}


// Reading the VERSIONINFO resource.
class ProjectRCVersion {
public:
    ProjectRCVersion (TModule *module);
    virtual ~ProjectRCVersion ();

    BOOL GetProductName (LPSTR &prodName);
    BOOL GetProductVersion (LPSTR &prodVersion);
    BOOL GetCopyright (LPSTR &copyright);
    BOOL GetDebug (LPSTR &debug);

protected:
    LPBYTE      TransBlock;
    void FAR    *FVData;

private:
    // Don't allow this object to be copied.
    ProjectRCVersion (const ProjectRCVersion &);
    ProjectRCVersion & operator =(const ProjectRCVersion &);
};


#endif                                      // __lgbkabtd_h sentry.

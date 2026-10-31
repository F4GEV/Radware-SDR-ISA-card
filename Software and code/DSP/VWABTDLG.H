#if !defined(__vwabtdlg_h)              // Sentry, use file only if it's not already included.
#define __vwabtdlg_h

/*  Project view
    Comer Communications, Inc.
    Copyright © 1994. All Rights Reserved.

    SUBSYSTEM:    view.exe Application
    FILE:         vwabtdlg.h
    AUTHOR:       Alan Comer


    OVERVIEW
    ========
    Class definition for viewAboutDlg (TDialog).      
*/


#include <owl\owlpch.h>
#pragma hdrstop

#include "viewapp.rh"                  // Definition of all resources.


//{{TDialog = viewAboutDlg}}
class viewAboutDlg : public TDialog {
public:
    viewAboutDlg (TWindow *parent, TResId resId = IDD_ABOUT, TModule *module = 0);
    virtual ~viewAboutDlg ();

//{{viewAboutDlgVIRTUAL_BEGIN}}
public:
    void SetupWindow ();
//{{viewAboutDlgVIRTUAL_END}}
};    //{{viewAboutDlg}}


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


#endif                                      // __vwabtdlg_h sentry.

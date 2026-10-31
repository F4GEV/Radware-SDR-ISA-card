PSA SDK 1.0 README.TXT

This file contains information that is supplemental to the 1.0 Release Note.


Installation:

There are two installation phases for installing Revision 1.0 of the Personal
Sound Architecture (PSA) Software Development Kit (SDK).  The first phase
installs the bulk of the files to your system.  The second phase installs 
and configures the PSA SDK device driver to your system.  Both these steps are
necessary and should be performed in the following order.  Note that both 
installation phases must be performed under Windows and not MS-DOS.  

The first phase is accomplished by inserting the installation disk into your
floppy drive and running the program "install.exe".  Follow the directions
on your screen.  This phase will install all SDK programs and files except for
the PSA SDK device driver.  

The second phase installs the PSA SDK device driver and is installed like any 
Windows installable device driver.  Insert the installation disk into your 
floppy drive.  Run the Drivers applet in the Control Panel application located
in the Main program group.  Select Add, Unlisted/Updated and when prompted, 
choose the PSA SDK Device Driver.  After installing the device driver, you will
be prompted for IRQ and DMA settings for each PSA board installed in your 
system.  Note that this release does not contain DMA support, but you must 
select a DMA channel anyway.  Be sure you IRQ selection does not conflict with
other I/O devices in your system.  After making your selections, restart 
Windows in order for the changes to take effect.  


User Modifications:

After performing the above described installation procedures, you may have to
perform the following modifications:

     - If the SDK was installed in a directory other than c:\adi_dsp\scope, and
       you are using Borland 'C', the include paths in the project file 
       APP.PRJ, located in {install_dir}\example\example1\host have to be
       changed using the Options pulldown in the IDE. 

     - If using Borland 'C', the directory specification for the Borland
       system include files may have to be changed.
       

Documentation Errata:

The following should replace the Supported Callbacks paragraph in section 
2.1.6 DSP Manager Programming of the SCOPE Technical Reference:

      "Because the host processor and the DSP processor are loosely 
      coupled, there may be times when the services requested by the 
      application from the DSP may not occur immediately. Due to this 
      asynchronous nature, the DSP manager relies on the Windows callback 
      mechanism to notify clients of asynchronous events, such as the arrival 
      of a message. Clients are notified by a callback function or a windows 
      message procedure, as specified in the dspLoadAlg and dspPreloadAlg 
      API functions. The user should be aware that windows message 
      procedures are called by Windows according to the Windows scheduling 
      algorithm, and therefore may not provide sufficient real-time response. 
      Callback functions, however, are called directly by the DSP Manager, 
      often during interrupt context, and therefore provide a more real time 
      response. Because they may be called during interrupt context, only a 
      subset of Windows and DSP Manager function calls are allowed. Functions 
      calls allowed in callback functions are: "


Bulletin Board Information:

An SDK Forum has been set up on the SPD Bulletin Board Service.  Located
in the Signal Computing Information section on the BBS.  Use this area
for communicating with ADI and other SDK customers.  Also located in 
this section is the SDK Frequently Asked Questions (FAQ) list.  The BBS
can be reached at (617)461-4258 or (617)461-4259.
  



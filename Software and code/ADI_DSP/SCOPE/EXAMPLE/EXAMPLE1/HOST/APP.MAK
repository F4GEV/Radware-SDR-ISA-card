#--------------------------------------
# APP.MAK make file for APP.EXE
# intended for use with Microsoft C
#--------------------------------------

CC      = cl -c -AL -G2sw -Ow -W3 -Zp -Tp -I..\..\..\include
LINK    = link /nod
WINLIB  = slibcew oldnames libw ddeml ..\..\..\bin\dspapi.lib

app.exe: app.obj app.def
        $(LINK) app, app, NUL, $(WINLIB), app 

app.obj: app.c
        $(CC) app.c


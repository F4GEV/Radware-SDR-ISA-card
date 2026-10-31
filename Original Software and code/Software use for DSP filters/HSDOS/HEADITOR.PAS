(************************** LOCAL COMPILER DIRECTIVES **********************)
{$N+,E+} {Numeric Processing ON (80x87 Mode), 80x87 Emulation ON}
{$D-}    {Debug Information OFF}
{$L-}    {Local Symbol Information OFF}
{$A+}    {Word alignment}

{$B-}    {Short-Circuit Boolean evaluation}
{$F+}    {Force far calls}
{$I+}    {Input/Output checking ON}
{$O-}    {Allow to be overlayed}
{$R-}    {Range checking OFF}
{$S-}    {Stack checking OFF}
{$V+}    {Var-String checking ON}
(***************************************************************************)

PROGRAM headitor;
{

=============================================================================

        This is an example  source code file intended to illustrate how to
        integrate a user-defined external module into Hypersignal, as well
        as illustrate the format  required for a binary time waveform data
        file. This example edits the header in a Hypersignal waveform file.

                                  Hyperception
                            9550 Skillman LB125 #302
                                Dallas, TX 75243
                                  214-343-8525

=============================================================================

}
Uses Crt;

CONST num_items=13;

      ESC=27;    {escape key}

      EXTKEY=0;  {Extended Key Codes}

      UCUR=72;
      DCUR=80;
      RCUR=77;
      LCUR=75;
      HCUR=71;
      ECUR=79;
      INS=82;
      DEL=83;
      BACK=8;
      F1=59;
      F10=68;


LABEL 5,10,999;


{====== DECLARATIONS REQUIRED FOR PTRACKER PROGRAM ===============}

VAR              Amp: Integer;     {max amplitude for whole file}
           Framesize: Integer;     {number of samples/frame for analysis period}
               Fs_Lo: Integer;     {sampling frequency}
              FFTord: Integer;     {order of FFT}
          Numelem_Lo: Integer;     {number of samples in file}
             Overlap: Integer;     {either FFT or LPC Autocorr. overlap amount}
            Typedata: Integer;     {1=autocor. data, 2=filter data, 0=else}
           Typedata2: Integer;     {number of channels interleaved in waveform}
               Fs_Hi: Integer;     {samp. freq. "High" value (see users man.)}
          Numelem_Hi: Integer;     {number of samples "High" value}

        i,j,insert_enable,ordkey,status,CODE: integer;

        abort,save: boolean;

        scale_factor: single;
        extended_header: byte;
        data_type: byte;
        window_type: byte;
        complex: byte;
        num_channels: shortint;
        lpc_order: shortint;

        fields: array[0..num_items-1] of string[37];

        FILENAME: string[60];

        Z_TXT,Y: TEXT;
        Z: FILE OF INTEGER;

        NUMELEM,FS: REAL;

        key: char;

{$I CONFIG.DOC}    { CONFIGURATION FILE READ/WRITE, DESCRIPTION }


{===========================================================================}

PROCEDURE CURSOROFF;
BEGIN
{
     MOV  AH,1
     MOV  CH,38H
     MOV  CL,7
     INT  10H
}
     INLINE
     ($B4/$1/$B5/$38/$B1/$7/$CD/$10);
END;

PROCEDURE CURSORON;
BEGIN
{
     MOV  AH,1
     MOV  CH,6
     MOV  CL,7
     INT  10H
}
     INLINE
     ($B4/$1/$B5/$6/$B1/$7/$CD/$10);
END;

PROCEDURE BEEP;
BEGIN
    SOUND(3000);
    DELAY(100);
    SOUND(2000);
    DELAY(100);
    SOUND(3000);
    DELAY(100);
    NOSOUND;
END;

PROCEDURE BEEP_ERROR;
BEGIN
    SOUND(200);
    DELAY(250);
    SOUND(1000);
    DELAY(250);
    NOSOUND;
END;


{===================== START OF MAIN ROUTINE, PTRACKER ======================}

BEGIN { echoread }

        FILENAME:='';

{$I-}
      { GET HEADITOR PROGRAM CONFIG FILE (NOT TO BE CONFUSED WITH HYPERSIGNAL
        CONFIGURATION FILE }

        ASSIGN(Z_TXT, 'HEADITOR.CNF');
        RESET(Z_TXT);
        if IORESULT=0 THEN BEGIN
            READLN(Z_TXT, FILENAME);
            CLOSE(Z_TXT);
        END;

{============ read in Hypersignal configuration file HSxxxx.CNF if needed }

        if FILENAME='' then begin

            config_read(status);
            if status<>0 then begin

              { error code here, if any }
               
            end
            else FILENAME:=FIELD[2,3,0]+FIELD[2,3,1]+FIELD[2,3,2]+'.TIM'
        end;

{============ begin processing this module is designed to do ============}

5:      Amp:=0;
        Framesize:=1;
        Fs_Lo:=0;
        FFTord:=0;
        Numelem_Lo:=0;
        Overlap:=0;
        Typedata:=0;
        Typedata2:=0;
        Fs_Hi:=0;
        Numelem_Hi:=0;

        FS:=0;
        NUMELEM:=1;
        data_type:=0;
        extended_header:=0;
        complex:=0;
        lpc_order:=0;
        window_type:=0;
        scale_factor:=1;

        ASSIGN(Z, FILENAME);
        RESET(Z);

        if IORESULT=0 then begin
            READ(Z, Amp);
            READ(Z, Framesize);
            READ(Z, Fs_Lo);
            READ(Z, FFTord);
            READ(Z, Numelem_Lo);
            READ(Z, Overlap);
            READ(Z, Typedata);
            READ(Z, Typedata2);
            READ(Z, Fs_Hi);
            READ(Z, Numelem_Hi);

            CLOSE(Z);

            NUMELEM:=16384.0*Numelem_Hi+Numelem_Lo;

            FS:=32767.0*Fs_Hi+Fs_Lo;

            data_type:=Typedata AND 3;

            if (Typedata AND 4)<>0 then extended_header:=1
            else extended_header:=0;

            if (Typedata AND 8)<>0 then complex:=1
            else complex:=0;

            lpc_order:=(Typedata SHR 4) AND 31;         

            window_type:=(Typedata SHR 9) AND 7;

            scale_factor:=EXP(((Typedata AND $FFFFF000) DIV 4096)*LN(2));

            num_channels:=(Typedata2 AND 63)+1;
        end
        else beep_error;

        fields[0]:=FILENAME;

        Str(Amp, fields[1]);
        Str(Framesize, fields[2]);
        Str(Overlap, fields[3]);
        Str(num_channels, fields[4]);
        Str(Fs, fields[5]);
        Str(Numelem, fields[6]);

        Str(data_type, fields[7]);
        Str(extended_header, fields[8]);
        Str(complex, fields[9]);
        Str(lpc_order, fields[10]);
        Str(window_type, fields[11]);
        Str(scale_factor, fields[12]);

        TEXTBACKGROUND(BLACK);    {user-defined module responsible for screen}
        TEXTCOLOR(WHITE);
        CLRSCR;
 
        GOTOXY(20,2);
        WRITE('WAVEFORM HEADER DISPLAY AND EDITING');

        GOTOXY(1,5);
        WRITE('                    Drive+Path+Filename: ',fields[0]);

        GOTOXY(1,6);
        WRITE('                    Max |amp| (0-32767): ',fields[1]);

        GOTOXY(1,7);
        WRITE('                     Framesize 0-32767): ',fields[2]);
   
        GOTOXY(1,8);
        WRITE('                Overlap (0-Framesize-1): ',fields[3]);

        GOTOXY(1,9);
        WRITE('                    Num Channels (0-63): ',fields[4]);
   
        GOTOXY(1,10);
        WRITE('                     Sampling Frequency: ',fields[5]);
   
        GOTOXY(1,11);
        WRITE('                     Number of Elements: ',fields[6]);

        GOTOXY(1,12);
        WRITE(' Data Type (0=tim, 1=LPC, 2=H[n], 3=ud): ',fields[7]);
   
        GOTOXY(1,13);
        WRITE('          Extended Header (0=no, 1=yes): ',fields[8]);
   
        GOTOXY(1,14);
        WRITE('             Complex Data (0=no, 1=yes): ',fields[9]);
   
        GOTOXY(1,15);
        WRITE('                       LPC Order (0-31): ',fields[10]);
   
        GOTOXY(1,16);
        WRITE('Window (0=R,1=Hm,2=Hn,3=Bl,4=Bt,5-7=ud): ',fields[11]);
   
        GOTOXY(1,17);
        WRITE('             Scaling Factor (1/256-128): ',fields[12]);
       
        GOTOXY(1,20);
        WRITE('                     ESC=quit, F1=reload, F10=save and exit');
       

        j:=0;       { line }

10:     i:=0;       { cursor position }

        save:=false;
        abort:=false;

        insert_enable:=0;  { insert mode }

        cursoron;

        repeat
            gotoxy(42, 5+j);
            write(fields[j]);
            clreol;
            if i>LENGTH(fields[j]) then i:=LENGTH(fields[j]);
            gotoxy(42+i, 5+j);

            key:=UpCase(readkey);

            if ORD(key)=EXTKEY then begin
                ordkey:=ORD(readkey);

                if ordkey=UCUR then begin
                    if j>0 then j:=j-1
                    else j:=num_items-1;

                    insert_enable:=0;
                end
                else if ordkey=DCUR then begin
                    if j<num_items-1 then j:=j+1
                    else j:=0;

                    insert_enable:=0;
                end
                else if ordkey=RCUR then begin
                    if i<LENGTH(fields[j]) then i:=i+1
                    else i:=LENGTH(fields[j]);
                end
                else if ordkey=LCUR then begin
                    if i>0 then i:=i-1
                    else i:=0;
                end
                else if ordkey=HCUR then begin
                    i:=0;
                end
                else if ordkey=ECUR then begin
                    i:=LENGTH(fields[j]);
                end
                else if ordkey=INS then begin
                    insert_enable:=1-insert_enable;
                end
                else if ordkey=DEL then begin
                    if i<LENGTH(fields[j]) then begin
                        DELETE(fields[j], i+1, 1);
                    end
                end
                else if ordkey=F1 then begin
                    FILENAME:=fields[0];

                    goto 5;
                end
                else if ordkey=F10 then save:=true
            end
            else if ORD(key)=ESC then abort:=true
            else if ORD(key)=13 then begin
                if i>0 then begin
                    fields[j][0]:=CHR(i);
                    clreol;
                end;

                if j<num_items-1 then j:=j+1
                else j:=0;

                insert_enable:=0;
            end
            else if ORD(key)=BACK then begin
                if i>0 then begin
                    DELETE(fields[j], i, 1);
                    i:=i-1;
                end
            end
            else if ((key>='0') and (key<='9')) or
                    ((key>='A') and (key<='Z')) or
                    (key='.') or (key='-') or (key=' ') or
                    (key='\') or (key=':') then begin

                if i+1>LENGTH(fields[j]) then fields[j]:=fields[j]+key
                else begin
                    if insert_enable=0 then fields[j][i+1]:=key
                    else INSERT(key, fields[j], i+1);
                end;

                i:=i+1;
            end
            else beep;

        until save or abort;

        cursoroff;

        if abort then goto 999;

        FILENAME:=fields[0];

        j:=1;
        Val(fields[j], Amp, CODE);
        if (Amp<0) or (Amp>32767) or (CODE<>0) then begin
            beep_error;
            goto 10;
        end;

        j:=j+1;
        Val(fields[j], Framesize, CODE);
        if (Framesize<0) or (Framesize>32767) or (CODE<>0) then begin
            beep_error;
            goto 10;
        end;

        j:=j+1;
        Val(fields[j], Overlap, CODE);
        if (Overlap<0) or (Overlap>Framesize-1) or (CODE<>0) then begin
            beep_error;
            goto 10;
        end;

        j:=j+1;
        Val(fields[j], num_channels, CODE);
        if (num_channels<1) or (num_channels>63) or (CODE<>0) then begin
            beep_error;
            goto 10;
        end;

        j:=j+1;
        Val(fields[j], Fs, CODE);
        if (Fs<0) or (Fs>1073709056) or (CODE<>0) then begin
            beep_error;
            goto 10;
        end;

        j:=j+1;
        Val(fields[j], Numelem, CODE);
        if (Numelem<1) or (Numelem>268451840) or (CODE<>0) then begin
            beep_error;
            goto 10;
        end;

        j:=j+1;
        Val(fields[j], data_type, CODE);
        if (data_type<0) or (data_type>3) or (CODE<>0) then begin
            beep_error;
            goto 10;
        end;

        j:=j+1;
        Val(fields[j], extended_header, CODE);
        if (extended_header<0) or (extended_header>1) or (CODE<>0) then begin
            beep_error;
            goto 10;
        end;

        j:=j+1;
        Val(fields[j], complex, CODE);
        if (complex<0) or (complex>1) or (CODE<>0) then begin
            beep_error;
            goto 10;
        end;

        j:=j+1;
        Val(fields[j], LPC_order, CODE);
        if (LPC_order<0) or (LPC_order>31) or (CODE<>0) then begin
            beep_error;
            goto 10;
        end;

        j:=j+1;
        Val(fields[j], window_type, CODE);
        if (window_type<0) or (window_type>7) or (CODE<>0) then begin
            beep_error;
            goto 10;
        end;

        j:=j+1;
        Val(fields[j], scale_factor, CODE);
        if (scale_factor<1/256) or (scale_factor>128) or (CODE<>0) then begin
            beep_error;
            goto 10;
        end;

        Numelem_Hi:=TRUNC(NUMELEM/16384);
        Numelem_Lo:=ROUND(NUMELEM-Numelem_Hi*16384.0);

        FS_Hi:=TRUNC(FS/32767);
        FS_Lo:=ROUND(FS-32767.0*FS_Hi);

        Typedata:=data_type;

        Typedata:=Typedata OR (extended_header SHL 2);

        Typedata:=Typedata OR (complex SHL 3);

        Typedata:=Typedata OR (LPC_order SHL 4);

        Typedata:=Typedata OR (window_type SHL 9);

        Typedata:=Typedata OR (ROUND(LN(scale_factor)/LN(2)) SHL 12);

        Typedata2:=(Typedata2 AND $FFC0) OR (num_channels-1);


        ASSIGN(Z, FILENAME);
        RESET(Z);

        j:=0;
        if IORESULT<>0 then begin
            beep_error;
            goto 10;
        end;

        WRITE(Z, Amp);
        WRITE(Z, Framesize);
        WRITE(Z, Fs_Lo);
        WRITE(Z, FFTord);
        WRITE(Z, Numelem_Lo);
        WRITE(Z, Overlap);
        WRITE(Z, Typedata);
        WRITE(Z, Typedata2);
        WRITE(Z, Fs_Hi);
        WRITE(Z, Numelem_Hi);

        CLOSE(Z);

        ASSIGN(Z_TXT, 'HEADITOR.CNF');
        REWRITE(Z_TXT);
        if IORESULT=0 THEN BEGIN
            WRITELN(Z_TXT, FILENAME);
            CLOSE(Z_TXT);
        END;

999:    clrscr;
        cursoron;
END.

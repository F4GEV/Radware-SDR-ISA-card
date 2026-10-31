PROGRAM Ptracker;
{

=============================================================================

        This is an example  source code file intended to illustrate how to
        integrate a user-defined external module into Hypersignal, as well
        as illustrate the format  required for a binary time waveform data
        file. This example is a basic peak tracker module which processes
        an input frequency-domain filename, and creates both an output time
        domain waveform containing a sequence of peak locations, and also a
        frequency domain waveform containing a replicate of the input wave-
        form but with only the peak locations in each frame set to a non-
        zero value (arbitrarily chosen as the max. value in the input wave-
        form).

        The PTRACKER.EXE file on the distribution diskettes was compiled
        from PTRACKER.PAS using Turbo Pascal 5.5 (Type: 'TPC ptracker <CR>').

                                  Hyperception
                            9550 Skillman LB125 #302
                                Dallas, TX 75243
                                  214-343-8525

=============================================================================

}
Uses Crt;

TYPE    FileRecord  = record
                 Amp: Integer;     {max amplitude for whole file}
           Framesize: Integer;     {number of samples/frame for analysis period}
                  Fs: Integer;     {sampling frequency}
              FFTORD: Integer;     {order of FFT}
             Numelem: Integer;     {number of samples in file}
             Overlap: Integer;     {either FFT or LPC Autocorr. overlap amount}
            Typedata: Integer;     {1=autocor. data, 2=filter data, 0=else}
        Num_Channels: Integer;     {number of channels interleaved in waveform}
               Fs_Hi: Integer;     {samp. freq. "High" value (see users man.)}
          Numelem_Hi: Integer;     {number of samples "High" value}
       end;


{====== DECLARATIONS REQUIRED FOR PTRACKER PROGRAM ===============}

VAR     FileData:                        FileRecord;
        Y:                               Text;
        Status:                          Integer;
        total_numelem,num_frames,j:      Longint;
        mag_length,cur_max,cur_pos,
        i,k,zero,next_samp:              Integer;
        Time_Numelem_Hi,Time_Numelem,
        Time_Framesize:                  Integer;
        Z,ZF,ZT:                         File of Integer;
        Fileout,FREQFILENAME:            String[25];
        K1:                              char;


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


{===================== START OF MAIN ROUTINE, PTRACKER ======================}

BEGIN { Ptracker }

        TEXTBACKGROUND(BLACK);    {user-defined module responsible for screen}
        TEXTCOLOR(WHITE);
        CLRSCR;
        CURSOROFF;
        GOTOXY(1,1);

WRITELN('This is a basic peak tracker module intended to form a skeletal');
WRITELN('foundation for peak tracking and additional, related analysis.');
WRITELN('The input frequency file is assumed to be the frequency-domain');
WRITELN('filename currently listed in the Hypersignal User-Setup area.');
WRITELN('Two output files are created: the first a time-domain file containing');
WRITELN('a consecutive sequence of maximum peak locations from the input file');
WRITELN('and the second, essentially another frequency-domain file,');
WRITELN('containing one non-zero point in each frame FFT frame representing');
WRITELN('the maximum peak from the input file frame.  The time_domain output');
WRITELN('is suitable for filtering and othe subsequent processing, while the');
WRITELN('frequency-domain output file is suitable for comparison viewing in');
WRITELN('frequency-domain displays with the original input file. Both output');
WRITELN('filenames are created by attaching "_p" to the input filename.');

WRITELN;
WRITE('Enter Esc to abort, any other key to continue: ');
CURSORON;
K1:=ReadKey;
CURSOROFF;
if K1=CHR(27) then begin
    CURSORON;
    exit;
end
else if K1=CHR(0) then K1:=ReadKey;

{============ read in Hypersignal configuration file HSWORK.CNF ===========}

        config_read(status);
        if status<>0 then begin
            cursoron;
            exit;
        end;

{============ begin processing this module is designed to do ============}

        FREQFILENAME:=FIELD[2,3,3]; { current user-setup area FREQ FILENAME }
        Fileout:=COPY(FREQFILENAME, 1, 6)+'_p';

        ASSIGN(Z, DRIVE+PATH+FREQFILENAME+'.FRQ');
        ASSIGN(ZF, DRIVE+PATH+Fileout+'.FRQ');
        ASSIGN(ZT, DRIVE+PATH+Fileout+'.TIM');

{$I-}
        RESET(Z);
        if IORESULT<>0 then begin
            WRITELN;
            WRITELN('INPUT FREQUENCY FILE NOT FOUND; NO PROCESSING PERFORMED');
            cursoron;
            exit;
        end;

        READ(Z, FileData.Amp, 
                FileData.Framesize,
                FileData.Fs,
                FileData.FFTORD,
                FileData.Numelem,
                FileData.Overlap,
                FileData.Typedata,
                FileData.Num_Channels,
                FileData.Fs_Hi,
                FileData.Numelem_Hi);

        total_numelem:=ROUND(16384.0*FileData.Numelem_Hi+FileData.Numelem);
        mag_length:=1 SHL (FileData.FFTORD-1);
        num_frames:=ROUND(total_numelem/(mag_length+1));

        REWRITE(ZT);
        REWRITE(ZF);

        WRITE(ZF, FileData.Amp, 
                  FileData.Framesize,
                  FileData.Fs,
                  FileData.FFTORD,
                  FileData.Numelem,
                  FileData.Overlap,
                  FileData.Typedata,
                  FileData.Num_Channels,
                  FileData.Fs_Hi,
                  FileData.Numelem_Hi);

        Time_Numelem_Hi:=TRUNC(num_frames/16384);
        Time_Numelem:=ROUND(num_frames-Time_Numelem_Hi*16384.0);

        if num_frames<=32767 then Time_Framesize:=num_frames
        else Time_Framesize:=32767;

        WRITE(ZT, mag_length, 
                  Time_Framesize,
                  FileData.Fs,
                  FileData.FFTORD,
                  Time_Numelem,
                  FileData.Overlap,
                  FileData.Typedata,
                  FileData.Num_Channels,
                  FileData.Fs_Hi,
                  Time_Numelem_Hi);

        zero:=0;

        for j:=0 to num_frames-1 do begin
            cur_pos:=0;
            cur_max:=0;

            for k:=0 to mag_length do begin   { find max, max pos. in frame }
                READ(Z, next_samp);

                if next_samp>cur_max then begin
                    cur_max:=next_samp;
                    cur_pos:=k;
                end;
            end;

            for k:=0 to mag_length do begin   { output to f-domain file }
                if k=cur_pos then WRITE(ZF, FileData.Amp)
                else WRITE(ZF, zero);
            end;

            WRITE(ZT, cur_pos);                 { output to t-domain file }
        end;

        CLOSE(Z);
        CLOSE(ZF);
        CLOSE(ZT);

{============ update any configuration file values, if desired ============}

        FIELD[2,3,2]:=Fileout;         { update TIME filename }
        FREQDUALFILE:=Fileout;         { update FREQ secondary-display name }
        MEM_STATEY[0]:=0;              { set initial menu selection
                                             to time display }

{============ write Hypersignal configuration file back out ==========}

        config_write(status);

        cursoron;
END.

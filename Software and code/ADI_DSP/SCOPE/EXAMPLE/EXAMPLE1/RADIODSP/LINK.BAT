set sdk=d:\adi_dsp\scope
ld21 -i linkfile -a %sdk%\bin\psa\psa.ach -e alg -user %sdk%\bin\psa\libdebug.a -lib -x -g 
set sdk_inc=
pause DSP File Assembled


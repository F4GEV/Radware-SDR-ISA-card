set sdk=..\..\..

g21 -a %sdk%\bin\psa\psa.ach -I%sdk%\include alg.c  -c 
g21 -a %sdk%\bin\psa\psa.ach -I%sdk%\include fft.dsp -c      
g21 -a %sdk%\bin\psa\psa.ach -I%sdk%\include gen.dsp -c 
g21 -a %sdk%\bin\psa\psa.ach -I%sdk%\include sin.dsp -c
g21 -a %sdk%\bin\psa\psa.ach -I%sdk%\include fir.dsp -c
g21 -a %sdk%\bin\psa\psa.ach -I%sdk%\include regutil.dsp -c
ld21 -i linkfile -a %sdk%\bin\psa\psa.ach -e alg -user %sdk%\bin\psa\libdebug.a -lib -x -g 
spl21 alg alg -ld 

set sdk_inc=


set sdk=d:\adi_dsp\scope

g21 -a %sdk%\bin\psa\psa.ach -I%sdk%\include alg.c  -c 
ld21 -i linkfile -a %sdk%\bin\psa\psa.ach -e alg -user %sdk%\bin\psa\libshell.a -lib -x -g 
spl21 alg alg -ld 

set sdk_inc=


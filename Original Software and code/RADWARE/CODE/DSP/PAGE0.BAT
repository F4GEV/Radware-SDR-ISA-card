asm21 page0.dsp -Id:\radware\code\dsp -c
pause
ld21 -i linkfile -a pssos.ach -e -lib page0 -x -g
spl21 radware radware -ld 

copy radware.ld ..\radware.ld
copy radware.ld d:\scope\example\example1\hosttest\radware.ld
pause DSP File Assembled


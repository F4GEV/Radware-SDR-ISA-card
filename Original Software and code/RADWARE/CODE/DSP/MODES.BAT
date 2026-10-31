asm21 modes.dsp -Id:\radware\code\dsp -c
ld21 -i linkfile -a pssos.ach -e radware -lib -x -g
spl21 radware radware -ld 

copy radware.ld ..\radware.ld
copy radware.ld d:\scope\example\example1\hosttest\radware.ld
pause DSP File Assembled


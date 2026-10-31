Warning: This file is extremely experimental!

Now that I have that out of the way,

Welcome!
  This file is the first attempt to allow our users to write programs for use
with the DSP100 card. If you download this and have ANY problems at all,
please call me. As this is the first release, I do expect it to be relatively
unclear, but until I get feedback, I will not know what to improve. I had 
been slowly improving it overall, but that was progressing too slowly.
  You will want to open and read the RADLIB.H file. I have documented this so
that you can see what each function or class does. Remember that you must
call Load_DSP() before any other function if you want to access the DSP 100.
Alternately, you may call Simulation_Mode() first and then you will never be
able to access the DSP 100. I find this very useful for debugging. If the 
bug goes away, you will know that it is either in the library or it is a
result of feedback from the library. 
  You should also note that if the Load_DSP() function does not detect the
DSP 100 card, it will switch to simulation mode automatically.
  Enjoy.

  Alan D. Comer


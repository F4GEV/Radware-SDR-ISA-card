#include <stdio.h>
#include <math.h>

int count;

main()

{
  FILE *stream;
  double temp;
  int arctan;

  stream = fopen("d:arctan","w");
  for (count=0;count<256;count++)
  {
    temp = atan((count)/16.0/*-8*/);
    arctan = temp * 0x29F0; /*0x2A6E*/
//    arctan = temp * 0x5870-0x8000;
//    arctan = temp * 0x2C38+0x4000;
    fprintf(stream,"%04X00\n",arctan);
  }
  fclose(stream);
}

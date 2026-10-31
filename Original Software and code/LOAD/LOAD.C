#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <process.h>
#include <string.h>
#include <time.h>
#include "load.h"
//#include "c:\msvc\sa\include\progctl.h"
#include "pssini.h"

/* PSS port addresses */

#define PSS_DATA_614     0x220
#define PSS_STATUS_614   0x222
#define PSS_CONTROL_614  0x222
#define PSS_ID_VERS_614  0x224

#define PSS_DATA_615     0x230
#define PSS_CONTROL_615  0x232
#define PSS_STATUS_615   0x234
#define PSS_INDEXED_615  0x236

/* configuration addresses */

#define PSS_CONFIG_614   0x230
#define WSS_CONFIG_614   0x232
#define SB_CONFIG_614    0x234
#define CD_CONFIG_614    0x236
#define MIDI_CONFIG_614  0x238
#define UART_CONFIG_614  0x23a

#define ID_VERSION_615   0
#define PSS_CONFIG_615   1
#define CD_CONFIG_615    2
#define MIDI_CONFIG_615  3
#define UART_CONFIG_615  4
#define SB_CONFIG_615    6
#define WSS_CONFIG_615   7
#define PIO_615          8

/* PSS equates */

#define PSS_FLAG3        0x0800
#define PSS_FLAG2        0x0400
#define PSS_FLAG1        0x1000
#define PSS_FLAG0        0x0800

#define PSS_WRITE_EMPTY  0x8000
#define PSS_READ_FULL    0x4000

/* global stuff */

#define DELAY 100    /* timeout in milliseconds */

int port_offset=0,display=1;
time_t timeout;

void do_hard_reset(unsigned wControl)
{
 outpw(wControl,0x2000);               // reset the DSP
 timeout=clock()+DELAY;
 while (timeout>clock())
  outpw(wControl,0x0000);   // set the DSP reset line high
 if (display) puts("Hardware reset performed.");
} /* do_hard_reset() */

int isblank(char *line)
{
 if (*line==0 || *line=='\n') return TRUE;
 
 while (*line!=0)
  {
   if (*line==' ' || *line=='\n') 
    line++;
   else    
    return FALSE;
  }
  
 return TRUE;
} /* isblank() */

int findeq(char *str)
{
 int i;
 
 for (i=0; *str!=0; str++,i++)
  if (*str=='=') return i;
 return -1;
 
} /*findeq()*/

int getval(char *str)
{
 int t,u;
 
 if (*str=='0' && str[1]=='x')
  {
   u=sscanf(str,"%x",&t);
   if (u==0) t=0;
  }
 else
  t=atoi(str);
 
 return t;  
}

void PSS_write(int data)
{
 while(!(inpw(PSS_STATUS_614+port_offset) & PSS_WRITE_EMPTY));
 outpw(PSS_DATA_614+port_offset,data);
}


void set_amp()
{
 if (amp.volume > 6) amp.volume = 6;
 if (amp.volume < -64) amp.volume = -64;
 if (amp.bass > 15) amp.bass = 15;
 if (amp.bass < -12) amp.bass = -12;
 if (amp.treble > 12) amp.treble = 12;
 if (amp.treble < -12) amp.treble = -12;

 PSS_write(SET_MASTER_COMMAND);
 PSS_write(MASTER_VOLUME_LEFT | (252 + amp.volume/2));
 PSS_write(SET_MASTER_COMMAND);
 PSS_write(MASTER_VOLUME_RIGHT | (252 + amp.volume/2));
 PSS_write(SET_MASTER_COMMAND);
 PSS_write(MASTER_BASS | (246 + amp.bass/3));
 PSS_write(SET_MASTER_COMMAND);
 PSS_write(MASTER_TREBLE | (246 + amp.treble/3));
 PSS_write(SET_MASTER_COMMAND);
 PSS_write(MASTER_SWITCH | amp.mode);
 
} // set_amp()

void process_tokens(char *tokens[],int num_tokens,int lineno,char *linein)
{
 int cmdno,tokno,val,p,settno,type,*valp,valfound;
 struct type0 *c0;
 struct type1 *c1;

// find command #
 for (cmdno=0; cmdno<NUM_CMDS; cmdno++)
  if (!strcmp(tokens[0],cmds[cmdno].cmd)) break;
 if (cmdno==NUM_CMDS)  //uh-oh
  {
   printf("Illegal command %s in line %d of ECHODSP.INI:\n%s",
          strupr(tokens[0]),lineno,linein);
   exit(1);
  }
 type=cmds[cmdno].type;  
 p=(num_tokens==2 && !strcmp(tokens[1],"off")) ? FALSE : TRUE;
    
// it's a valid command, so turn it on or off

 switch (type)
  {
   case 0: cmds[cmdno].ptr.t0->on=p;
           break;
   case 1: cmds[cmdno].ptr.t1->on=p;
           break;             
   case 2: cmds[cmdno].ptr.t2->on=p;
  }  
 if (!p) return;
 
 for (tokno=1; tokno<num_tokens; tokno++)
  {
// split token into setting and value  
   p=findeq(tokens[tokno]);
   if (p<0) 
    valfound=FALSE;
   else
    {
     val=getval(tokens[tokno]+p+1);
     tokens[tokno][p]=0;
     valfound=TRUE;
    }     
   if (!strcmp(tokens[tokno],"off"))
    {
     printf("Error in line %d:\n",lineno);
     printf("%sThe setting 'off' may not be combined with other settings.",
            linein);
     exit(1);
    } 
    
   for (settno=0; settno<type_setts[type].num_settings; settno++)
    if (!strcmp(type_setts[type].settings[settno].settname,tokens[tokno]))
     break;
   if (settno==type_setts[type].num_settings)
    {
     printf("Illegal setting '%s' on line %d:\n%s",tokens[tokno],
            lineno,linein);
     exit(1);       
    }
   if (valfound && !type_setts[type].settings[settno].isvalued)
    {
     printf("A value may not be associated with setting '%s':\n%s",
            tokens[tokno],linein);
     exit(1);
    }   
   switch (type)
    {
     case 0: c0=cmds[cmdno].ptr.t0;
             switch (settno)
              {
               case 0: valp=&c0->irq; 
                       break;
               case 1: valp=&c0->dma;
                       break;
               case 2: valp=&c0->port;
              }
             break;
     case 1: c1=cmds[cmdno].ptr.t1;
             switch (settno)
              {
               case 0: valp=&c1->volume;
                       break;
               case 1: valp=&c1->treble;
                       break;
               case 2: valp=&c1->bass;
                       break;
               case 3: c1->mode=0x00ce; //stereo
                       strcpy(c1->mode_name,"Stereo");
                       break;
               case 4: c1->mode=0x00c6; //mono
                       strcpy(c1->mode_name,"Mono");
                       break;
               case 5: c1->mode=0x00de; //spatial   
                       strcpy(c1->mode_name,"Spatial");
                       break;
               case 6: c1->mode=0x00d6; //pseudo stereo
                       strcpy(c1->mode_name,"Pseudo-stereo");
                       break;
              }
    } 
   if (*valp==ILLEGAL_SETTING) 
    {
     printf("Error in line %d:\n%s",lineno,linein);
     printf("The setting '%s' may not be used with the command '%s'.",
            tokens[tokno],tokens[0]);
     exit(1);
    }
   if (type_setts[type].settings[settno].isvalued) *valp=val;
  }
// check each setting for valid setting, valid setting value (by type)
 
} /* process_tokens() */

void *testalloc(size_t num,size_t size) 
{
 void *temp;
 
 if ((temp=calloc(num,size))==NULL)
  {
   puts("Memory allocation error.");
   exit(1);
  }
 return temp;
} //testalloc
 
void init_structs()
{
 cmds[0].ptr.t0=&pss;
 cmds[1].ptr.t0=&wss;
 cmds[2].ptr.t0=&sb;
 cmds[3].ptr.t0=&midi;
 cmds[4].ptr.t1=&amp;
 cmds[5].ptr.t2=&joystick;

 type_setts[0].num_settings=3;
 type_setts[0].settings=testalloc(3,sizeof(struct sett));
 type_setts[1].num_settings=7;
 type_setts[1].settings=testalloc(7,sizeof(struct sett)); 
 type_setts[2].num_settings=1;
 type_setts[2].settings=testalloc(1,sizeof(struct sett));
 type_setts[0].settings[0].settname="irq";
 type_setts[0].settings[0].isvalued=TRUE;
 type_setts[0].settings[1].settname="dma"; 
 type_setts[0].settings[1].isvalued=TRUE;
 type_setts[0].settings[2].settname="port";
 type_setts[0].settings[2].isvalued=TRUE;
 type_setts[1].settings[0].settname="volume";
 type_setts[1].settings[0].isvalued=TRUE;
 type_setts[1].settings[1].settname="treble";
 type_setts[1].settings[1].isvalued=TRUE;
 type_setts[1].settings[2].settname="bass";
 type_setts[1].settings[2].isvalued=TRUE;
 type_setts[1].settings[3].settname="stereo";
 type_setts[1].settings[3].isvalued=FALSE;
 type_setts[1].settings[4].settname="mono";
 type_setts[1].settings[4].isvalued=FALSE;
 type_setts[1].settings[5].settname="spatial";
 type_setts[1].settings[5].isvalued=FALSE;
 type_setts[1].settings[6].settname="pseudo";
 type_setts[1].settings[6].isvalued=FALSE;
 type_setts[2].settings[0].settname="on";
 type_setts[2].settings[0].isvalued=FALSE;
} /* init_structs() */

void read_inifile(FILE *pssini)
{
int num_tokens,lineno=1;
char linein[MAX_LINE+1],line[MAX_LINE+1],
     *tokens[MAX_TOKENS],*tok,seps[]=" \t\n";

 init_structs();
 
 while (!feof(pssini)) /* Read one line at a time and split into tokens */
 {
  fgets(linein,MAX_LINE,pssini);
  strcpy(line,linein);
  if (!isblank(line))   /* skip blank and comment lines */
   {
    _strlwr(line);  /* non-case sensitive */
    num_tokens=1;
    tokens[0]=strtok(line,seps);
    if (tokens[0]==NULL)
     {
      printf("Parsing error in line %d of ECHODSP.INI\n%s",lineno,linein);
      exit(1);
     }
    if (strcmp(tokens[0],"rem"))  /* if tokens[0]!="rem" */
     {
      do
       {
        tok=strtok(NULL,seps);
        if (num_tokens>MAX_TOKENS)
         {
          printf("Too many tokens in line %d of ECHODSP.INI:\n%s",
                 lineno,linein);
          exit(1);
         }
        if (tok!=NULL) 
         {
          tokens[num_tokens++]=tok;
         }
       }
      while (tok!=NULL);
      process_tokens(tokens,num_tokens,lineno,linein);
     }
       
    lineno++;
   }
 }
 
}  /* read_inifile() */

void set_regvals()
{
 int i,j;
 struct type0 *p;
 
 for (i=0; cmds[i].type==0; i++)
  {
   p=cmds[i].ptr.t0;
   if (p->irq >= 0 && p->on)
    {
     for (j=p->good_irq_lo; j < p->good_irq_hi; j++)
      if (p->irq==irqs[j].irq) 
       {
        p->irq_idx=j;
        break;
       }
     if (j==p->good_irq_hi)
      {
       printf("IRQ%d is not valid for '%s'.\n",p->irq,cmds[i].cmd);
       printf("Legal values are: ");
       for (j=p->good_irq_lo; j < p->good_irq_hi; j++) 
        printf("%d ",irqs[j].irq);
       printf("\n");
       exit(1);
      }
    }
   if (p->dma >= 0)
    {     
     for (j=p->good_dma_lo; j < p->good_dma_hi; j++)
      if (p->dma==dmas[j].dma) 
       {
        p->dma_idx=j;
        break;
       }
     if (j==p->good_dma_hi)
      {
       printf("DMA%d is not valid for '%s'.\n",p->dma,cmds[i].cmd);
       printf("Legal values are: ");
       for (j=p->good_dma_lo; j < p->good_dma_hi; j++) 
        printf("%d ",dmas[j].dma);
       printf("\n");
       exit(1);
      }
    }
  }  // loop to test irq and dma values
  
  if (wss.on && ((wss.port < 0x100 || wss.port > 0xff3) ||
      ((wss.port & 0xf)!=0 && (wss.port & 0xf)!=4)))
   { 
    printf("Valid WSS port address are from 0x100 to 0xff3 ");
    printf("and must end in a 0 or 4.\n");
    exit(1);
   }
   
  if (midi.on && 
      ((midi.port < 0x100 || midi.port > 0xffc) || (midi.port & 3)))
   {
    printf("Valid WSS port address are from 0x100 to 0xff3 ");
    printf("and must be divisible by 4.\n");
    exit(1);
   }
   
 if (pss.on)
  pss.regval=irqs[pss.irq_idx].mask | dmas[pss.dma_idx].mask |
             ((joystick.on) ? 0x400 : 0);
  else pss.regval=0;
 if (wss.on) wss.regval=wss.port << 4; else wss.regval=0;
 if (sb.on) sb.regval=irqs[sb.irq_idx].mask | DMA1 | 0x2200; else sb.regval=0;
 if (midi.on) midi.regval=(midi.port << 4) | irqs[midi.irq_idx].mask;
  else midi.regval=0;
   
} // set_regvals()

void SP_write(unsigned SP_reg,int reg_value)
{
 outp(0x534,SP_reg);
 outp(0x535,reg_value);
}
void main(int nargs,char *arg[])
{
unsigned wData,wStatus,wControl;
int i,j,force_reset=FALSE,asic_id,iVersion;
int bNewASIC = 0;
float fVersion;
long count=0;
FILE *fin,*pssini;
char filename[_MAX_PATH],echopath[_MAX_PATH];

if (nargs < 2 || nargs > 4)
 {
  puts("Usage: LOAD filename [-n] [-r] (.ld assumed)");
  exit(1);
 }

/* Find ECHODSP.INI */

if ((pssini=fopen("ECHODSP.INI","rt"))==NULL)
 {
  printf("Unable to open %s\n",echopath);
  exit(1);
 }
 
read_inifile(pssini);
set_regvals();

// parse command line arguments

for (i=1; i<nargs; i++)
 {
  if (arg[i][0]=='-') 
   {
    if(strcmp(arg[i],"-n") == 0)
     display = FALSE;
    if (strcmp(arg[i],"-r")==0)
     force_reset=TRUE;
   }
  else strcpy(filename,arg[i]);
 }      


asic_id=inpw(PSS_ID_VERS_614);
if ((asic_id & 0xff00)!=0x4500)
 {
  port_offset=0x20;
  asic_id=inpw(PSS_ID_VERS_614+port_offset);
  if ((asic_id & 0xff00)!=0x4500)
   asic_id=0;
 }

if (asic_id)
 {
  printf("Echo ASIC detected at %#x.\n",PSS_DATA_614+port_offset);
  printf("Rev %d ASIC found.\n",asic_id & 0xFF);
  goto foundit;
 } 
 else 
  {
   port_offset=0;
   outpw(PSS_CONTROL_615,ID_VERSION_615);
   asic_id=inpw(PSS_INDEXED_615);
   if ((asic_id & 0xff00)!=0x4500)
    {
     port_offset=0x20;
     outpw(PSS_CONTROL_615+port_offset,ID_VERSION_615);
     asic_id=inpw(PSS_INDEXED_615+port_offset);
     if ((asic_id & 0xff00)!=0x4500)
      asic_id=0;
    }
  }
if (asic_id)
 {
  printf("Echo ESC615 detected at %#x.\n",PSS_DATA_615+port_offset);
  printf("Rev %d ASIC found.\n",asic_id & 0xFF);
  bNewASIC = 1;
 } 
 else
  {
   printf("Echo ASIC not detected!\n");
   exit(1);
  }

foundit:;
strcat(filename, ".ld");
if ((fin = fopen(filename, "rb")) == NULL)
 {
  printf("%s: cannot open %s\n", arg[0], filename);
  exit(1);
 }

if(bNewASIC)
 {
  outpw(PSS_CONTROL_615+port_offset,PSS_CONFIG_615);
  outpw(PSS_INDEXED_615+port_offset,pss.regval);
  outpw(PSS_CONTROL_615+port_offset,WSS_CONFIG_615);
  outpw(PSS_INDEXED_615+port_offset,wss.regval);
  outpw(PSS_CONTROL_615+port_offset,SB_CONFIG_615);
  outpw(PSS_INDEXED_615+port_offset,sb.regval);
  outpw(PSS_CONTROL_615+port_offset,MIDI_CONFIG_615);
  outpw(PSS_INDEXED_615+port_offset,midi.regval);
  wData   = PSS_DATA_615+port_offset;
  wStatus = PSS_STATUS_615+port_offset;
  wControl= PSS_CONTROL_615+port_offset;
 }
 else
 {
  outpw(PSS_CONFIG_614+port_offset,pss.regval);
  outpw(WSS_CONFIG_614+port_offset,wss.regval);
  outpw(SB_CONFIG_614+port_offset,sb.regval);
  outpw(MIDI_CONFIG_614+port_offset,midi.regval);
  wData   = PSS_DATA_614+port_offset;
  wStatus = PSS_STATUS_614+port_offset;
  wControl= PSS_CONTROL_614+port_offset;

 }

j=fgetc(fin);                   // get first byte of boot sequence
count++;

if (force_reset)
 {
  outpw(wData,j);  // latch out first byte of boot
  do_hard_reset(wControl);
 }
else
 {
  outpw(wData,0x00fe); //send reboot command to existing code
  timeout=clock()+DELAY;
  while (inpw(wData)!=0X5555 && timeout>clock());
  outpw(wData,j);  // latch out first byte of boot
  if (clock()>timeout)
   {
    outpw(wData,j);
    do_hard_reset(wControl);
   }
  else              
   if (display) puts("Software reset performed.");
 }
  
while (!feof(fin))
 {
  if (!(inpw(wStatus) & PSS_FLAG3))
   {
    timeout=clock()+DELAY;
    while (timeout>clock() && !(inpw(wStatus) & PSS_FLAG3));
    if (clock()>timeout) 
     {
      printf("Timeout error after %d bytes.\a\n",count);
      exit(1);
     }
   }
  outpw(wData,fgetc(fin));
  count++;
 }

outpw(wData,0);

timeout=DELAY+clock();
while (!(inpw(wStatus) & PSS_READ_FULL) && timeout>clock());
if (clock()>timeout)
 {
  puts("Timeout error: version number not read.");
  exit(1);
 }
iVersion=inpw(wData);
fVersion=(float)((iVersion & 0xff00)>>8)+(float)((iVersion & 0xff)/100.0);

if (display) 
 {
  strupr(filename);
  printf("\n%s Version %.2f",filename,fVersion);
  printf(" (%ld bytes) loaded into PSS at 0x%x.\n\n",count,0x220+port_offset);
  if (pss.on)                          
   printf(" PSS: IRQ = %d, DMA = %d\n",pss.irq,pss.dma);
  else
   puts(" PSS: Deactivated");
  if (sb.on)
   printf(" SB emulation: IRQ = %d, DMA = %d\n",sb.irq,sb.dma);
  else
   puts(" SB emulation: Deactivated");
  if (wss.on)
   printf(" WSS emulation: ADD = 0x%X\n",wss.port);
  else
   puts(" WSS emulation: Deactivated");
  if (midi.on)
   printf(" MPU-401 emulation: ADD = 0x%X, IRQ = %d\n", midi.port,midi.irq);
  else
   puts(" MPU-401 emulation: Deactivated");
  if (joystick.on)
   puts(" Game port: Activated");
  else
   puts(" Game port: Deactivated");
  printf(" PSS status: 0x%x\n",inpw(wStatus));
  if (amp.on)
   {
    puts("\n     Amplifier settings:\n");
    printf("      Volume: %4d db\n",amp.volume);
    printf("      Treble: %4d db\n",amp.treble);
    printf("        Bass: %4d db\n",amp.bass);
    printf("        Mode: %s\n",amp.mode_name);
   }
  else puts("\n    Amplifier settings not changed.");
 }
 
 if (amp.on) set_amp();
}  /* main() */


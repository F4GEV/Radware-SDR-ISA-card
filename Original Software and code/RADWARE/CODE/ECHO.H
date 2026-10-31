/* This is the header file for the ECHO Personal Sound System card.
It contains all of the defines and function prototypes that you may need
to write code for the ECHO card.  These prototypes are for functions that
are included in the ECHO object code library for the card. */

/* 9/9/93  File created.       JCB (Joe Ballantyne) */

#define REPORTALLERRORS 1

/* Maximum size of DSP program and data memory in bytes. */
#define DSP_MAX_MEM	0xc000

/* Number of available IRQ and DMA channels. */
#define NUMIRQS  7
#define NUMDMAS  6

/* PC DMA chip defines. */
#define DMAREAD	0x08
#define DMAAUTOINIT	0x10
#define SINGLEBYTEXFER	0x40

/* ECHO PSS configuration registers' io port address offsets from pssbase. */
#define PSS_CONFIG    0x0010
#define WSS_CONFIG    0x0012
#define SB_CONFIG     0x0014
#define CD_CONFIG 	 0x0016
#define MIDI_CONFIG   0x0018

/* Configuration register bit equates */
#define ADDRMASK  0xffc0
#define PORTMASK4	0xffc0
#define PORTMASK16	0xff00

#define NOIRQ		0x0000
#define IRQ3		0x0008
#define IRQ5		0x0010
#define IRQ7		0x0018
#define IRQ9		0x0020
#define IRQ10		0x0028
#define IRQ11       0x0030
#define IRQ12		0x0038
#define IRQMASK		0x0038

#define NODMA		0x0000
#define DMA0		0x0001
#define DMA1		0x0002
#define DMA3		0x0003
#define DMA5		0x0005
#define DMA6		0x0006
#define DMA7		0x0007
#define DMAMASK		0x0007		

/* PSS Configuration register bit equates */
#define GAME		0x0400
#define IRQSRC		0x0200
#define IRQ			0x0100
#define DMASRC		0x0080
#define DMA			0x0040

/* SB (SoundBlaster) register address offsets from SB base address. */
#define SB_PCRESET	0x0006
#define SB_PCWSTATUS	0x000C
#define SB_PCRSTATUS	0x000E
#define SB_PCRDATA	0x000A
#define SB_PCWDATA	0x000C

/* Music register address offsets from SB base address. */
#define SBMUSICSTATUS	0x0008
#define SBMUSICADDR		0x0008
#define SBMUSICDATA		0x0009

/* WSS register address offsets from WSS base address. */
#define WSS_STATUS				0x0003
#define WSS_CODECINDEXADDR		0x0004
#define WSS_CODECINDEXDATA		0x0005
#define WSS_CODECSTATUS			0x0006
#define WSS_CODECDATA			0x0007

/* SM (SoundMaster) register address offsets from SB base address. */
#define SM_DADATA		0x000F
#define SM_ADDATA		0x000F
#define SM_TIMCNTL	0x000B

/* CD-ROM Configuration register bit equates */ 
#define CD_IRQ_POL 	0x0040 /* when this bit low, PC interupt when CDROM IRQ is low */

/* MIDI register address offsets from MIDI base address. */
#define MIDI_DATA		0x0000
#define MIDI_STATUS	0x0001
#define MIDI_COMMAND	0x0001

/* MIDI Status register bit equates */
#define	MIDI_RE	0x0080
#define	MIDI_TF	0x0040

/* PSS registers' io port address offsets from pssbase. */
#define PSS_DATA      0x0000
#define PSS_STATUS    0x0002
#define PSS_CONTROL   0x0002
#define PSS_ID_VERS   0x0004
#define PSS_IRQ_ACK   0x0004 /* clears both the IRQ and DMA_TC status bits */
#define PSS_PIO		  0x001a
 
/* PSS Status register bit equates */
#define PSS_WE       	0x8000
#define PSS_RF       	0x4000
#define PSS_IRQ		 	0x2000
#define PSS_DMATC		0x1000
#define PSS_FLAG3		0x0800
#define PSS_FLAG2		0x0400

/* PSS Control register bit equates */
#define PSS_WEIE       	0x8000
#define PSS_RFIE       	0x4000
#define PSS_RESET	 	0x2000
#define PSS_FLAG1		0x1000
#define PSS_FLAG0		0x0800

/* PSS digitally controlled power amplifier equates. */
#define SET_AMP_COMMAND		0x0005
#define AMP_VOLUME_LEFT		0x0000
#define AMP_VOLUME_RIGHT	0x0100
#define AMP_BASS				0x0200
#define AMP_TREBLE			0x0300
#define AMP_SWITCH			0x0800


#ifdef ECHOLIB

/* Here are the global variable definitions for the Echo PSS library code. */
int chiprevision = 1;
int irqs[NUMIRQS] = {3,5,7,9,10,11,12};
int irq_avail[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
int irq_sel[13] = {NOIRQ,NOIRQ,NOIRQ,IRQ3,NOIRQ,IRQ5,NOIRQ,IRQ7,NOIRQ,IRQ9,IRQ10,
			IRQ11,IRQ12};
int irq_tst[13] = {0x00,0x00,0x00,0x00,0x08,0x10,0x00,0x18,0x00,0x20,0x28,
			0x30,0x38};
int dmas[NUMDMAS] = {0,1,3,5,6,7};
int dma_avail[8] = {0,0,0,0,0,0,0,0};
int dma_sel[8] = {DMA0,DMA1,NODMA,DMA3,NODMA,DMA5,DMA6,DMA7};	
int dma_tst[8] = {1,2,0,3,0,4,5,6};
int psscreg = 0;
unsigned int pssbase=0x220,sbbase=0x220,wssbase=0x530,midibase=0x330,cdrombase=0x340,gpbase=0x201;
long subsystemfailurecount=0,overallfailurecount=0;


#else

/* These are the global variable declarations for code that uses the 
Echo PSS library code. */
extern int chiprevision;      
extern int irqs[], irq_avail[], irq_sel[], irq_tst[];
extern int dmas[], dma_avail[], dma_sel[], dma_tst[];
extern int psscreg;
extern unsigned int pssbase,sbbase,wssbase,midibase,cdrombase,gpbase;
extern long subsystemfailurecount,overallfailurecount;
		
#endif

/* Here are the function prototypes for the functions in the Echo PSS
library. */

void logerror();
unsigned getdspword(void);
int putdspword(unsigned data);
int getmidibyte(unsigned int *data);
int putmidibyte(unsigned data);
void putbootword(unsigned data);
void useirq(int cnfgaddr,int irqid);
void releaseirq(int cnfgaddr);
int checkirq(int irqid);
void maskpcirq(int irq);
int checkpcirq(int irq);
int clearpcirq(int irq);
void usedma(int cnfgaddr,int dmaid);
void releasedma(int cnfgaddr);
int checkdma(int dmaid);
void turnonpcdma(int dma);
void turnoffpcdma(int dma);
void maskpcdma(int dma);
void unmaskpcdma(int dma);
void setuppcdma(int dma,unsigned char mode,long address,unsigned int count);
int checkpcdma(int dma);
void setport(int cnfgaddr,int addr);
void releaseport(int cnfgaddr);
void setamplifier(int volume, int bass, int treble);
int memload(unsigned *data, int numwords);
void bootload(char *data, unsigned numbytes);
void fileload(char *name, unsigned maxbuf, int bootflag);
int bootdsp(char *name, char *data, unsigned numbytes);
int fullduplex1848(char *data,int numsamples);
int fastfullduplex1848(char *data,int numsamples);
int noclickasmfullduplex1848(char __far *data,int numsamples);
int bestasmfullduplex1848(char __far *playdata,char __far *recorddata,int numsamples);
void get1848state(unsigned char *state);
void set1848state(unsigned char *newstate);



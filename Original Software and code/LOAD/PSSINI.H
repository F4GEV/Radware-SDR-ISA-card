#define NUM_CMDS 6
#define MAX_LINE 256
#define MAX_TOKENS 10
#define NUM_CMD_TYPES 3
#define ILLEGAL_SETTING -10000

// irq masks
#define NUM_IRQS 7                                                  
#define IRQ3          0x0008
#define IRQ5          0x0010
#define IRQ7          0x0018
#define IRQ9          0x0020
#define IRQ10         0x0028
#define IRQ11         0x0030
#define IRQ12         0x0038

//dma masks
#define NUM_DMAS 6
#define DMA0          0x0001
#define DMA1          0x0002
#define DMA3          0x0003
#define DMA5          0x0005
#define DMA6          0x0006
#define DMA7          0x0007

#define TRUE -1
#define FALSE 0

struct {
	int irq;
	int mask;
       } irqs[NUM_IRQS]={{3,IRQ3},{5,IRQ5},{7,IRQ7},{9,IRQ9},
			 {10,IRQ10},{11,IRQ11},{12,IRQ12}};
			    
struct {
	int dma;
	int mask;
       } dmas[NUM_DMAS]={{0,DMA0},{1,DMA1},{3,DMA3},
			 {5,DMA5},{6,DMA6},{7,DMA7}}; 

      
struct type0
 {
  int on;
  int irq;
  int dma;
  int port;
  int good_dma_lo;
  int good_dma_hi;
  int dma_idx;
  int good_irq_lo;
  int good_irq_hi;
  int irq_idx;
  int regval;
 } pss={FALSE,9,6,ILLEGAL_SETTING,3,NUM_DMAS,0,0,NUM_IRQS,0,0},   
   wss={FALSE,ILLEGAL_SETTING,ILLEGAL_SETTING,0x530,-1,-1,-1,-1,-1,-1,0},
   sb={FALSE,7,1,ILLEGAL_SETTING,1,2,0,0,3,0,0},
   midi={FALSE,5,ILLEGAL_SETTING,0x330,-1,-1,-1,0,NUM_IRQS,0,0}; 

struct type1
 {
  int on;
  int volume;
  int mode;
  int treble;
  int bass;
  char mode_name[14];
 } amp={FALSE,0,0x00ce,0,0,"Stereo"};  /* default to stereo */
   /* type 1 */
                  
struct type2
 { 
  int on;
 } joystick={FALSE};

struct 
 {
  char cmd[9];
  int type;
  union
   {
    struct type0 *t0;
    struct type1 *t1;
    struct type2 *t2;
   } ptr;
 } cmds[NUM_CMDS]={{"pss",0,NULL},
				   {"wss",0,NULL},
				   {"sb",0,NULL},
				   {"midi",0,NULL},
				   {"amp",1,NULL},
				   {"joystick",2,NULL}};

struct sett
 {
  char *settname;
  int  isvalued;
 };
 
struct setts
 {
  int num_settings;
  struct sett *settings;
 };

struct setts type_setts[NUM_CMD_TYPES]; 
 

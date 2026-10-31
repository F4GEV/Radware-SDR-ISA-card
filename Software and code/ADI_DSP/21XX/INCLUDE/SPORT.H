#ifndef __SPORT_DEFINED
#define __SPORT_DEFINED

int sport_start(int sport);
int sport_stop(int sport);

int sport_read(int sport);
int sport_write(int sport, int value);

/*
 * This file contains macros and subroutines to support the 
 * serial ports of the ADSP-21xx digital signal processors.
 */

/*
 * This macro reads a value from the RX0 or RX1 registers.
 */

#define sport_read(a) \
    ({ int __sport_value; \
	   if (a == 0) \
	       asm volatile ("%0=RX0;" : "=d" (__sport_value)); \
	   else \
	       asm volatile ("%0=RX1;" : "=d" (__sport_value)); \
		   __sport_value;})

/*
 * This macro writes a value to the specified serial port
 * transmit register.
 */

#define sport_write(a, b) \
    ({ \
	   if (a == 0) \
	       asm volatile ("TX0=%0;" : : "d" (b) : "TX0"); \
	   else \
	       asm volatile ("TX1=%0;" : : "d" (b) : "TX1"); })

/* 
 * This macro disables the specified serial port 
 */

#define sport_stop(a) \
    asm volatile ("AR=DM(0x3fff);AR=AR AND %0;DM(0x3fff)=AR;" \
		  : : "A" (~(0x1000 >> a)) : "AR")

/*
 * This macro starts the specified serial port.  When serial port
 * one is started, the configuration bit is also set.
 */

#define sport_start(a) \
    ({ if (a == 0) \
	   asm volatile ("AR=DM(0x3fff);AR=AR OR %0;DM(0x3fff)=AR;" \
			 : : "A" (0x1000) : "AR"); \
    else \
	   asm volatile ("AR=DM(0x3fff);AR=AR OR %0;DM(0x3fff)=AR;" \
			 : : "A" (0x0c00) : "AR"); })


struct ___autobuffer_control_register
{
    unsigned int __reserved:4;        /* Unused                              */
    unsigned int tireg:3;             /* Transmit autobuffer I-register      */
    unsigned int tmreg:2;             /* Transmit autobuffer M-register      */
    unsigned int rireg:3;             /* Receive autobuffer I-register       */
    unsigned int rmreg:2;             /* Receive autobuffer M-register       */
    unsigned int transmit_enable:1;   /* Enable transmit autobuffering       */
    unsigned int receive_enable:1;    /* Enable receive autobuffering        */
};

struct __serial_port_one
{
    struct ___autobuffer_control_register autobuffer;

    int rfsdiv;
    int sclkdiv;

    struct
    {
	const unsigned int flag_out:1;/* Flag out status READ ONLY           */
	unsigned int isclk:1;         /* Internal serial clock generation    */
	unsigned int rfsr:1;          /* Receive frame sync required         */
	unsigned int rfsw:1;          /* Receive frame sync width            */
	unsigned int tfsr:1;          /* Transmit frame sync required        */
	unsigned int tfsw:1;          /* Transmit frame sync width           */
	unsigned int itfs:1;          /* Internal transmit frame sync enable */
	unsigned int irfs:1;          /* Internal receive frame sync enable  */
	unsigned int invtfs:1;        /* Invert transmit frame sync          */
	unsigned int invrfs:1;        /* Invert receive frame sync           */
	unsigned int dtype:2;         /* Data format                         */
	unsigned int slen:4;          /* Serial word length                  */
    } control;
};

struct __serial_port_zero
{
    struct ___autobuffer_control_register autobuffer;

    int rfsdiv;
    int sclkdiv;

    union
    {
	struct
	{
	    unsigned int mce:1;   /* Multichannel enable                 */
	    unsigned int isclk:1; /* Internal serial clock generation    */
	    unsigned int rfsr:1;  /* Receive frame sync required         */
	    unsigned int rfsw:1;  /* Receive frame sync width            */
	    unsigned int tfsr:1;  /* Transmit frame sync required        */
	    unsigned int tfsw:1;  /* Transmit frame sync width           */
	    unsigned int itfs:1;  /* Internal transmit frame sync enable */
	    unsigned int irfs:1;  /* Internal receive frame sync enable  */
	    unsigned int invtfs:1;/* Invert transmit frame sync          */
	    unsigned int invrfs:1;/* Invert receive frame sync           */
	    unsigned int dtype:2; /* Data format                         */
	    unsigned int slen:4;  /* Serial word length                  */
	} single;

	struct
	{
	    unsigned int mce:1;   /* Multichannel enable                 */
	    unsigned int isclk:1; /* Internal serial clock generation    */
	    unsigned int mfd:4;   /* Multichannel frame delay            */
	    unsigned int mcl:1;   /* Multichannel length                 */
	    unsigned int irfs:1;  /* Internal receive frame sync enable  */
	    unsigned int invtdv:1;/* Invert transmit data enabled        */
	    unsigned int invrfs:1;/* Invert receive frame sync           */
	    unsigned int dtype:2; /* Data format                         */
	    unsigned int slen:4;  /* Serial word length                  */
	} multi;

    } control;

    struct
    {
	struct
	{
	    unsigned int channel15:1;
	    unsigned int channel14:1;
	    unsigned int channel13:1;
	    unsigned int channel12:1;
	    unsigned int channel11:1;
	    unsigned int channel10:1;
	    unsigned int channel9:1;
	    unsigned int channel8:1;
	    unsigned int channel7:1;
	    unsigned int channel6:1;
	    unsigned int channel5:1;
	    unsigned int channel4:1;
	    unsigned int channel3:1;
	    unsigned int channel2:1;
	    unsigned int channel1:1;
	    unsigned int channel0:1;
	    unsigned int channel31:1;
	    unsigned int channel30:1;
	    unsigned int channel29:1;
	    unsigned int channel28:1;
	    unsigned int channel27:1;
	    unsigned int channel26:1;
	    unsigned int channel25:1;
	    unsigned int channel24:1;
	    unsigned int channel23:1;
	    unsigned int channel22:1;
	    unsigned int channel21:1;
	    unsigned int channel20:1;
	    unsigned int channel19:1;
	    unsigned int channel18:1;
	    unsigned int channel17:1;
	    unsigned int channel16:1;
	} transmit;
	struct
	{
	    unsigned int channel15:1;
	    unsigned int channel14:1;
	    unsigned int channel13:1;
	    unsigned int channel12:1;
	    unsigned int channel11:1;
	    unsigned int channel10:1;
	    unsigned int channel9:1;
	    unsigned int channel8:1;
	    unsigned int channel7:1;
	    unsigned int channel6:1;
	    unsigned int channel5:1;
	    unsigned int channel4:1;
	    unsigned int channel3:1;
	    unsigned int channel2:1;
	    unsigned int channel1:1;
	    unsigned int channel0:1;
	    unsigned int channel31:1;
	    unsigned int channel30:1;
	    unsigned int channel29:1;
	    unsigned int channel28:1;
	    unsigned int channel27:1;
	    unsigned int channel26:1;
	    unsigned int channel25:1;
	    unsigned int channel24:1;
	    unsigned int channel23:1;
	    unsigned int channel22:1;
	    unsigned int channel21:1;
	    unsigned int channel20:1;
	    unsigned int channel19:1;
	    unsigned int channel18:1;
	    unsigned int channel17:1;
	    unsigned int channel16:1;
	} receive;
    } multichannel;
};

#define sport0 (*(volatile struct __serial_port_zero *)0x3ff3)
#define sport1 (*(volatile struct __serial_port_one *)0x3fef)

/* Serial clock source: Defaults to ISCLK_EXTERNAL */
#define ISCLK_INTERNAL 1
#define ISCLK_EXTERNAL 0

/* Internal SCLK frequency is based on the following
   formula:

                         CLKOUT(frequency)
   SCLK(frequency) = -------------------------
                         2 * (SCLKDIV+1)

   The COMPUTE_SCLKDIV macro accepts the processor's
   operating frequency and the desired sclk frequency 
   and returns the necessary SCLKDIV.

*/

#define COMPUTE_SCLKDIV(clkout, sclk_freq) ((clkout/(2*sclk_freq))-1)

/* Serial port framing options */

/* Values for tfsr and rfsr fields, default: FRAME_SYNC_NOT_REQUIRED */
#define FRAME_SYNC_REQUIRED 1
#define FRAME_SYNC_NOT_REQUIRED 0

/* Values for itfs and irfs fields, default FRAME_SYNC_EXTERNAL */
#define FRAME_SYNC_INTERNAL 1
#define FRAME_SYNC_EXTERNAL 0

/* Values for tfsw and rfsw fields, default FRAME_SYNC_NORMAL */
#define FRAME_SYNC_ALTERNATE 1
#define FRAME_SYNC_NORMAL 0

/* Values for invtfs and invrfs, default FRAME_SYNC_ACTIVE_HIGH */
#define FRAME_SYNC_INVERTED 1
#define FRAME_SYNC_ACTIVE_HIGH 0

/* Sport data companding */

#define DTYPE_RIGHT_JUSTIFY_ZERO_FILL 0
#define DTYPE_RIGHT_JUSTIFY_SIGN_EXTEND 1
#define DTYPE_MU_LAW_COMPANDING 2
#define DTYPE_A_LAW_COMPANDING 3

/* Sport Word Length */

#define SLEN_WORD_LENGTH(a) a - 1

/* Serial port autobuffer control */
#define AUTOBUFFER_REG_I2 2
#define AUTOBUFFER_REG_I3 3
#define AUTOBUFFER_REG_1 1
#define AUTOBUFFER_ENABLE 1
#define AUTOBUFFER_DISABLE 0

/* Multichannel Operation for Serial port zero ONLY! */

/* Values for mce, default MULTICHANNEL_DISABLE */
#define MULTICHANNEL_ENABLE 1
#define MULTICHANNEL_DISABLE 0

#define MULTICHANNEL_LENGTH_32 1
#define MULTICHANNEL_LENGTH_24 0

#define MULTICHANNEL_FRAME_DELAY(a) a

/* Value for invtdv, default MULTICHANNEL_TDV_ACTIVE_HIGH */
#define MULTICHANNEL_TDV_ACTIVE_LOW 1
#define MULTICHANNEL_TDV_ACTIVE_HIGH 0

#endif






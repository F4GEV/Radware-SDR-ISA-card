#define ram_bank_reg DMPGREG
#define tscale_reg   0x3ffb
#define tcount_reg   0x3ffc
#define tperiod_reg  0x3ffd

{Set this constant to 1 if external clock is 20 MHz, assuming 16 if 0}
{Not used in this code (yet), but it is nice to know this when programming
the timer and music DAC}

#define TWENTY_MHZ    0

#if TWENTY_MHZ
#define MICROSECOND_DELAY_COUNT 20
#define I2CDELAY 50
#else
#define MICROSECOND_DELAY_COUNT 16
#define I2CDELAY 40
#endif

{Some algorithms need all internal DM. RAM page variable is needed at
interrupt time and by foreground. Either it is in internal DM or stored
in PM where it can't be bank switched}

#define RAM_PAGE_PM   0

{Interrupt processing uses I7. Foreground tasks should not touch I7
without disabling interrupts. If foreground processing can't live
without I7, set this value to 1. If you change this to 1, you must
set FASTIRQ2 to 0.  Interrupt routine also uses M5 and sets it to 1,
but it saves and restores the contents of M5}

#define SAVEI7        0

{When FASTIRQ2 is set to 1, we use low memory for interrupt vector. This
wipes out serial port handlers. We ain't using them so lets not let this
memory go to waste.}

#define FASTIRQ2      1

#define DEBUG         0

/* the following structure reflects Microsoft's WAVEFORMATEX structure
that describes registered .wav types. At open time this structure is
provided to the .wav device driver. This structure is downloaded to the DSP.
Only the downloaded CODEC software really knows the format of this structure.
audiOS/fd merely copies this data from the PC into memory accessable to
the software compression/decompression routines. */

#define BufferSize     load_parms     /* DMA buffer size */
#define FormatTag      load_parms + 1 /* format type */
#define Channels       load_parms + 2 /* number of channels (i.e. mono, stereo...) */
#define SamplesPerSec  load_parms + 3 /* sample rate */
#define AvgBytesPerSec load_parms + 5 /* for buffer estimation */
#define BlockAlign     load_parms + 7 /* block size of data */
#define BitsPerSample  load_parms + 8 /* Number of bits per sample of mono data */
#define bSize          load_parms + 9 /* The count in bytes of the size of */
                                      /* extra information (after cbSize) */
#define wSamplesPerBlock load_parms + 10
#define NumCoef       load_parms + 11 /* number of ADPCM coefficients */
#define aCoef         load_parms + 12 /* coefficient table */

/* WAVE form wFormatTag IDs. These formats are defined by Microsoft
 for registered audio .wav types */
/* WAVE form wFormatTag IDs */
#define WAVE_FORMAT_PCM     1
#define  WAVE_FORMAT_UNKNOWN	0x0000	/*  Microsoft Corporation  */
#define  WAVE_FORMAT_ADPCM	0x0002	/*  Microsoft Corporation  */
#define  WAVE_FORMAT_IBM_CVSD	0x0005	/*  IBM Corporation  */
#define  WAVE_FORMAT_ALAW	0x0006	/*  Microsoft Corporation  */
#define  WAVE_FORMAT_MULAW	0x0007	/*  Microsoft Corporation  */
#define  WAVE_FORMAT_OKI_ADPCM	0x0010	/*  OKI  */
#define  WAVE_FORMAT_DVI_ADPCM	0x0011	/*  Intel Corporation  */
#define  WAVE_FORMAT_IMA_ADPCM	(WAVE_FORMAT_DVI_ADPCM)	/*  Intel Corporation  */
#define  WAVE_FORMAT_MEDIASPACE_ADPCM	0x0012	/*  Videologic  */
#define  WAVE_FORMAT_SIERRA_ADPCM	0x0013	/*  Sierra Semiconductor Corp  */
#define  WAVE_FORMAT_G723_ADPCM	0x0014	/*  Antex Electronics Corporation  */
#define  WAVE_FORMAT_DIGISTD	0x0015	/*  DSP Solutions, Inc.  */
#define  WAVE_FORMAT_DIGIFIX	0x0016	/*  DSP Solutions, Inc.  */
#define  WAVE_FORMAT_DIALOGIC_OKI_ADPCM	0x0017	/*  Dialogic Corporation  */
#define  WAVE_FORMAT_YAMAHA_ADPCM	0x0020	/*  Yamaha Corporation of America  */
#define  WAVE_FORMAT_SONARC	0x0021	/*  Speech Compression  */
#define  WAVE_FORMAT_DSPGROUP_TRUESPEECH	0x0022	/*  DSP Group, Inc  */
#define  WAVE_FORMAT_ECHOSC1	0x0023	/*  Echo Speech Corporation  */
#define  WAVE_FORMAT_AUDIOFILE_AF36	0x0024	/*    */
#define  WAVE_FORMAT_APTX	0x0025	/*  Audio Processing Technology  */
#define  WAVE_FORMAT_AUDIOFILE_AF10	0x0026	/*    */
#define  WAVE_FORMAT_DOLBY_AC2	0x0030	/*  Dolby Laboratories  */
#define  WAVE_FORMAT_GSM610	0x0031	/*  Microsoft Corporation  */
#define  WAVE_FORMAT_ANTEX_ADPCME	0x0033	/*  Antex Electronics Corporation  */
#define  WAVE_FORMAT_CONTROL_RES_VQLPC	0x0034	/*  Control Resources Limited  */
#define  WAVE_FORMAT_DIGIREAL	0x0035	/*  DSP Solutions, Inc.  */
#define  WAVE_FORMAT_DIGIADPCM	0x0036	/*  DSP Solutions, Inc.  */
#define  WAVE_FORMAT_CONTROL_RES_CR10	0x0037	/*  Control Resources Limited  */
#define  WAVE_FORMAT_NMS_VBXADPCM	0x0038	/*  Natural MicroSystems  */
#define  WAVE_FORMAT_ECHOSC3	0x003A	/*  Echo Speech Corporation  */
#define  WAVE_FORMAT_G721_ADPCM	0x0040	/*  Antex Electronics Corporation  */
#define  WAVE_FORMAT_MPEG	0x0050	/*  Microsoft Corporation  */
#define  WAVE_FORMAT_CREATIVE_ADPCM	0x0200	/*  Creative Labs, Inc  */
/* Analog compiler can't hack strings this big
#define  WAVE_FORMAT_CREATIVE_FASTSPEECH8	0x0202	/*  Creative Labs, Inc  */
#define  WAVE_FORMAT_CREATIVE_FASTSPEECH10	0x0203	/*  Creative Labs, Inc  */
*/
#define  WAVE_FORMAT_FM_TOWNS_SND	0x0300	/*  Fujitsu Corp.  */
#define  WAVE_FORMAT_OLIGSM	0x1000	/*  Ing C. Olivetti & C., S.p.A.  */
#define  WAVE_FORMAT_OLIADPCM	0x1001	/*  Ing C. Olivetti & C., S.p.A.  */
#define  WAVE_FORMAT_OLICELP	0x1002	/*  Ing C. Olivetti & C., S.p.A.  */
#define  WAVE_FORMAT_OLISBC	0x1003	/*  Ing C. Olivetti & C., S.p.A.  */
#define  WAVE_FORMAT_OLIOPR	0x1004	/*  Ing C. Olivetti & C., S.p.A.  */

/*
//  the WAVE_FORMAT_DEVELOPMENT format tag can be used during the
//  development phase of a new wave format.  Before shipping, you MUST
//  acquire an official format tag from Microsoft.
//
*/
#define WAVE_FORMAT_DEVELOPMENT         (0xFFFF)

/* offsets to .wav algorithm structure */
#define a_type    0          /* Contains Microsoft wave_format tag ID */
#define a_mode    1          /* Full Duplex, Record, Play modes */
#define a_SP_size 2          /* Size in SAMPLES of Sound Port side */
#define a_init    3          /* Vector to Init code */
#define a_process 4          /* Vector to process code */
#define mode A_vector + a_mode
#define SP_chunk_size A_vector + a_SP_size

#define A_VECTOR_LENGTH 5      /* Length of the above structure */
#define MODE_FULL_DUPLEX       0
#define MODE_PLAY              1
#define MODE_RECORD           -1

#define PAN_MID   15           /* Pan command Mid */
#define PAN_RIGHT  0           /* Pan command Right */
#define PAN_LEFT  31           /* Pan command Left */

#define PSS_DONE PSS_FLAG2
/* buffer sizes */
#define LOAD_PARM_SIZE 30
/* 2048 is a nice size if we ever run Dolby AC/2. For other stuff it's
a bit large */
#define SOUND_PORT_BUFFER_SIZE 2048
#define HALF_SOUND_PORT_BUFFER_SIZE 1024
#define PC_BUFFER_SIZE  512

#define version 0x0033   /*msb byte represents integer value and lsb byte */
                         /*represents a decimal value from .01 to .99 */



.MACRO delay_us(%0);
    ax0 = %0;
    call delay_us_;
.ENDMACRO;



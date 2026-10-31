{ ESC614.H }

{*****************************************************************************

ADSP-2115 - AD1848 ESC614 (ECHO PSS - SOUNDPORT) INTERFACE PROGRAM
Milo Street, ECHO Speech Corporation           January 1993

This program provides the constants used by echo.dsp.

*****************************************************************************}

{PSS control register constants}

.CONST PSS_DMA_ENABLE   = 0x8000;
.CONST PSS_FLAG3        = 0x4000;
.CONST PSS_FLAG2        = 0x2000;
.CONST PSS_IRQ_REQ      = 0x1000;

{PSS status register constants}

.CONST PSS_WRITE_EMPTY  = 0x8000;
.CONST PSS_READ_FULL    = 0x4000;
.CONST PSS_DMA_READY    = 0x2000;
.CONST PSS_FLAG1        = 0x1000;
.CONST PSS_FLAG0        = 0x0800;

{IRQ2 ENABLE constants }

.CONST PSS_RF_IRQ_ENABLE       = 0x8000;
.CONST PSS_WE_IRQ_ENABLE       = 0x4000;
.CONST SP_WE_IRQ_ENABLE        = 0x2000;
.CONST MIDI_RF_IRQ_ENABLE      = 0X0040;
.CONST MIDI_WE_IRQ_ENABLE      = 0X0020;
.CONST MIDI_UART_RF_IRQ_ENABLE = 0X0010;
.CONST MIDI_UART_WE_IRQ_ENABLE = 0X0008;
.CONST PSSDMA_IRQ_ENABLE       = 0X0004;
{
.CONST UART_RF_IRQ_ENABLE      = 0X0004;
.CONST UART_WE_IRQ_ENABLE      = 0X0002;
}
.CONST UART_STAT_IRQ_ENABLE    = 0X0001;

{IRQ2 DISABLE constants }

.CONST PSS_RF_IRQ_DISABLE       = 0x7fff;
.CONST PSS_WE_IRQ_DISABLE       = 0xbfff;
.CONST SP_WE_IRQ_DISABLE        = 0xdfff;
.CONST MIDI_RF_IRQ_DISABLE      = 0Xffbf;
.CONST MIDI_WE_IRQ_DISABLE      = 0Xffdf;
.CONST MIDI_UART_RF_IRQ_DISABLE = 0Xffef;
.CONST MIDI_UART_WE_IRQ_DISABLE = 0Xfff7;
.CONST PSSDMA_IRQ_DISABLE       = 0Xfffb;
{
.CONST UART_RF_IRQ_DISABLE      = 0Xfffb;
.CONST UART_WE_IRQ_DISABLE      = 0Xfffd;
}
.CONST UART_STAT_IRQ_DISABLE    = 0Xfffe;

{ MIDI status constants }

.CONST MIDI_READ_FULL   = 0X8000;
.CONST MIDI_WRITE_EMPTY = 0X4000;
.CONST MIDI_CMD_FULL    = 0X2000;
.CONST MIDI_RCVR_FULL   = 0X1000;
.CONST MIDI_TRANS_EMPTY = 0X0800;

{ I2C constants }

.CONST PIO_I2C_CLOCK_HIGH = 0x0202;
.CONST PIO_I2C_CLOCK_LOW  = 0xfffd;
.CONST PIO_I2C_DATA_HIGH  = 0x0101;
.CONST PIO_I2C_DATA_LOW   = 0xfffe;


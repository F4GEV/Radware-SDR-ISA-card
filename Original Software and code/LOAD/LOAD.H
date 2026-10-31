/* amplifier chip constants */

#define SET_MASTER_COMMAND  0x0010
#define MASTER_VOLUME_LEFT  0x0000
#define MASTER_VOLUME_RIGHT 0x0100
#define MASTER_BASS         0x0200
#define MASTER_TREBLE       0x0300
#define MASTER_SWITCH       0x0800

/* SoundPort register addresses */

#define SP_lin_source_ctrl   0x00
#define SP_rin_source_ctrl   0x01
#define SP_lin_gain_ctrl   0x10
#define SP_rin_gain_ctrl   0x11
#define SP_laux1_ctrl      0x02
#define SP_raux1_ctrl      0x03
#define SP_laux2_ctrl      0x04
#define SP_raux2_ctrl      0x05
#define SP_lout_ctrl       0x06
#define SP_rout_ctrl       0x07
#define SP_clk_format      0x48
#define SP_int_conf        0x09
#define SP_int_conf_mce    0x49
#define SP_pin_ctrl        0x0a
#define SP_test_init       0x0b
#define SP_misc_ctrl       0x0c
#define SP_mix_ctrl        0x0d
#define SP_dma_ucnt        0x0e
#define SP_dma_lcnt        0x0f

/*  gain constants  */

#define gain_0      0x00
#define gain_1_5    0x01
#define gain_3      0x02
#define gain_4_5    0x03
#define gain_6      0x04
#define gain_7_5    0x05
#define gain_9      0x06
#define gain_10_5   0x07
#define gain_12     0x08
#define gain_13_5   0x09
#define gain_15     0x0a
#define gain_16_5   0x0b
#define gain_18     0x0c
#define gain_19_5   0x0d
#define gain_21     0x0e
#define gain_22_5   0x0f
#define mute       0xffff

/*  attenuation constants  */

#define atten_0      0x00
#define atten_1_5    0x01
#define atten_3      0x02
#define atten_4_5    0x03
#define atten_6      0x04
#define atten_7_5    0x05
#define atten_9      0x06
#define atten_10_5   0x07
#define atten_12     0x08
#define atten_13_5   0x09
#define atten_15     0x0a
#define atten_16_5   0x0b
#define atten_18     0x0c
#define atten_19_5   0x0d
#define atten_21     0x0e
#define atten_22_5   0x0f



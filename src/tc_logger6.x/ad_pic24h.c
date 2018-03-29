/*
 * File:   ad_pic24f.c
 * Author: cwold
 *
 * Created on February 9, 2016, 1:00 PM
 */


#include "p24hj128gp202.h"
#include "diskio.h"
#include "ad_pic24h.h"
#include "ad7173_8.h"



/* Socket controls  (Platform dependent) */
#define AD_CS_LOW()  _LATB12 = 0	/* AD CS = L */
#define AD_CS_HIGH() _LATB12 = 1	/* AD CS = H */
#define GPS_CS_HIGH() _LATB14 = 1   /* GPS CS = H */
#define RTCC_CS_HIGH()  _LATB1 = 1   /* RTCC CS = H */
#define RTCC_CS_LOW()   _LATB1 = 0  /* RTCC CS = L */

/* SPI2 bit rate controls */
#define	AD_CLOCK()	SPI2CON1 = 0X006B /* Set sclk2 to 20 MHz with FCY of 40 MHz */


static volatile
UINT Timer3;		/* 1000Hz decrement timer */

/* Definitions for MMC/SDC command */
#define COMMS_READ  (0B01000000) /* READ OPERATION */
#define COMMS_WRITE (0B00000000) /* WRITE OPERATION */
#define COMMS_STAT  (0B00000000) /* STATUS REGISTER */
#define COMMS_ADC   (0B00000001) /* ADC MODE REGISTER */
#define COMMS_IF    (0B00000010) /* INTERFACE REGISTER */
#define COMMS_CHECK (0B00000011) /* CHECKSUM REGISTER */
#define COMMS_DATA  (0B00000100) /* DATA REGISTER */
#define COMMS_GPIO  (0B00000110) /* GPIO CONFIG. REGISTER */
#define COMMS_ID    (0B00000111) /* ID REGISTER */
#define COMMS_CHAN0 (0B00010000) /* CHAN0 REGISTER */
#define COMMS_CHAN1 (0B00010001) /* CHAN1 REGISTER */  
#define COMMS_CHAN2 (0B00010010)    
#define COMMS_CHAN3 (0B00010011)    
#define COMMS_CHAN4 (0B00010100)     
#define COMMS_CHAN5 (0B00010101)     
#define COMMS_CHAN6 (0B00010110)    
#define COMMS_CHAN7 (0B00010111)    
#define COMMS_CHAN8 (0B00011000)    
#define COMMS_CHAN9 (0B00011001)     
#define COMMS_CHAN10 (0B00011010)    
#define COMMS_CHAN11 (0B00011011)     
#define COMMS_CHAN12 (0B00011100)     
#define COMMS_CHAN13 (0B00011101)     
#define COMMS_CHAN14 (0B00011110)     
#define COMMS_CHAN15 (0B00011111)
#define COMMS_SETUP0 (0B00100000) /* SETUP CONFIG REGISTER 0 */
#define COMMS_SETUP1 (0B00100001)
#define COMMS_SETUP2 (0B00100010)
#define COMMS_SETUP3 (0B00100011)
#define COMMS_SETUP4 (0B00100100)
#define COMMS_SETUP5 (0B00100101)
#define COMMS_SETUP6 (0B00100110)
#define COMMS_SETUP7 (0B00100111)
#define COMMS_FILTER0 (0B00101000)
#define COMMS_FILTER1 (0B00101001)
#define COMMS_FILTER2 (0B00101010)
#define COMMS_FILTER3 (0B00101011)
#define COMMS_FILTER4 (0B00101100)
#define COMMS_FILTER5 (0B00101101)
#define COMMS_FILTER6 (0B00101110)
#define COMMS_FILTER7 (0B00101111)
#define COMMS_OFFSET0 (0B00110000) /*OFFSET 0 REGISTER */
#define COMMS_OFFSET1 (0B00110001)
#define COMMS_OFFSET2 (0B00110010)
#define COMMS_OFFSET3 (0B00110011)
#define COMMS_OFFSET4 (0B00110100)
#define COMMS_OFFSET5 (0B00110101)
#define COMMS_OFFSET6 (0B00110110)
#define COMMS_OFFSET7 (0B00110111)
#define COMMS_GAIN0 (0B00111000) /*GAIN 0 REGISTER*/
#define COMMS_GAIN1 (0B00111001)
#define COMMS_GAIN2 (0B00111010)
#define COMMS_GAIN3 (0B00111011)
#define COMMS_GAIN4 (0B00111100)
#define COMMS_GAIN5 (0B00111101)
#define COMMS_GAIN6 (0B00111110)
#define COMMS_GAIN7 (0B00111111)
#define ADCH_INT_REF_EN  (0X80)
#define ADCH_INT_REF_DIS (0X00)
#define ADCH_SING_CYC_DIS (0X00)
#define ADCH_SING_CYC_EN (0X20)
#define ADCH_DELAY_0 (0X00)
#define ADCH_DELAY_32 (0X01)
#define ADCH_DELAY_128 (0X02)
#define ADCH_DELAY_320 (0X03)
#define ADCH_DELAY_800 (0X04)
#define ADCH_DELAY_1600 (0X05)
#define ADCH_DELAY_4000 (0X06)
#define ADCH_DELAY_8000 (0X07)
#define ADCL_MODE_CONTINUOUS (0X00)
#define ADCL_MODE_SINGLE  (0X10)
#define ADCL_MODE_STANDBY (0X20)
#define ADCL_MODE_PWDN    (0X30)
#define ADCL_INT_OFF_CAL (0X40)
#define ADCL_SYS_OFF_CAL (0X60)
#define ADCL_SYS_GAIN_CAL 0X70
#define ADCL_INT_OSC   (0X00)
#define ADCL_INT_OSC_XTAL2 (0X01)
#define ADCL_EXT_OSC_XTAL2 (0X02)
#define ADCL_EXT_OSC_XTAL1_XTAL2 (0X03)
#define IFH_ALT_SYNC_DIS   (0X00)
#define IFH_ALT_SYNC_EN (0X10)
#define IFH_IO_STREN_L (0X00)
#define IFH_IO_STREN_H (0X08)
#define IFH_HIDE_DELAY_DIS (0X04)
#define IFH_HIDE_DELAY_EN (0X00)
#define IFH_DOUT_RESET_EN (0X01)
#define IFH_DOUT_RESET_DIS (0X00)
#define IFL_CONT_READ_EN (0X80)
#define IFL_CONT_READ_DIS (0X00)
#define IFL_DATA_STAT_EN (0X40)
#define IFL_DATA_STAT_DIS (0X00)
#define IFL_REG_CHECK_EN (0X20)
#define IFL_REG_CHECK_DIS (0X00)
#define IFL_CRC_RD_EN (0X04)
#define IFL_CRC_RDWR_EN (0X08)
#define IFL_CRC_DIS (0X00)
#define IFL_24BIT_WL (0X00)
#define IFL_16BIT_WL (0X01)
#define GPIOH_SYNC_DIS (0X00)
#define CHH_EN (0X80)
#define CHH_DIS (0X00)
#define CHH_SETUP0 (0X00)
#define CHH_SETUP1 (0X10)
#define CHH_SETUP2 (0X20)
#define CHH_SETUP3 (0X30)
#define CHH_SETUP4 (0X40)
#define CHH_SETUP5 (0X50)
#define CHH_SETUP6 (0X60)
#define CHH_SETUP7 (0X70)
#define CHH_AINPOS_AIN0 (0X00)
#define CHL_AINPOS_AIN0 (0X00)
#define CHH_AINPOS_AIN1 (0X00)
#define CHL_AINPOS_AIN1 (0X20)
#define CHH_AINPOS_AIN2 (0X00)
#define CHL_AINPOS_AIN2 (0X40)
#define CHH_AINPOS_AIN3 (0X00)
#define CHL_AINPOS_AIN3 (0X60)
#define CHH_AINPOS_AIN4 (0X00)
#define CHL_AINPOS_AIN4 (0X80)
#define CHH_AINPOS_AIN5 (0X00)
#define CHL_AINPOS_AIN5 (0XA0)
#define CHH_AINPOS_AIN6 (0X00)
#define CHL_AINPOS_AIN6 (0XC0)
#define CHH_AINPOS_AIN7 (0X00)
#define CHL_AINPOS_AIN7 (0XE0)
#define CHH_AINPOS_AIN8 (0X01)
#define CHL_AINPOS_AIN8 (0X00)
#define CHH_AINPOS_AIN9 (0X01)
#define CHL_AINPOS_AIN9 (0X02)
#define CHH_AINPOS_AIN10 (0X01)
#define CHL_AINPOS_AIN10 (0X40)
#define CHH_AINPOS_AIN11 (0X01)
#define CHL_AINPOS_AIN11 (0X60)
#define CHH_AINPOS_AIN12 (0X01)
#define CHL_AINPOS_AIN12 (0X80)
#define CHH_AINPOS_AIN13 (0X01)
#define CHL_AINPOS_AIN13 (0XA0)
#define CHH_AINPOS_AIN14 (0X01)
#define CHL_AINPOS_AIN14 (0XC0)
#define CHH_AINPOS_AIN15 (0X01)
#define CHL_AINPOS_AIN15 (0XE0)
#define CHH_AINPOS_AIN16 (0X02)
#define CHL_AINPOS_AIN16 (0X00)
#define CHH_AINPOS_TEMPP (0X02)
#define CHL_AINPOS_TEMPP (0X20)
#define CHH_AINPOS_TEMPN (0X02
#define CHL_AINPOS_TEMPN (0X40)
#define CHH_AINPOS_REFP (0X02)
#define CHL_AINPOS_REFP (0XA0)
#define CHH_AINPOS_REFN (0X02)
#define CHL_AINPOS_REFN (0XC0)
#define CHL_AINNEG_AIN0 (0X00)
#define CHL_AINNEG_AIN1 (0X01)
#define CHL_AINNEG_AIN2 (0X02)
#define CHL_AINNEG_AIN3 (0X03)
#define CHL_AINNEG_AIN4 (0X04)
#define CHL_AINNEG_AIN5 (0X05)
#define CHL_AINNEG_AIN6 (0X06)
#define CHL_AINNEG_AIN7 (0X07)
#define CHL_AINNEG_AIN8 (0X08)
#define CHL_AINNEG_AIN9 (0X09)
#define CHL_AINNEG_AIN10 (0X0A)
#define CHL_AINNEG_AIN11 (0X0B)
#define CHL_AINNEG_AIN12 (0X0C)
#define CHL_AINNEG_AIN13 (0X0D)
#define CHL_AINNEG_AIN14 (0X0E)
#define CHL_AINNEG_AIN15 (0X0F)
#define CHL_AINNEG_AIN16 (0X10)
#define CHL_AINNEG_TEMPP (0X11)
#define CHL_AINNEG_TEMPN (0X12)
#define CHL_AINNEG_REFP (0X15)
#define CHL_AINNEG_REFN (0X16)
#define SETUPH_BIPOLAR (0X10)
#define SETUPH_UNIPOLAR (0X00)
#define SETUPH_REF_BUF_EN (0X0C)
#define SETUPH_REF_BUF_DIS (0X00)
#define SETUPH_AIN_BUF_EN (0X03)
#define SETUPH_AIN_BUF_DIS (0X00)
#define SETUPL_BURNOUT_EN (0X80)
#define SETUPL_BURNOUT_DIS (0X00)
#define SETUPL_BUFCHOP_EN (0X60)
#define SETUPL_BUFCHOP_DIS (0X00)
#define SETUPL_EXT_REF1 (0X00)
#define SETUPL_EXT_REF2 (0X10)
#define SETUPL_REF_INT (0X20)
#define SETUPL_REF_AVDD (0X30)
#define FILTERH_SINC3_MAP_EN (0X80)
#define FILTERH_SINC3_MAP_DIS (0X00)
#define FILTERH_ENH_FILT_EN (0X08)
#define FILTERH_ENH_FILT_DIS (0X00)
#define FILTERH_ENH_FILT_27 (0X02)
#define FILTERH_ENH_FILT_25 (0X03)
#define FILTERH_ENH_FILT_20 (0X05)
#define FILTERH_ENH_FILT_16 (0X06)
#define FILTERL_ORDER_SINC5 (0X00)
#define FITLERL_ORDER_SINC3 (0X60)
#define FILTERL_31250_SPS (0X00)
#define FILTERL_15625_SPS (0X06)
#define FILTERL_10417_SPS (0X07)
#define FILTERL_5208_SPS (0X08)
#define FILTERL_2597_SPS (0X09)
#define FILTERL_1007_SPS (0X0A)
#define FILTERL_503_SPS (0X0B)
#define FILTERL_381_SPS (0X0C)
#define FILTERL_200_SPS (0X0D)
#define FILTERL_100_SPS (0X0E)
#define FILTERL_59_SPS (0X0F)
#define FILTERL_49_SPS (0X10)
#define FILTERL_20_SPS (0X11)
#define FILTERL_16_SPS (0X12)
#define FILTERL_10_SPS (0X13)
#define FILTERL_5_SPS (0X14)
#define FILTERL_2_SPS (0X15)
#define FILTERL_1_SPS (0X16)

#define SDI2    PORTBbits.RB5   /* RB10 INPUT */

static BYTE ad_xchg_spi (BYTE dat);

static
void ad_power_on (void)
{
    _SPIEN_2 = 0;					/* Turn on socket power, delay >1ms (Nothing to do) */
	AD_CLOCK();	/* Enable SPI2 */
    SPI2CON2 = 0x0000;
    _SPIEN_2 = 1;
}






/*-----------------------------------------------------------------------*/
/* Transmit/Receive data to/from ad via SPI  (Platform dependent)       */
/*-----------------------------------------------------------------------*/

/* Single byte SPI transfer */

static
BYTE ad_xchg_spi (BYTE dat)
{
	SPI2BUF = dat;
	while (!_SPIRBF_2) ;
	return (BYTE)SPI2BUF;
}


/*-----------------------------------------------------------------------*/
/* Deselect the a/d and release SPI bus                                  */
/*-----------------------------------------------------------------------*/

static
void ad_deselect (void)
{
    AD_CS_HIGH(); /* set CS# high */
    ;
   
}



/*-----------------------------------------------------------------------*/
/* Select the a/d                                                        */
/*-----------------------------------------------------------------------*/

static
void ad_select (void)	
{
    GPS_CS_HIGH();
    RTCC_CS_HIGH();
    AD_CS_LOW();	/* Set CS# low */
    
    ;
}

static
void toggle_deselect (void)
{
    AD_CS_HIGH();
    ;
    ;
    ;
    ;
    ;
    ;
    ;
    ;
    ;
    ;
    ;
    ;
    ;
    ;
    ;
    AD_CS_LOW();
    ;
}
/*-----------------------------------------------------------------------*/
/* read the status register                                              */
/*-----------------------------------------------------------------------*/
//static
//BYTE read_status()
//{
//    BYTE res = 0;
//    
//    //need to add code here
//    
//    return res;
//}
/*----------------------------------------------------------------------*/
/* do an internal offset and slope calibration                          */
/*----------------------------------------------------------------------*/
//static
//BYTE ad_internal_cal(
//BYTE chan
//)

//{
////need to add code here
//
//
//return 1;
//   }

/******************************************************************************/
/*********enable burnout detect for TC testing*********************************/
/******************************************************************************/
void ADInputTest(void) 
{
    char n;
    
    
    ad_power_on();
    ad_select();
    for (n = 8; n; n--) ad_xchg_spi(0xFF);	/* 64 dummy clocks */
    
    ad_select();
        
    ad_xchg_spi(COMMS_WRITE | COMMS_CHAN0);
    ad_xchg_spi(CHH_EN | CHH_SETUP0 | CHH_AINPOS_AIN0);
    ad_xchg_spi(CHL_AINPOS_AIN0 | CHL_AINNEG_AIN1);
    
    ad_xchg_spi(COMMS_WRITE | COMMS_CHAN1);
    ad_xchg_spi(CHH_EN | CHH_SETUP0 | CHH_AINPOS_AIN2);
    ad_xchg_spi(CHL_AINPOS_AIN2 | CHL_AINNEG_AIN3);
    
    ad_xchg_spi(COMMS_WRITE | COMMS_CHAN2);
    ad_xchg_spi(CHH_EN | CHH_SETUP0 | CHH_AINPOS_AIN4);
    ad_xchg_spi(CHL_AINPOS_AIN4 | CHL_AINNEG_AIN5);
    
    ad_xchg_spi(COMMS_WRITE | COMMS_CHAN3);
    ad_xchg_spi(CHH_EN | CHH_SETUP0 | CHH_AINPOS_AIN6);
    ad_xchg_spi(CHL_AINPOS_AIN6 | CHL_AINNEG_AIN7);
   
    ad_xchg_spi(COMMS_WRITE | COMMS_CHAN4);
    ad_xchg_spi(CHH_EN | CHH_SETUP0 | CHH_AINPOS_AIN8);
    ad_xchg_spi(CHL_AINPOS_AIN8 | CHL_AINNEG_AIN9);
   
    ad_xchg_spi(COMMS_WRITE | COMMS_CHAN5);
    ad_xchg_spi(CHH_EN | CHH_SETUP0 | CHH_AINPOS_AIN10);
    ad_xchg_spi(CHL_AINPOS_AIN10 | CHL_AINNEG_AIN11);
    
    ad_xchg_spi(COMMS_WRITE | COMMS_CHAN6);
    ad_xchg_spi(CHH_EN |  CHH_SETUP0 | CHH_AINPOS_AIN12);
    ad_xchg_spi(CHL_AINPOS_AIN12 | CHL_AINNEG_AIN13);
    
    ad_xchg_spi(COMMS_WRITE | COMMS_CHAN7);
    ad_xchg_spi(CHH_EN | CHH_SETUP0 | CHH_AINPOS_AIN14);
    ad_xchg_spi(CHL_AINPOS_AIN14 | CHL_AINNEG_AIN15);
    
    ad_xchg_spi(COMMS_WRITE | COMMS_SETUP0);
    ad_xchg_spi(SETUPH_BIPOLAR | SETUPH_REF_BUF_EN | SETUPH_AIN_BUF_EN);
    ad_xchg_spi(SETUPL_BURNOUT_EN | SETUPL_BUFCHOP_DIS | SETUPL_REF_INT);
    
    ad_xchg_spi(COMMS_WRITE | COMMS_FILTER0);
    ad_xchg_spi(FILTERH_SINC3_MAP_DIS | FILTERH_ENH_FILT_DIS);
    ad_xchg_spi(FILTERL_ORDER_SINC5 | FILTERL_1007_SPS);
    
    ad_xchg_spi(COMMS_WRITE | COMMS_ADC);
    ad_xchg_spi(ADCH_INT_REF_EN | ADCH_SING_CYC_EN | ADCH_DELAY_0);
    ad_xchg_spi(ADCL_MODE_SINGLE | ADCL_INT_OSC);
    
    ad_xchg_spi(COMMS_WRITE | COMMS_IF);
    ad_xchg_spi(IFH_ALT_SYNC_DIS | IFH_IO_STREN_H | IFH_HIDE_DELAY_EN | IFH_DOUT_RESET_DIS);
    ad_xchg_spi(IFL_CONT_READ_DIS | IFL_DATA_STAT_EN | IFL_REG_CHECK_DIS | IFL_CRC_DIS | IFL_24BIT_WL);

    ad_deselect();
}
/*-----------------------------------------------------------------*/
/* initialize the a/d                                              */
/*-----------------------------------------------------------------*/

void ad_init(void)
{
    char n;
    
    
    ad_power_on();
    ad_select();
    for (n = 8; n; n--) ad_xchg_spi(0xFF);	/* 64 dummy clocks */
    
    toggle_deselect();
    
    ad_xchg_spi(COMMS_WRITE | COMMS_CHAN0);
    ad_xchg_spi(CHH_EN | CHH_SETUP0 | CHH_AINPOS_AIN0);
    ad_xchg_spi(CHL_AINPOS_AIN0 | CHL_AINNEG_AIN1);
    
    ad_xchg_spi(COMMS_WRITE | COMMS_CHAN1);
    ad_xchg_spi(CHH_EN | CHH_SETUP0 | CHH_AINPOS_AIN2);
    ad_xchg_spi(CHL_AINPOS_AIN2 | CHL_AINNEG_AIN3);
    
    ad_xchg_spi(COMMS_WRITE | COMMS_CHAN2);
    ad_xchg_spi(CHH_EN | CHH_SETUP0 | CHH_AINPOS_AIN4);
    ad_xchg_spi(CHL_AINPOS_AIN4 | CHL_AINNEG_AIN5);
    
    ad_xchg_spi(COMMS_WRITE | COMMS_CHAN3);
    ad_xchg_spi(CHH_EN | CHH_SETUP0 | CHH_AINPOS_AIN6);
    ad_xchg_spi(CHL_AINPOS_AIN6 | CHL_AINNEG_AIN7);
   
    ad_xchg_spi(COMMS_WRITE | COMMS_CHAN4);
    ad_xchg_spi(CHH_EN | CHH_SETUP0 | CHH_AINPOS_AIN8);
    ad_xchg_spi(CHL_AINPOS_AIN8 | CHL_AINNEG_AIN9);
   
    ad_xchg_spi(COMMS_WRITE | COMMS_CHAN5);
    ad_xchg_spi(CHH_EN | CHH_SETUP0 | CHH_AINPOS_AIN10);
    ad_xchg_spi(CHL_AINPOS_AIN10 | CHL_AINNEG_AIN11);
    
    ad_xchg_spi(COMMS_WRITE | COMMS_CHAN6);
    ad_xchg_spi(CHH_EN |  CHH_SETUP0 | CHH_AINPOS_AIN12);
    ad_xchg_spi(CHL_AINPOS_AIN12 | CHL_AINNEG_AIN13);
    
    ad_xchg_spi(COMMS_WRITE | COMMS_CHAN7);
    ad_xchg_spi(CHH_EN | CHH_SETUP0 | CHH_AINPOS_AIN14);
    ad_xchg_spi(CHL_AINPOS_AIN14 | CHL_AINNEG_AIN15);
    
    ad_xchg_spi(COMMS_WRITE | COMMS_SETUP0);
    ad_xchg_spi(SETUPH_BIPOLAR | SETUPH_REF_BUF_EN | SETUPH_AIN_BUF_EN);
    ad_xchg_spi(SETUPL_BURNOUT_DIS | SETUPL_BUFCHOP_DIS | SETUPL_REF_INT);
    
    ad_xchg_spi(COMMS_WRITE | COMMS_FILTER0);
    ad_xchg_spi(FILTERH_SINC3_MAP_DIS | FILTERH_ENH_FILT_DIS);
    ad_xchg_spi(FILTERL_ORDER_SINC5 | FILTERL_31250_SPS);
    
    ad_xchg_spi(COMMS_WRITE | COMMS_ADC);
    ad_xchg_spi(ADCH_INT_REF_EN | ADCH_SING_CYC_DIS | ADCH_DELAY_0);
    ad_xchg_spi(ADCL_MODE_SINGLE | ADCL_INT_OSC);
    
    ad_xchg_spi(COMMS_WRITE | COMMS_IF);
    ad_xchg_spi(IFH_ALT_SYNC_DIS | IFH_IO_STREN_H | IFH_HIDE_DELAY_EN | IFH_DOUT_RESET_DIS);
    ad_xchg_spi(IFL_CONT_READ_DIS | IFL_DATA_STAT_EN | IFL_REG_CHECK_DIS | IFL_CRC_DIS | IFL_24BIT_WL);

    ad_deselect();
  
}

/*-------------------------------------------------------------------*/
/* get the a/d id, 0xX9                                              */
/*-------------------------------------------------------------------*/

//WORD get_ad_id(void)
WORD get_ad_id(BYTE passed_byte)
{
    WORD arg = 0;
    

    ad_select();
    // toggle_deselect();
    passed_byte = ad_xchg_spi(0x47); // comms: write adc mode
    // toggle_deselect();
    //while(SDI2);    /* wait for SDI2, ~RDY to go low */
    arg = ((WORD)ad_xchg_spi(0xFF) << 8)
        | ((WORD)ad_xchg_spi(0xFF));        
    ad_deselect();

    return arg;
   
}

/*-------------------------------------------------------------------*/
/* do an a/d conversion of the selected channel                      */
/*-------------------------------------------------------------------*/

  void get_ad_chan(
//BYTE chan
DWORD *ADBuffer
)
{  
    DWORD arg = 0;
    BYTE i = 0;
ad_power_on();    
ad_select();

ad_xchg_spi(COMMS_WRITE | COMMS_ADC);
ad_xchg_spi(ADCH_INT_REF_EN | ADCH_SING_CYC_EN | ADCH_DELAY_0);
ad_xchg_spi(ADCL_MODE_CONTINUOUS | ADCL_INT_OSC);


//toggle_deselect();  
for (i=0;i<8;i++){
    
//toggle_deselect();   
ad_xchg_spi(COMMS_READ | COMMS_DATA);

while(SDI2);    /* wait for SDI2, ~RDY to go low */


arg = (((DWORD) ad_xchg_spi(0x00)) << 24)
        | (((DWORD) ad_xchg_spi(0x00)) << 16) /* put the 24 bit result into a 32 bit register */
        | (((WORD)ad_xchg_spi(0x00)) << 8)
        | ((WORD)ad_xchg_spi(0x00));  //this byte contains the channel number      
ADBuffer[i] = arg;

}
ad_deselect();

//    return arg;
}


/*-----------------------------------------------------------------------*/
/* Device Timer Driven Procedure                                         */
/*-----------------------------------------------------------------------*/
/* This function must be called by timer interrupt in period of 1ms      */

void ad_timerproc (void)
{
	/*BYTE s;*/
	UINT n;


	n = Timer3;					/* 1000Hz decrement timer with zero stopped */
	if (n) Timer3 = --n;



	
}

void ad_power_off (void)
{
    ad_power_on();    
    ad_select();

    ad_xchg_spi(COMMS_WRITE | COMMS_ADC);
    ad_xchg_spi(ADCH_INT_REF_EN | ADCH_SING_CYC_EN | ADCH_DELAY_0);
    ad_xchg_spi(ADCL_MODE_STANDBY | ADCL_INT_OSC);
    ad_xchg_spi(COMMS_WRITE | COMMS_ADC);
    ad_xchg_spi(ADCH_INT_REF_EN | ADCH_SING_CYC_EN | ADCH_DELAY_0);
    ad_xchg_spi(ADCL_MODE_PWDN | ADCL_INT_OSC);
    
    ad_deselect();
    
}
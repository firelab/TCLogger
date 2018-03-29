/*
 * File:   ad_pic24f.c
 * Author: cwold
 *
 * Created on February 9, 2016, 1:00 PM
 */


#include "p24FJ128GA010.h"
#include "diskio.h"
#include "ad_pic24f.h"




/* Socket controls  (Platform dependent) */
#define AD_CS_LOW()  _LATB9 = 0	/* AD CS = L */
#define AD_CS_HIGH() _LATB9 = 1	/* AD CS = H */

/* SPI2 bit rate controls */
#define	AD_CLOCK()	SPI2CON1 = 0x013E /* Set sclk2 to 4 MHz with FCY of 16 MHz */


static volatile
UINT Timer3;		/* 1000Hz decrement timer */

/* Definitions for MMC/SDC command */
#define ADRSTATUS (0B01000000)	/* READ STATUS REGISTER */
#define ADWSTATUS (0B00000000) /* WRITE STATUS REGISTER */
#define ADRMODE   (0B01001000) /* READ MODE REGISTER */
#define ADWMODE   (0B00001000) /* WRITE MODE REGISTER */
#define ADRCONF   (0B01010000) /* READ CONFIG. REGISTER */
#define ADWCONF   (0B00010000) /* WRITE CONFIG. REGISTER */
#define ADDATAS   (0B01011000) /* SINGLE DATA READ */
#define ADDATAC   (0B01011100) /* CONTINUOUS DATA READ */
#define ADID      (0B01100000) /* READ ID REGISTER */
#define ADRIO     (0B01101000) /* READ IO REGISTER */
#define ADWIO     (0B00101000) /* WRITE IO REGISTER */
#define ADROFF    (0B01110000) /* READ OFFSET REGISTER */
#define ADWOFF    (0B00110000) /* WRITE OFFSET REGISTER */
#define ADRFS     (0B01111000) /* READ FULL SCALE REGISTER */
#define ADWFS     (0B00111000) /* WRITE FULL SCALE REGISER */
#define _470SPS    (0x01)         /* 470 SPS, 4ms SETTLE */
#define _242SPS    (0x02)         /* 242 SPS, 8ms SETTLE */
#define _123SPS    (0x03)         /* 123 SPS, 16ms SETTLE */
#define _62SPS     (0x04)         /* 62 SPS, 32ms SETTLE */
#define _50SPS     (0x05)         /* 50 SPS, 40ms SETTLE */
#define _39SPS     (0x06)         /* 39 SPS, 48ms SETTLE */
#define _33SPS     (0x07)         /* 33.2 SPS, 60ms SETTLE */
#define _20SPS     (0x08)         /* 19.6 SPS, 101ms SETTLE, 90dB@60Hz ONLY */
#define _17SPS     (0x09)         /* 16.7 SPS, 120ms SETTLE, 80dB@50Hz ONLY*/
#define _16SPS     (0x0A)       /* 16.7 SPS, 120ms SETTLE, 65dB */
#define _12SPS     (0x0B)       /* 12.5 SPS, 160ms SETTLE, 66dB */
#define _10SPS     (0x0C)       /* 10.0 SPS, 200ms SETTLE, 69dB */
#define _8SPS      (0x0D)       /* 8.33 SPS, 240ms SETTLE, 70dB */
#define _6SPS      (0x0E)       /* 6.25 SPS, 320ms SETTLE, 72dB */
#define _4SPS      (0x0F)       /* 4.17 SPS, 480ms SETTLE, 74dB */
#define CC_MODE     (0x00)        /* CONTINUOUS CONVERSION MODE */
#define SC_MODE     (0x20)         /* SINGLE CONVERSION MODE */
#define IDLE_MODE   (0x40)         /* IDLE MODE */
#define PD_MODE     (0x60)         /* POWER DOWN MODE */
#define IZS_MODE    (0x80)         /* INTERNAL ZERO-SCALE CALIBRATION */
#define IFS_MODE    (0xA0)         /* INTERNAL FULL-SCALE CALIBRATION */
#define SZS_MODE    (0xC0)         /* SYSTEM ZERO-SCALE CALIBRATION */
#define SFS_MODE    (0xE0)         /* SYSTEM FULL-SCALE CALIBRATION */
#define UNI         (0x01)      /* UNIPOLAR A/D CONVERSION */
#define BI          (0x00)      /* BIPOLAR A/D CONVERSION */
#define BUF         (0x10)      /* BUFFERED INPUT */
#define NO_BUF      (0x00)      /* NO INPUT BUFFER */
#define BO          (0x20)      /* BURN OUT DETECTION */
#define NO_BO       (0x00)      /* NO BURN OUT DETECTION */
#define G1          (0x00)      /* GAIN OF 1 */
#define G2          (0x01)      /* GAIN OF 2 */
#define G4          (0x02)      /* GAIN OF 4 */
#define G8          (0x03)      /* GAIN OF 8 */
#define G16         (0x04)      /* GAIN OF 16 */
#define G32         (0x05)      /* GAIN OF 32 */
#define G64         (0x06)      /* GAIN OF 64 */
#define G128        (0x07)      /* GAIN OF 128 */
#define CHAN1       (0x00)         /* A/D CHANNEL 1 */
#define CHAN2       (0x01)         /* A/D CHANNEL 2 */
#define CHAN3       (0x02)         /* A/D CHANNEL 3 */
#define SDI2    PORTGbits.RG7   /* RG7 INPUT */
#define SDO2    _RG8            /* RG8 OUTPUT */
#define SCK2    _RG2            /* RG2 OUTPUT */

static
void ad_power_on (void)
{
	;					/* Turn on socket power, delay >1ms (Nothing to do) */

	SPI2CON1 = 0x007E;	/* Enable SPI2 */
	SPI2CON2 = 0x0000;
	_SPI2EN = 1;
}

static
void ad_power_off (void)
{
	_SPI2EN = 0;			/* Disable SPI1 */

	;					/* Turn off port and socket power (Nothing to do) */
}


/*-----------------------------------------------------------------------*/
/* Transmit/Receive data to/from ad via SPI  (Platform dependent)       */
/*-----------------------------------------------------------------------*/

/* Single byte SPI transfer */

static
BYTE ad_xchg_spi (BYTE dat)
{
	SPI2BUF = dat;
	while (!_SPI2RBF) ;
	return (BYTE)SPI2BUF;
}


/*-----------------------------------------------------------------------*/
/* Deselect the a/d and release SPI bus                                  */
/*-----------------------------------------------------------------------*/

static
void ad_deselect (void)
{
    AD_CS_HIGH(); /* set CS# high */
}



/*-----------------------------------------------------------------------*/
/* Select the a/d                                                        */
/*-----------------------------------------------------------------------*/

static
void ad_select (void)	
{
	AD_CS_LOW();	/* Set CS# low */
}

/*-----------------------------------------------------------------------*/
/* read the status register                                              */
/*-----------------------------------------------------------------------*/
static
BYTE read_status()
{
    BYTE res; 
    
    ad_xchg_spi(ADWSTATUS);
    res = ad_xchg_spi(ADRSTATUS);
    
    return res;
}
/*----------------------------------------------------------------------*/
/* do an internal offset and slope calibration                          */
/*----------------------------------------------------------------------*/
static
BYTE ad_internal_cal(
BYTE chan
)

{

ad_select();

ad_xchg_spi(ADWCONF);   /* write to a/d configuration register */
ad_xchg_spi(BI | G2);
ad_xchg_spi(BUF | chan);

ad_xchg_spi(ADWMODE);   /* write to mode register */
ad_xchg_spi(IZS_MODE);  /* internal zero scale calibration */
ad_xchg_spi(_4SPS);     /* 4SPS */

while(SDI2);    /* wait for SDI2, ~RDY to go low */

ad_xchg_spi(ADWCONF);   /* write to a/d configuration register */
ad_xchg_spi(BI | G2);
ad_xchg_spi(BUF | chan);

ad_xchg_spi(ADWMODE);   /* write to mode register */
ad_xchg_spi(IFS_MODE);  /* internal full scale calibration */
ad_xchg_spi(_4SPS);     /* 4SPS */

while(SDI2);

ad_deselect();

return 1;
   }

/*-----------------------------------------------------------------*/
/* initialize the a/d                                              */
/*-----------------------------------------------------------------*/

void ad_init(void)
{
    INT n;
    
    
    AD_CLOCK();   /* set master spi clock rate */
    ad_power_on();
    ad_select();
    for (n = 10; n; n--) ad_xchg_spi(0xFF);	/* 32 dummy clocks */
    Timer3 = 600;
    while (Timer3);		    /* wait 600ms */

    ad_internal_cal(0);  /* do an internal calibration of each channel */
    ad_internal_cal(1);
    ad_internal_cal(2);
    

    ad_deselect();
  
}

/*-------------------------------------------------------------------*/
/* get the a/d id, 0xX9                                              */
/*-------------------------------------------------------------------*/

BYTE get_ad_id(void)

{
    BYTE arg = 0;
    
    ad_select();
      
    ad_xchg_spi(ADID);   
    
   arg = ad_xchg_spi(0xFF);
        
   ad_deselect();
    return arg;
}

/*-------------------------------------------------------------------*/
/* do an a/d conversion of the selected channel                      */
/*-------------------------------------------------------------------*/

DWORD get_ad_chan(
BYTE chan
)
{  
    DWORD arg = 0;
    

ad_select();

ad_xchg_spi(ADWCONF);
ad_xchg_spi(BI | G2);
ad_xchg_spi(BUF | chan);

ad_xchg_spi(ADWMODE);
ad_xchg_spi(SC_MODE);
ad_xchg_spi(_242SPS);

while(SDI2);       /*wait for SDI2, ~RDY, to go low*/

ad_xchg_spi(0x58);

arg = (((DWORD) ad_xchg_spi(0xFF)) << 16) /* put the 24 bit result into a 32 bit register */
    | ((WORD)ad_xchg_spi(0xFF) << 8)
    | ((WORD)ad_xchg_spi(0xFF));        

ad_deselect();

    return arg;
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

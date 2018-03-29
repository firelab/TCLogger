/*
 * File:   ad_pic24f.c
 * Author: cwold
 *
 * Created on February 9, 2016, 1:00 PM
 */

#include "p24hj128gp202.h"
#include "diskio.h"
#include "gps_pic24h.h"

// Debug
//#include "xprintf.h"
#include "uart_pic24h.h"

/* Socket controls  (Platform dependent) */
#define GPS_CS_LOW()  _LATB14 = 0	/* GPS CS = L */
#define GPS_CS_HIGH() _LATB14 = 1	/* GPS CS = H */
#define GPS_RESET_LOW() _LATA4 = 0
#define GPS_RESET_HIGH() _LATA4 = 1
#define GPS_ON_OFF() _LATB2
#define AD_CS_HIGH() _LATB12 = 1   /* AD CS = H */
#define RTCC_CS_HIGH()  _LATB1 = 1   /* RTCC CS = H */
#define RTCC_CS_LOW()   _LATB1 = 0  /* RTCC CS = L */

/* SPI2 bit rate controls */
#define	GPS_CLOCK() SPI2CON1 = 0x032B /* Set sclk2 to 6.67MHz with FCY of 40 MHz */

#define COMMS_WRITE (0B00000000) /* WRITE OPERATION */
#define COMMS_GPIO  (0B00000110) /* GPIO CONFIG. REGISTER */

typedef unsigned char bool;
#define true    1
#define false   0

static volatile
UINT Timer5;		/* 1000Hz decrement timer */
char _gps_rx_buff[1024];
const UINT _NUM_BYTES = 1024;
/* Definitions for MMC/SDC command */


#define MID129 {0xA0,0xA2,0x00,0x18,0x81,0x02,0x01,0x01,0x00,0x01,0x01,0x01,0x05,0x01,0x01,0x01,0x00,0x01, \
                0x00,0x01,0x00,0x00,0x00,0x01,0x00,0x00,0x12,0xC0,0x01,0x65,0xB0,0xB3}  //put in NMEA mode, 32 bytes

//#define MID117 {0x24,0x50,0x53,0x52,0x46,0x31,0x31,0x37,0x2C,0x31,0x30,0x2A,0x30,0x44,0x0D,0x0A} //orderly shutdown, 16 bytes
#define MID117 {0x24,0x50,0x53,0x52,0x46,0x31,0x31,0x37,0x2C,0x31,0x36,0x2A,0x30,0x42,0x0D,0x0A} //orderly shutdown, 16 bytes

#define MID125 {0x24,0x50,0x53,0x52,0x46,0x31,0x32,0x35,0x2A,0x32,0x31,0x0D,0x0A} //poll sofware version string, 13 bytes

#define MID103 {0x24,0x50,0x53,0x52,0x46,0x31,0x30,0x31,0x2C,0x30,0x32,0x2C,0x30,0x30, \
                0x2C,0x30,0x30,0x2C,0x30,0x31,0x2A,0x32,0x35,0x0D,0x0A} //MID103, GLL off, 25 bytes

#define GLL_OFF {0x24,0x50,0x53,0x52,0x46,0x31,0x30,0x31,0x2C,0x30,0x32,0x2C,0x30,0x30, \
                0x2C,0x30,0x30,0x2C,0x30,0x31,0x2A,0x32,0x35,0x0D,0x0A} //MID103, GLL off, 25 bytes

#define GSA_OFF {0x24,0x50,0x53,0x52,0x46,0x31,0x30,0x33,0x2C,0x30,0x32,0x2C,0x30,0x30, \
                0x2C,0x30,0x30,0x2C,0x30,0x31,0x2A,0x32,0x36,0x0D,0x0A} //MID103, GSA off, 25 bytes

#define GSA_ON {0x24,0x50,0x53,0x52,0x46,0x31,0x30,0x33,0x2C,0x30,0x32,0x2C,0x30,0x30, \
                0x2C,0x30,0x31,0x2C,0x30,0x31,0x2A,0x32,0x37,0x0D,0x0A} //MID103, GSA on, 25 bytes

#define GSV_OFF {0x24,0x50,0x53,0x52,0x46,0x31,0x30,0x33,0x2C,0x30,0x33,0x2C,0x30,0x30, \
                0x2C,0x30,0x30,0x2C,0x30,0x31,0x2A,0x32,0x37,0x0D,0x0A} //MID103, GSV off, 25 bytes

#define GSV_ON  {0x24,0x50,0x53,0x52,0x46,0x31,0x30,0x33,0x2C,0x30,0x33,0x2C,0x30,0x30, \
                0x2C,0x30,0x31,0x2C,0x30,0x31,0x2A,0x32,0x36,0x0D,0x0A} //MID103, GSV on, 25 bytes{

#define VTG_OFF {0x24,0x50,0x53,0x52,0x46,0x31,0x30,0x33,0x2C,0x30,0x35,0x2C,0x30,0x30, \
                0x2C,0x30,0x30,0x2C,0x30,0x31,0x2A,0x32,0x31,0x0D,0x0A} //MID103, VTG off, 25 bytes

#define ZDA_OFF {0x24,0x50,0x53,0x52,0x46,0x31,0x30,0x33,0x2C,0x30,0x38,0x2C,0x30,0x30, \
                0x2C,0x30,0x30,0x2C,0x30,0x31,0x2A,0x32,0x43,0x0D,0x0A} //MID103, ZDA off, 25 bytes

#define ZDA_ON {0x24,0x50,0x53,0x52,0x46,0x31,0x30,0x33,0x2C,0x30,0x38,0x2C,0x30,0x30, \
                0x2C,0x30,0x31,0x2C,0x30,0x31,0x2A,0x32,0x44,0x0D,0x0A} //MID103, ZDA on, 25 bytes

#define MID100 {0x24,0x50,0x53,0x52,0x46,0x31,0x30,0x30,0x2C,0x30,0x2C,0x31,0x31,0x35,0x32,0x30,0x30,0x2C, \
                0x38,0x2C,0x31,0x2C,0x30,0x2A,0x30,0x34,0x0D,0x0A}  //MID100, Switch to OSP Protocol, 28 bytes

#define ID132  {0xA0,0xA2,0x00,0x02,0x84,0x00,0x00,0x84,0xB0,0xB3} //ID132, Software version, 10 bytes

#define MID105 {0x24,0x50,0x53,0x52,0x46,0x31,0x30,0x35,0x2C,0x31,0x2A,0x33,0x46,0x0D,0x0A} //MID105, debug on, 15 bytes

#define SDI2    PORTBbits.RB10   /* RB10 INPUT */



/*****************************************************************************/
/* Local function prototypes */
/*****************************************************************************/
//static void get_zda_ut_date(const char* const gps_rx_buff, const UINT BUFFER_SIZE, const UINT nmea_start, BYTE* const ut_date);
static void get_zda_utc_time(const char* const gps_rx_buff, const UINT BUFFER_SIZE, const UINT nmea_start, BYTE* const utc_time);
static void get_rmc_utc_time(const char* const gps_rx_buff, const UINT BUFFER_SIZE, const UINT nmea_start, BYTE* const utc_time);
static void get_rmc_validity(const char* const gps_rx_buff, const UINT BUFFER_SIZE, const UINT nmea_start, BYTE* const gprmc_validity);
static void get_rmc_latitude(const char* const gps_rx_buff, const UINT BUFFER_SIZE, const UINT nmea_start, BYTE* const latitude);
static void get_rmc_north_or_south(const char* const gps_rx_buff, const UINT BUFFER_SIZE, const UINT nmea_start, BYTE* const north_or_south);
static void get_rmc_longitude(const char* const gps_rx_buff, const UINT BUFFER_SIZE, const UINT nmea_start, BYTE* const longitude);
static void get_rmc_east_or_west(const char* const gps_rx_buff, const UINT BUFFER_SIZE, const UINT nmea_start, BYTE* const east_or_west);
static void get_rmc_ut_date(const char* const gps_rx_buff, const UINT BUFFER_SIZE, const UINT nmea_start, BYTE* const ut_date);
static void get_gga_utc_time(const char* const gps_rx_buff, const UINT BUFFER_SIZE, const UINT nmea_start, BYTE* const utc_time);
static void get_gga_validity(const char* const gps_rx_buff, const UINT BUFFER_SIZE, const UINT nmea_start, BYTE* const fix_validity);
static void get_gga_latitude(const char* const gps_rx_buff, const UINT BUFFER_SIZE, const UINT nmea_start, BYTE* const latitude);
static void get_gga_north_or_south(const char* const gps_rx_buff, const UINT BUFFER_SIZE, const UINT nmea_start, BYTE* const north_or_south);
static void get_gga_longitude(const char* const gps_rx_buff, const UINT BUFFER_SIZE, const UINT nmea_start, BYTE* const longitude);
static void get_gga_east_or_west(const char* const gps_rx_buff, const UINT BUFFER_SIZE, const UINT nmea_start, BYTE* const east_or_west);
static void get_gga_satellites(const char* const gps_rx_buff, const UINT BUFFER_SIZE, const UINT nmea_start, BYTE* const satellites);
//static void get_gsv_satellites(const char* const gps_rx_buff, const UINT BUFFER_SIZE, const UINT nmea_start, BYTE* const satellites);
static int find_nmea_message(const char* const target_message_name, const char* const gps_rx_buff, const UINT BUFFER_SIZE, /*out*/ UINT* const nmea_start);
static int check_checksum(const char* const target_message_name, const char* const gps_rx_buff, const UINT BUFFER_SIZE, const UINT nmea_start, const UINT nmea_end);
static BYTE gps_xchg_spi (BYTE dat);

/*-----------------------------------------------------------------------*/
/* Select the gps                                                        */
/*-----------------------------------------------------------------------*/

static void gps_select (void)
{
    AD_CS_HIGH();   /* Make sure AD isn't selected */
    RTCC_CS_HIGH(); //deselect the rtcc
    GPS_CS_LOW();	/* Set CS# low */
;
    
}

static void gps_deselect (void)
{
    GPS_CS_HIGH(); /* set CS# high */
;
    
}



/*-----------------------------------------------------------------------*/
/* Transmit/Receive data to/from ad via SPI  (Platform dependent)       */
/*-----------------------------------------------------------------------*/

/* Single byte SPI transfer */

static BYTE gps_xchg_spi (BYTE dat)
{
//    gps_spi();
   	SPI2BUF = dat;
	while (!_SPIRBF_2);
	return (BYTE)SPI2BUF;
}


/*-----------------------------------------------------------------------*/
/* Deselect the a/d and release SPI bus                                  */
/*-----------------------------------------------------------------------*/


/* Block SPI transfers */

static void xmit_gps_multi (
	const char* gps_tx_buff,	/* Data to be sent */
	UINT cnt,  /* Number of bytes to send */
    BYTE* rxbuff,
    UINT rxcnt
)
{
    UINT i;
//    gps_spi();
    for(i = 0; i < cnt; i++)
    {
        SPI2BUF = *gps_tx_buff++;
        while (!_SPIRBF_2);
        *rxbuff++ = SPI2BUF;
        rxcnt += 1;
    }
}


//static void rcvr_gps_multi (
//	BYTE* rxbuff,		/* Buffer to store received data */
//	UINT cnt		/* Number of bytes to receive */
//)
//{
//    UINT i;
// 
//    for(i = 0; i < cnt; ++i)
//    {
//        SPI2BUF = 0xFF;
//        while (!_SPIRBF_2);
//        *rxbuff++ = SPI2BUF;
//    }
//   
//
//}

/*static void gps_toggle_deselect (void)
{
    GPS_CS_HIGH();
    gps_xchg_spi(0xFF);
    GPS_CS_LOW();
    
}*/

static void gps_spi (void)
{

    _SPIEN_2 = 0;
    GPS_CLOCK();	/* Enable SPI2 */
    SPI2CON2 = 0x0000;
    _SPIEN_2 = 1;
}

void gps_reset (void)
{
    GPS_RESET_LOW();
    Timer5 = 200;
    while(Timer5);
    GPS_RESET_HIGH();
}

static void gps_power_on (void)
{
	gps_reset();
    Timer5 = 50;
    while(Timer5);
    
    GPS_ON_OFF() = 1;
    Timer5 = 200;
    while(Timer5);
    GPS_ON_OFF() = 0;
    
    Timer5 = 50;
    while(Timer5);
    
   GPS_ON_OFF() = 1;
    Timer5 = 200;
    while(Timer5);
    GPS_ON_OFF() = 0;
        
}
//static void gps_power_off (void)
//{
////    GPS_ON_OFF() = 1;
////    Timer5 = 200;
////    while(Timer5);
////    GPS_ON_OFF() = 0;
//    
////     Timer5 = 50;
////    while(Timer5);
////    send_mid117();
////    Timer5 = 50;
////    while(Timer5);
////   GPS_ON_OFF() = 1;
////    Timer5 = 200;
////    while(Timer5);
////    GPS_ON_OFF() = 0;
//
//}

void gps_shutdown (void)
{
        
    gps_spi();
    gps_select();
    send_mid117();
    _LATB11 = 1;
    Timer5 = 2000;
    while(Timer5);
    _LATB11 = 0;
    gps_deselect();
    GPS_ON_OFF() = 1;
    Timer5 = 200;
    while(Timer5);
    GPS_ON_OFF() = 0;
    
    
}

void gps_fill_buffer(char* gps_rx_buff, UINT num_bytes)
{
    UINT i = 0;

    gps_spi();
    gps_select();

    while(i < num_bytes)
    {
        Timer5 = 1;
        while(Timer5){;}
        gps_rx_buff[i] = gps_xchg_spi(0xA7);
               i++;
             gps_rx_buff[i] = gps_xchg_spi(0xB4);
    
            i++;
   
    }
        
    gps_deselect();
  
    
}

/*-----------------------------------------------------------------*/
/* initialize the GPS                                              */
/*-----------------------------------------------------------------*/

void gps_init(void)
{
   
    
    gps_power_on();
    Timer5 = 1000;
    while(Timer5){;}

    
}
/*-------------------------------------------------------------------*/
/*                      */
/*-------------------------------------------------------------------*/
void send_mid129 (void)
{
    const BYTE buff[] = MID129;
    BYTE rxbuff[32];
    UINT rxcnt;
    rxcnt = 0;
    
    gps_spi();
    gps_select();
    xmit_gps_multi(buff,32,rxbuff,rxcnt);
    gps_deselect(); 
}
/*-------------------------------------------------------------------*/
/*                      */
/*-------------------------------------------------------------------*/
void send_mid103 (void)
{
    const BYTE buff[] = MID103;
    BYTE rxbuff[25];
    UINT rxcnt;
    rxcnt = 0;
    gps_spi();
    gps_select();
    xmit_gps_multi(buff,25,rxbuff,rxcnt);
    gps_deselect(); 
}
/*----------------------------------------------------------------------*/
/*                                                                      */
/*----------------------------------------------------------------------*/
void send_mid125 (void)
{
    const BYTE buff[] = MID125;
    BYTE rxbuff[13];
    UINT rxcnt;
    rxcnt = 0;
    gps_spi();
    gps_select();
    xmit_gps_multi(buff,13,rxbuff,rxcnt);
    gps_deselect();
    
}
/*----------------------------------------------------------------------*/
/*                                                                      */
/*----------------------------------------------------------------------*/
void send_mid117 (void)
{
    const BYTE buff[] = MID117;
    BYTE rxbuff[16];
    UINT rxcnt;
    rxcnt = 0;
    gps_spi();
    gps_select();
    xmit_gps_multi(buff,16,rxbuff,rxcnt);
    gps_deselect(); 
}
/*----------------------------------------------------------------------*/
/*                                                                      */
/*----------------------------------------------------------------------*/
void send_mid100 (void)
{
    const BYTE buff[] = MID100;
    BYTE rxbuff[28];
    UINT rxcnt;
    rxcnt = 0;
    gps_spi();
    gps_select();
    xmit_gps_multi(buff,28,rxbuff,rxcnt);
    gps_deselect(); 
}
/*----------------------------------------------------------------------*/
/*                                                                      */
/*----------------------------------------------------------------------*/
void send_mid105 (void)
{
    const BYTE buff[] = MID105;
    BYTE rxbuff[15];
    UINT rxcnt;
    rxcnt = 0;
    gps_spi();
    gps_select();
    xmit_gps_multi(buff,15,rxbuff,rxcnt);
    gps_deselect(); 
}
/*----------------------------------------------------------------------*/
/*                                                                      */
/*----------------------------------------------------------------------*/
void send_id132 (void)
{
    const BYTE buff[] = ID132;
    BYTE rxbuff[10];
    UINT rxcnt;
    rxcnt = 0;
    gps_spi();
    gps_select();
    xmit_gps_multi(buff,10,rxbuff,rxcnt);
    gps_deselect(); 
}
/*----------------------------------------------------------------------*/
/*                                                                      */
/*----------------------------------------------------------------------*/
void send_gsv_off (void)
{
    const BYTE buff[] = GSV_OFF;
    BYTE rxbuff[25];
    UINT rxcnt;
    rxcnt = 0;
    gps_spi();
    gps_select();
    xmit_gps_multi(buff,25,rxbuff,rxcnt);
    gps_deselect(); 
}
/*----------------------------------------------------------------------*/
/*                                                                      */
/*----------------------------------------------------------------------*/
void send_gsv_on (void)
{
    const BYTE buff[] = GSV_ON;
    BYTE rxbuff[25];
    UINT rxcnt;
    rxcnt = 0;
    gps_spi();
    gps_select();
    xmit_gps_multi(buff,25,rxbuff,rxcnt);
    gps_deselect(); 
}
/*----------------------------------------------------------------------*/
/*                                                                      */
/*----------------------------------------------------------------------*/

void send_gsa_off (void)
{
    const BYTE buff[] = GSA_OFF;
    BYTE rxbuff[25];
    UINT rxcnt;
    rxcnt = 0;
    gps_spi();
    gps_select();
    xmit_gps_multi(buff,25,rxbuff,rxcnt);
    gps_deselect(); 
}
/*----------------------------------------------------------------------*/
/*                                                                      */
/*----------------------------------------------------------------------*/

void send_gsa_on (void)
{
    const BYTE buff[] = GSA_ON;
    BYTE rxbuff[25];
    UINT rxcnt;
    rxcnt = 0;
    gps_spi();
    gps_select();
    xmit_gps_multi(buff,25,rxbuff,rxcnt);
    gps_deselect(); 
}
/*----------------------------------------------------------------------*/
/*                                                                      */
/*----------------------------------------------------------------------*/

void send_zda_on (void)
{
    const BYTE buff[] = ZDA_ON;
    BYTE rxbuff[25];
    UINT rxcnt;
    rxcnt = 0;
    gps_spi();
    gps_select();
    xmit_gps_multi(buff,25,rxbuff,rxcnt);
    gps_deselect(); 
}
/*----------------------------------------------------------------------*/
/*                                                                      */
/*----------------------------------------------------------------------*/

DWORD  get_gps(void)
{  
    DWORD arg = 0;
    gps_select();
    gps_deselect();
    return arg;
}


/*-----------------------------------------------------------------------*/
/* Device Timer Driven Procedure                                         */
/*-----------------------------------------------------------------------*/
/* This function must be called by timer interrupt in period of 1ms      */

void gps_timerproc (void)
{
	/*BYTE s;*/
	UINT n;
	n = Timer5;					/* 1000Hz decrement timer with zero stopped */
	if (n) Timer5 = --n;
}

// find_nmea_message returns 0 on success, 1 on failure
void gps_parse_zda(
    const char* const gps_rx_buff,
    const UINT BUFFER_SIZE, 
    BYTE* const utc_time)

{
    char target_message_name[6];

    UINT nmea_start = 0;
    bool is_zda_found = false;
    gps_spi();
    // GPZDA is the target message
    target_message_name[0] = 'G';
    target_message_name[1] = 'P';
    target_message_name[2] = 'Z';
    target_message_name[3] = 'D';
    target_message_name[4] = 'A';
    target_message_name[5] = '\0';

    // Find GPZDA message in the buffer
    is_zda_found = find_nmea_message(target_message_name, gps_rx_buff, BUFFER_SIZE, &nmea_start);

    if (is_zda_found)
    {
        // Parse the GPRMC message for needed data
        get_zda_utc_time(gps_rx_buff, BUFFER_SIZE, nmea_start, utc_time);

    }
}

/*******************************************************************************/
void gps_parse_rmc(
    const char* const gps_rx_buff,
    const UINT BUFFER_SIZE, 
    BYTE* const utc_time,
    BYTE* const gprmc_validity,
    BYTE* const latitude,
    BYTE* const north_or_south,
    BYTE* const longitude,
    BYTE* const east_or_west,
    BYTE* const ut_date)
{
    char target_message_name[6];

    UINT nmea_start = 0;
    bool is_rmc_found = false;
    gps_spi();
    // GPRMC is the target message
    target_message_name[0] = 'G';
    target_message_name[1] = 'P';
    target_message_name[2] = 'R';
    target_message_name[3] = 'M';
    target_message_name[4] = 'C';
    target_message_name[5] = '\0';

    // Find GPRMC message in the buffer
    is_rmc_found = find_nmea_message(target_message_name, gps_rx_buff, BUFFER_SIZE, &nmea_start);

    if (is_rmc_found)
    {
        // Parse the GPRMC message for needed data
        get_rmc_utc_time(gps_rx_buff, BUFFER_SIZE, nmea_start, utc_time);
        get_rmc_validity(gps_rx_buff, BUFFER_SIZE, nmea_start, gprmc_validity);
        if (gprmc_validity[0] == 0x41) //  0x41 or ASCII 'A' means valid data
        {
            get_rmc_latitude(gps_rx_buff, BUFFER_SIZE, nmea_start, latitude);
            get_rmc_north_or_south(gps_rx_buff, BUFFER_SIZE, nmea_start, north_or_south);
            get_rmc_longitude(gps_rx_buff, BUFFER_SIZE, nmea_start, longitude);
            get_rmc_east_or_west(gps_rx_buff, BUFFER_SIZE, nmea_start, east_or_west);
        }
        get_rmc_ut_date(gps_rx_buff, BUFFER_SIZE, nmea_start, ut_date);
    }
}
// find_nmea_message returns 0 on success, 1 on failure
//void gps_parse_gsv(
//    const BYTE* const gps_rx_buff,
//    const UINT BUFFER_SIZE, 
//    BYTE* const satellites)
//{
//    char target_message_name[6];
//
//    UINT nmea_start = 0;
//    bool is_gsv_found = false;
//    gps_spi();
//    // GPRMC is the target message
//    target_message_name[0] = 'G';
//    target_message_name[1] = 'P';
//    target_message_name[2] = 'G';
//    target_message_name[3] = 'S';
//    target_message_name[4] = 'V';
//    target_message_name[5] = '\0';
//
//    // Find GPRMC message in the buffer
//    is_gsv_found = find_nmea_message(target_message_name, gps_rx_buff, BUFFER_SIZE, &nmea_start);
//
//    if (is_gsv_found)
//    {
//        // Parse the GPRMC message for needed data
//        get_gsv_satellites(gps_rx_buff, BUFFER_SIZE, nmea_start, satellites);
//    }
//}
// find_nmea_message returns 0 on success, 1 on failure
void gps_parse_gga(
    const char* const gps_rx_buff,
    const UINT BUFFER_SIZE, 
    BYTE* const utc_time,
    BYTE* const latitude,
    BYTE* const north_or_south,
    BYTE* const longitude,
    BYTE* const east_or_west,
    BYTE* const fix_validity,    
    BYTE* const satellites)
{
    char target_message_name[6];

    UINT nmea_start = 0;
    bool is_gga_found = false;
    gps_spi();
    // GPRMC is the target message
    target_message_name[0] = 'G';
    target_message_name[1] = 'P';
    target_message_name[2] = 'G';
    target_message_name[3] = 'G';
    target_message_name[4] = 'A';
    target_message_name[5] = '\0';

    // Find NMEA message in the buffer
    is_gga_found = find_nmea_message(target_message_name, gps_rx_buff, BUFFER_SIZE, &nmea_start);

    if (is_gga_found)
    {
        // Parse the NMEA message for needed data
        
        get_gga_utc_time(gps_rx_buff, BUFFER_SIZE, nmea_start, utc_time);
        get_gga_latitude(gps_rx_buff, BUFFER_SIZE, nmea_start, latitude);
        get_gga_north_or_south(gps_rx_buff, BUFFER_SIZE, nmea_start, north_or_south);
        get_gga_longitude(gps_rx_buff, BUFFER_SIZE, nmea_start, longitude);
        get_gga_east_or_west(gps_rx_buff, BUFFER_SIZE, nmea_start, east_or_west);
        get_gga_validity(gps_rx_buff, BUFFER_SIZE, nmea_start, fix_validity);
        get_gga_satellites(gps_rx_buff, BUFFER_SIZE, nmea_start, satellites);
        
       
    }
}
// find_nmea_message /* 0:Failed, 1:Successful */


static int find_nmea_message(
    const char* const target_message_name,
    const char* const gps_rx_buff,
    const UINT BUFFER_SIZE, 
    /*out*/ UINT* const nmea_start)
{
    //xdev_in(uart_getc);		/* Join UART and console */
    //xdev_out(uart_putc);

    bool is_end_of_target_found = false;
    bool is_message_received = false;
    bool is_target_message_received = false;
    bool is_valid_checksum = false;

    UINT i = BUFFER_SIZE;
  
    UINT local_nmea_end = 0;
    UINT local_nmea_start = 0;
 
    *nmea_start = 0;

    // Find target message in the buffer
    while (!is_target_message_received && (i > 0))
    {
        is_message_received = false;
        is_end_of_target_found = false;
        local_nmea_start = 0;
        local_nmea_end = 0;

        // Find the message start symbol
        while (!is_message_received && (i > 0))
        {
            if (gps_rx_buff[i] == 0x24)
            {
                local_nmea_start = i;
                is_message_received = true;
            }
            i--;
        }
        if (is_message_received)
        {
//            xprintf("NMEA message received!\n");
        }

        // Check for target message
        if ((local_nmea_start + 5) < BUFFER_SIZE)
        {
            if (gps_rx_buff[local_nmea_start + 1] == target_message_name[0] &&
                gps_rx_buff[local_nmea_start + 2] == target_message_name[1] &&
                gps_rx_buff[local_nmea_start + 3] == target_message_name[2] &&
                gps_rx_buff[local_nmea_start + 4] == target_message_name[3] &&
                gps_rx_buff[local_nmea_start + 5] == target_message_name[4])
            {
                is_target_message_received = true;
            }
            else
            {
                is_target_message_received = false;
            }
        }

        // Find end of target message
        if (is_target_message_received)
        {
//            xprintf("%s message received!\n", target_message_name);
//            xprintf("%s message start index is %u\n", target_message_name, local_nmea_start);
            // Find end of message
            i = local_nmea_start;
            while ((!is_end_of_target_found) && (i < BUFFER_SIZE))
            {
                if (gps_rx_buff[i] == 0x0A)
                {
                    local_nmea_end = i;
                    if (local_nmea_end > 4) // 
                    {
 //                       xprintf("%s message end index is %u\n", target_message_name, local_nmea_end);
                        is_end_of_target_found = true;
                    }
                }
                i++;
            }
        }

        // Check the checksum
        if (is_end_of_target_found)
        {
            is_valid_checksum = check_checksum(target_message_name, gps_rx_buff, BUFFER_SIZE, local_nmea_start, local_nmea_end);
        }
    }
    if (is_valid_checksum)
    {
        *nmea_start = local_nmea_start; // store the start index of the nmea message
        return 1;
    }
    else
    {
        return 0;
    }
}


static void get_rmc_utc_time(const char* const gps_rx_buff, const UINT BUFFER_SIZE, const UINT nmea_start, BYTE* const utc_time)
{
    // Locate and store GPRMC time data, found after first comma
    UINT comma_count = 0;
    UINT i = nmea_start;
    UINT time_start = 0;
    UINT time_end = 0;
    while ((comma_count < 1) && (i < BUFFER_SIZE))
    {
        if (gps_rx_buff[i] == 0x2C)
        {
            i++;
            comma_count++;
        }
        else
        {
            i++;
        }
    }
    time_start = i;
    while ((gps_rx_buff[i] != 0x2C) && (i < BUFFER_SIZE))
    {
        i++;
    }
    time_end = i;

    for (i = time_start; i < time_end; i++)
    {
        if ((i - time_start) < 10)
        {
            utc_time[i - time_start] = gps_rx_buff[i];
        }
    }
}

static void get_rmc_validity(
    const char* const gps_rx_buff,
    const UINT BUFFER_SIZE, 
    const UINT nmea_start,
    BYTE* const gprmc_validity)
{
    // Locate and GPRMC validity indicator, found after second comma
    UINT comma_count = 0;
    UINT i = nmea_start;

    while ((comma_count < 2) && (i < BUFFER_SIZE))
    {
        if (gps_rx_buff[i] == 0x2C)
        {
            i++;
            comma_count++;
        }
        else
        {
            i++;
        }
    }
    // GPS fix validity indicator is in 1st byte after stored comma location
    gprmc_validity[0] = gps_rx_buff[i];
}

static void get_rmc_latitude(
    const char* const gps_rx_buff,
    const UINT BUFFER_SIZE,
    const UINT nmea_start,
    BYTE* const latitude)
{
    // Locate and store GPRMC latitude data, found after third comma
    UINT comma_count = 0;
    UINT i = nmea_start;
    UINT latitude_start = 0;
    UINT latitude_end = 0;
    while ((comma_count < 3) && (i < BUFFER_SIZE))
    {
        if (gps_rx_buff[i] == 0x2C)
        {
            i++;
            comma_count++;
        }
        else
        {
            i++;
        }
    }
    latitude_start = i;
    while ((gps_rx_buff[i] != 0x2C) && (i < BUFFER_SIZE))
    {
        i++;
    }
    latitude_end = i;

    for (i = latitude_start; i < latitude_end; i++)
    {
        if ((i - latitude_start) < 16)
        {
            latitude[i - latitude_start] = gps_rx_buff[i];
        }
    }
}

static void get_rmc_north_or_south(
    const char* const gps_rx_buff, 
    const UINT BUFFER_SIZE, 
    const UINT nmea_start,
    BYTE* const north_or_south)
{
    // Locate and store GPRMC North or South data, found after forth comma
    UINT comma_count = 0;
    UINT i = nmea_start;

    while ((comma_count < 4) && (i < BUFFER_SIZE))
    {
        if (gps_rx_buff[i] == 0x2C)
        {
            i++;
            comma_count++;
        }
        else
        {
            i++;
        }
    }
    north_or_south[0] = gps_rx_buff[i];
}

static void get_rmc_longitude(
    const char* const gps_rx_buff,
    const UINT BUFFER_SIZE,
    const UINT nmea_start,
    BYTE* const longitude)
{
    // Locate and store GPRMC lognitude data, found after fifth comma
    UINT comma_count = 0;
    UINT i = nmea_start;
    UINT longitude_start = 0;
    UINT longitude_end = 0;
    while ((comma_count < 5) && (i < BUFFER_SIZE))
    {
        if (gps_rx_buff[i] == 0x2C)
        {
            i++;
            comma_count++;
        }
        else
        {
            i++;
        }
    }
    longitude_start = i;
    while ((gps_rx_buff[i] != 0x2C) && (i < BUFFER_SIZE))
    {
        i++;
    }
    longitude_end = i;

    for (i = longitude_start; i < longitude_end; i++)
    {
        if ((i - longitude_start) < 16)
        {
            longitude[i - longitude_start] = gps_rx_buff[i];
        }
    }
}

static void get_rmc_east_or_west(
    const char* const gps_rx_buff, 
    const UINT BUFFER_SIZE, 
    const UINT nmea_start,
    BYTE* const east_or_west)
{
    // Locate and store GPRMC North or South data, found after sixth comma
    UINT comma_count = 0;
    UINT i = nmea_start;

    while ((comma_count < 6) && (i < BUFFER_SIZE))
    {
        if (gps_rx_buff[i] == 0x2C)
        {
            i++;
            comma_count++;
        }
        else
        {
            i++;
        }
    }
    east_or_west[0] = gps_rx_buff[i];
}

static void get_rmc_ut_date(
    const char* const gps_rx_buff, 
    const UINT BUFFER_SIZE, 
    const UINT nmea_start,
    BYTE* const ut_date)
{
    // Locate and store GPRMC ut date data, found after ninth comma
    UINT comma_count = 0;
    UINT i = nmea_start;
    UINT date_start = 0;
    UINT date_end = 0;

    while ((comma_count < 9) && (i < BUFFER_SIZE))
    {
        if (gps_rx_buff[i] == 0x2C)
        {
            i++;
            comma_count++;
        }
        else
        {
            i++;
        }
    }
    date_start = i;
    date_end = date_start + 6;

    for (i = date_start; i < date_end; i++)
    {
        ut_date[i - date_start] = gps_rx_buff[i];
    }
}

static void get_gga_utc_time(const char* const gps_rx_buff, const UINT BUFFER_SIZE, const UINT nmea_start, BYTE* const utc_time)
{
    // Locate and store GPRMC time data, found after first comma
    UINT comma_count = 0;
    UINT i = nmea_start;
    UINT time_start = 0;
    UINT time_end = 0;
    while ((comma_count < 1) && (i < BUFFER_SIZE))
    {
        if (gps_rx_buff[i] == 0x2C)
        {
            i++;
            comma_count++;
        }
        else
        {
            i++;
        }
    }
    time_start = i;
    while ((gps_rx_buff[i] != 0x2C) && (i < BUFFER_SIZE))
    {
        i++;
    }
    time_end = i;

    for (i = time_start; i < time_end; i++)
    {
        if ((i - time_start) < 10)
        {
            utc_time[i - time_start] = gps_rx_buff[i];
        }
    }
}


static void get_gga_latitude(
    const char* const gps_rx_buff,
    const UINT BUFFER_SIZE,
    const UINT nmea_start,
    BYTE* const latitude)
{
    // Locate and store GPRMC latitude data, found after third comma
    UINT comma_count = 0;
    UINT i = nmea_start;
    UINT latitude_start = 0;
    UINT latitude_end = 0;
    while ((comma_count < 2) && (i < BUFFER_SIZE))
    {
        if (gps_rx_buff[i] == 0x2C)
        {
            i++;
            comma_count++;
        }
        else
        {
            i++;
        }
    }
    latitude_start = i;
    while ((gps_rx_buff[i] != 0x2C) && (i < BUFFER_SIZE))
    {
        i++;
    }
    latitude_end = i;

    for (i = latitude_start; i < latitude_end; i++)
    {
        if ((i - latitude_start) < 16)
        {
            latitude[i - latitude_start] = gps_rx_buff[i];
        }
    }
}

static void get_gga_north_or_south(
    const char* const gps_rx_buff, 
    const UINT BUFFER_SIZE, 
    const UINT nmea_start,
    BYTE* const north_or_south)
{
    // Locate and store GPRMC North or South data, found after forth comma
    UINT comma_count = 0;
    UINT i = nmea_start;

    while ((comma_count < 3) && (i < BUFFER_SIZE))
    {
        if (gps_rx_buff[i] == 0x2C)
        {
            i++;
            comma_count++;
        }
        else
        {
            i++;
        }
    }
    north_or_south[0] = gps_rx_buff[i];
}

static void get_gga_longitude(
    const char* const gps_rx_buff,
    const UINT BUFFER_SIZE,
    const UINT nmea_start,
    BYTE* const longitude)
{
    // Locate and store GPRMC longitude data, found after fifth comma
    UINT comma_count = 0;
    UINT i = nmea_start;
    UINT longitude_start = 0;
    UINT longitude_end = 0;
    while ((comma_count < 4) && (i < BUFFER_SIZE))
    {
        if (gps_rx_buff[i] == 0x2C)
        {
            i++;
            comma_count++;
        }
        else
        {
            i++;
        }
    }
    longitude_start = i;
    while ((gps_rx_buff[i] != 0x2C) && (i < BUFFER_SIZE))
    {
        i++;
    }
    longitude_end = i;

    for (i = longitude_start; i < longitude_end; i++)
    {
        if ((i - longitude_start) < 16)
        {
            longitude[i - longitude_start] = gps_rx_buff[i];
        }
    }
}

static void get_gga_east_or_west(
    const char* const gps_rx_buff, 
    const UINT BUFFER_SIZE, 
    const UINT nmea_start,
    BYTE* const east_or_west)
{
    // Locate and store GPRMC North or South data, found after sixth comma
    UINT comma_count = 0;
    UINT i = nmea_start;

    while ((comma_count < 5) && (i < BUFFER_SIZE))
    {
        if (gps_rx_buff[i] == 0x2C)
        {
            i++;
            comma_count++;
        }
        else
        {
            i++;
        }
    }
    east_or_west[0] = gps_rx_buff[i];
}
static void get_gga_validity(
    const char* const gps_rx_buff,
    const UINT BUFFER_SIZE, 
    const UINT nmea_start,
    BYTE* const fix_validity)
{
    // Locate and GPRMC validity indicator, found after second comma
    UINT comma_count = 0;
    UINT i = nmea_start;

    while ((comma_count < 6) && (i < BUFFER_SIZE))
    {
        if (gps_rx_buff[i] == 0x2C)
        {
            i++;
            comma_count++;
        }
        else
        {
            i++;
        }
    }
    // GPS fix validity indicator is in 1st byte after stored comma location
    fix_validity[0] = gps_rx_buff[i];
}
//static void get_msg_satellites(
//    const char* const gps_rx_buff,
//    const UINT BUFFER_SIZE, 
//    const UINT nmea_start,
//    BYTE* const satellites)
//{
//    // Locate and GPRMC validity indicator, found after second comma
//    UINT comma_count = 0;
//    UINT i = nmea_start;
//
//    while ((comma_count < 7) && (i < BUFFER_SIZE))
//    {
//        if (gps_rx_buff[i] == 0x2C)
//        {
//            i++;
//            comma_count++;
//        }
//        else
//        {
//            i++;
//        }
//    }
//    // GPS fix validity indicator is in 1st byte after stored comma location
//    satellites[0] = gps_rx_buff[i];
//}

static void get_gga_satellites(
    const char* const gps_rx_buff,
    const UINT BUFFER_SIZE,
    const UINT nmea_start,
    BYTE* const satellites)
{
    // Locate and store GPGGA satellite data, found after sixth comma
    UINT comma_count = 0;
    UINT i = nmea_start;
    UINT satellites_start = 0;
    UINT satellites_end = 0;
    while ((comma_count < 7) && (i < BUFFER_SIZE))
    {
        if (gps_rx_buff[i] == 0x2C)
        {
            i++;
            comma_count++;
        }
        else
        {
            i++;
        }
    }
    satellites_start = i;
    while ((gps_rx_buff[i] != 0x2C) && (i < BUFFER_SIZE))
    {
        i++;
    }
    satellites_end = i;

    for (i = satellites_start; i < satellites_end; i++)
    {
        if ((i - satellites_start) < 3)
        {
            satellites[i - satellites_start] = gps_rx_buff[i];
        }
    }
}

//static void get_gsv_satellites(
//    const char* const gps_rx_buff,
//    const UINT BUFFER_SIZE,
//    const UINT nmea_start,
//    BYTE* const satellites)
//{
//    UINT comma_count = 0;
//    UINT i = nmea_start;
//    UINT satellites_start = 0;
//    UINT satellites_end = 0;
//    while ((comma_count < 3) && (i < BUFFER_SIZE))
//    {
//        if (gps_rx_buff[i] == 0x2C)
//        {
//            i++;
//            comma_count++;
//        }
//        else
//        {
//            i++;
//        }
//    }
//    satellites_start = i;
//    while ((gps_rx_buff[i] != 0x2C) && (i < BUFFER_SIZE))
//    {
//        i++;
//    }
//    satellites_end = i;
//
//    for (i = satellites_start; i < satellites_end; i++)
//    {
//        if ((i - satellites_start) < 3)
//        {
//            satellites[i - satellites_start] = gps_rx_buff[i];
//        }
//    }
//}

static void get_zda_utc_time(const char* const gps_rx_buff, const UINT BUFFER_SIZE, const UINT nmea_start, BYTE* const utc_time)
{
    // Locate and store GPZDZ time data, found after first comma
    UINT comma_count = 0;
    UINT i = nmea_start;
    UINT time_start = 0;
    UINT time_end = 0;
    while ((comma_count < 1) && (i < BUFFER_SIZE))
    {
        if (gps_rx_buff[i] == 0x2C)
        {
            i++;
            comma_count++;
        }
        else
        {
            i++;
        }
    }
    time_start = i;
    while ((gps_rx_buff[i] != 0x2C) && (i < BUFFER_SIZE))
    {
        i++;
    }
    time_end = i;

    for (i = time_start; i < time_end; i++)
    {
        if ((i - time_start) < 10)
        {
            utc_time[i - time_start] = gps_rx_buff[i];
        }
    }
}

//static void get_zda_ut_date(
//    const char* const gps_rx_buff, 
//    const UINT BUFFER_SIZE, 
//    const UINT nmea_start,
//    BYTE* const ut_date)
//{
//    // Locate and store GPZDA ut date data, found after 2nd comma
//    UINT comma_count = 0;
//    UINT i = nmea_start;
//    UINT date_start = 0;
//    UINT date_end = 0;
//
//    while ((comma_count < 2) && (i < BUFFER_SIZE))
//    {
//        if (gps_rx_buff[i] == 0x2C)
//        {
//            i++;
//            comma_count++;
//        }
//        else
//        {
//            i++;
//        }
//    }
//    date_start = i;
//    date_end = date_start + 6;
//
//    for (i = date_start; i < date_end; i++)
//    {
//        ut_date[i - date_start] = gps_rx_buff[i];
//    }
//}

static int check_checksum(
    const char* const target_message_name, 
    const char* const gps_rx_buff, 
    const UINT BUFFER_SIZE, 
    const UINT nmea_start, 
    const UINT nmea_end)
{
    //xdev_in(uart_getc);		/* Join UART and console */
    //xdev_out(uart_putc);

    bool is_valid_checksum = false;

    BYTE nmea_checksum[2];

    char checksum_string[5];
    char* temp_char_ptr;

    long converted_received_checksum = 0;

    UINT computed_checksum = 0;
    UINT nmea_checksummed_end = 0;
    UINT nmea_checksummed_start = 0;
    UINT i = 0;

    // Locate and store checksum
    if (nmea_end > 4) // 
    {
        nmea_checksum[0] = gps_rx_buff[nmea_end - 3];
        nmea_checksum[1] = gps_rx_buff[nmea_end - 2];

        // Locate the checksummed message
        nmea_checksummed_start = nmea_start + 1;
        nmea_checksummed_end = nmea_end - 5;
    }

    // Compute the checksum
    computed_checksum = 0;
    for (i = nmea_checksummed_start; i <= nmea_checksummed_end; i++)
    {
        computed_checksum ^= gps_rx_buff[i];
    }

    checksum_string[0] = '0';
    checksum_string[1] = 'x';
    checksum_string[2] = nmea_checksum[0];
    checksum_string[3] = nmea_checksum[1];
    checksum_string[4] = '\0';

    temp_char_ptr = checksum_string;
    xatoi((const char**)&temp_char_ptr, &converted_received_checksum);

//    xprintf("Received %s checksum is %c%c\n", target_message_name, nmea_checksum[0], nmea_checksum[1]);
//    xprintf("Computed %s checksum is %X\n", target_message_name, computed_checksum);

    if (converted_received_checksum == computed_checksum)
    {
//        xprintf("%s checksum is valid!\n", target_message_name);
        is_valid_checksum = true;
    }
    else
    {
 //       xprintf("ERROR: %s checksum is invalid!\n", target_message_name);
    }

    return is_valid_checksum;
}

/******************************************************************************
*
*                        Microchip RTCC BSP Module
*
******************************************************************************
* FileName:           rtcc.c
* Dependencies:       rtcc.h
* Processor:          PIC24/dsPIC30/dsPIC33
* Compiler:           XC16
* Company:            Microchip Technology, Inc.
*
* Software License Agreement
*
* The software supplied herewith by Microchip Technology Incorporated
* (the "Company") for its PICmicro(R) Microcontroller is intended and
* supplied to you, the Company's customer, for use solely and
* exclusively on Microchip PICmicro Microcontroller products. The
* software is owned by the Company and/or its supplier, and is
* protected under applicable copyright laws. All rights are reserved.
* Any use in violation of the foregoing restrictions may subject the
* user to criminal sanctions under applicable laws, as well as to
* civil liability for the breach of the terms and conditions of this
* license.
*
* THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
* WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
* TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
* IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
* CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
*
********************************************************************/


#include "rtcc_pic24h.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "p24hj128gp202.h"
#include "diskio.h"


/* Socket controls  (Platform dependent) */
#define AD_CS_LOW()     _LATB12 = 0	/* AD CS = L */
#define AD_CS_HIGH()    _LATB12 = 1	/* AD CS = H */
#define GPS_CS_HIGH()   _LATB14 = 1   /* GPS CS = H */
#define GPS_CS_LOW()    _LATB14 = 0 /* GPS CS = L */
#define RTCC_CS_HIGH()  _LATB1 = 1   /* RTCC CS = H */
#define RTCC_CS_LOW()   _LATB1 = 0  /* RTCC CS = L */

#define COMMS_READ  (0B01000000) /* READ OPERATION */
#define COMMS_WRITE (0B00000000) /* WRITE OPERATION */
#define COMMS_GPIO  (0B00000110) /* GPIO CONFIG. REGISTER */

#define	RTCC_CLOCK() SPI2CON1 = 0x0323 /* Set sclk2 to 5.0MHz with FCY of 40 MHz */

#define EEREAD  (0B00000011)    //read data from ee memory array beginning at selected address
#define EEWRITE (0B00000010)    //write data to ee memeory array beginning at selected address
#define EEWDI   (0B00000100)    //reset the write enable latch (disable write operations)
#define EEWREN  (0B00000110)    //set the write enable latch (enable write operations)
#define SRREAD  (0B00000101)    //read status register
#define SRWRITE (0B00000001)    //write status register
#define READ    (0B00010011)    //read rtcc/sram array beginning at selected address
#define WRITE   (0B00010010)    //write rtcc/sram data to memory array beginning at selected address
#define UNLOCK  (0B00010100)    //unlock id locations
#define IDWRITE (0B00110010)    //write to the id locations
#define IDREAD  (0B00110011)    //read the id locations
#define CLRRAM  (0B01010100)    //clear ram location to '0'
#define HUND    (0x00)
#define SEC     (0x01)
#define MIN     (0x02)
#define HOUR    (0x03)
#define DAY     (0x04)
#define DATE    (0x05)
#define MONTH   (0x06)
#define YEAR    (0x07)
#define CONT    (0x08)
#define CAL     (0x09)
#define START_OSC   (0x80)
#define MIL     (0xBF)
#define VBATEN  (0x08)
#define CALSGN  (0x03)          //CALSGN address
#define NEG     (0x80)          //negative calibration
#define POS     (0x7F)          //positive calibration
#define SQWE    (0x40)
#define FREQ32  (0x03)



static void rtcc_select (void);


uint8_t RTCC_DecToBCD (uint8_t value);
uint8_t RTCC_BCDToDec (uint8_t value);

/*-----------------------------------------------------------------------*/
/* Transmit/Receive data to/from ad via SPI  (Platform dependent)       */
/*-----------------------------------------------------------------------*/

/* Single byte SPI transfer */

static
BYTE rtcc_xchg_spi (BYTE dat)
{
	SPI2BUF = dat;
	while (!_SPIRBF_2) ;
	return (BYTE)SPI2BUF;
}

static void rtcc_spi (void)
{

    _SPIEN_2 = 0;
    RTCC_CLOCK();	/* Enable SPI2 */
    SPI2CON2 = 0x0000;
    _SPIEN_2 = 1;
}

/*-----------------------------------------------------------------------*/
/* Deselect the a/d and release SPI bus                                  */
/*-----------------------------------------------------------------------*/


void rtcc_deselect (void)
{
    _SPIEN_2 = 0;
    RTCC_CS_HIGH();
   ;
}


/*-----------------------------------------------------------------------*/
/* Select the rtcc                                                        */
/*-----------------------------------------------------------------------*/

static
void rtcc_select (void)	
{
    GPS_CS_HIGH();
    AD_CS_HIGH();	
    RTCC_CS_LOW();
 ;
    
}
//static
//void rtcc_toggle_deselect (void)
//{
//    RTCC_CS_HIGH();
//    ;
//    ;
//    ;
//    ;
//    ;
//    ;
//    ;
//    ;
//    ;
//    ;
//    ;
//    ;
//    ;
//    ;
//    ;
//    RTCC_CS_LOW();
//    ;
//    
//}

void rtcc_osc_on(void)
{
    
    rtcc_spi();
    rtcc_select();
    rtcc_xchg_spi(WRITE);
    rtcc_xchg_spi(CONT);
//   rtcc_xchg_spi(SQWE | FREQ32); //sqwe and 32khz out
    rtcc_xchg_spi(0x00); //SQWE disable
//    rtcc_toggle_deselect();
//    rtcc_xchg_spi(WRITE);
//    rtcc_xchg_spi(HOUR);
//    rtcc_xchg_spi(0x00);
//    rtcc_xchg_spi(SEC);
//    rtcc_xchg_spi(START_OSC);  // Turn on the oscillator
    rtcc_deselect();
    
}

void RTCC_Initialize (RTCC_DATETIME * value)
{
//    uint8_t rtcc_cal;
//    uint8_t rtcc_calsgn;

    rtcc_spi();
    rtcc_select();
//    rtcc_xchg_spi(READ);                    //send read command
//    rtcc_xchg_spi(CAL);                     //read cal register
//    rtcc_cal = rtcc_xchg_spi(0xFF);
//    rtcc_toggle_deselect();
//
//    
//    if(rtcc_cal != 0)
//    {                //something in cal so set calset true
//    rtcc_xchg_spi(READ);
//    rtcc_xchg_spi(HOUR);                    //since calset is ture, read cal sign
//    value->calsgn = (rtcc_xchg_spi(0xFF) & 0x80); //if calsgn = 1, neg cal sign
//    rtcc_calsgn = value->calsgn;
//    } //if(rtcc_cal != 0)
//    rtcc_toggle_deselect;

    rtcc_xchg_spi(WRITE);                   //write time to rtcc
    rtcc_xchg_spi(SEC);
    rtcc_xchg_spi(START_OSC | (RTCC_DecToBCD (value->second)));  // Turn on the oscillator
    rtcc_xchg_spi(RTCC_DecToBCD (value->minute));

//    if(rtcc_cal)     //if calset is true, preserve calsgn settings
//    {
//        if(rtcc_calsgn)
//        {
//            rtcc_xchg_spi(rtcc_calsgn | (MIL & (RTCC_DecToBCD (value->hour))));
//        } //calsgn negative = 0x80
//        else
//        {
//            rtcc_xchg_spi(0x7f & (MIL & (RTCC_DecToBCD (value->hour))));
//        } //calsgn positive = 0x00
//    } //if calset is true,
//    else
//    {
     rtcc_xchg_spi(POS & (MIL & (RTCC_DecToBCD (value->hour))));          // default CALSGN negative, enable 24hr clock

//    }


    rtcc_xchg_spi(VBATEN | (RTCC_DecToBCD (value->weekday)));    //enable battery backup
    rtcc_xchg_spi(RTCC_DecToBCD (value->day));
    rtcc_xchg_spi(RTCC_DecToBCD (value->month));
    rtcc_xchg_spi(RTCC_DecToBCD (value->year));
//    if(!rtcc_cal)                // if calset not true, the set default calibration
//    {
    rtcc_xchg_spi(0x00);  //place 0x00 in control register
    rtcc_xchg_spi(0x00);  //default calibration = 90ppm
//    } //if !rtcc_cal
    rtcc_deselect();
    
     
}


void RTCC_TimeGet (RTCC_DATETIME * value)
{

    rtcc_spi();                             //initialize rtcc spi
    rtcc_select();                          //select the rtcc
    rtcc_xchg_spi(READ);                    //send read command
    rtcc_xchg_spi(SEC);                     //first address to read is seconds
    value->second = rtcc_xchg_spi(0xFF);    //put seconds in value
    value->minute = rtcc_xchg_spi(0xFF);    //put minustes in value
    value->hour = rtcc_xchg_spi(0xFF);      //put hours in value
    value->weekday = rtcc_xchg_spi(0xFF);   //put weekday in value
    value->day = rtcc_xchg_spi(0xFF);       //put day in value
    value->month = rtcc_xchg_spi(0xFF);     //put month in value
    value->year = rtcc_xchg_spi(0xFF);      //put year in value
    value->cont = rtcc_xchg_spi(0xFF);
    value->cal = rtcc_xchg_spi(0xFF);
    value->calsgn = (value->hour >> 7);

    rtcc_deselect();

    value->year = RTCC_BCDToDec(value->year);          //convert bcd to decimal
    value->month = RTCC_BCDToDec(value->month & 0x1F);
    value->day = RTCC_BCDToDec(value->day & 0x3F);
    value->weekday = RTCC_BCDToDec(value->weekday & 0x07);
    value->hour = RTCC_BCDToDec(value->hour & 0x3F);
    value->minute = RTCC_BCDToDec(value->minute & 0x7F);
    value->second = RTCC_BCDToDec(value->second & 0x7F);
    value->cont = value->cont & 0xFF;
    value->cal = value->cal & 0xFF;
    

}

// Note : value must be < 100
uint8_t RTCC_DecToBCD (uint8_t value)
{
    return (((value / 10)) << 4) | (value % 10);
}

uint8_t RTCC_BCDToDec (uint8_t value)
{
    return ((value >> 4) * 10) + (value & 0x0F);
}


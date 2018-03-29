/*---------------------------------------------------------------------------------------------------------------------------
 * AD7173-8  Analog Devices A to D header
 *
 */

// #ifndef __AD7173_8_H
#define __AD7173_8_H

#define COMMS COMMS
extern volatile unsigned int COMMS;
__extension__ typedef struct tagCOMMSBITS {
  union {
    struct {
      unsigned RA:6;
      unsigned RW:1;
      unsigned WEN:1;
     
      
    };
    struct {
       unsigned RA0:1;
       unsigned RA1:1;
       unsigned RA2:1;
       unsigned RA3:1;
       unsigned RA4:1;
       unsigned RA5:1;
    };
  };
} COMMSBITS;
extern volatile COMMSBITS COMMSbits;

#define STATUS STATUS
extern volatile unsigned int STATUS;
__extension__ typedef struct tagSTATUSBITS {
    union {
        struct {
            unsigned CHAN : 4;
            unsigned REG_ERROR:1;
            unsigned CRC_ERROR:1;
            unsigned ADC_ERROR:1;
            unsigned RDY:1;
        };
        struct {
            unsigned CHAN0:1;
            unsigned CHAN1:1;
            unsigned CHAN2:1;
            unsigned CHAN3:1;
        };
    };
 } STATUSBITS;
 extern volatile STATUSBITS STATUSbits;

#define ADCMODE ADCMODE
extern volatile unsigned int ADCMODE;
__extension__ typedef struct tagADCMODEBITS {
     union {
         struct {
            unsigned : 2;
            unsigned CLOCKSEL : 2;
            unsigned MODE : 3;
            unsigned : 1;
            unsigned DELAY : 3;
            unsigned : 2;
            unsigned SING_CYC : 1;
            unsigned : 1;
            unsigned REF_EN : 1;
       };
        struct {
            unsigned CLOCKSEL0:1;
            unsigned CLOCKSEL1:1;
            unsigned MODE0:1;
            unsigned MODE1:1;
            unsigned MODE2:1;
            unsigned DELAY0:1;
            unsigned DELAY1:1;
            unsigned DELAY2:1;
       };
     };
} ADCMODEBITS;
extern volatile ADCMODEBITS ADCMODEbits;

#define IFMODE IFMODE
extern volatile unsigned int IFMODE;
__extension__ typedef struct tagIFMODEBITS {
    union {
        struct {
            unsigned WL16 : 1;
            unsigned : 1;
            unsigned CRC_EN : 2;
            unsigned : 1;
            unsigned REG_CHECK : 1;
            unsigned DATA_STAT : 1;
            unsigned CONTREAD : 1;
            unsigned DOUT_RESET : 1;
            unsigned : 1;
            unsigned HIDE_DELAY : 1;
            unsigned IOSTRENGTH : 1;
            unsigned ALT_SYNC : 1;
            unsigned : 3;
        };
        struct {
            unsigned CRC_EN0:1;
            unsigned CRC_EN1:1;
        };
    };
} IFMODEBITS;
extern volatile IFMODEBITS IFMODEbits;

#define REGCHECK REGCHECK
extern volatile unsigned long REGCHECK;
typedef struct tagREGCHECKBITS {
    unsigned long REGISTER_CHECK : 24;
} REGCHECKBITS;
extern volatile REGCHECKBITS REGCHECKbits;

#define DATA DATA
extern volatile unsigned long DATA;
typedef struct tagDATABITS {
    unsigned long DATA : 24;
} DATABITS;
extern volatile DATABITS DATAbits;

#define GPIOCON GPIOCON
extern volatile unsigned int GPIOCON;
__extension__ typedef struct tagGPIOCONBITS {
    union {
        struct {
            unsigned GP_DATA0:1;
            unsigned GP_DATA1:1;
            unsigned OP_EN:2;
            unsigned IP_EN:2;
            unsigned GP_DATA2:1;
            unsigned GP_DATA3:1;
            unsigned ERR_DAT:1;
            unsigned ERR_EN:2;
            unsigned SYNC_EN:1;
            unsigned MUX_IO:1;
            unsigned OP_EN2_3:1;
            unsigned PDSW:1;
            unsigned :1;
        };
        struct {
            unsigned OP_EN0:1;
            unsigned OP_EN1:1;
            unsigned IP_EN0:1;
            unsigned IP_EN1:1;
            unsigned :3;
            unsigned ERR_EN0:1;
            unsigned ERR_EN1:1;
       };
    };
} GPIOCONBITS;
extern volatile GPIOCONBITS GPIOCONbits;

#define ID ID
extern volatile unsigned int ID;
typedef struct tagIDBITS{
    struct {
        unsigned ID : 16;
    };
} IDBITS;
extern volatile IDBITS IDbits;

#define CH0 CH0
extern volatile unsigned int CH0;
__extension__ typedef struct tagCH0BITS {
    union {
        struct {
            unsigned AINNEG0_:5;
            unsigned AINPOS0_:5;
            unsigned :2;
            unsigned SETUP_SEL0_:3;
            unsigned CH_EN0:1;
        };
        struct {
            unsigned AINNEG0_0:1;
            unsigned AINNEG0_1:1;
            unsigned AINNEG0_2:1;
            unsigned AINNEG0_3:1;
            unsigned AINNEG0_4:1;
            unsigned AINPOS0_0:1;
            unsigned AINPOS0_1:1;
            unsigned AINPOS0_2:1;
            unsigned AINPOS0_3:1;
            unsigned AINPOS0_4:1;
            unsigned :2;
            unsigned SETUP_SEL0_0:1;
            unsigned SETUP_SEL0_1:1;
            unsigned SETUP_SEL0_2:1;
        };
    };
} CH0BITS;
extern volatile CH0BITS CH0bits;

#define CH1 CH1
extern volatile unsigned int CH1;
__extension__ typedef struct tagCH1BITS {
    union {
        struct {
            unsigned AINNEG1_:5;
            unsigned AINPOS1_:5;
            unsigned :2;
            unsigned SETUP_SEL1_:3;
            unsigned CH_EN1:1;
        };
        struct {
            unsigned AINNEG1_0:1;
            unsigned AINNEG1_1:1;
            unsigned AINNEG1_2:1;
            unsigned AINNEG1_3:1;
            unsigned AINNEG1_4:1;
            unsigned AINPOS1_0:1;
            unsigned AINPOS1_1:1;
            unsigned AINPOS1_2:1;
            unsigned AINPOS1_3:1;
            unsigned AINPOS1_4:1;
            unsigned :2;
            unsigned SETUP_SEL1_0:1;
            unsigned SETUP_SEL1_1:1;
            unsigned SETUP_SEL1_2:1;
        };
    };
} CH1BITS;
extern volatile CH1BITS CH1bits;

#define CH2 CH2
extern volatile unsigned int CH2;
__extension__ typedef struct tagCH2BITS {
    union {
        struct {
            unsigned AINNEG2_:5;
            unsigned AINPOS2_:5;
            unsigned :2;
            unsigned SETUP_SEL2_:3;
            unsigned CH_EN2:1;
        };
        struct {
            unsigned AINNEG2_0:1;
            unsigned AINNEG2_1:1;
            unsigned AINNEG2_2:1;
            unsigned AINNEG2_3:1;
            unsigned AINNEG2_4:1;
            unsigned AINPOS2_0:1;
            unsigned AINPOS2_1:1;
            unsigned AINPOS2_2:1;
            unsigned AINPOS2_3:1;
            unsigned AINPOS2_4:1;
            unsigned :2;
            unsigned SETUP_SEL2_0:1;
            unsigned SETUP_SEL2_1:1;
            unsigned SETUP_SEL2_2:1;
        };
    };
} CH2BITS;
extern volatile CH2BITS CH2bits;

#define CH3 CH3
extern volatile unsigned int CH3;
__extension__ typedef struct tagCH3BITS {
    union {
        struct {
            unsigned AINNEG3_:5;
            unsigned AINPOS3_:5;
            unsigned :2;
            unsigned SETUP_SEL3_:3;
            unsigned CH_EN3:1;
        };
        struct {
            unsigned AINNEG3_0:1;
            unsigned AINNEG3_1:1;
            unsigned AINNEG3_2:1;
            unsigned AINNEG3_3:1;
            unsigned AINNEG3_4:1;
            unsigned AINPOS3_0:1;
            unsigned AINPOS3_1:1;
            unsigned AINPOS3_2:1;
            unsigned AINPOS3_3:1;
            unsigned AINPOS3_4:1;
            unsigned :2;
            unsigned SETUP_SEL3_0:1;
            unsigned SETUP_SEL3_1:1;
            unsigned SETUP_SEL3_2:1;
        };
    };
} CH3BITS;
extern volatile CH3BITS CH3bits;

#define CH4 CH4
extern volatile unsigned int CH4;
__extension__ typedef struct tagCH4BITS {
    union {
        struct {
            unsigned AINNEG4_:5;
            unsigned AINPOS4_:5;
            unsigned :2;
            unsigned SETUP_SEL4_:3;
            unsigned CH_EN4:1;
        };
        struct {
            unsigned AINNEG4_0:1;
            unsigned AINNEG4_1:1;
            unsigned AINNEG4_2:1;
            unsigned AINNEG4_3:1;
            unsigned AINNEG4_4:1;
            unsigned AINPOS4_0:1;
            unsigned AINPOS4_1:1;
            unsigned AINPOS4_2:1;
            unsigned AINPOS4_3:1;
            unsigned AINPOS4_4:1;
            unsigned :2;
            unsigned SETUP_SEL4_0:1;
            unsigned SETUP_SEL4_1:1;
            unsigned SETUP_SEL4_2:1;
        };
    };
} CH4BITS;
extern volatile CH4BITS CH4bits;

#define CH5 CH5
extern volatile unsigned int CH5;
__extension__ typedef struct tagCH5BITS {
    union {
        struct {
            unsigned AINNEG5_:5;
            unsigned AINPOS5_:5;
            unsigned :2;
            unsigned SETUP_SEL5_:3;
            unsigned CH_EN5:1;
        };
        struct {
            unsigned AINNEG5_0:1;
            unsigned AINNEG5_1:1;
            unsigned AINNEG5_2:1;
            unsigned AINNEG5_3:1;
            unsigned AINNEG5_4:1;
            unsigned AINPOS5_0:1;
            unsigned AINPOS5_1:1;
            unsigned AINPOS5_2:1;
            unsigned AINPOS5_3:1;
            unsigned AINPOS5_4:1;
            unsigned :2;
            unsigned SETUP_SEL5_0:1;
            unsigned SETUP_SEL5_1:1;
            unsigned SETUP_SEL5_2:1;
        };
    };
} CH5BITS;
extern volatile CH5BITS CH5bits;

#define CH6 CH6
extern volatile unsigned int CH6;
__extension__ typedef struct tagCH6BITS {
    union {
        struct {
            unsigned AINNEG6_:5;
            unsigned AINPOS6_:5;
            unsigned :2;
            unsigned SETUP_SEL6_:3;
            unsigned CH_EN6:1;
        };
        struct {
            unsigned AINNEG6_0:1;
            unsigned AINNEG6_1:1;
            unsigned AINNEG6_2:1;
            unsigned AINNEG6_3:1;
            unsigned AINNEG6_4:1;
            unsigned AINPOS6_0:1;
            unsigned AINPOS6_1:1;
            unsigned AINPOS6_2:1;
            unsigned AINPOS6_3:1;
            unsigned AINPOS6_4:1;
            unsigned :2;
            unsigned SETUP_SEL6_0:1;
            unsigned SETUP_SEL6_1:1;
            unsigned SETUP_SEL6_2:1;
        };
    };
} CH6BITS;
extern volatile CH6BITS CH6bits;

#define CH7 CH7
extern volatile unsigned int CH7;
__extension__ typedef struct tagCH7BITS {
    union {
        struct {
            unsigned AINNEG7_:5;
            unsigned AINPOS7_:5;
            unsigned :2;
            unsigned SETUP_SEL7_:3;
            unsigned CH_EN7:1;
        };
        struct {
            unsigned AINNEG7_0:1;
            unsigned AINNEG7_1:1;
            unsigned AINNEG7_2:1;
            unsigned AINNEG7_3:1;
            unsigned AINNEG7_4:1;
            unsigned AINPOS7_0:1;
            unsigned AINPOS7_1:1;
            unsigned AINPOS7_2:1;
            unsigned AINPOS7_3:1;
            unsigned AINPOS7_4:1;
            unsigned :2;
            unsigned SETUP_SEL7_0:1;
            unsigned SETUP_SEL7_1:1;
            unsigned SETUP_SEL7_2:1;
        };
    };
} CH7BITS;
extern volatile CH7BITS CH7bits;

#define CH8 CH8
extern volatile unsigned int CH8;
__extension__ typedef struct tagCH8BITS {
    union {
        struct {
            unsigned AINNEG8_:5;
            unsigned AINPOS8_:5;
            unsigned :2;
            unsigned SETUP_SEL8_:3;
            unsigned CH_EN8:1;
        };
        struct {
            unsigned AINNEG8_0:1;
            unsigned AINNEG8_1:1;
            unsigned AINNEG8_2:1;
            unsigned AINNEG8_3:1;
            unsigned AINNEG8_4:1;
            unsigned AINPOS8_0:1;
            unsigned AINPOS8_1:1;
            unsigned AINPOS8_2:1;
            unsigned AINPOS8_3:1;
            unsigned AINPOS8_4:1;
            unsigned :2;
            unsigned SETUP_SEL8_0:1;
            unsigned SETUP_SEL8_1:1;
            unsigned SETUP_SEL8_2:1;
        };
    };
} CH8BITS;
extern volatile CH8BITS CH8bits;

#define CH9 CH9
extern volatile unsigned int CH9;
__extension__ typedef struct tagCH9BITS {
    union {
        struct {
            unsigned AINNEG9_:5;
            unsigned AINPOS9_:5;
            unsigned :2;
            unsigned SETUP_SEL9_:3;
            unsigned CH_EN9:1;
        };
        struct {
            unsigned AINNEG9_0:1;
            unsigned AINNEG9_1:1;
            unsigned AINNEG9_2:1;
            unsigned AINNEG9_3:1;
            unsigned AINNEG9_4:1;
            unsigned AINPOS9_0:1;
            unsigned AINPOS9_1:1;
            unsigned AINPOS9_2:1;
            unsigned AINPOS9_3:1;
            unsigned AINPOS9_4:1;
            unsigned :2;
            unsigned SETUP_SEL9_0:1;
            unsigned SETUP_SEL9_1:1;
            unsigned SETUP_SEL9_2:1;
        };
    };
} CH9BITS;
extern volatile CH9BITS CH9bits;

#define CH10 CH10
extern volatile unsigned int CH10;
__extension__ typedef struct tagCH10BITS {
    union {
        struct {
            unsigned AINNEG10_:5;
            unsigned AINPOS10_:5;
            unsigned :2;
            unsigned SETUP_SEL10_:3;
            unsigned CH_EN10:1;
        };
        struct {
            unsigned AINNEG10_0:1;
            unsigned AINNEG10_1:1;
            unsigned AINNEG10_2:1;
            unsigned AINNEG10_3:1;
            unsigned AINNEG10_4:1;
            unsigned AINPOS10_0:1;
            unsigned AINPOS10_1:1;
            unsigned AINPOS10_2:1;
            unsigned AINPOS10_3:1;
            unsigned AINPOS10_4:1;
            unsigned :2;
            unsigned SETUP_SEL10_0:1;
            unsigned SETUP_SEL10_1:1;
            unsigned SETUP_SEL10_2:1;
        };
    };
} CH10BITS;
extern volatile CH10BITS CH10bits;

#define CH11 CH11
extern volatile unsigned int CH11;
__extension__ typedef struct tagCH11BITS {
    union {
        struct {
            unsigned AINNEG11_:5;
            unsigned AINPOS11_:5;
            unsigned :2;
            unsigned SETUP_SEL11_:3;
            unsigned CH_EN11:1;
        };
        struct {
            unsigned AINNEG11_0:1;
            unsigned AINNEG11_1:1;
            unsigned AINNEG11_2:1;
            unsigned AINNEG11_3:1;
            unsigned AINNEG11_4:1;
            unsigned AINPOS11_0:1;
            unsigned AINPOS11_1:1;
            unsigned AINPOS11_2:1;
            unsigned AINPOS11_3:1;
            unsigned AINPOS11_4:1;
            unsigned :2;
            unsigned SETUP_SEL11_0:1;
            unsigned SETUP_SEL11_1:1;
            unsigned SETUP_SEL11_2:1;
        };
    };
} CH11BITS;
extern volatile CH11BITS CH11bits;

#define CH12 CH12
extern volatile unsigned int CH12;
__extension__ typedef struct tagCH12BITS {
    union {
        struct {
            unsigned AINNEG12_:5;
            unsigned AINPOS12_:5;
            unsigned :2;
            unsigned SETUP_SEL12_:3;
            unsigned CH_EN12:1;
        };
        struct {
            unsigned AINNEG12_0:1;
            unsigned AINNEG12_1:1;
            unsigned AINNEG12_2:1;
            unsigned AINNEG12_3:1;
            unsigned AINNEG12_4:1;
            unsigned AINPOS12_0:1;
            unsigned AINPOS12_1:1;
            unsigned AINPOS12_2:1;
            unsigned AINPOS12_3:1;
            unsigned AINPOS12_4:1;
            unsigned :2;
            unsigned SETUP_SEL12_0:1;
            unsigned SETUP_SEL12_1:1;
            unsigned SETUP_SEL12_2:1;
        };
    };
} CH12BITS;
extern volatile CH12BITS CH12bits;

#define CH13 CH13
extern volatile unsigned int CH13;
__extension__ typedef struct tagCH13BITS {
    union {
        struct {
            unsigned AINNEG13_:5;
            unsigned AINPOS13_:5;
            unsigned :2;
            unsigned SETUP_SEL13_:3;
            unsigned CH_EN13:1;
        };
        struct {
            unsigned AINNEG13_0:1;
            unsigned AINNEG13_1:1;
            unsigned AINNEG13_2:1;
            unsigned AINNEG13_3:1;
            unsigned AINNEG13_4:1;
            unsigned AINPOS13_0:1;
            unsigned AINPOS13_1:1;
            unsigned AINPOS13_2:1;
            unsigned AINPOS13_3:1;
            unsigned AINPOS13_4:1;
            unsigned :2;
            unsigned SETUP_SEL13_0:1;
            unsigned SETUP_SEL13_1:1;
            unsigned SETUP_SEL13_2:1;
        };
    };
} CH13BITS;
extern volatile CH13BITS CH13bits;

#define CH14 CH14
extern volatile unsigned int CH14;
__extension__ typedef struct tagCH14BITS {
    union {
        struct {
            unsigned AINNEG14_:5;
            unsigned AINPOS14_:5;
            unsigned :2;
            unsigned SETUP_SEL14_:3;
            unsigned CH_EN14:1;
        };
        struct {
            unsigned AINNEG14_0:1;
            unsigned AINNEG14_1:1;
            unsigned AINNEG14_2:1;
            unsigned AINNEG14_3:1;
            unsigned AINNEG14_4:1;
            unsigned AINPOS14_0:1;
            unsigned AINPOS14_1:1;
            unsigned AINPOS14_2:1;
            unsigned AINPOS14_3:1;
            unsigned AINPOS14_4:1;
            unsigned :2;
            unsigned SETUP_SEL14_0:1;
            unsigned SETUP_SEL14_1:1;
            unsigned SETUP_SEL14_2:1;
        };
    };
} CH14BITS;
extern volatile CH14BITS CH14bits;

#define CH15 CH15
extern volatile unsigned int CH15;
__extension__ typedef struct tagCH15BITS {
    union {
        struct {
            unsigned AINNEG15_:5;
            unsigned AINPOS15_:5;
            unsigned :2;
            unsigned SETUP_SEL15_:3;
            unsigned CH_EN15:1;
        };
        struct {
            unsigned AINNEG15_0:1;
            unsigned AINNEG15_1:1;
            unsigned AINNEG15_2:1;
            unsigned AINNEG15_3:1;
            unsigned AINNEG15_4:1;
            unsigned AINPOS15_0:1;
            unsigned AINPOS15_1:1;
            unsigned AINPOS15_2:1;
            unsigned AINPOS15_3:1;
            unsigned AINPOS15_4:1;
            unsigned :2;
            unsigned SETUP_SEL15_0:1;
            unsigned SETUP_SEL15_1:1;
            unsigned SETUP_SEL15_2:1;
        };
    };
} CH15BITS;
extern volatile CH15BITS CH15bits;

#define SETUPCON0 SETUPCON0
extern volatile unsigned int SETUPCON0;
__extension__ typedef struct tagSETUPCON0BITS {
    union {
        struct {
            unsigned :4;
            unsigned REF_SEL0_:2;
            unsigned BRFCHOPMAX0:1;
            unsigned BURNOUT_EN0:1;
            unsigned AIN_BUF0_:2;
            unsigned REF_BUF0_:2;
            unsigned BI_UNIPOLAR0:1;
            unsigned :3;
        };
        struct {
            unsigned REF_SEL0_0:1;
            unsigned REF_SEL0_1:1;
            unsigned :2;
            unsigned AIN_BUF0_0:1;
            unsigned AIN_BUF0_1:1;
            unsigned REF_BUF0_0:1;
            unsigned REF_BUF1_1:1;
        };
    };
} SETUPCON0BITS;
extern volatile SETUPCON0BITS SETUPCON0bits;

#define SETUPCON1 SETUPCON1
extern volatile unsigned int SETUPCON1;
__extension__ typedef struct tagSETUPCON1BITS {
    union {
        struct {
            unsigned :4;
            unsigned REF_SEL1_:2;
            unsigned BRFCHOPMAX1:1;
            unsigned BURNOUT_EN1:1;
            unsigned AIN_BUF1_:2;
            unsigned REF_BUF1_:2;
            unsigned BI_UNIPOLAR1:1;
            unsigned :3;
        };
        struct {
            unsigned REF_SEL1_0:1;
            unsigned REF_SEL1_1:1;
            unsigned :2;
            unsigned AIN_BUF1_0:1;
            unsigned AIN_BUF1_1:1;
            unsigned REF_BUF1_0:1;
            unsigned REF_BUF1_1:1;
        };
    };
} SETUPCON1BITS;
extern volatile SETUPCON1BITS SETUPCON1bits;

#define SETUPCON2 SETUPCON2
extern volatile unsigned int SETUPCON2;
__extension__ typedef struct tagSETUPCON2BITS {
    union {
        struct {
            unsigned :4;
            unsigned REF_SEL2_:2;
            unsigned BRFCHOPMAX2:1;
            unsigned BURNOUT_EN2:1;
            unsigned AIN_BUF2_:2;
            unsigned REF_BUF2_:2;
            unsigned BI_UNIPOLAR2:1;
            unsigned :3;
        };
        struct {
            unsigned REF_SEL2_0:1;
            unsigned REF_SEL2_1:1;
            unsigned :2;
            unsigned AIN_BUF2_0:1;
            unsigned AIN_BUF2_1:1;
            unsigned REF_BUF2_0:1;
            unsigned REF_BUF2_1:1;
        };
    };
} SETUPCON2BITS;
extern volatile SETUPCON2BITS SETUPCON2bits;

#define SETUPCON3 SETUPCON3
extern volatile unsigned int SETUPCON3;
__extension__ typedef struct tagSETUPCON3BITS {
    union {
        struct {
            unsigned :4;
            unsigned REF_SEL3_:2;
            unsigned BRFCHOPMAX3:1;
            unsigned BURNOUT_EN3:1;
            unsigned AIN_BUF3_:2;
            unsigned REF_BUF3_:2;
            unsigned BI_UNIPOLAR3:1;
            unsigned :3;
        };
        struct {
            unsigned REF_SEL3_0:1;
            unsigned REF_SEL3_1:1;
            unsigned :2;
            unsigned AIN_BUF3_0:1;
            unsigned AIN_BUF3_1:1;
            unsigned REF_BUF3_0:1;
            unsigned REF_BUF3_1:1;
        };
    };
} SETUPCON3BITS;
extern volatile SETUPCON3BITS SETUPCON3bits;

#define SETUPCON4 SETUPCON4
extern volatile unsigned int SETUPCON4;
__extension__ typedef struct tagSETUPCON4BITS {
    union {
        struct {
            unsigned :4;
            unsigned REF_SEL4_:2;
            unsigned BRFCHOPMAX4:1;
            unsigned BURNOUT_EN4:1;
            unsigned AIN_BUF4_:2;
            unsigned REF_BUF4_:2;
            unsigned BI_UNIPOLAR4:1;
            unsigned :3;
        };
        struct {
            unsigned REF_SEL4_0:1;
            unsigned REF_SEL4_1:1;
            unsigned :2;
            unsigned AIN_BUF4_0:1;
            unsigned AIN_BUF4_1:1;
            unsigned REF_BUF4_0:1;
            unsigned REF_BUF4_1:1;
        };
    };
} SETUPCON4BITS;
extern volatile SETUPCON4BITS SETUPCON4bits;

#define SETUPCON5 SETUPCON5
extern volatile unsigned int SETUPCON5;
__extension__ typedef struct tagSETUPCON5BITS {
    union {
        struct {
            unsigned :4;
            unsigned REF_SEL5_:2;
            unsigned BRFCHOPMAX5:1;
            unsigned BURNOUT_EN5:1;
            unsigned AIN_BUF5_:2;
            unsigned REF_BUF5_:2;
            unsigned BI_UNIPOLAR5:1;
            unsigned :3;
        };
        struct {
            unsigned REF_SEL5_0:1;
            unsigned REF_SEL5_1:1;
            unsigned :2;
            unsigned AIN_BUF5_0:1;
            unsigned AIN_BUF5_1:1;
            unsigned REF_BUF5_0:1;
            unsigned REF_BUF5_1:1;
        };
    };
} SETUPCON5BITS;
extern volatile SETUPCON5BITS SETUPCON5bits;

#define SETUPCON6 SETUPCON6
extern volatile unsigned int SETUPCON6;
__extension__ typedef struct tagSETUPCON6BITS {
    union {
        struct {
            unsigned :4;
            unsigned REF_SEL6_:2;
            unsigned BRFCHOPMAX6:1;
            unsigned BURNOUT_EN6:1;
            unsigned AIN_BUF6_:2;
            unsigned REF_BUF6_:2;
            unsigned BI_UNIPOLAR6:1;
            unsigned :3;
        };
        struct {
            unsigned REF_SEL6_0:1;
            unsigned REF_SEL6_1:1;
            unsigned :2;
            unsigned AIN_BUF6_0:1;
            unsigned AIN_BUF6_1:1;
            unsigned REF_BUF6_0:1;
            unsigned REF_BUF6_1:1;
        };
    };
} SETUPCON6BITS;
extern volatile SETUPCON6BITS SETUPCON6bits;

#define SETUPCON7 SETUPCON7
extern volatile unsigned int SETUPCON7;
__extension__ typedef struct tagSETUPCON7BITS {
    union {
        struct {
            unsigned :4;
            unsigned REF_SEL7_:2;
            unsigned BRFCHOPMAX7:1;
            unsigned BURNOUT_EN7:1;
            unsigned AIN_BUF7_:2;
            unsigned REF_BUF7_:2;
            unsigned BI_UNIPOLAR7:1;
            unsigned :3;
        };
        struct {
            unsigned REF_SEL7_0:1;
            unsigned REF_SEL7_1:1;
            unsigned :2;
            unsigned AIN_BUF7_0:1;
            unsigned AIN_BUF7_1:1;
            unsigned REF_BUF7_0:1;
            unsigned REF_BUF7_1:1;
        };
    };
} SETUPCON7BITS;
extern volatile SETUPCON7BITS SETUPCON7bits;

#define FILTCON0 FILTCON0
extern volatile unsigned int FILTCON0;
__extension__ typedef struct tagFILTCON0BITS {
    union {
        struct {
            unsigned ODR0_:5;
            unsigned ORDER0_:2;
            unsigned :1;
            unsigned ENHFILT0_:3;
            unsigned ENHFILTEN0:1;
            unsigned :3;
            unsigned SINC3MAP0:1;
        };
        struct {
            unsigned ODR0_0:1;
            unsigned ODR0_1:1;
            unsigned ODR0_2:1;
            unsigned ODR0_3:1;
            unsigned ODR0_4:1;
            unsigned ORDER0_0:1;
            unsigned ORDER0_1:1;
            unsigned :1;
            unsigned ENHFILT0_0:1;
            unsigned ENHFILT0_1:1;
            unsigned ENHFILT0_2:1;
        };
    };
} FILTCON0BITS;
extern volatile FILTCON0BITS FILTCON0bits;

#define FILTCON1 FILTCON1
extern volatile unsigned int FILTCON1;
__extension__ typedef struct tagFILTCON1BITS {
    union {
        struct {
            unsigned ODR1_:5;
            unsigned ORDER1_:2;
            unsigned :1;
            unsigned ENHFILT1_:3;
            unsigned ENHFILTEN1:1;
            unsigned :3;
            unsigned SINC3MAP1:1;
        };
        struct {
            unsigned ODR1_0:1;
            unsigned ODR1_1:1;
            unsigned ODR1_2:1;
            unsigned ODR1_3:1;
            unsigned ODR1_4:1;
            unsigned ORDER1_0:1;
            unsigned ORDER1_1:1;
            unsigned :1;
            unsigned ENHFILT1_0:1;
            unsigned ENHFILT1_1:1;
            unsigned ENHFILT1_2:1;
        };
    };
} FILTCON1BITS;
extern volatile FILTCON1BITS FILTCON1bits;

#define FILTCON2 FILTCON2
extern volatile unsigned int FILTCON2;
__extension__ typedef struct tagFILTCON2BITS {
    union {
        struct {
            unsigned ODR2_:5;
            unsigned ORDER2_:2;
            unsigned :1;
            unsigned ENHFILT2_:3;
            unsigned ENHFILTEN2:1;
            unsigned :3;
            unsigned SINC3MAP2:1;
        };
        struct {
            unsigned ODR2_0:1;
            unsigned ODR2_1:1;
            unsigned ODR2_2:1;
            unsigned ODR2_3:1;
            unsigned ODR2_4:1;
            unsigned ORDER2_0:1;
            unsigned ORDER2_1:1;
            unsigned :1;
            unsigned ENHFILT2_0:1;
            unsigned ENHFILT2_1:1;
            unsigned ENHFILT2_2:1;
        };
    };
} FILTCON2BITS;
extern volatile FILTCON2BITS FILTCON2bits;

#define FILTCON3 FILTCON3
extern volatile unsigned int FILTCON3;
__extension__ typedef struct tagFILTCON3BITS {
    union {
        struct {
            unsigned ODR3_:5;
            unsigned ORDER3_:2;
            unsigned :1;
            unsigned ENHFILT3_:3;
            unsigned ENHFILTEN3:1;
            unsigned :3;
            unsigned SINC3MAP3:1;
        };
        struct {
            unsigned ODR3_0:1;
            unsigned ODR3_1:1;
            unsigned ODR3_2:1;
            unsigned ODR3_3:1;
            unsigned ODR3_4:1;
            unsigned ORDER3_0:1;
            unsigned ORDER3_1:1;
            unsigned :1;
            unsigned ENHFILT3_0:1;
            unsigned ENHFILT3_1:1;
            unsigned ENHFILT3_2:1;
        };
    };
} FILTCON3BITS;
extern volatile FILTCON3BITS FILTCON3bits;

#define FILTCON4 FILTCON4
extern volatile unsigned int FILTCON4;
__extension__ typedef struct tagFILTCON4BITS {
    union {
        struct {
            unsigned ODR4_:5;
            unsigned ORDER4_:2;
            unsigned :1;
            unsigned ENHFILT4_:3;
            unsigned ENHFILTEN4:1;
            unsigned :3;
            unsigned SINC3MAP4:1;
        };
        struct {
            unsigned ODR4_0:1;
            unsigned ODR4_1:1;
            unsigned ODR4_2:1;
            unsigned ODR4_3:1;
            unsigned ODR4_4:1;
            unsigned ORDER4_0:1;
            unsigned ORDER4_1:1;
            unsigned :1;
            unsigned ENHFILT4_0:1;
            unsigned ENHFILT4_1:1;
            unsigned ENHFILT4_2:1;
        };
    };
} FILTCON4BITS;
extern volatile FILTCON4BITS FILTCON4bits;

#define FILTCON5 FILTCON5
extern volatile unsigned int FILTCON5;
__extension__ typedef struct tagFILTCON5BITS {
    union {
        struct {
            unsigned ODR5_:5;
            unsigned ORDER5_:2;
            unsigned :1;
            unsigned ENHFILT5_:3;
            unsigned ENHFILTEN5:1;
            unsigned :3;
            unsigned SINC3MAP5:1;
        };
        struct {
            unsigned ODR5_0:1;
            unsigned ODR5_1:1;
            unsigned ODR5_2:1;
            unsigned ODR5_3:1;
            unsigned ODR5_4:1;
            unsigned ORDER5_0:1;
            unsigned ORDER5_1:1;
            unsigned :1;
            unsigned ENHFILT5_0:1;
            unsigned ENHFILT5_1:1;
            unsigned ENHFILT5_2:1;
        };
    };
} FILTCON5BITS;
extern volatile FILTCON5BITS FILTCON5bits;

#define FILTCON6 FILTCON6
extern volatile unsigned int FILTCON6;
__extension__ typedef struct tagFILTCON6BITS {
    union {
        struct {
            unsigned ODR6_:5;
            unsigned ORDER6_:2;
            unsigned :1;
            unsigned ENHFILT6_:3;
            unsigned ENHFILTEN6:1;
            unsigned :3;
            unsigned SINC3MAP6:1;
        };
        struct {
            unsigned ODR6_0:1;
            unsigned ODR6_1:1;
            unsigned ODR6_2:1;
            unsigned ODR6_3:1;
            unsigned ODR6_4:1;
            unsigned ORDER6_0:1;
            unsigned ORDER6_1:1;
            unsigned :1;
            unsigned ENHFILT6_0:1;
            unsigned ENHFILT6_1:1;
            unsigned ENHFILT6_2:1;
        };
    };
} FILTCON6BITS;
extern volatile FILTCON6BITS FILTCON6bits;

#define FILTCON7 FILTCON7
extern volatile unsigned int FILTCON7;
__extension__ typedef struct tagFILTCON7BITS {
    union {
        struct {
            unsigned ODR7_:5;
            unsigned ORDER7_:2;
            unsigned :1;
            unsigned ENHFILT7_:3;
            unsigned ENHFILTEN7:1;
            unsigned :3;
            unsigned SINC3MAP7:1;
        };
        struct {
            unsigned ODR7_0:1;
            unsigned ODR7_1:1;
            unsigned ODR7_2:1;
            unsigned ODR7_3:1;
            unsigned ODR7_4:1;
            unsigned ORDER7_0:1;
            unsigned ORDER7_1:1;
            unsigned :1;
            unsigned ENHFILT7_0:1;
            unsigned ENHFILT7_1:1;
            unsigned ENHFILT7_2:1;
        };
    };
} FILTCON7BITS;
extern volatile FILTCON7BITS FILTCON7bits;

#define OFFSET0 OFFSET0
extern volatile unsigned long OFFSET0;
typedef struct tagOFFSET0BITS {
    struct {
        unsigned long OFFSET:24;
    };
} OFFSET0BITS;
extern volatile OFFSET0BITS OFFSET0bits;

#define OFFSET1 OFFSET1
extern volatile unsigned long OFFSET1;
typedef struct tagOFFSET1BITS {
    struct {
        unsigned long OFFSET:24;
    };
} OFFSET1BITS;

#define OFFSET2 OFFSET2
extern volatile unsigned long OFFSET2;
typedef struct tagOFFSET2BITS {
    struct {
        unsigned long OFFSET:24;
    };
} OFFSET2BITS;
extern volatile OFFSET0BITS OFFSET0bits;

#define OFFSET3 OFFSET3
extern volatile unsigned long OFFSET3;
typedef struct tagOFFSET3BITS {
    struct {
        unsigned long OFFSET:24;
    };
} OFFSET3BITS;
extern volatile OFFSET0BITS OFFSET0bits;

#define OFFSET4 OFFSET4
extern volatile unsigned long OFFSET4;
typedef struct tagOFFSET4BITS {
    struct {
        unsigned long OFFSET:24;
    };
} OFFSET4BITS;
extern volatile OFFSET0BITS OFFSET0bits;

#define OFFSET5 OFFSET5
extern volatile unsigned long OFFSET5;
typedef struct tagOFFSET5BITS {
    struct {
        unsigned long OFFSET:24;
    };
} OFFSET5BITS;
extern volatile OFFSET0BITS OFFSET0bits;

#define OFFSET6 OFFSET6
extern volatile unsigned long OFFSET6;
typedef struct tagOFFSET6BITS {
    struct {
        unsigned long OFFSET:24;
    };
} OFFSET6BITS;
extern volatile OFFSET0BITS OFFSET0bits;

#define OFFSET7 OFFSET7
extern volatile unsigned long OFFSET7;
typedef struct tagOFFSET7BITS {
    struct {
        unsigned long OFFSET:24;
    };
} OFFSET7BITS;
extern volatile OFFSET0BITS OFFSET0bits;

#define GAIN0 GAIN0
extern volatile unsigned long GAIN0;
typedef struct tagGAIN0BITS {
    struct {
        unsigned long GAIN:24;
    };
} GAIN0BITS;
extern volatile GAIN0BITS GAIN0bits;

#define GAIN1 GAIN1
extern volatile unsigned long GAIN1;
typedef struct tagGAIN1BITS {
    struct {
        unsigned long GAIN:24;
    };
} GAIN1BITS;
extern volatile GAIN1BITS GAIN1bits;

#define GAIN2 GAIN2
extern volatile unsigned long GAIN2;
typedef struct tagGAIN2BITS {
    struct {
        unsigned long GAIN:24;
    };
} GAIN2BITS;
extern volatile GAIN2BITS GAIN2bits;

#define GAIN3 GAIN3
extern volatile unsigned long GAIN3;
typedef struct tagGAIN3BITS {
    struct {
        unsigned long GAIN:24;
    };
} GAIN3BITS;
extern volatile GAIN3BITS GAIN3bits;

#define GAIN4 GAIN4
extern volatile unsigned long GAIN4;
typedef struct tagGAIN4BITS {
    struct {
        unsigned long GAIN:24;
    };
} GAIN4BITS;
extern volatile GAIN4BITS GAIN4bits;

#define GAIN5 GAIN5
extern volatile unsigned long GAIN5;
typedef struct tagGAIN5BITS {
    struct {
        unsigned long GAIN:24;
    };
} GAIN5BITS;
extern volatile GAIN5BITS GAIN5bits;

#define GAIN6 GAIN6
extern volatile unsigned long GAIN6;
typedef struct tagGAIN6BITS {
    struct {
        unsigned long GAIN:24;
    };
} GAIN6BITS;
extern volatile GAIN6BITS GAIN6bits;

#define GAIN7 GAIN7
extern volatile unsigned long GAIN7;
typedef struct tagGAIN7BITS {
    struct {
        unsigned long GAIN:24;
    };
} GAIN7BITS;
extern volatile GAIN7BITS GAIN7bits;

/* COMMS */
#define _WEN COMMSbits.WEN
#define _RW COMMSbits.RW
#define _ADRA0 COMMSbits.RA0
#define _ADRA1 COMMSbits.RA1
#define _ADRA2 COMMSbits.RA2
#define _ADRA3 COMMSbits.RA3
#define _ADRA4 COMMSbits.RA4
#define _ADRA5 COMMSbits.RA5

/* STATUS */
#define _RDY STATUSbits.RDY
#define _ADC_ERROR STATUSbits.ADC_ERROR
#define _CRC_ERROR STATUSbits.CRC_ERROR
#define _REG_ERROR STATUSbits.REG_ERROR
#define _CHAN0 STATUSbits.CHAN0
#define _CHAN1 STATUSbits.CHAN1
#define _CHAN2 STATUSbits.CHAN2
#define _CHAN3 STATUSbits.CHAN3

/* ADCMODE */
#define _REF_EN ADCMODEbits.REF_EN
#define _SING_CYC ADCMODEbits.SING_CYC
#define _DELAY0 ADCMODEbits.DELAY0
#define _DELAY1 ADCMODEbits.DELAY1
#define _DELAY2 ADCMODEbits.DELAY2
#define _MODE0 ADCMODEbits.MODE0
#define _MODE1 ADCMODEbits.MODE1
#define _MODE2 ADCMODEbits.MODE2
#define _CLOCKSEL0 ADCMODEbits.CLOCKSEL0
#define _CLOCKSEL1 ADCMODEbits.CLOCKSEL1

/* IFMODE */
#define _ALT_SYNC IFMODEbits.ALT_SYNC
#define _IOSTRENGTH IFMODEbits.IOSTRENGTH
#define _HIDE_DELAY IFMODEbits.HIDE_DELAY
#define _DOUT_RESET IFMODEbits.DOUT_RESET
#define _CONTREAD IFMODEbits.CONTREAD
#define _DATA_STAT IFMODEbits.DATA_STAT
#define _REG_CHECK IFMODEbits.REG_CHECK
#define _CRC_EN IFMODEbits.CRC_EN
#define _WL16 IFMODEbits.WL16

/* REGCHECK */
#define _REGISTER_CHECK REGCHECKbits.REGISTER_CHECK

/* DATA */
#define _ADDATA DATAbits.DATA

/* GPIOCON */
#define _PDSW GPIOCONbits.PDSW
#define _OP_EN2_3 GPIOCONbits.OP_EN2_3
#define _MUX_IO GPIOCONbits.MUX_IO
#define _SYNC_EN GPIOCONbits.SYNC_EN
#define _ERR_EN0 GPIOCONbits.ERR_EN0
#define _ERR_EN1 GPIOCONbits.ERR_EN1
#define _ERR_DAT GPIOCONbits.ERR_DAT
#define _GP_DATA3 GPIOCONbits.GP_DATA3
#define _GP_DATA2 GPIOCONbits.GP_DATA2
#define _GP_DATA1 GPIOCONbits.GP_DATA1
#define _GP_DATA0 GPIOCONbits.GP_DATA0
#define _IP_EN0 GPIOCONbits.IP_EN0
#define _IP_EN1 GPIOCONbits.IP_EN1
#define _OP_EN0 GPIOCONbits.OP_EN0
#define _OP_EN1 GPIOCONbits.OP_EN1

/* ID */
#define _ID IDbits.ID

/* CH0 */
#define _CH_EN

#define _STATUS_REG 0x00
#define _ADCMODE_REG 0x01
#define _IFMODE_REG 0x02
#define _REGCHECK_REG 0x03
#define _DATA_REG 0x04
#define _GPIOCON_REG 0x06
#define _ID_REG 0x07
#define _CH0_REG 0x10
#define _CH1_REG 0x11
#define _CH2_REG 0x12
#define _CH3_REG 0x13
#define _CH4_REG 0x14
#define _CH5_REG 0x15
#define _CH6_REG 0x16
#define _CH7_REG 0x17
#define _CH8_REG 0x18
#define _CH9_REG 0x19
#define _CH10_REG 0x1A
#define _CH11_REG 0x1B
#define _CH12_REG 0x1C
#define _CH13_REG 0x1D
#define _CH14_REG 0x1E
#define _CH15_REG 0x1F
#define _SETUPCON0_REG 0x20
#define _SETUPCON1_REG 0x21
#define _SETUPCON2_REG 0x22
#define _SETUPCON3_REG 0x23
#define _SETUPCON4_REG 0x24
#define _SETUPCON5_REG 0x25
#define _SETUPCON6_REG 0x26
#define _SETUPCON7_REG 0x27
#define _FILTCON0_REG 0x28
#define _FILTCON1_REG 0x29
#define _FILTCON2_REG 0x2A
#define _FILTCON3_REG 0x2B
#define _FILTCON4_REG 0x2C
#define _FILTCON5_REG 0x2D
#define _FILTCON6_REG 0x2E
#define _FILTCON7_REG 0x2F
#define _OFFSET0_REG 0x30
#define _OFFSET1_REG 0x31
#define _OFFSET2_REG 0x32
#define _OFFSET3_REG 0x33
#define _OFFSET4_REG 0x34
#define _OFFSET5_REG 0x35
#define _OFFSET6_REG 0x36
#define _OFFSET7_REG 0x37
#define _GAIN0_REG 0x38
#define _GAIN1_REG 0x39
#define _GAIN2_REG 0x3A
#define _GAIN3_REG 0x3B
#define _GAIN4_REG 0x3C
#define _GAIN5_REG 0x3D
#define _GAIN6_REG 0x3E
#define _GAIN7_REG 0x3F
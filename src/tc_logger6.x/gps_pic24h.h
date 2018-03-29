/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.


#define _GPS_PIC24H_H_


#include "integer.h"

// Public GPS chip interface
DWORD get_gps(void);
void gps_init(void);
void gps_reset(void);
void gps_timerproc(void);
void gps_shutdown(void);
void gps_fill_buffer(char* const gps_rx_buff, UINT num_bytes);

void gps_parse_zda(const char* const gps_rx_buff, const UINT BUFFER_SIZE, BYTE* const utc_time);
void gps_parse_rmc(const char* const gps_rx_buff, const UINT BUFFER_SIZE, BYTE* const utc_time, BYTE* const gprmc_validity, BYTE* const lattitude,
                    BYTE* const north_or_south, BYTE* const longitude, BYTE* const east_or_west, BYTE* const ut_date);
void gps_parse_gga(const char* const gps_rx_buff, const UINT BUFFER_SIZE, BYTE* const utc_time,  BYTE* const lattitude, BYTE* const north_or_south,
                    BYTE* const longitude, BYTE* const east_or_west, BYTE* const fix_validity, BYTE* const satellites);
void gps_parse_gsv(const char* const gps_rx_buff, const UINT BUFFER_SIZE, BYTE* const satellites);

void send_mid103(void);
void send_mid125(void);
void send_mid117(void);
void send_mid100(void);
void send_mid129(void);
void send_gsv_off(void);
void send_gsv_on(void);
void send_gsa_off(void);
void send_gsa_on(void);
void send_zda_on(void);
void send_id132(void);
void send_mid105(void);

/*
 * File:   logger_pic24h.c
 * Author: cwold
 *
 * Created on February 13, 2017, 7:06 AM
 */



#include "integer.h"
#include "logger_pic24h.h"
#include "stdlib.h"







void logger_init(const char* const Buff, const UINT BUFFER_SIZE, struct logger_setup *pls)
{
    // Locate and store sample interval and channel measurand
    UINT comma_count = 0;
    INT i = 0, j = 7;
    INT value_start = -1;
    INT value_end = 0;
    char st[8] = "00000000";
    
//    INT temp[8] = {1,1,1,1,1,1,1,1};
  
    
    while ((Buff[i] != 0x2C) && (i < BUFFER_SIZE))
    {
       
        if (Buff[i] == 0x2C)
        {
            i++;
            comma_count++;
        }
        else
        {
            i++;
        }
    }
    value_end = i-1;
    for (i = value_end; i > value_start; i--)
    { 
       
            st[j] = Buff[i];
            j--;
    }
   
    pls->ms = atoi(st);
    value_start = value_end + 2;
    i = value_end + 2;
    j = 0;
    while ((i < BUFFER_SIZE) && (j < 8))
    {
    while (Buff[i] != 0x2C) 
    {
       
       i++;
    }
    
    value_start = i - 1;
    
    pls->measurand[j] = (BYTE) (Buff[value_start] - 0x30);
   
    j++;
    i++;
}
 
}
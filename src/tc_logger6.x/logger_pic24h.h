

/* 
 * File:   
 * Author: cew
 * Comments:
 * Revision history: 
 */


#include "integer.h"



struct logger_setup{
    DWORD ms;
    UINT measurand[8];
    
};


void logger_init(const char* const Buff, const UINT BUFFER_SIZE, struct logger_setup *pls);
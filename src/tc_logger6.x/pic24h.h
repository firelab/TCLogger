#define FCY		40000000UL

#define _DI()		__asm__ volatile("disi #0x3FFF") /* interrupts disabled for this # of counts */
#define _EI()		__asm__ volatile("disi #0")      /* interrupts enabled */
#define _pwrdwn()   __asm__ volatile("pwrsav #0")    /* equivalent to sleep() */
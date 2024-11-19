#ifndef _EXTIMER_H_
#define _EXTIMER_H_
#include <stddef.h>
#include <stdlib.h>
#define uint8_t unsigned char
#define uint16_t unsigned int
#define uint32_t unsigned long


#define table_max
#define timer_table_length  4


typedef void (*onTimeout_f)(void);



/**
 * @brief exTimer object
 * 
 */
typedef struct exTimer
{
    onTimeout_f timer_callback;
    uint16_t interval;    
    uint8_t Enable;
    uint32_t TickPrev;
    
}exTimer_t;


void  exTimerManagerCreate(void);
void exTimer_add(exTimer_t *newTimer);
void pollingExTimer();
void exTimer_tick_inc(void);

#endif //_EXTIMER_H_


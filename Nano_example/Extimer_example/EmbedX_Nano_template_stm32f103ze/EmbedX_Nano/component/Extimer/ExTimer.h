#ifndef _EXTIMER_H_
#define _EXTIMER_H_

#include "stdint.h"

typedef void (*onTimeout_f)(void* data);
/**
 * @brief exTimerListNode
 * 
 */
typedef struct exTimerNode{
    struct exTimer *timer;
    struct exTimerNode * next;
}exTimerNode_t;

/**
 * @brief exTimer manager  
 * @note list of timers
 * 
 */
typedef struct exTimerManager
{
    exTimerNode_t * head;
    exTimerNode_t * tail;
}exTimerManager_t;


/**
 * @brief exTimerAttributes
 * 
 */
typedef struct exTimerAttr
{
    const uint8_t *name;
    uint32_t interval;    
    uint8_t Enable;
}exTimerAttr_t;


/**
 * @brief exTimer object
 * 
 */
typedef struct exTimer
{
    onTimeout_f timer_callback;
    exTimerAttr_t *attr;
    uint32_t TickPrev;
    
}exTimer_t;


exTimerManager_t  exTimerManagerCreate(void);
void exTimer_init(exTimer_t *extimer,exTimerAttr_t *attr,onTimeout_f callback);
void exTimer_add(exTimerManager_t* manager,exTimer_t *newTimer);
void exTimer_remove(exTimerManager_t* manager,const uint8_t* name);
void enableExTimer(exTimerManager_t* manager, const uint8_t* name);
void disableExTimer(exTimerManager_t* manager, const uint8_t* name);
void setExTimerInterval(exTimerManager_t* manager, const uint8_t* name,uint32_t set_interval);
void pollingExTimer(exTimerManager_t* manager);
void exTimer_tick_inc(void);

#endif //_EXTIMER_H_


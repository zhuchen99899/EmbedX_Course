#ifndef _EXTIMER_H_
#define _EXTIMER_H_

#include "stdint.h"
#include "stdbool.h"
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
    uint8_t isTimeOut;
}exTimerAttr_t;


/**
 * @brief exTimer object
 * 
 */
typedef struct exTimer
{
    void (*onTimeout)(void* data);
    exTimerAttr_t attr;
    uint32_t TickPrev;
    uint32_t TickCost;
    uint32_t TickError;
}exTimer_t;


exTimerManager_t  exTimerManagerCreate(void)
{
exTimerManager_t manager;
manager.head=NULL;
manager.tail=NULL;
return manager;
}

/**
 * @brief add new timer to the manager 
 * 
 * @param manager List
 * @param newTimer new timer
 * @note add to the cycle list
 */
void exTimer_add(exTimerManager_t* manager,exTimer_t *newTimer);
void removeTimerNode(exTimerManager_t* manager, exTimer_t* timerToRemove);
exTimerNode_t* findExTimer(exTimerManager_t* manager, const uint8_t* name);
void enableExTimer(exTimerManager_t* manager, const uint8_t* name);
void disableExTimer(exTimerManager_t* manager, const uint8_t* name);
bool exTimerIsTimeOut(exTimerManager_t* manager, const uint8_t* name);
void setExTimerInterval(exTimerManager_t* manager, const uint8_t* name,uint32_t set_interval);
void pollingExTimer(exTimerManager_t* manager);



#endif //_EXTIMER_H_


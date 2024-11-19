/**
 * @file exTimer.h
 * @author ZC (387646983@qq.com)
 * @brief 
 * @version 0.1
 * @date 2024-11-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef _EXTIMER_H_
#define _EXTIMER_H_



#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"



typedef void (*onTimeout_f)(void* data);
/**
 * @brief exTimerListNode
 * 
 */
typedef struct exTimerNode{
    struct exTimer *timer; ///< Extimer
    struct exTimerNode * next; ///< list node
}exTimerNode_t;

/**
 * @brief exTimer manager  
 * @note cycle list of timers
 * 
 */
typedef struct exTimerManager
{
    exTimerNode_t * head; ///< the head list node or list
    exTimerNode_t * tail; ///< the tail list node or list
}exTimerManager_t;


/**
 * @brief exTimerAttributes
 * @note tiemres attributes
 */
typedef struct exTimerAttr
{
    const uint8_t *name; ///< the name of the ExTimer 
    uint32_t interval;   ///< the interval of the Extimer
    uint8_t Enable;      ///< enable/or disable the Extimer 0:disable 1:enable
}exTimerAttr_t;


/**
 * @brief exTimer object
 * 
 */
typedef struct exTimer
{
    onTimeout_f timer_callback; ///< callback function of Extimer 
    exTimerAttr_t *attr;    ///< Extimer attribute
    uint32_t TickPrev;      ///< previous recording or Extimer
}exTimer_t;


exTimerManager_t  exTimerManagerCreate(void);
void exTimer_init(exTimer_t *extimer,exTimerAttr_t *attr,onTimeout_f callback);
void exTimer_add(exTimerManager_t* manager,exTimer_t *newTimer);
void exTimer_remove(exTimerManager_t* manager,const uint8_t* name);
void enableExTimer(exTimerManager_t* manager, const uint8_t* name);
void disableExTimer(exTimerManager_t* manager, const uint8_t* name);
void setExTimerInterval(exTimerManager_t* manager, const uint8_t* name,uint32_t set_interval);
void pollingExTimer(exTimerManager_t* manager);
void exTimer_tick_inc(uint32_t tick_base);


#ifdef __cplusplus
}
#endif

#endif //_EXTIMER_H_


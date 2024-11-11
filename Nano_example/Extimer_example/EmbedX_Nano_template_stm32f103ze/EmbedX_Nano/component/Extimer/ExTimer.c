/**
 * @file exTimer.c
 * @author ZC (387646983@qq.com)
 * @brief 
 * @version 0.1
 * @date 2024-11-04
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "exTimer.h"
#include "stdlib.h"
#include "stddef.h"
#include "string.h"




// ExTimer system tick
static volatile uint32_t sysTickUptime = 0;

/**
 * @brief  create a  ExTimer list manager (cycle list)
 * 
 * @return exTimerManager_t 
 */
exTimerManager_t  exTimerManagerCreate(void)
{
exTimerManager_t manager;
manager.head=NULL;
manager.tail=NULL;
return manager;
}

/**
 * @brief init exTimer 
 * 
 * @param extimer exTimer structure
 * @param attr  exTimer attribute
 * @param callback  when exTimer is triggered ,invoke callback function
 */
void exTimer_init(exTimer_t *extimer,exTimerAttr_t *attr,onTimeout_f callback)
{
    extimer->attr = attr;
    extimer->timer_callback = callback;
    extimer->TickPrev = 0;
}


/**
 * @brief add new timer to the manager 
 * 
 * @param manager the ExTimer list manager
 * @param newTimer new timer
 * @note add to the cycle list
 */
void exTimer_add(exTimerManager_t* manager,exTimer_t *newTimer)
{
    
    exTimerNode_t* newNode = (exTimerNode_t*)malloc(sizeof(exTimerNode_t));
    newNode->timer = newTimer;
    newNode->next = NULL;

    if (manager->head == NULL) {
        // if list is null, new node become head and circulate itself，
        newNode->next = newNode;
        manager->head = newNode;
        manager->tail = newNode;
    } else {
        // add newnode to the tail,and refresh tail point
        manager->tail->next = newNode;
        newNode->next = manager->head;  // cycle
        manager->tail = newNode;
    }
}

/**
 * @brief find the name of the timer in the List 
 * 
 * @param manager  the ExTimer list manager
 * @param name  ExTimer name
 * @return exTimerNode_t*  extimer list node
 */
static exTimerNode_t* findExTimer(exTimerManager_t* manager, const uint8_t* name) {
    if (manager->head == NULL) return NULL;

    exTimerNode_t* current = manager->head;
    do {
        if (strcmp((const char*)current->timer->attr->name, (const char*)name) == 0) {
            return current;
        }
        current = current->next;
    } while (current != manager->head);

    return NULL; // can't find matching exTimerNode
}

/**
 * @brief remove the Extimer from the list
 * 
 * @param manager  the ExTimer list manager
 * @param name  ExTimer Name 
 */
void exTimer_remove(exTimerManager_t* manager,const uint8_t* name) {
    if (manager->head == NULL) return; // extimer is NULL
    exTimerNode_t* timerNodeToRemove = findExTimer(manager, name);
    exTimerNode_t* current = manager->head;

    exTimerNode_t* prev = manager->tail;
    do {
        if (current == timerNodeToRemove) {
            if (current == manager->head && current == manager->tail) {
                // the List only have one node 
                manager->head = NULL;
                manager->tail = NULL;
            } else {
                if (current == manager->head) {
                    // remove the head node 
                    manager->head = current->next;
                    manager->tail->next = manager->head;
                } else {
                    // remove the nonHead Nodes
                    prev->next = current->next;
                    if (current == manager->tail) {
                        // if remove the tail node , fresh new tail node
                        manager->tail = prev;
                    }
                }
            }
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    } while (current != manager->head);
}


/**
 * @brief  enable the Extimer from the list
 * 
 * @param manager the ExTimer list manager
 * @param name ExTimer Name 
 */
void enableExTimer(exTimerManager_t* manager, const uint8_t* name) {
    exTimerNode_t* timerNode = findExTimer(manager, name);
    if (timerNode != NULL) {
        timerNode->timer->attr->Enable = 1; // enable ExTimer
    }
}

/**
 * @brief disable the Extimer from the list
 * 
 * @param manager the ExTimer list manager
 * @param name ExTimer Name 
 */
void disableExTimer(exTimerManager_t* manager, const uint8_t* name) {
    exTimerNode_t* timerNode = findExTimer(manager, name);
    if (timerNode != NULL) {
        timerNode->timer->attr->Enable = 0; // disable ExTimer
    }
}


/**
 * @brief Set the ExTimer timing Interval
 * 
 * @param manager  the ExTimer list manager
 * @param name ExTimer Name 
 * @param set_interval timing Interval to set
 */
void setExTimerInterval(exTimerManager_t* manager, const uint8_t* name,uint32_t set_interval)
{
    exTimerNode_t* timerNode = findExTimer(manager, name);
    if (timerNode != NULL) {
        timerNode->timer->attr->interval= set_interval; 
    }
}

/**
  * @brief  caculate time tick deviation 
  * @param  nowTick: now time tick
  * @param  prevTick:previous time tick
  * @retval time tick deviation 
  */
static uint32_t GetTickElaps(uint32_t nowTick, uint32_t prevTick)
{
    uint32_t actTime = nowTick;

    if(actTime >= prevTick)
    { //don't overflow
        prevTick = actTime - prevTick;
    }
    else
    {  //overflow
        prevTick = /*UINT32_MAX*/0xFFFFFFFF - prevTick + 1;
        prevTick += actTime;
    }

    return prevTick;
}
/**
 * @brief  Polling all Extimer for Extimerlist
 * 
 * @param manager  the ExTimer list manager
 */
void pollingExTimer(exTimerManager_t* manager) {
    if (manager->head == NULL) return;

    exTimerNode_t* current = manager->head;
    do {
        if (current->timer->attr->Enable) {
            // timer must be enabled
            uint32_t elapsTime = GetTickElaps(sysTickUptime,current->timer->TickPrev);
            if ( elapsTime>= current->timer->attr->interval) {
                    //The setting interval time has come

                // fresh timer previous time ticks
                current->timer->TickPrev = sysTickUptime;
                
                // trigger timer callback
                current->timer->timer_callback((void *)current->timer);
     
            }
        }
        current = current->next;
    } while (current != manager->head);
}


/**
 * @brief interface for Extimer 
 * 
 * @param tick_base  Extiemr tick base
 */
void exTimer_tick_inc(uint32_t tick_base){

sysTickUptime=sysTickUptime+tick_base;

}
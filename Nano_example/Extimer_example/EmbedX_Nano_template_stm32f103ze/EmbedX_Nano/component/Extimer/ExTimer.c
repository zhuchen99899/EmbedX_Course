/**
 * @file ExTimer.c
 * @author ZC (387646983@qq.com)
 * @brief 
 * @version 0.1
 * @date 2024-11-04
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "ExTimer.h"
#include "stdlib.h"
#include "stddef.h"
#include "string.h"

// ExTimer system tick
static volatile uint32_t sysTickUptime = 0;

/**
 * @brief  create a timer manager (cycle list)
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

void exTimer_init(exTimer_t *extimer,exTimerAttr_t *attr,onTimeout_f callback)
{
    extimer->attr = attr;
    extimer->timer_callback = callback;
    extimer->TickPrev = 0;
}


/**
 * @brief add new timer to the manager 
 * 
 * @param manager List
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

static exTimerNode_t* findExTimer(exTimerManager_t* manager, const uint8_t* name) {
    if (manager->head == NULL) return NULL;

    exTimerNode_t* current = manager->head;
    do {
        if (strcmp((const char*)current->timer->attr->name, (const char*)name) == 0) {
            return current;
        }
        current = current->next;
    } while (current != manager->head);

    return NULL; // 未找到匹配的定时器
}

void exTimer_remove(exTimerManager_t* manager,const uint8_t* name) {
    if (manager->head == NULL) return; // 链表为空
    exTimerNode_t* timerNodeToRemove = findExTimer(manager, name);
    exTimerNode_t* current = manager->head;

    exTimerNode_t* prev = manager->tail;

    do {
        if (current == timerNodeToRemove) {
            if (current == manager->head && current == manager->tail) {
                // 链表只有一个节点的情况
                manager->head = NULL;
                manager->tail = NULL;
            } else {
                if (current == manager->head) {
                    // 删除头节点的情况
                    manager->head = current->next;
                    manager->tail->next = manager->head;
                } else {
                    // 删除非头节点的情况
                    prev->next = current->next;
                    if (current == manager->tail) {
                        // 如果删除的是尾节点，更新尾指针
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



void enableExTimer(exTimerManager_t* manager, const uint8_t* name) {
    exTimerNode_t* timerNode = findExTimer(manager, name);
    if (timerNode != NULL) {
        timerNode->timer->attr->Enable = 1; // 设置使能标志
    }
}

void disableExTimer(exTimerManager_t* manager, const uint8_t* name) {
    exTimerNode_t* timerNode = findExTimer(manager, name);
    if (timerNode != NULL) {
        timerNode->timer->attr->Enable = 0; // 取消使能标志
    }
}


void setExTimerInterval(exTimerManager_t* manager, const uint8_t* name,uint32_t set_interval)
{
    exTimerNode_t* timerNode = findExTimer(manager, name);
    if (timerNode != NULL) {
        timerNode->timer->attr->interval= set_interval; 
    }
}

/**
  * @brief  时间差判定
  * @param  nowTick:当前时间
  * @param  prevTick:上一个时间
  * @retval 时间差
  */
static uint32_t GetTickElaps(uint32_t nowTick, uint32_t prevTick)
{
    uint32_t actTime = nowTick;

    if(actTime >= prevTick)
    { //没有溢出
        prevTick = actTime - prevTick;
    }
    else
    {  //产生溢出
        prevTick = /*UINT32_MAX*/0xFFFFFFFF - prevTick + 1;
        prevTick += actTime;
    }

    return prevTick;
}

void pollingExTimer(exTimerManager_t* manager) {
    if (manager->head == NULL) return;

    exTimerNode_t* current = manager->head;
    do {
        if (current->timer->attr->Enable) {
            // timer must be enabled
            uint32_t elapsTime = GetTickElaps(sysTickUptime,current->timer->TickPrev);
            if ( elapsTime>= current->timer->attr->interval) {

                // 更新定时器的上一次触发时间
                current->timer->TickPrev = sysTickUptime;
                // 触发回调函数

                current->timer->timer_callback((void *)current->timer);
     
            }
        }
        current = current->next;
    } while (current != manager->head);
}

void exTimer_tick_inc(void){

sysTickUptime=sysTickUptime+1;

}
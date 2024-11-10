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

void removeTimerNode(exTimerManager_t* manager, exTimer_t* timerToRemove) {
    if (manager->head == NULL) return; // 链表为空

    exTimerNode_t* current = manager->head;

    exTimerNode_t* prev = manager->tail;

    do {
        if (current->timer == timerToRemove) {
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


exTimerNode_t* findExTimer(exTimerManager_t* manager, const uint8_t* name) {
    if (manager->head == NULL) return NULL;

    exTimerNode_t* current = manager->head;
    do {
        if (strcmp((const char*)current->timer->attr.name, (const char*)name) == 0) {
            return current;
        }
        current = current->next;
    } while (current != manager->head);

    return NULL; // 未找到匹配的定时器
}

void enableExTimer(exTimerManager_t* manager, const uint8_t* name) {
    exTimerNode_t* timerNode = findExTimer(manager, name);
    if (timerNode != NULL) {
        timerNode->timer->attr.Enable = 1; // 设置使能标志
    }
}

void disableExTimer(exTimerManager_t* manager, const uint8_t* name) {
    exTimerNode_t* timerNode = findExTimer(manager, name);
    if (timerNode != NULL) {
        timerNode->timer->attr.Enable = 0; // 取消使能标志
    }
}


void setExTimerInterval(exTimerManager_t* manager, const uint8_t* name,uint32_t set_interval)
{
    exTimerNode_t* timerNode = findExTimer(manager, name);
    if (timerNode != NULL) {
        timerNode->timer->attr.interval= set_interval; // 取消一次性定时器
    }

}

bool exTimerIsTimeOut(exTimerManager_t* manager, const uint8_t* name)
{
    bool ret=false;
    exTimerNode_t* timerNode = findExTimer(manager, name);
    if (timerNode != NULL) {
        ret = timerNode->timer->attr.isTimeOut;
        return ret; // time out state

        timerNode->timer->attr.isTimeOut; //reset timer state
    }
    return false;
}


void pollingExTimer(exTimerManager_t* manager) {
    if (manager->head == NULL) return;

    exTimerNode_t* current = manager->head;
    do {
        if (current->timer->attr.Enable) {
            // 检查是否超时
            //if ((getCurrentTick() - current->timer->TickPrev) >= current->timer->attr.interval) {
            if (current->timer->TickCost <= (getCurrentTick() - current->timer->TickPrev)) {
                // 设置超时标志位
                current->timer->attr.isTimeOut = true; //if not inquire ，the state will not clear

                // 触发回调函数
                if (current->timer->onTimeout) {
                    current->timer->onTimeout(current->timer);
                }

                    // 更新定时器的上一次触发时间
                    current->timer->TickPrev = getCurrentTick();

            }
        }
        current = current->next;
    } while (current != manager->head);
}

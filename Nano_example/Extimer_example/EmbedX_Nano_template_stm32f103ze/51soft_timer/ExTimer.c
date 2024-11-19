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



static exTimer_t exTimer_table[timer_table_length];

// ExTimer system tick
static uint32_t sysTickUptime = 0;

/**
 * @brief  create a timer manager (cycle list)
 * 
 * @return exTimerManager_t 
 */
void  exTimerManagerCreate(void)
{
		uint8_t i;
    for (i = 0; i < timer_table_length; i++) {
				exTimer_table[i].interval=0;
				exTimer_table[i].Enable=0;
        exTimer_table[i].timer_callback = NULL;
        exTimer_table[i].TickPrev = NULL;
    }
}


void exTimer_add(exTimer_t *newTimer)
{		uint8_t i;
    for ( i = 0; i < timer_table_length; i++) {
        if (exTimer_table[i].timer_callback == NULL) {
            exTimer_table[i] = *newTimer;
            return;
        }
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

void pollingExTimer(void) {
    
		uint8_t i;
    exTimer_t * current = NULL;

				for (i = 0; i < timer_table_length; i++) {
					 current=&exTimer_table[i];
        if (current->Enable) {
            // timer must be enabled
            uint32_t elapsTime = GetTickElaps(sysTickUptime,current->TickPrev);
            if ( elapsTime>= current->interval) {

                // 更新定时器的上一次触发时间
                current->TickPrev = sysTickUptime;
                // 触发回调函数
							current->timer_callback();
                if (current->timer_callback != NULL) {
                    
                }
     
            }
        }
			}
}

void exTimer_tick_inc(void){

sysTickUptime=sysTickUptime+1;

}
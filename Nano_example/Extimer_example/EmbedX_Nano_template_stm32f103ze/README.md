# EmbedX_Nano实现的一个软定时器

- 仅教学使用

实现一个软件定时器，最小实现可见根目录[51soft_timer](./51soft_timer)

# 51soft_Timer 

可在STC89C52单片机上包含头文件运行

## 使用方法：

- 加入头文

```c
#include "Extimer.h"
```

- 在1ms定时器中加入 `exTimer_tick_inc` 为软件定时器

```c
void xxx() interrupt x
{
	TL0 = xxxx; //设置定时器初值
    TH0 = xxxx; //设置定时器初值
    exTimer_tick_inc();

}
```

- 初始化`exTimer` 管理器

```c
exTimerManagerCreate();
```
- 创建一个`timer0` 定时器

```c
exTimer_t timer0;
```
- 设置定时间隔

```
timer0.interval=1000;//定时间隔
timer0.Enable=1;//启用定时器
timer0.timer_callback=LED_blinky;//定时器完成后执行的函数
```

- 将定时器加入到`exTimer`管理器

```
exTimer_add(&timer0);
```

在主循环中调用`pollingExtimer`

```
while(1)
{
	pollingExTimer();

}
```

## 51soft_Timer 实验现象

- P2_0 口LED灯闪烁1s闪烁一次

- P2_1口LED灯闪烁2s闪烁一次

- P2_2口LED灯闪烁3s闪烁一次

# EmbedX_Nano ExTimer的实验现象
- LED1 1s闪烁一次
- LED2 2s闪烁一次

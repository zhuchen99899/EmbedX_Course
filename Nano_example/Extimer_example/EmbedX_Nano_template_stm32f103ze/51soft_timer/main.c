#include "reg52.h"
#include "Extimer.h"
sbit P2_0=P2^0;
sbit P2_1=P2^1;
sbit P2_2=P2^2;



void timer0_Int() interrupt 1 //1ms_int
{
	TL0 = 0x66;		
	TH0 = 0xFC;		
	exTimer_tick_inc();
}




void Timer0Init(void)		//1ms @11.0592MHz
{
	TMOD &= 0xF0;		
	TMOD |= 0x01;		
	TL0 = 0x66;		
	TH0 = 0xFC;		
	TF0 = 0;		
	TR0 = 1;		
	ET0=1;
	EA=1;
}


void LED_blinky(void )
{
    
		P2_0=!P2_0;
}

void LED_blinky2(void)
{


		P2_1=!P2_1;
}

void LED_blinky3(void)
{
	P2_2=!P2_2;
	
}

void main()
{
	
			
		
    exTimer_t LEDtimer;
    exTimer_t LEDtimer2;
	
		exTimer_t LED3;

		Timer0Init();
	
		exTimerManagerCreate();
	

    LEDtimer.interval=1000;
    LEDtimer.Enable=1;
		LEDtimer.timer_callback=LED_blinky;
	
    LEDtimer2.interval=2000;
    LEDtimer2.Enable=1;
		LEDtimer2.timer_callback=LED_blinky2;
	
		LED3.interval=3000;
		LED3.Enable=1;
		LED3.timer_callback=LED_blinky3;
		
    exTimer_add(&LEDtimer);
    exTimer_add(&LEDtimer2);
		exTimer_add(&LED3);

	while(1)
	{
	pollingExTimer();

	}

}
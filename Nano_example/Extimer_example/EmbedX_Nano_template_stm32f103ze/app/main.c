
#include "sys.h"
#include "delay.h"
#include "exTimer.h"

exTimerManager_t ExTimerManager_handle;

        // HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET); 	//LED0��Ӧ����PB5���ͣ�������ͬ��LED0(0)
        // HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_SET);   	//LED1��Ӧ����PE5���ߣ��𣬵�ͬ��LED1(1)
        // delay_ms(500);											//��ʱ500ms
        // HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET);   	//LED0��Ӧ����PB5���ߣ��𣬵�ͬ��LED0(1)
        // HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_RESET); 	//LED1��Ӧ����PE5���ͣ�������ͬ��LED1(0)
        // delay_ms(500);  
void LED_blinky(void *arg)
{
    UNUSED(arg);//(void) arg; 
    static int i;
    
    if(i==1)
    {
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET);
        i=0;
    }
    else if  (i==0)                         	//��ʱ500ms 
    {
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET);
        i=1;
    }
}

void LED_blinky2(void *arg)
{
    static int i;
    UNUSED(arg);//(void) arg; 
    if(i==1)
    {
        HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_SET);
        i=0;
    }
    else if  (i==0)                         	//��ʱ500ms 
    {
        HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_RESET);
        i=1;
    }

}

int main()
{
		
	HAL_Init();                    	 	//��ʼ��HAL��    
    Stm32_Clock_Init(RCC_PLL_MUL9);   	//����ʱ��,72M
	delay_init(72);
    ExTimerManager_handle=exTimerManagerCreate();
    exTimer_t LEDtimer;
    exTimer_t LEDtimer2;
    exTimerAttr_t LEDtimer_attr;
    exTimerAttr_t LEDtimer_attr2;
    LEDtimer_attr.name=(const unsigned char *)"LED_blinky";
    LEDtimer_attr.interval=1000;
    LEDtimer_attr.Enable=1;

    LEDtimer_attr2.name=(const unsigned char *)"LED_blinky2";
    LEDtimer_attr2.interval=2000;
    LEDtimer_attr2.Enable=1;

    exTimer_init(&LEDtimer,&LEDtimer_attr,LED_blinky);
    exTimer_init(&LEDtimer2,&LEDtimer_attr2,LED_blinky2);
    exTimer_add(&ExTimerManager_handle,&LEDtimer);
    exTimer_add(&ExTimerManager_handle,&LEDtimer2);

    // disableExTimer(&ExTimerManager_handle,(const unsigned char *)"LED_blinky");
    // setExTimerInterval(&ExTimerManager_handle,(const unsigned char *)"LED_blinky2",500);

	GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE();           	//����GPIOBʱ��
	__HAL_RCC_GPIOE_CLK_ENABLE();           	//����GPIOEʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_5; 				//PB5
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//�������
    GPIO_Initure.Pull=GPIO_PULLUP;          	//����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;    //����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);

	GPIO_Initure.Pin=GPIO_PIN_5; 				//PE5
	HAL_GPIO_Init(GPIOE,&GPIO_Initure);
	
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET);	//PB5��1��Ĭ�ϳ�ʼ�������
    HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_SET);	//PE5��1��Ĭ�ϳ�ʼ�������

    while (1)
    {
		pollingExTimer(&ExTimerManager_handle);
    }
    
}


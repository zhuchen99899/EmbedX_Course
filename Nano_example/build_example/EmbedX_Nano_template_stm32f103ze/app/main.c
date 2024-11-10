
#include "sys.h"
#include "delay.h"
int main()
{
		
	HAL_Init();                    	 	//��ʼ��HAL��    
    Stm32_Clock_Init(RCC_PLL_MUL9);   	//����ʱ��,72M
	delay_init(72);
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
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET); 	//LED0��Ӧ����PB5���ͣ�������ͬ��LED0(0)
        HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_SET);   	//LED1��Ӧ����PE5���ߣ��𣬵�ͬ��LED1(1)
        delay_ms(500);											//��ʱ500ms
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET);   	//LED0��Ӧ����PB5���ߣ��𣬵�ͬ��LED0(1)
        HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_RESET); 	//LED1��Ӧ����PE5���ͣ�������ͬ��LED1(0)
        delay_ms(500);                                      	//��ʱ500ms 
    
    }
    
}


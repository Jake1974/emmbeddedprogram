#include "stm32f10x.h"                  // Device header
#include "Time_Delay.h"

int main(void)
{				
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
		
	while (1)
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_6);
		Delay_ms(500);
		GPIO_SetBits(GPIOB,GPIO_Pin_6);
		Delay_ms(500);
	}
}

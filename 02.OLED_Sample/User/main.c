#include "stm32f10x.h"                  // Device header
#include "OLED.h"

int main(void)
{				
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);	

	//OLED Initialize
	OLED_Init();	
	
	//Show some information		
	OLED_ShowString(1, 3, "Hello World!");
	
	while (1)
	{
		
	}
}

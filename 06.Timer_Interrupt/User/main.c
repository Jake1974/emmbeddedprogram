#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "timer.h"

uint16_t Num;

int main(void)
{			
	//Must enable GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
			
	Timer_Init();

	//OLED Initialize
	OLED_Init();	
	OLED_ShowString(1, 1, "Num:");	
	
	while (1)
	{
		OLED_ShowNum(1, 5, Num, 5);
	}
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Num ++;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}


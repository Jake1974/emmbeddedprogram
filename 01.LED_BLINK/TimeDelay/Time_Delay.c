#include "Time_Delay.h"

/**
  * @brief  micro second delay
  * @param  xus,range：0~233015
  * @retval none
  */
void Delay_us(uint32_t xus)
{
	SysTick->LOAD = 72 * xus;				//SET Reload value
	SysTick->VAL = 0x00;					//clear the current value
	SysTick->CTRL = 0x00000005;				//set HCLK,start timer
	while(!(SysTick->CTRL & 0x00010000));	//wait until to 0
	SysTick->CTRL = 0x00000004;				//close the timer
}

/**
  * @brief  milliseconds delay
	* @param  xms, range:0~4294967295
  * @retval none
  */
void Delay_ms(uint32_t xms)
{
	while(xms--)
	{
		Delay_us(1000);
	}
}
 
/**
  * @brief  seconds delay
  * @param  xs range:0~4294967295
  * @retval none
  */
void Delay_s(uint32_t xs)
{
	while(xs--)
	{
		Delay_ms(1000);
	}
} 

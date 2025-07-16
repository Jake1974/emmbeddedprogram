#include "Time_Delay.h"
/**
  * @brief  microseconds delay
  * @param  xus range:0~233015
  * @retval none
  */
void Delay_us(uint32_t xus)
{
	SysTick->LOAD = 72 * xus;				      //Reload Value
	SysTick->VAL = 0x00;					        //Clear the counter
	SysTick->CTRL = 0x00000005;				    //Start timer
	while(!(SysTick->CTRL & 0x00010000));	//Wait until to 0
	SysTick->CTRL = 0x00000004;				    //close timer
}

/**
  * @brief  milliseconds delay
  * @param  xms range:0~4294967295
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

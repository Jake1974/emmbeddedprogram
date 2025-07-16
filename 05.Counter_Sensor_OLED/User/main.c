#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "CountSensor.h"

int main(void)
{		
	//CountSensor initialize
	CountSensor_Init();
	//OLED Initialize
	OLED_Init();	

	while (1)
	{
		OLED_ShowNum(1, 7, CountSensor_Get(), 5);
	}
}

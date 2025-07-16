#include "OLED_Font.h"
#include "OLED.h"

/*Pin Configuration*/
#define OLED_W_SCL(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)(x))
#define OLED_W_SDA(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_9, (BitAction)(x))

/*Pin Intialization*/
void OLED_I2C_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		
	
		OLED_W_SCL(1);
		OLED_W_SDA(1);
}

/**
  * @brief  I2C communication start
  * @param  None
  * @retval None
  */
void OLED_I2C_Start(void)
{
	OLED_W_SDA(1);
	OLED_W_SCL(1);
	OLED_W_SDA(0);
	OLED_W_SCL(0);
}

/**
  * @brief  I2C communication stop
  * @param  none
  * @retval none
  */
void OLED_I2C_Stop(void)
{
	OLED_W_SDA(0);
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}

/**
  * @brief  Send one byte using I2C protocol
  * @param  Byte byte to send
  * @retval None
  */
void OLED_I2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		OLED_W_SDA(Byte & (0x80 >> i));
		OLED_W_SCL(1);
		OLED_W_SCL(0);
	}
	OLED_W_SCL(1);	//Addtional tick
	OLED_W_SCL(0);
}

/**
  * @brief  OLED Write Command
  * @param  Command command to wrote
  * @retval None
  */
void OLED_WriteCommand(uint8_t Command)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//slave address
	OLED_I2C_SendByte(0x00);		//write command
	OLED_I2C_SendByte(Command); 
	OLED_I2C_Stop();
}

/**
  * @brief  Write Data
  * @param  Data data to write
  * @retval 无
  */
void OLED_WriteData(uint8_t Data)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//save address
	OLED_I2C_SendByte(0x40);		//write data command
	OLED_I2C_SendByte(Data);
	OLED_I2C_Stop();
}

/**
  * @brief  OLED Set cursor position. Left-Top as original coordinate, downwards and rightwards as direction.
  * @param  Y range：0~7
  * @param  X range: 0~127
  * @retval None
  */
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
	OLED_WriteCommand(0xB0 | Y);					//Y Position
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//X Position High bits
	OLED_WriteCommand(0x00 | (X & 0x0F));			//X Position low bits
}

/**
  * @brief  Clear OLED
  * @param  None
  * @retval None
  */
void OLED_Clear(void)
{  
	uint8_t i, j;
	for (j = 0; j < 8; j++)
	{
		OLED_SetCursor(j, 0);
		for(i = 0; i < 128; i++)
		{
			OLED_WriteData(0x00);
		}
	}
}

/**
  * @brief  Show one character
  * @param  Line line position，range：1~4
  * @param  Column column position，range：1~16
  * @param  Char character to show，range：ASCII characters
  * @retval None
  */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);		//Set curson in top-half section
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i]);			//show top-half section
	}
	OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);	//SET curson in bottom-half section
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);		//show bottom-half section
	}
}

/**
  * @brief  Show character array
  * @param  Line row to start，range：1~4
  * @param  Column column to start，range：1~16
  * @param  String string to show
  * @retval None
  */
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		OLED_ShowChar(Line, Column + i, String[i]);
	}
}

/**
  * @brief  OLED Pow function
  * @retval Y powed by Y
  */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}

/**
  * @brief  OLED Show unsinged numbers
  * @param  Line, range：1~4
  * @param  Column, range：1~16
  * @param  Number, range：0~4294967295
  * @param  Length, range：1~10
  * @retval None
  */
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED show signed number
	* @param  Line,range:1~4
	* @param  Column,range:1~16
	* @param  Number range: -2147483648~2147483647
	* @param  Length range:1~10
  * @retval None
  */
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
	uint8_t i;
	uint32_t Number1;
	if (Number >= 0)
	{
		OLED_ShowChar(Line, Column, '+');
		Number1 = Number;
	}
	else
	{
		OLED_ShowChar(Line, Column, '-');
		Number1 = -Number;
	}
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED show hex number
  * @param  Line,range:1~4
	* @param  Column,range:1~16
	* @param  Number range: 0~0xFFFFFFFF
	* @param  Length range:1~8
  * @retval None
  */
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)							
	{
		SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
		if (SingleNumber < 10)
		{
			OLED_ShowChar(Line, Column + i, SingleNumber + '0');
		}
		else
		{
			OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
		}
	}
}

/**
  * @brief  OLED show binary number
	* @param  Line range:1~4
	* @param  Column range:1~16
	* @param  Number range:0~1111 1111 1111 1111
	* @param  Length range:1~16
  * @retval none
  */
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
	}
}

/**
  * @brief  OLED initialize
  * @param  none
  * @retval none
  */
void OLED_Init(void)
{
	uint32_t i, j;
	
	for (i = 0; i < 1000; i++)			//power delay
	{
		for (j = 0; j < 1000; j++);
	}
	
	OLED_I2C_Init();			//Pin initialize
	
	OLED_WriteCommand(0xAE);	//clear showing
	
	OLED_WriteCommand(0xD5);	//set frequency
	OLED_WriteCommand(0x80);
	
	OLED_WriteCommand(0xA8);	//set AFIO
	OLED_WriteCommand(0x3F);
	
	OLED_WriteCommand(0xD3);	//set offset
	OLED_WriteCommand(0x00);
	
	OLED_WriteCommand(0x40);	//enable showing
	
	OLED_WriteCommand(0xA1);	//direction: 0xA1-normal, 0xA0 left-right inversion
	
	OLED_WriteCommand(0xC8);	//direction: 0xC8-normal, 0xC0 to-down inversion

	OLED_WriteCommand(0xDA);	//Set Pin Configuration
	OLED_WriteCommand(0x12);
	
	OLED_WriteCommand(0x81);	//contrast ratio
	OLED_WriteCommand(0xCF);

	OLED_WriteCommand(0xD9);	//pre-charged cycle
	OLED_WriteCommand(0xF1);

	OLED_WriteCommand(0xDB);	//VCOMH
	OLED_WriteCommand(0x30);

	OLED_WriteCommand(0xA4);	//enable showing on or off

	OLED_WriteCommand(0xA6);	//enable normal or inversion showing

	OLED_WriteCommand(0x8D);	//charge pump
	OLED_WriteCommand(0x14);

	OLED_WriteCommand(0xAF);	//start to show
		
	OLED_Clear();				//OLED clear
}

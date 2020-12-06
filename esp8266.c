#include "esp8266.h"

void ESP8266_SendATCommand(u8*Command,u8*Ask)
{
	//memset(Command,0,8);
	USART2_SendStr(Command);
	
}
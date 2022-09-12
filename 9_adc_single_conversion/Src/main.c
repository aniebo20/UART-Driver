#include <stdio.h>
#include <stdint.h>
#include "stm32f7xx.h"
#include "uart.h"
#include "adc.h"

uint32_t sensor_value;

int main()
{

	uart3_tx_init();
	pa3_adc_init();


	while(1)
	{
		start_conversion();
		sensor_value = adc_read();
		printf("Temperature Sensor value: %d \n\r", (int)&sensor_value);
	}
}




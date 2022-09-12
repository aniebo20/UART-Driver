#include <stdio.h>
#include <stdint.h>
#include "stm32f7xx.h"
#include "uart.h"

#define GPIOBEN				(1U<<1)					/*For enabling the peripheral clock on the RCC register*/
#define PIN14				(1U<<14)				/*For the data output register*/
#define PIN7				(1U<<7)					/*For the data output register*/
#define PIN0				(1U)					/*For the data output register*/

#define RED_LED_PIN			(PIN14)
#define BLUE_LED__PIN		(PIN7)
#define GREEN_LED_PIN		(PIN0)

char password[8] = "Jeremiah";
char input[8];
char check;
int main()
{
	/*IMPLEMENT THE DRIVER IN A PASSWORD CHECKER APPLICATION*/
	RCC->AHB1ENR |= GPIOBEN;

	GPIOB->MODER |= ((1U) | (1U<<14) | (1U<<28));
	GPIOB->MODER &= ~((1U<<1) | (1U<<15) | (1U<<29));

	uart3_rxtx_init();

	while(1)
	{
		//Turn OFF LEDs and reset check
		GPIOB->ODR &= ~(GREEN_LED_PIN| BLUE_LED__PIN | RED_LED_PIN);
		check = '1';


		/*IF TX PROPERLY TRANSMITS "Jeremiah", AND RX RECEIVES IT GREEN LED IS SET
		 * IF IT FAILS, RED LED IS SET*/
		for (int i = 0; i < 8; i++)
			input[i] = uart3_read();

		for (int i = 0; i < 8; i++)
			if (input[i] != password[i])
				check = 0;


		if(check)
			GPIOB->ODR |= (GREEN_LED_PIN);
		else
			GPIOB->ODR |= (RED_LED_PIN);

		for (int i = 0; i < 2; i++)
		{
			GPIOB->ODR ^= (BLUE_LED__PIN); //Flash BLUE PIN to indicate end of word
			for (int i = 0; i < 1000000; i++){}
		}
	}
}




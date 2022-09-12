
#include "stm32f7xx.h"
#include "adc.h"

#define ADC3EN					(1U<<10)						//Enable Clock for ADC1 on APB2 BUS
#define GPIOAEN					(1U)							//Enable Clock for port A on AHB1
#define	GPIOA_3					(3U<<6)							//Set PA2 as analog
#define ADC3_CH3				(3U)
#define ADC3_SQR1_LEN_1			(0U)
#define SR_EOC					(1U<<1)
#define CR2_ADON				(1)
#define CR2_SWSTART				(1U<<30)
//adc pin will be ADC3_IN3 which is PA3


void pa3_adc_init(void)
{
	/*Configure the ADC GPIO Pin
	 * 1.Enable Clock Access to GPIOA
	 * 2. Set mode of PA2 to Analog*/

	//1.
	RCC->AHB1ENR |= GPIOAEN;
	//2.
	GPIOA->MODER |= GPIOA_3;

	/*Configure the ADC module
	 * 1. Enable clock access to ADC
	 * 2. Configure ADC parameters:
	 * 	a) Conversion Sequence start - What is the first channel you want to sample in the sequence register?
	 * 	b) Conversion Sequence length - How many channels are used?
	 * 	c) Enable ADC module*/

	//1.
	RCC->APB2ENR |= ADC3EN;
	//2.
	//a)
	ADC3->SQR3 = ADC3_CH3;
	//b)
	ADC3->SQR1 = ADC3_SQR1_LEN_1;
	//c)
	ADC3->CR2 |= CR2_ADON;
}

void start_conversion(void)
{
	/*Start ADC conversion*/
	ADC3->CR2 |= CR2_SWSTART;
}

uint32_t adc_read(void)
{
	/*1.Wait for conversion to be complete
	 *2.Read conversion*/

	//1.
	while ((ADC3->SR & SR_EOC)) {};

	//2.
	return ADC3->DR;


}


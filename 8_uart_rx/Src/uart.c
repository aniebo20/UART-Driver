#include "uart.h"
#define UART3_EN			(1U<<18)				/*For enabling the USART3 clock on RCC APB1 register*/
#define GPIODEN				(1U<<3)					/*For enabling the peripheral clock on the RCC register*/
#define CR1_TE				(1U<<3)
#define CR1_RE				(1U<<2)
#define CR1_UE				(1U)
#define ISR_TXE				(1U<<7)
#define ISR_RXNE			(1U<<5)
#define ISR_TC				(1U<<6)

/*USART2 RX LINE IS PD9
 *USART2 TX LINE IS PD8 */

#define SYS_FREQ			16000000				/*Default System Clock Frequency (HSI frequency)*/
#define	APB1_CLK			SYS_FREQ

#define UART_BAUDRATE		115200



static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);


void uart3_write(int ch);

int __io_putchar(int ch)
{
	uart3_write(ch);
	return ch;
}

void uart3_rxtx_init(void)
{
	/*Configure UART GPIO PIN
	 * 1. Enable Clock access to GPIOD
	 * 2. Set PD8 mode to alternate function mode
	 * 3. Set PD9 mode to alternate function mode
	 * 4. Set PD8 alternate function type to UART_TX (AF07)
	 * 5. Set PD9 alternate function type to UART_RX (AF07)*/
	//1.
	RCC->AHB1ENR |= GPIODEN;
	//2.
	GPIOD->MODER |= (1U<<17);
	GPIOD->MODER &= ~(1U<<16);
	//3.
	GPIOD->MODER |= (1U<<19);
	GPIOD->MODER &= ~(1U<<18);
	//4.
	GPIOD->AFR[1] &= ~(1U<<3);
	GPIOD->AFR[1] |= (7U);
	//5.
	GPIOD->AFR[1] &= ~(1U<<7);
	GPIOD->AFR[1] |= (7U<<4);


	/*Configure UART module
	 * 1. Enable clock access to UART2
	 * 2. Configure baud rate
	 * 3. Configure the transfer direction for both directions
	 * 4. Enable UART module*/
	//1.
	RCC->APB1ENR |= UART3_EN;
	//2.
	uart_set_baudrate(USART3, APB1_CLK, UART_BAUDRATE);
	//3.
	USART3->CR1 = CR1_TE | CR1_RE;
	//4.
	USART3->CR1 |= CR1_UE;

}

void uart3_tx_init(void)
{
	/*Configure UART GPIO PIN
	 * 1. Enable Clock access to GPIOD
	 * 2. Set PD8 mode to alternate function mode
	 * 3. Set the alternate function type to UART_TX (AF07)*/
	//1.
	RCC->AHB1ENR |= GPIODEN;
	//2.
	GPIOD->MODER |= (1U<<17);
	GPIOD->MODER &= ~(1U<<16);
	//3.
	GPIOD->AFR[1] &= ~(1U<<3);
	GPIOD->AFR[1] |= (7U);


	/*Configure UART module
	 * 1. Enable clock access to UART2
	 * 2. Configure baud rate
	 * 3. Configure the transfer direction
	 * 4. Enable UART module*/
	//1.
	RCC->APB1ENR |= UART3_EN;
	//2.
	uart_set_baudrate(USART3, APB1_CLK, UART_BAUDRATE);
	//3.
	USART3->CR1 = CR1_TE;
	//4.
	USART3->CR1 |= CR1_UE;

}

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate)
{
	USARTx->BRR = compute_uart_bd(PeriphClk, BaudRate);
}

static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate)
{
	return (PeriphClk + (BaudRate / 2U))/BaudRate;
}

char uart3_read(void)
{
	/*
	 * 1. Ensure receive data register is not empty
	 * 2. Read from receive data register*/

	//1.
	while(!(USART3->ISR & ISR_RXNE)){}

	return (USART3->RDR /*& ~(0b11111111)*/);
}

void uart3_write(int ch)
{
	/*
	 * 1. Ensure transmit data register is empty
	 * 2. Write to transmit data register*/

	//1.
	while(!(USART3->ISR & ISR_TXE)){}
	//2.
	USART3->TDR = (ch & 0xFF);
}

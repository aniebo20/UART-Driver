#ifndef UART_H_
#define UART_H_
#include <stdint.h>

#include "stm32f7xx.h"

void uart3_tx_init(void);
char uart3_read(void);
void uart3_rxtx_init(void);

#endif /* UART_H_ */


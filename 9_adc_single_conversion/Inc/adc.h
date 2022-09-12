/*
 * adc.h
 *
 *  Created on: Sep 9, 2022
 *      Author: jerry
 */

#ifndef ADC_H_
#define ADC_H_
#include <stdint.h>
void pa3_adc_init(void);
void start_conversion(void);
uint32_t adc_read(void);
#endif /* ADC_H_ */

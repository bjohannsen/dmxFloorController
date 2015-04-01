/*
 * adc_handler.h
 *
 * Author: @dreimalbe
 */
#ifndef ADC_HANDLER_H
#define ADC_HANDLER_H

#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define ADC_NUM_CHANNELS 1

void adc_init();
uint16_t adc_get_value(uint8_t channel);

#endif

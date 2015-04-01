/*
 * adc_handler.c
 *
 * Author: @dreimalbe
 */
#include "adc_handler.h"

volatile uint16_t _adc_data[ADC_NUM_CHANNELS];

ISR(ADC_vect)
{
	static uint8_t channel = 0;
	static uint8_t update_channel = 0;
	uint8_t tmp;
	if(update_channel == 0)
	{
		_adc_data[channel] = ADCW;
		channel = (channel+1)%ADC_NUM_CHANNELS;
		ADMUX = (ADMUX & ~(0x1F)) | (channel & 0x1F);
		update_channel = 3;
	}
	else
	{
		tmp = ADCW;
		update_channel--;
	}
}

uint16_t adc_get_value(uint8_t c)
{
  return _adc_data[c];
}

void adc_init() 
{
	ADMUX |= (1<<REFS0);
	ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS0) | (1<<ADATE) | (1<<ADIE);
	ADCSRB &= ~((1<<ADTS2 ) | (1<<ADTS1) | (1<<ADTS0));
	ADCSRA |= (1<<ADSC);
}


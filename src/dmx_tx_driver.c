/*
 * dmx_tx_driver.c
 *
 * Author: @dreimalbe
 */
#include "dmx_tx_driver.h"

volatile uint8_t dmx_state;
volatile uint16_t dmx_current_channel;
volatile uint16_t dmx_num_channels;
volatile uint8_t* dmx_data;

ISR(DMX_TIMER_COMPA_vect)
{
	dmx_handleFrame();
}

ISR(DMX_USART_TX_ISR_vect)
{
	if(dmx_state == DMX_STATE_DATA)
	{
		if(dmx_current_channel >= dmx_num_channels)
		{
			dmx_state = DMX_STATE_START;
			dmx_handleFrame();
		}
		else
		{
			DMX_UDR = dmx_data[dmx_current_channel++];
		}
	}
}

void dmx_handleFrame()
{
	cli();
	if(dmx_state == DMX_STATE_START)
	{
		dmx_UARTDisable();
		dmx_current_channel = 0;
		DMX_OCRA = DMX_TIMER1_COMPARE_BASE * DMX_RESET_TIME;
		DMX_PIN_SET_LOW;
		DMX_TCNT = 0;
		DMX_TIMSK |= (1<<DMX_OCIEA);
		DMX_TIFR &= ~(1<<DMX_OCFA);
		dmx_state = DMX_STATE_RESET;
	}
	else if(dmx_state == DMX_STATE_RESET)
	{
		DMX_OCRA = DMX_TIMER1_COMPARE_BASE * DMX_MARK_TIME;
		DMX_PIN_SET_HIGH;
		DMX_TCNT = 0;
		DMX_TIFR &= ~(1<<DMX_OCFA);
		dmx_state = DMX_STATE_DATA;
	}
	else if(dmx_state == DMX_STATE_DATA)
	{
		DMX_TIMSK &= ~(1<<DMX_OCIEA);
		DMX_UDR = 0;
		dmx_UARTEnable();
	}
	sei();
}

void dmx_UARTEnable()
{
	DDRD &= ~(DMX_PIN);
	DMX_UCSRB |= (1<<DMX_TXEN) | (1<<DMX_TXCIE);
}

void dmx_UARTDisable()
{
	DMX_UCSRB &= ~(1<<DMX_TXEN);
	DDRD |= (DMX_PIN);
	DMX_PIN_SET_HIGH;
}

inline void dmx_UARTInit()
{
	DMX_UART_UBRRH = (unsigned char)(DMX_UBRR_VAL>>8);
	DMX_UART_UBRRL = (unsigned char)DMX_UBRR_VAL;
	DMX_UCSRC = (3<<DMX_UCSZ0) | (1<<DMX_USBS);
}

inline void dmx_start(){
	dmx_current_channel = 0;
	dmx_state = DMX_STATE_START;
	dmx_handleFrame();
}

inline void dmx_stop()
{
	dmx_state = DMX_STATE_STOP;
}

void dmx_init(uint16_t num_channels, uint8_t* source)
{
	if(num_channels > DMX_MAX_CHANNELS)
	{
		dmx_num_channels = DMX_MAX_CHANNELS;
	}
	else
	{
		dmx_num_channels = num_channels;
	}
	dmx_data = source;
	dmx_UARTInit();

	// DMX Timer
	DMX_TCCRA |= (1<<DMX_WGM1);
	DMX_TCCRB = (1<<DMX_CS1);
}

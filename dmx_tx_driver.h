#ifndef DMX_DRIVER4313_H_
#define DMX_DRIVER4313_H_

#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

/* DMX *******************************************/
 #define DMX_STATE_START 0
#define DMX_STATE_RESET 1
#define DMX_STATE_DATA 2
#define DMX_STATE_STOP 3

#define DMX_RESET_TIME				22 //88 //us
#define DMX_MARK_TIME				 2 //8 //us

#define DMX_STARTBYTE_VALUE			0x00

#define DMX_MAX_CHANNELS			512
/* IO ********************************************/
#define DMX_PIN 				(1<<PIND1)

#define DMX_PIN_SET_HIGH 		(PORTD |= (1<<PORTD1))
#define DMX_PIN_SET_LOW			(PORTD &= ~(1<<PORTD1))

#define DMX_UART0
#define DMX_TIMER1

void dmx_UARTDisable();
void dmx_UARTEnable();
void dmx_start();
void dmx_stop();
void dmx_init( uint16_t num_channels, uint8_t* source );
void dmx_setData( uint16_t channel, uint8_t data );
void dmx_setDataArray( uint16_t first_channel_id, void* data, uint16_t length );
void dmx_handleFrame();

/* Berechnungen ***********************************
 * hier nicht editieren! **************************
 **************************************************/
// UART ///////
#define DMX_BAUD 250000UL
#define DMX_UBRR_VAL ((F_CPU+DMX_BAUD*8)/(DMX_BAUD*16)-1)
#define DMX_BAUD_REAL (F_CPU/(16*(DMX_UBRR_VAL+1)))

#define DMX_BAUD_ERROR ((DMX_BAUD_REAL*1000)/DMX_BAUD)
 #if ((DMX_BAUD_ERROR<990) || (DMX_BAUD_ERROR>1010))
	#error Systematischer Fehler der Baudrate grösser 1% und damit zu hoch!
#endif

// Timer1 (DMX, 4us)
#define DMX_US_TIME					4 //TODO!
#define DMX_TIMER1_PRESCALER		8
#define DMX_TIMER1_COMPARE_BASE		((DMX_US_TIME*(F_CPU/DMX_TIMER1_PRESCALER))/1000000)

/* Register Mappings********************************
 ***************************************************/
#ifdef DMX_UART0
#define DMX_UART_UBRRH 	UBRR0H
#define DMX_UART_UBRRL 	UBRR0L
#define DMX_UCSRB		UCSR0B
  #define DMX_TXEN		TXEN0
  #define DMX_TXCIE		TXCIE0
#define DMX_UCSRC		UCSR0C
  #define DMX_UCSZ0		UCSZ00
  #define DMX_USBS		USBS0
#define DMX_UDR			UDR0
#define DMX_USART_TX_ISR_vect	USART_TX_vect
#endif

#ifdef DMX_UART1
#define DMX_UART_UBRRH 	UBRR1H
#define DMX_UART_UBRRL 	UBRR1L
#define DMX_UCSRB		UCSR1B
  #define DMX_TXEN		TXEN1
  #define DMX_TXCIE		TXCIE1
#define DMX_UCSRC	   	UCSR1C
  #define DMX_UCSZ0		UCSZ10
  #define DMX_USBS		USBS1
#define DMX_UDR			UDR1
#define DMX_USART_TX_ISR_vect	USART_TXC_vect
#endif

#ifdef DMX_UART
#define DMX_UART_UBRRH 	UBRRH
#define DMX_UART_UBRRL 	UBRRL
#define DMX_UCSRB		UCSRB
  #define DMX_TXEN		TXEN
  #define DMX_TXCIE		TXCIE
#define DMX_UCSRC	   	UCSRC
  #define DMX_UCSZ0		UCSZ0
  #define DMX_USBS		USBS
#define DMX_UDR			UDR
#define DMX_USART_TX_ISR_vect	USART_TX_vect
#endif

#ifdef DMX_TIMER1
 #ifdef TIMSK1
  #define DMX_TIMSK		TIMSK1
  #define DMX_TIFR		TIFR1
 #else
  #define DMX_TIMSK		TIMSK
  #define DMX_TIFR		TIFR
 #endif
  #define DMX_TCCRA		TCCR1A
   #define DMX_WGM1		WGM12
  #define DMX_TCCRB		TCCR1B
   #define DMX_CS1		CS11
   #define DMX_OCIEA	OCIE1A
   #define DMX_OCFA		OCF1A
  #define DMX_OCRA		OCR1A
 #define DMX_TCNT		TCNT1
#define DMX_TIMER_COMPA_vect TIMER1_COMPA_vect
#endif

#ifdef DMX_TIMER2
#define DMX_TCCRA		TCCR2A
  #define DMX_WGM1		WGM22
#define DMX_TCCRB		TCCR2B
  #define DMX_CS1		CS21
#define DMX_TIMSK		TIMSK2
  #define DMX_OCIEA		OCIE2A
#define DMX_TIFR		TIFR2
  #define DMX_OCFA		OCF2A
#define DMX_OCRA		OCR2A
#define DMX_TCNT		TCNT2
#endif

#endif /* DMX_DRIVER4313_H_ */

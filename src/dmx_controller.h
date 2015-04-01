/*
 * dmx_controller.h
 *
 * Author: @dreimalbe
 */
#ifndef DMX_CONTROLLER_H
#define DMX_CONTROLLER_H

#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "debounce.h"
#include "dmx_tx_driver.h"
#include "adc_handler.h"

#define CONFIG_4_DEVICES		4
#define CONFIG_2_DEVICES		2
#define CONFIG_SETUP_DIRECT		1
#define CONFIG_SETUP_INDIRECT	2

/* USER SETTINGS *****************************/
/*********************************************/

//Fading Speeds
#define POST_FLASH_FADE_FACTOR 	15
#define COLOR_FADE_FACTOR 		1
#define FAST_TO_BASEDIM_FACTOR	10
#define MEDIUM_TO_BASEDIM_FACTOR 7
#define SLOW_TO_BASEDIM_FACTOR	5

//Setup
#define CONFIG_NUM_DEVICES		CONFIG_4_DEVICES
//#define CONFIG_NUM_DEVICES	CONFIG_2_DEVICES
#define CONFIG_SETUP			CONFIG_SETUP_DIRECT
//#define CONFIG_SETUP			CONFIG_SETUP_INDIRECT

// allowed: 100, 80, 70, 60, 50, 40, 30%
#define INTESITY_DIM	 70
#define INTESITY_BASE	100

/* USER SETTINGS - END ***********************/
/*********************************************/

#define LED_PORT PORTD
#define LED1_PIN (1<<PORTD4)
#define LED2_PIN (1<<PORTD5)
#define LED3_PIN (1<<PORTD6)
#define LED4_PIN (1<<PORTD7)
#define LED_MASK ( LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN )


// MODI
#define BTN_BLACKOUT 0
#define BTN_COLOR_CHANGE 1
#define BTN_EFFECT 2
#define BTN_FLASH 3					// Welcher Button ist Taster
#define BLACKOUT_CODE 4
#define DEBOUNCE_INTERVAL 	10		// Entprell-Intervall in ms

#if CONFIG_NUM_DEVICES == CONFIG_4_DEVICES
#define MAX_SCENE_SIZE 	45
#endif
#if CONFIG_NUM_DEVICES == CONFIG_2_DEVICES
#define MAX_SCENE_SIZE  22
#endif


/* Datenstrukturen *******************************
 *************************************************
 *************************************************/

struct scene {
	uint8_t time;
	uint8_t animation;
	uint8_t dmx_data[MAX_SCENE_SIZE];
};

typedef struct scene scene_t;

#define ANIMATION_NONE 0
#define ANIMATION_BASE_ALTER_FADE 1
#define ANIMATION_FADE_TO_BASEDIM_SLOW 2
#define ANIMATION_FADE_TO_BASEDIM_MEDIUM 4
#define ANIMATION_FADE_TO_BASEDIM_FAST 8
#define ANIMATION_FADE_TO_BASEDIM	(ANIMATION_FADE_TO_BASEDIM_SLOW | ANIMATION_FADE_TO_BASEDIM_MEDIUM | ANIMATION_FADE_TO_BASEDIM_FAST)
#define ANIMATION_FADE_FACTOR 1

#define DMX_BASE_ID			1
#define DMX_BASEDIM_ID		2
#define DMX_ALTER_ID		3
#define DMX_ALTERDIM_ID		4

#define DMX_BASE_INDEX 		(DMX_BASE_ID-1)
#define DMX_BASEDIM_INDEX	(DMX_BASEDIM_ID-1)
#define DMX_ALTER_INDEX		(DMX_ALTER_ID-1)
#define DMX_ALTERDIM_INDEX  (DMX_ALTERDIM_ID-1)

#define NUM_COLORS 	5
#define RGB			3

#define STATE_POST_FLASH 1
#define STATE_START	0

#define DIMMING_ADC_MAX		127


/* Funktionsprototypen ***************************
 *************************************************
 *************************************************/
void setCurrentScene( void * p );
void cue_handle( uint8_t cid );
void cue_checkColorsForLED252(uint16_t startchannel);
void calculateDimming();
 
// Timer0 (Entprellen)
#define TIMER0_PRESCALER		1024
#define DEBOUNCE_COMPARE		((DEBOUNCE_INTERVAL*(F_CPU/TIMER0_PRESCALER))/1000)
#if (DEBOUNCE_COMPARE>255)
	#error Debounce Intervall zu hoch!
#endif


// Befehle ///////////////////////////////////////
#define LED_STATUS_PORT			PORTD
#define LED_BANK 				(1<<PORTD2)
#define LED_COLOR 				(1<<PORTD3)
#define SET_COLOR_LED 			(LED_STATUS_PORT |= LED_COLOR )
#define CLEAR_COLOR_LED 		(LED_STATUS_PORT &= ~(LED_COLOR) )
#define SET_BANK_LED 			(LED_STATUS_PORT |= LED_BANK )
#define CLEAR_BANK_LED			(LED_STATUS_PORT &= ~(LED_BANK) )

#define LED_ONBOARD_PORT		PORTC
#define LED_ONBOARD_YELLOW		(1<<PORTC5)
#define LED_ONBOARD_RED			(1<<PORTC4)

#define SET_ONBOARD_YELLOW		(PORTC |= LED_ONBOARD_YELLOW)
#define CLEAR_ONBOARD_YELLOW	(PORTC &= ~(LED_ONBOARD_YELLOW))
#define SET_ONBOARD_RED			(PORTC |= LED_ONBOARD_RED)
#define CLEAR_ONBOARD_RED		(PORTC &= ~(LED_ONBOARD_RED))

#define BANK_LED_INTERVAL				20

#endif

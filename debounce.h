#ifndef DEBOUNCE_H_
#define DEBOUNCE_H_
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

// Taster
#define DEBOUNCE_BTN_PIN_C PINC
#define DEBOUNCE_BTN_PIN_B PINB
#define DEBOUNCE_BTN1_PIN (1<<PINC1)
#define DEBOUNCE_BTN2_PIN (1<<PINC2)
#define DEBOUNCE_BTN3_PIN (1<<PINC3)
#define DEBOUNCE_BTN4_PIN (1<<PINB2)
#define DEBOUNCE_BTN_MASK_C (DEBOUNCE_BTN1_PIN | DEBOUNCE_BTN2_PIN | DEBOUNCE_BTN3_PIN)
#define DEBOUNCE_BTN_MASK_B (DEBOUNCE_BTN4_PIN)

#define DEBOUNCE_NUM_BUTTONS 	4

#define DEBOUNCE_REPEAT_MASK     (0x01 | 0x02 | 0x04)
#define DEBOUNCE_REPEAT_START    50
#define DEBOUNCE_REPEAT_NEXT     5

void debounce_check();
void debounce_init();
uint8_t* debounce_getButtons();
uint8_t debounce_getKeyShortpressed(uint8_t btn_mask);
uint8_t debounce_getKeyLongppressed(uint8_t btns);
uint8_t debounce_getKeyRepeat(uint8_t btns);
uint8_t debounce_getKeyDepress(uint8_t btns);
uint8_t debounce_getKeyPress(uint8_t btns);

#endif /* DEBOUNCE_H_ */

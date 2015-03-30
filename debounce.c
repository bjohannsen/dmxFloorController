#include "debounce.h"
/*
 * Debouncing of buttons
 * Based upon solution by Peter Dannegger (https://www.mikrocontroller.net/topic/48465)
 */

volatile uint8_t debounce_btn_state = 0;
volatile uint8_t debounce_btn_press = 0;
volatile uint8_t debounce_btn_depress = 0;
volatile uint8_t debounce_btn_repeat = 0;

uint8_t debounce_getbtns()
{
	uint8_t pins;

	pins = (PINC & DEBOUNCE_BTN_MASK_C);
	pins = pins >> 1;
	if( PINB & (1<<PINB2) )
	{
		pins |= (1<<PINC3);
	}
	return pins;
}


void debounce_check()
{
	static uint8_t counter0, counter1 = 0, rpt;
	uint8_t tmp_btns;

	tmp_btns = debounce_btn_state ^ ~(debounce_getbtns());
	counter0 = ~(counter0 & tmp_btns);
	counter1 = counter0 ^ (counter1 & tmp_btns);
	tmp_btns &= counter0 & counter1;
	debounce_btn_state ^= tmp_btns;
	debounce_btn_press |= debounce_btn_state & tmp_btns;
	debounce_btn_depress |= ~debounce_btn_state & tmp_btns;

	if ((debounce_btn_state & DEBOUNCE_REPEAT_MASK) == 0)
	{
		rpt = DEBOUNCE_REPEAT_START;
	}
	if (--rpt == 0)
	{
		rpt = DEBOUNCE_REPEAT_NEXT;
		debounce_btn_repeat |= debounce_btn_state & DEBOUNCE_REPEAT_MASK;
	}
}

uint8_t debounce_getKeyPress(uint8_t btns)
{
	cli();
	btns &= debounce_btn_press;
	debounce_btn_press ^= btns;
	sei();
	return btns;
}

uint8_t debounce_getKeyDepress(uint8_t btns)
{
	cli();
	btns &= debounce_btn_depress;
	debounce_btn_depress ^= btns;
	sei();
	return btns;
}

uint8_t debounce_getKeyRepeat(uint8_t btns)
{
	cli();
	btns &= debounce_btn_repeat;
	debounce_btn_repeat ^= btns;
	sei();
	return btns;
}

uint8_t debounce_getKeyShortpressed(uint8_t btn_mask)
{
  cli();
  return debounce_getKeyPress(~debounce_btn_state & btn_mask);
}

uint8_t debounce_getKeyLongppressed(uint8_t btns)
{
	return debounce_getKeyPress(debounce_getKeyRepeat(btns));
}

void debounce_init()
{
	PORTC |= (DEBOUNCE_BTN_MASK_C);
	PORTB |= (DEBOUNCE_BTN_MASK_B);
}

#include "dmx_controller.h"

#if CONFIG_NUM_DEVICES == CONFIG_2_DEVICES
 #if CONFIG_SETUP == CONFIG_SETUP_DIRECT
  #include "dmx_settings_2_direct.h"
 #endif
 #if CONFIG_SETUP == CONFIG_SETUP_INDIRECT
  #include "dmx_settings_2_indirect.h"
 #endif
#endif
#if CONFIG_NUM_DEVICES == CONFIG_4_DEVICES
 #if CONFIG_SETUP == CONFIG_SETUP_DIRECT
  #include "dmx_settings_4_direct.h"
 #endif
 #if CONFIG_SETUP == CONFIG_SETUP_INDIRECT
  #include "dmx_settings_4_indirect.h"
 #endif
#endif

#if CONFIG_NUM_DEVICES == CONFIG_2_DEVICES
volatile scene_t current_scene = { 0, ANIMATION_NONE, { DMX_252__1_Off, DMX_252__1_Off }};
volatile uint8_t current_animation_settings[ MAX_SCENE_SIZE ] = { DMX_252__1_Off, DMX_252__1_Off};
#endif
#if CONFIG_NUM_DEVICES == CONFIG_4_DEVICES
volatile scene_t current_scene = { 0, ANIMATION_NONE, { DMX_252__1_Off, DMX_252__1_Off, DMX_252__1_Off, DMX_252__1_Off, DOSE_OFF}};
volatile uint8_t current_animation_settings[ MAX_SCENE_SIZE ] = { DMX_252__1_Off, DMX_252__1_Off, DMX_252__1_Off, DMX_252__1_Off, DOSE_OFF};
volatile uint8_t dmx_stream[ MAX_SCENE_SIZE ] = { DMX_252__1_Off, DMX_252__1_Off, DMX_252__1_Off, DMX_252__1_Off, DOSE_OFF};
#endif

volatile uint8_t current_colors[4][RGB] = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };
volatile uint8_t current_color_id = 0;
volatile uint8_t current_bank = 0;
volatile uint8_t current_preset = 0;
volatile uint8_t last_preset = 0;
volatile uint8_t state = STATE_START;

volatile uint8_t LED[] = { LED1_PIN, LED2_PIN, LED3_PIN, LED4_PIN };

volatile uint8_t BANK[2][4] = { { CUE_BLACKOUT, CUE_STATIC_COLOR, CUE_COLORSTROBE, CUE_FLASH },
							  { CUE_BLACKOUT, CUE_FADING_COLOR, CUE_RUNLIGHT, CUE_FLASH } };

volatile uint8_t brightness = 100;

/*****
 * ISR Taster entprellen 
 * (nach Peter Dannegger, siehe mikrokontroller.net)
 *****/
ISR(TIMER0_COMPA_vect)
{
	static uint8_t bank_led_cnt = 0;
	debounce_check();
	cue_handle(current_preset);

	if(current_bank)
	{
		if(bank_led_cnt > BANK_LED_INTERVAL)
		{
			bank_led_cnt = 0;
		}
		else
		{
			if(bank_led_cnt > (BANK_LED_INTERVAL>>1))
			{
				SET_BANK_LED;
			}
			else
			{
				CLEAR_BANK_LED;
			}
			bank_led_cnt++;
		}
	}
	else
	{
		CLEAR_BANK_LED;
	}
}

/*****
 * Laden einer Szene einer Cue aus dem Flash
 *****/
void cue_loadSceneFromMem(uint8_t cid, uint8_t index)
{
	volatile void* pSceneInPgm = 0;
	volatile void* pCuesInPgm = 0;
	volatile void* pCurrentAnimationSettings = 0;
	volatile void* pCurrScene = 0;
	cli();

	pCuesInPgm = (void*) pgm_read_word(&CUES[ cid ]);
	pSceneInPgm = (void*) pgm_read_word(&SCENES[pgm_read_byte(pCuesInPgm+index)]);
	pCurrScene = &current_scene;
	pCurrentAnimationSettings = &current_animation_settings;

	if(state == STATE_POST_FLASH)
	{
		current_scene.time = pgm_read_byte( pSceneInPgm );
		current_scene.animation = pgm_read_byte( pSceneInPgm+1 );

		memcpy_P((void*)pCurrentAnimationSettings, (void*)pSceneInPgm+2, MAX_SCENE_SIZE);
		current_scene.dmx_data[1] =  0;
		current_scene.dmx_data[12] = 0;
		current_scene.dmx_data[23] = 0;
		current_scene.dmx_data[34] = 0;
		led252_checkColors(1,  (uint8_t*)(current_animation_settings), (uint8_t*)current_colors);
		led252_checkColors(12, (uint8_t*)(current_animation_settings), (uint8_t*)current_colors);
		led252_checkColors(23, (uint8_t*)(current_animation_settings), (uint8_t*)current_colors);
		led252_checkColors(34, (uint8_t*)(current_animation_settings), (uint8_t*)current_colors);
	}
	else
	{
		memcpy_P((void*) pCurrScene, (void*) pSceneInPgm, 2 + MAX_SCENE_SIZE);
		memcpy((void*)pCurrentAnimationSettings, (void*) pCurrScene+2, MAX_SCENE_SIZE);

		led252_checkColors(1,  (uint8_t*)(current_scene.dmx_data), (uint8_t*)current_colors);
		led252_checkColors(12, (uint8_t*)(current_scene.dmx_data), (uint8_t*)current_colors);
		led252_checkColors(23, (uint8_t*)(current_scene.dmx_data), (uint8_t*)current_colors);
		led252_checkColors(34, (uint8_t*)(current_scene.dmx_data), (uint8_t*)current_colors);
	}
	sei();
}

void cue_loadColorSchema(uint8_t colorid)
{
	uint8_t i = 0;
	cli();

	for (i = 0; i < RGB; i++)
	{
		current_colors[DMX_BASE_INDEX][i] = (uint8_t) pgm_read_byte( &COLORS_BASE[ colorid ][ i ]);
		current_colors[DMX_BASEDIM_INDEX][i] = (uint8_t) pgm_read_byte( &COLORS_BASEDIM[ colorid ][i]);
		current_colors[DMX_ALTER_INDEX][i] = (uint8_t) pgm_read_byte( &COLORS_ALTER[ colorid ][ i ]);
		current_colors[DMX_ALTERDIM_INDEX][i] = (uint8_t) pgm_read_byte( &COLORS_ALTERDIM[ colorid ][i]);
	}
	sei();
}

/*****
 * Abarbeitung der aktiven DMX-Cue
 *****/
void cue_handle(uint8_t cid)
{
	static uint8_t last_cid = 255;
	static uint8_t scene_index = 0;
	static uint8_t ticks = 0;

	uint8_t tmp;
	uint8_t animation_alive = 0;

	if (cid == last_cid)
	{
		if (ticks >= current_scene.time)
		{
			tmp = pgm_read_byte((void*)pgm_read_word(&CUES[cid])+scene_index);
			if (tmp == CUE_END_TAG || scene_index == MAX_SCENES_IN_CUE)
			{
				scene_index = 0;
			}
			cue_loadSceneFromMem(cid, scene_index++);
			ticks = 0;
		}
		else
		{
			if(state == STATE_POST_FLASH)
			{
				animation_alive += led252_postFlash(1, (uint8_t*)current_animation_settings, (uint8_t*)current_scene.dmx_data, POST_FLASH_FADE_FACTOR);
				animation_alive += led252_postFlash(12, (uint8_t*)current_animation_settings, (uint8_t*)current_scene.dmx_data, POST_FLASH_FADE_FACTOR);
				animation_alive += led252_postFlash(23, (uint8_t*)current_animation_settings, (uint8_t*)current_scene.dmx_data, POST_FLASH_FADE_FACTOR);
				animation_alive += led252_postFlash(34, (uint8_t*)current_animation_settings, (uint8_t*)current_scene.dmx_data, POST_FLASH_FADE_FACTOR);
				if(!animation_alive)
				{
					ticks = current_scene.time-1;
					state = STATE_START;
				}
				else
				{
					ticks = 0;
				}
			}
			else
			{
				if(current_scene.animation == ANIMATION_BASE_ALTER_FADE && !(ticks%2))
				{
					animation_alive += led252_colorFadeAnimation(1, (uint8_t*)current_animation_settings, (uint8_t*)current_scene.dmx_data, (uint8_t*)current_colors, COLOR_FADE_FACTOR);
					animation_alive += led252_colorFadeAnimation(12, (uint8_t*)current_animation_settings, (uint8_t*)current_scene.dmx_data, (uint8_t*)current_colors, COLOR_FADE_FACTOR);
					animation_alive += led252_colorFadeAnimation(23, (uint8_t*)current_animation_settings, (uint8_t*)current_scene.dmx_data, (uint8_t*)current_colors, COLOR_FADE_FACTOR);
					animation_alive += led252_colorFadeAnimation(34, (uint8_t*)current_animation_settings, (uint8_t*)current_scene.dmx_data, (uint8_t*)current_colors, COLOR_FADE_FACTOR);
					if(!animation_alive)
					{
						ticks = current_scene.time-1;
					} else {
						ticks = 0;
					}
				}

				if(current_scene.animation & ANIMATION_FADE_TO_BASEDIM)
				{
					if(current_scene.animation & ANIMATION_FADE_TO_BASEDIM_FAST)
					{
						tmp = FAST_TO_BASEDIM_FACTOR;
					}
					else if(current_scene.animation & ANIMATION_FADE_TO_BASEDIM_MEDIUM)
					{
						tmp = MEDIUM_TO_BASEDIM_FACTOR;
					}
					else
					{
						tmp = SLOW_TO_BASEDIM_FACTOR;
					}
					animation_alive = 0;
				}
			}
			ticks++;
		}
	}
	else
	{
		cue_loadSceneFromMem(cid, 0);
		ticks = 0;
		last_cid = cid;
		scene_index = 1;
	}

	//dimming
	led252_checkBrightness(1, (uint8_t*)current_animation_settings, (uint8_t*)current_scene.dmx_data, (uint8_t*)dmx_stream, brightness);
	led252_checkBrightness(12, (uint8_t*)current_animation_settings, (uint8_t*)current_scene.dmx_data, (uint8_t*)dmx_stream, brightness);
	led252_checkBrightness(23, (uint8_t*)current_animation_settings, (uint8_t*)current_scene.dmx_data, (uint8_t*)dmx_stream, brightness);
	led252_checkBrightness(34, (uint8_t*)current_animation_settings, (uint8_t*)current_scene.dmx_data, (uint8_t*)dmx_stream, brightness);
}

inline void init_gpio()
{
	DDRC |= (LED_ONBOARD_RED | LED_ONBOARD_YELLOW); // onBoard LEDs
	DDRD |= (LED_BANK | LED_COLOR | LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN ); // Status LEDs
}

inline void init_timer()
{
	OCR0A = DEBOUNCE_COMPARE;
	TCCR0B = (1 << CS02) | (1 << CS00);
	TCCR0A |= (1 << WGM01);
	TIMSK0 |= (1 << OCIE0A);
}

inline void cue_init(uint8_t cue_index, uint8_t color_index)
{
	current_preset = cue_index;
	current_color_id = color_index;
	LED_PORT &= ~LED_MASK;
	LED_PORT |= LED[cue_index];
	cue_loadColorSchema(current_color_id);
}

inline void switchColor() {
	current_color_id = (current_color_id + 1)%2;
	if (current_color_id == 0)
	{
		SET_COLOR_LED;
	}
	else
	{
		CLEAR_COLOR_LED;
	}
}

inline void checkSwitchColor(uint8_t index)
{
	if (current_preset != BANK[current_bank][index])
	{
		current_preset = BANK[current_bank][index];
	}
	else
	{
		switchColor();
		cue_loadColorSchema(current_color_id);
	}
}

inline void calculateDimming()
{
	uint8_t value = (adc_get_value(0) >> 3);

	if(value > 126)
	{
		SET_ONBOARD_YELLOW;
	}
	else
	{
		CLEAR_ONBOARD_YELLOW;
	}

	if(value < 4)
	{
		SET_ONBOARD_RED;
	}
	else
	{
		CLEAR_ONBOARD_RED;
	}

	brightness = value;
}


inline void switchBank()
{
	if(current_bank)
	{
		current_bank = 0;
	}
	else
	{
		current_bank = 1;
	}
}

inline void setLedToPreset(uint8_t preset)
{
		LED_PORT &= ~LED_MASK;
		LED_PORT |= LED[preset];
}

int main(void)
{
	uint8_t i;
	uint8_t num_btns = DEBOUNCE_NUM_BUTTONS;
	uint8_t sw_count = 0;

	init_gpio();
	init_timer();
	cue_init(CUE_STATIC_COLOR, 0);
	dmx_init(MAX_SCENE_SIZE, (uint8_t*)dmx_stream);
	debounce_init();
	adc_init();

	sei();
	dmx_start();

	for (;;)
	{
		if(sw_count == 0)
		{
			calculateDimming();
		}
		else
		{
			sw_count++;
			if(sw_count > 250)
			{
				sw_count = 0;
			}
		}
		for (i = 0; i < num_btns; i++)
		{
			// Flash/Highlight Button
			if (i == BTN_FLASH)
			{
				if (debounce_getKeyPress((1<<i)))
				{
					LED_PORT |= LED[3];
					last_preset = current_preset;
					current_preset = BANK[current_bank][i];
				}
				if (debounce_getKeyDepress((1<<i)))
				{
					LED_PORT &= ~LED[3];
					current_preset = last_preset;
					state = STATE_POST_FLASH;
				}
			}
			else
			{
				if (debounce_getKeyShortpressed((1<<i)))
				{
					state = STATE_START;
					setLedToPreset(i);
					checkSwitchColor(i);
				}
				if( debounce_getKeyLongppressed((1<<i)))
				{
					state = STATE_START;
					switchBank();
					setLedToPreset(i);
					current_preset = BANK[current_bank][i];
					if (i == BTN_COLOR_CHANGE || i == BTN_EFFECT )
					{
						cue_loadColorSchema(current_color_id);
					}
				}
			}
		}
	}
	return 0;
}

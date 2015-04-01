/*
 * dmx_settings_2-indirect.h
 *
 * Setup for 2 RGB bars in indirect use.
 *
 * Author: @dreimalbe
 */
#ifndef DMX_SETTING_FRONT_H_
#define DMX_SETTING_FRONT_H_

#include "../devices/dmx_device_dose.h"
#include "../devices/dmx_device_ledbar252.h"

#define _SETTINGS_DMX_4

/* BASE COLORS *************************
 ***************************************/
const uint8_t COLORS_BASE[NUM_COLORS][RGB] PROGMEM = {
	{ DMX_252_COLOR_RED },	//0
	{ DMX_252_COLOR_BLUE },	//1
	{ DMX_252_COLOR_TEAL },	//2
	{ DMX_252_COLOR_WHITE },	//3
	{ DMX_252_COLOR_BLACK }	//4
};

const uint8_t COLORS_BASEDIM[NUM_COLORS][RGB] PROGMEM = {
	{ DMX_252_COLOR_RED_DIM },	//0
	{ DMX_252_COLOR_BLUE_DIM },	//1
	{ DMX_252_COLOR_BLUE_DIM },	//2
	{ DMX_252_COLOR_WHITE },	//3
	{ DMX_252_COLOR_BLACK }	//4
};

const uint8_t COLORS_ALTER[NUM_COLORS][RGB] PROGMEM = {
	{ DMX_252_COLOR_ORANGE },	//0
	{ DMX_252_COLOR_TEAL },	//1
	{ DMX_252_COLOR_BLUE },	//2
	{ DMX_252_COLOR_WHITE },	//3
	{ DMX_252_COLOR_BLACK }	//4
};

const uint8_t COLORS_ALTERDIM[NUM_COLORS][RGB] PROGMEM = {
	{ DMX_252_COLOR_RED_DIM },	//0
	{ DMX_252_COLOR_BLUE_DIM },	//1
	{ DMX_252_COLOR_BLUE_DIM },	//2
	{ DMX_252_COLOR_WHITE },	//3
	{ DMX_252_COLOR_BLACK }	//4
};
/**************************************************/

#define CUE_END_TAG			255
#define MAX_SCENES_IN_CUE	12
#define NUM_OF_SCENES		21

// CONFIG
// DMX1: LedBar1
// DMX12: LedBar2
// DMX23: LedBar3
// DMX34: LedBar4

const scene_t s_blackout PROGMEM 		= { 10, ANIMATION_NONE, { DMX_252__1_Off, 	 DMX_252__1_Off	 } };
const scene_t s_flash_all PROGMEM 		= { 10, ANIMATION_NONE, { DMX_252__1F_White, DMX_252__1F_White, } };

const scene_t s_static_color PROGMEM 	= { 10, ANIMATION_FADE_TO_BASEDIM_FAST, { DMX_252__1_BaseDim, DMX_252__1_BaseDim } };
const scene_t s_fading_color_1 PROGMEM 	= { 40, ANIMATION_BASE_ALTER_FADE, { DMX_252__3_BaseAlter, DMX_252__3_AlterBase  } };
const scene_t s_fading_color_2 PROGMEM 	= { 40, ANIMATION_BASE_ALTER_FADE, { DMX_252__3_AlterBase, DMX_252__3_BaseAlter  } };

const scene_t s_strobe_base PROGMEM = { 8, ANIMATION_NONE, { DMX_252__1_BaseDim, DMX_252__1_BaseDim } };
const scene_t s_strobe00 PROGMEM    = { 12, ANIMATION_NONE, { DMX_252__Strobe_w_w, DMX_252__Strobe_Bw_ } };
const scene_t s_strobe01 PROGMEM    = { 10, ANIMATION_NONE, { DMX_252__Strobe_BwB, DMX_252__Strobe__ww } };
const scene_t s_strobe02 PROGMEM    = { 10, ANIMATION_NONE, { DMX_252__Strobe__Bw, DMX_252__Strobe_wBw } };
const scene_t s_strobe03 PROGMEM    = { 8, ANIMATION_NONE, { DMX_252__Strobe_wwB, DMX_252__Strobe_BwB } };
const scene_t s_strobe04 PROGMEM    = { 10, ANIMATION_NONE, { DMX_252__Strobe_wBw, DMX_252__Strobe__w_ } };
const scene_t s_strobe05 PROGMEM    = { 12, ANIMATION_NONE, { DMX_252__Strobe__ww, DMX_252__Strobe_w_w } };
const scene_t s_strobe06 PROGMEM    = { 8, ANIMATION_NONE, { DMX_252__Strobe_Bw_, DMX_252__Strobe_wBw } };
const scene_t s_strobe07 PROGMEM    = { 10, ANIMATION_NONE, { DMX_252__Strobe__ww, DMX_252__Strobe_w_w } };

// Lauflicht
const scene_t s_run00 PROGMEM = { 80, ANIMATION_FADE_TO_BASEDIM, { DMX_252__Run_0, DMX_252__Run_0 } };
const scene_t s_run01 PROGMEM = { 80, ANIMATION_FADE_TO_BASEDIM, { DMX_252__Run_1, DMX_252__Run_1 } };
const scene_t s_run02 PROGMEM = { 80, ANIMATION_FADE_TO_BASEDIM, { DMX_252__Run_2, DMX_252__Run_2 } };
const scene_t s_run03 PROGMEM = { 80, ANIMATION_FADE_TO_BASEDIM, { DMX_252__Run_3, DMX_252__Run_3 } };


const scene_t* SCENES[] PROGMEM = {
	  &s_blackout,			//0
	  &s_static_color,		//1
	  &s_fading_color_1,	//2
	  &s_fading_color_2,	//3
	  &s_flash_all,			//4
	  &s_strobe_base,		//5
	  &s_strobe00,			//6
	  &s_strobe01,			//7
	  &s_strobe02,			//8
	  &s_strobe03,			//9
	  &s_strobe04,			//10
	  &s_strobe05,			//11
	  &s_strobe06,			//12
	  &s_strobe07,			//13
	  &s_run00,				//14
	  &s_run01,				//15
	  &s_run02,				//16
	  &s_run03				//17
};

#define SCENE_BLACKOUT 			0
#define SCENE_STATIC_COLOR		1
#define SCENE_FADING_COLOR_1	2
#define SCENE_FADING_COLOR_2	3
#define SCENE_FLASH_ALL			4
#define SCENE_STROBE_BASE 		5
#define SCENE_RUN_BASE 			14

/*******************************************************/
const uint8_t c_blackout[] PROGMEM = { SCENE_BLACKOUT, CUE_END_TAG };
const uint8_t c_static_color[] PROGMEM = { SCENE_STATIC_COLOR, CUE_END_TAG };
const uint8_t c_color_strobe[] PROGMEM =
	{ SCENE_STROBE_BASE+1, SCENE_STROBE_BASE+2,  SCENE_STROBE_BASE, SCENE_STROBE_BASE+2,  SCENE_STROBE_BASE+3,
	 SCENE_STROBE_BASE+4,  SCENE_STROBE_BASE,  SCENE_STROBE_BASE+5,  SCENE_STROBE_BASE, SCENE_STROBE_BASE+6,
	 SCENE_STROBE_BASE+7, CUE_END_TAG };

const uint8_t c_flash[] PROGMEM = { SCENE_FLASH_ALL, CUE_END_TAG };
const uint8_t c_run[] PROGMEM = { SCENE_RUN_BASE, SCENE_RUN_BASE+1, SCENE_RUN_BASE+2, SCENE_RUN_BASE+3, CUE_END_TAG };

const uint8_t c_fading_color[] PROGMEM = { SCENE_FADING_COLOR_1, SCENE_FADING_COLOR_2, CUE_END_TAG };

#define CUE_BLACKOUT 0
#define CUE_STATIC_COLOR 1
#define CUE_FADING_COLOR 2
#define CUE_COLORSTROBE 3
#define CUE_FLASH 4
#define CUE_RUNLIGHT 5

const void* CUES[] PROGMEM = {
	  &c_blackout,	//0
	  &c_static_color,	//1
	  &c_fading_color,   //2
	  &c_color_strobe,	//3
	  &c_flash,			//4
	  &c_run			//5

};

#endif /* DMX_SETTING_FRONT_H_ */

#include "dmx_device_ledbar252.h"

uint8_t led252_fade(uint8_t* from, uint8_t* to, uint8_t fade_factor)
{
	uint8_t alive = 0;
	if(*from > *to)
	{
		if((*from-fade_factor) < *to)
		{
			*from = *to;
		}
		else
		{
			*from -= fade_factor;
			alive = 1;
		}
	}
	else if(*from < *to)
	{
		if((*from+fade_factor) > *to)
		{
			*from = *to;
		}
		else
		{
			*from += fade_factor;
			alive = 1;
		}
	}
	return alive;
}

uint8_t led252_postFlash(uint16_t channel, uint8_t* current_animation_settings, uint8_t* dmx_data, uint8_t fade_factor)
{
	uint8_t i = 0, alive = 0;
	for( i = channel+1; i < channel+DMX_252_NUM_CHANNELS-1; i++)
	{
		alive += led252_fade(&dmx_data[i], &current_animation_settings[i], fade_factor);
	}
	return alive;
}

inline uint8_t led252_doColorFade(uint16_t channel, uint8_t color_index, uint8_t fade_factor, uint8_t rgb, uint8_t* dmx_data, uint8_t* current_colors)
{
	return led252_fade(&dmx_data[channel], &current_colors[color_index * RGB + rgb], fade_factor);
}


uint8_t led252_fadeToBaseDimAnimation(uint16_t channel, uint8_t* current_animation_settings, uint8_t* dmx_data, uint8_t* current_colors, uint8_t fadefactor)
{
	uint8_t i = 0;
	uint8_t rgb = 0;
	uint8_t alive = 0;
	for(i = 1; i < DMX_252_NUM_CHANNELS-1; i++)
	{
		alive += led252_doColorFade(channel+i, DMX_BASEDIM_INDEX, fadefactor, rgb, dmx_data, current_colors);
		rgb = (rgb+1)%RGB;
	}
	return alive;
}

uint8_t led252_colorFadeAnimation(uint16_t channel, uint8_t* current_animation_settings, uint8_t* dmx_data, uint8_t* current_colors, uint8_t fadefactor)
{
	uint8_t i = 0;
	uint8_t rgb = 0;
	uint8_t alive = 0;
	for(i = 1; i < DMX_252_NUM_CHANNELS-1; i++)
	{
		switch(current_animation_settings[channel+i])
		{
			case DMX_ALTER_ID:
				alive += led252_doColorFade(channel+i, DMX_BASE_INDEX, fadefactor, rgb, dmx_data, current_colors);
				break;
			case DMX_BASE_ID:
				alive += led252_doColorFade(channel+i, DMX_ALTER_INDEX, fadefactor, rgb, dmx_data, current_colors);
				break;
			case DMX_BASEDIM_ID:
				alive += led252_doColorFade(channel+i, DMX_ALTERDIM_INDEX, fadefactor, rgb, dmx_data, current_colors);
				break;
			case DMX_ALTERDIM_ID:
				alive += led252_doColorFade(channel+i, DMX_BASEDIM_INDEX, fadefactor, rgb, dmx_data, current_colors);
				break;
		}
		rgb = (rgb+1)%RGB;
	}
	return alive;
}

void led252_checkColors(uint16_t channel, uint8_t* dmx_data, uint8_t* current_colors)
{
	uint8_t i = 0;
	uint8_t rgb = 0;
	for (i = 1; i < DMX_252_NUM_CHANNELS-1; i++)
	{
		switch(dmx_data[channel+i])
		{
			case DMX_ALTER_ID:
				dmx_data[channel+i] = current_colors[DMX_ALTER_INDEX * RGB + rgb];
				break;
			case DMX_BASE_ID:
				dmx_data[channel+i] = current_colors[DMX_BASE_INDEX * RGB + rgb];
				break;
			case DMX_ALTERDIM_ID:
				dmx_data[channel+i] = current_colors[DMX_ALTERDIM_INDEX  * RGB + rgb];
				break;
			case DMX_BASEDIM_ID:
				dmx_data[channel+i] = current_colors[DMX_BASEDIM_INDEX * RGB + rgb];
				break;
			default:
				break;
		}
		rgb = (rgb+1) % RGB;
	}
}

void led252_checkBrightness(uint16_t basechannel, uint8_t* current_animation_settings, uint8_t* dmx_data_in, uint8_t* dmx_data_out, uint8_t brightness)
{
	uint8_t i = 0;
	uint8_t channel = basechannel-1;
	uint16_t tmp = 0;

	dmx_data_out[channel]=dmx_data_in[channel]; // copy MODE
	dmx_data_out[channel+1]=dmx_data_in[channel+1]; // copy SHUTTER

	for(i = channel+2; i < (channel+DMX_252_NUM_CHANNELS); i++)
	{
		switch(current_animation_settings[i])
		{
			case DMX_ALTERDIM_ID:
			case DMX_ALTER_ID:
			case DMX_BASEDIM_ID:
			case DMX_BASE_ID:
				tmp = dmx_data_in[i]*brightness;
				tmp = tmp / DIMMING_ADC_MAX;
				dmx_data_out[i] =(uint8_t)tmp;
				break;
			default:
				dmx_data_out[i] = dmx_data_in[i];
		}
	}
}

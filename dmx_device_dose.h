#ifndef DMX_DEVICE_DOSE_H
#define DMX_DEVICE_DOSE_H

#define DOSE_CHANNEL 	4

#define DOSE_POWER		0x03
#define DOSE_POWER_0	0x00
#define DOSE_POWER_MIN	0x01
#define	DOSE_POWER_HALF	0x02
#define DOSE_POWER_FULL	0x03

#define DOSE_STROBE		0x0C
#define DOSE_STROBE_0	0x00
#define DOSE_STROBE_1	0x04
#define DOSE_STROBE_4	0x08
#define DOSE_STROBE_10	0x0C

#define DOSE_OFF		0x00
#define DOSE_RED_MIN		DOSE_POWER_MIN
#define DOSE_RED_HALF	DOSE_POWER_HALF
#define DOSE_RED_FULL	DOSE_POWER_FULL
#define DOSE_RED_FULL_10	( DOSE_POWER_FULL | DOSE_STROBE_10 )

#define DOSE_WHITE_MIN	( DOSE_POWER_MIN << DOSE_CHANNEL )
#define DOSE_WHITE_HALF	( DOSE_POWER_HALF << DOSE_CHANNEL )
#define DOSE_WHITE_FULL	( DOSE_POWER_FULL << DOSE_CHANNEL )
#define DOSE_WHITE_FULL_1 ( ( DOSE_POWER_FULL | DOSE_STROBE_1 ) << DOSE_CHANNEL )
#define DOSE_WHITE_HALF_4 ( ( DOSE_POWER_FULL | DOSE_STROBE_4 ) << DOSE_CHANNEL )
#define DOSE_WHITE_FULL_10 ( ( DOSE_POWER_FULL | DOSE_STROBE_10 ) << DOSE_CHANNEL )

#define DOSE_MODE_STROBE	( DOSE_RED_FULL_10 | DOSE_WHITE_FULL_1 )
#define DOSE_MODE_FLASH		( DOSE_RED_MIN | DOSE_WHITE_FULL_10 )

#endif

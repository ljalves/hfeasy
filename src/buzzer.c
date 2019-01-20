/* HFeasy

Copyright (c) 2019 Luis Alves

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#include "hfeasy.h"


#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

static hftimer_handle_t buzzer_timer;

struct buzzer_sound {
	uint16_t func;
	uint16_t freq;
	uint16_t time; /* 10's of ms */
	uint16_t value;
};

enum {
	BUZZER_FUNC_END,
	BUZZER_FUNC_TIME,
	BUZZER_FUNC_INC,
	BUZZER_FUNC_DEC,
};

static const struct buzzer_sound buzzer_tone1[] = {
	{ BUZZER_FUNC_TIME, 300, 10, 0 },
	{ BUZZER_FUNC_INC, 200, 5, 10 },
	{ BUZZER_FUNC_END },
};

static const struct buzzer_sound buzzer_tone2[] = {
	{ BUZZER_FUNC_TIME, 400, 10, 0 },
	{ BUZZER_FUNC_TIME, 200, 10, 0 },
	{ BUZZER_FUNC_INC, 200, 10, 20 },
	{ BUZZER_FUNC_END },
};

static const struct buzzer_sound buzzer_tone3[] = {
	{ BUZZER_FUNC_INC, 200, 10, 20 },
	{ BUZZER_FUNC_INC, 200, 10, 20 },
	{ BUZZER_FUNC_INC, 200, 10, 20 },
	{ BUZZER_FUNC_INC, 200, 10, 20 },
	{ BUZZER_FUNC_END },
};

static const struct buzzer_sound buzzer_tone4[] = {
	{ BUZZER_FUNC_TIME, 2637, 12, 0 },
	{ BUZZER_FUNC_TIME, 2637, 12, 0 },
	{ BUZZER_FUNC_TIME, 0, 12, 0 },
	{ BUZZER_FUNC_TIME, 2637, 12, 0 },
	{ BUZZER_FUNC_TIME, 0, 12, 0 },
	{ BUZZER_FUNC_TIME, 2093, 12, 0 },
	{ BUZZER_FUNC_TIME, 2637, 12, 0 },
	{ BUZZER_FUNC_TIME, 0, 12, 0 },
	{ BUZZER_FUNC_TIME, 3136, 12, 0 },
	{ BUZZER_FUNC_TIME, 0, 12, 0 },
	{ BUZZER_FUNC_TIME, 0, 12, 0 },
	{ BUZZER_FUNC_TIME, 0, 12, 0 },
	{ BUZZER_FUNC_TIME, 1568, 12, 0 },
	{ BUZZER_FUNC_TIME, 0, 12, 0 },
	{ BUZZER_FUNC_TIME, 0, 12, 0 },
	{ BUZZER_FUNC_TIME, 0, 12, 0 },
	{ BUZZER_FUNC_TIME, 2093, 12, 0 },
	{ BUZZER_FUNC_TIME, 0, 12, 0 },
	{ BUZZER_FUNC_TIME, 0, 12, 0 },
	{ BUZZER_FUNC_TIME, 1568, 12, 0 },
	{ BUZZER_FUNC_TIME, 0, 12, 0 },
	{ BUZZER_FUNC_TIME, 0, 12, 0 },
	{ BUZZER_FUNC_TIME, 1319, 12, 0 },
	{ BUZZER_FUNC_TIME, 0, 12, 0 },
	{ BUZZER_FUNC_TIME, 0, 12, 0 },
	{ BUZZER_FUNC_TIME, 1760, 12, 0 },
	{ BUZZER_FUNC_TIME, 0, 12, 0 },
	{ BUZZER_FUNC_TIME, 1976, 12, 0 },
	{ BUZZER_FUNC_TIME, 0, 12, 0 },
	{ BUZZER_FUNC_TIME, 1865, 12, 0 },
	{ BUZZER_FUNC_TIME, 1760, 12, 0 },
	{ BUZZER_FUNC_TIME, 0, 12, 0 },
	{ BUZZER_FUNC_TIME, 1568, 9, 0 },
	{ BUZZER_FUNC_TIME, 2637, 9, 0 },
	{ BUZZER_FUNC_TIME, 3136, 9, 0 },
	{ BUZZER_FUNC_TIME, 3520, 12, 0 },
	{ BUZZER_FUNC_TIME, 0, 12, 0 },
	{ BUZZER_FUNC_TIME, 2794, 12, 0 },
	{ BUZZER_FUNC_TIME, 3136, 12, 0 },
	{ BUZZER_FUNC_TIME, 0, 12, 0 },
	{ BUZZER_FUNC_TIME, 2637, 12, 0 },
	{ BUZZER_FUNC_TIME, 0, 12, 0 },
	{ BUZZER_FUNC_TIME, 2093, 12, 0 },
	{ BUZZER_FUNC_TIME, 2349, 12, 0 },
	{ BUZZER_FUNC_TIME, 1976, 12, 0 },
	{ BUZZER_FUNC_TIME, 0, 12, 0 },
	{ BUZZER_FUNC_TIME, 0, 12, 0 },
	{ BUZZER_FUNC_TIME, 2093, 12, 0 },
	{ BUZZER_FUNC_TIME, 0, 12, 0 },
	{ BUZZER_FUNC_TIME, 0, 12, 0 },
	{ BUZZER_FUNC_TIME, 1568, 12, 0 },
	{ BUZZER_FUNC_TIME, 0, 12, 0 },
	{ BUZZER_FUNC_TIME, 0, 12, 0 },
	{ BUZZER_FUNC_TIME, 1319, 12, 0 },
	{ BUZZER_FUNC_TIME, 0, 12, 0 },
	{ BUZZER_FUNC_TIME, 0, 12, 0 },
	{ BUZZER_FUNC_TIME, 1760, 12, 0 },
	{ BUZZER_FUNC_TIME, 0, 12, 0 },
	{ BUZZER_FUNC_TIME, 1976, 12, 0 },
	{ BUZZER_FUNC_TIME, 0, 12, 0 },
	{ BUZZER_FUNC_TIME, 1865, 12, 0 },
	{ BUZZER_FUNC_TIME, 1760, 12, 0 },
	{ BUZZER_FUNC_TIME, 0, 12, 0 },
	{ BUZZER_FUNC_TIME, 1568, 9, 0 },
	{ BUZZER_FUNC_TIME, 2637, 9, 0 },
	{ BUZZER_FUNC_TIME, 3136, 9, 0 },
	{ BUZZER_FUNC_TIME, 3520, 12, 0 },
	{ BUZZER_FUNC_TIME, 0, 12, 0 },
	{ BUZZER_FUNC_TIME, 2794, 12, 0 },
	{ BUZZER_FUNC_TIME, 3136, 12, 0 },
	{ BUZZER_FUNC_TIME, 0, 12, 0 },
	{ BUZZER_FUNC_TIME, 2637, 12, 0 },
	{ BUZZER_FUNC_TIME, 0, 12, 0 },
	{ BUZZER_FUNC_TIME, 2093, 12, 0 },
	{ BUZZER_FUNC_TIME, 2349, 12, 0 },
	{ BUZZER_FUNC_TIME, 1976, 12, 0 },
	{ BUZZER_FUNC_TIME, 0, 12, 0 },
	{ BUZZER_FUNC_TIME, 0, 12, 0 },
	{ BUZZER_FUNC_END },
};


static const struct buzzer_sound *buzzer_tones[] = {
	buzzer_tone1,
	buzzer_tone2,
	buzzer_tone3,
	buzzer_tone4,
};


static struct buzzer_state_s {
	const struct buzzer_sound *tone;
	uint16_t freq;
	uint8_t	cnt;
} buzzer_state;
	

static void USER_FUNC buzzer_timer_handler(hftimer_handle_t timer)
{
	switch (buzzer_state.tone->func) {
		default:
		case BUZZER_FUNC_END:
			hfgpio_pwm_disable(GPIO_BUZZER);
			hfgpio_fset_out_low(GPIO_BUZZER);
			break;
		case BUZZER_FUNC_TIME:
			if (buzzer_state.cnt >= buzzer_state.tone->time) {
				buzzer_state.tone++;
				buzzer_state.cnt = 0;
				hftimer_start(buzzer_timer);
				break;
			} else if (buzzer_state.cnt == 0) {
				hfgpio_pwm_enable(GPIO_BUZZER, buzzer_state.tone->freq, 1);
			}
			buzzer_state.cnt++;
			hftimer_start(buzzer_timer);
			break;
		case BUZZER_FUNC_INC:
		case BUZZER_FUNC_DEC:
			if (buzzer_state.cnt == buzzer_state.tone->time) {
				buzzer_state.tone++;
				buzzer_state.cnt = 0;
				hftimer_start(buzzer_timer);
				break;
			} else if (buzzer_state.cnt == 0) {
				buzzer_state.freq = buzzer_state.tone->freq;
			}
			hfgpio_pwm_enable(GPIO_BUZZER, buzzer_state.freq, 1);
			buzzer_state.cnt++;
			if (buzzer_state.tone->func == BUZZER_FUNC_INC)
				buzzer_state.freq += buzzer_state.tone->value;
			else
				buzzer_state.freq -= buzzer_state.tone->value;
			hftimer_start(buzzer_timer);
			break;
	}
}

void USER_FUNC buzzer_play(uint8_t tone)
{
	hftimer_stop(buzzer_timer);
	hfgpio_pwm_disable(GPIO_BUZZER);
	hfgpio_fset_out_low(GPIO_BUZZER);

	buzzer_state.cnt = 0;
	buzzer_state.tone = buzzer_tones[tone];
	hftimer_start(buzzer_timer);
}



void USER_FUNC buzzer_init(void)
{
#if defined(__LPT100F__)
	hfgpio_configure_fpin(GPIO_BUZZER, HFM_IO_OUTPUT_0);
	buzzer_timer = hftimer_create("buzzer", 10, false, HFTIMER_ID_BUZZER, buzzer_timer_handler, 0);
#endif
}

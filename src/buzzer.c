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

#ifdef HAS_BUZZER

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


int notes[] = { 0,
262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494,
523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932, 988,
1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976,
2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951
};


const char song1[] = "The Simpsons:d=4,o=5,b=160:c.6,e6,f#6,8a6,g.6,e6,c6,8a,8f#,8f#,8f#,2g,8p,8p,8f#,8f#,8f#,8g,a#.,8c6,8c6,8c6,c6";
const char song2[] = "Indiana:d=4,o=5,b=250:e,8p,8f,8g,8p,1c6,8p.,d,8p,8e,1f,p.,g,8p,8a,8b,8p,1f6,p,a,8p,8b,2c6,2d6,2e6,e,8p,8f,8g,8p,1c6,p,d6,8p,8e6,1f.6,g,8p,8g,e.6,8p,d6,8p,8g,e.6,8p,d6,8p,8g,f.6,8p,e6,8p,8d6,2c6";
const char song3[] = "TakeOnMe:d=4,o=4,b=160:8f#5,8f#5,8f#5,8d5,8p,8b,8p,8e5,8p,8e5,8p,8e5,8g#5,8g#5,8a5,8b5,8a5,8a5,8a5,8e5,8p,8d5,8p,8f#5,8p,8f#5,8p,8f#5,8e5,8e5,8f#5,8e5,8f#5,8f#5,8f#5,8d5,8p,8b,8p,8e5,8p,8e5,8p,8e5,8g#5,8g#5,8a5,8b5,8a5,8a5,8a5,8e5,8p,8d5,8p,8f#5,8p,8f#5,8p,8f#5,8e5,8e5";
const char song4[] = "Entertainer:d=4,o=5,b=140:8d,8d#,8e,c6,8e,c6,8e,2c.6,8c6,8d6,8d#6,8e6,8c6,8d6,e6,8b,d6,2c6,p,8d,8d#,8e,c6,8e,c6,8e,2c.6,8p,8a,8g,8f#,8a,8c6,e6,8d6,8c6,8a,2d6";
const char song5[] = "Muppets:d=4,o=5,b=250:c6,c6,a,b,8a,b,g,p,c6,c6,a,8b,8a,8p,g.,p,e,e,g,f,8e,f,8c6,8c,8d,e,8e,8e,8p,8e,g,2p,c6,c6,a,b,8a,b,g,p,c6,c6,a,8b,a,g.,p,e,e,g,f,8e,f,8c6,8c,8d,e,8e,d,8d,c";
const char song6[] = "Xfiles:d=4,o=5,b=125:e,b,a,b,d6,2b.,1p,e,b,a,b,e6,2b.,1p,g6,f#6,e6,d6,e6,2b.,1p,g6,f#6,e6,d6,f#6,2b.,1p,e,b,a,b,d6,2b.,1p,e,b,a,b,e6,2b.,1p,e6,2b.";
const char song7[] = "Looney:d=4,o=5,b=140:32p,c6,8f6,8e6,8d6,8c6,a.,8c6,8f6,8e6,8d6,8d#6,e.6,8e6,8e6,8c6,8d6,8c6,8e6,8c6,8d6,8a,8c6,8g,8a#,8a,8f";
const char song8[] = "20thCenFox:d=16,o=5,b=140:b,8p,b,b,2b,p,c6,32p,b,32p,c6,32p,b,32p,c6,32p,b,8p,b,b,b,32p,b,32p,b,32p,b,32p,b,32p,b,32p,b,32p,g#,32p,a,32p,b,8p,b,b,2b,4p,8e,8g#,8b,1c#6,8f#,8a,8c#6,1e6,8a,8c#6,8e6,1e6,8b,8g#,8a,2b";
const char song9[] = "Bond:d=4,o=5,b=80:32p,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d#6,16d#6,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d6,16c#6,16c#7,c.7,16g#6,16f#6,g#.6";
const char song10[] = "MASH:d=8,o=5,b=140:4a,4g,f#,g,p,f#,p,g,p,f#,p,2e.,p,f#,e,4f#,e,f#,p,e,p,4d.,p,f#,4e,d,e,p,d,p,e,p,d,p,2c#.,p,d,c#,4d,c#,d,p,e,p,4f#,p,a,p,4b,a,b,p,a,p,b,p,2a.,4p,a,b,a,4b,a,b,p,2a.,a,4f#,a,b,p,d6,p,4e.6,d6,b,p,a,p,2b";
const char song11[] = "StarWars:d=4,o=5,b=45:32p,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#.6,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#6";
const char song12[] = "GoodBad:d=4,o=5,b=56:32p,32a#,32d#6,32a#,32d#6,8a#.,16f#.,16g#.,d#,32a#,32d#6,32a#,32d#6,8a#.,16f#.,16g#.,c#6,32a#,32d#6,32a#,32d#6,8a#.,16f#.,32f.,32d#.,c#,32a#,32d#6,32a#,32d#6,8a#.,16g#.,d#";
const char song13[] = "TopGun:d=4,o=4,b=31:32p,16c#,16g#,16g#,32f#,32f,32f#,32f,16d#,16d#,32c#,32d#,16f,32d#,32f,16f#,32f,32c#,16f,d#,16c#,16g#,16g#,32f#,32f,32f#,32f,16d#,16d#,32c#,32d#,16f,32d#,32f,16f#,32f,32c#,g#";
const char song14[] = "A-Team:d=8,o=5,b=125:4d#6,a#,2d#6,16p,g#,4a#,4d#.,p,16g,16a#,d#6,a#,f6,2d#6,16p,c#.6,16c6,16a#,g#.,2a#";
const char song15[] = "Flinstones:d=4,o=5,b=40:32p,16f6,16a#,16a#6,32g6,16f6,16a#.,16f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c6,d6,16f6,16a#.,16a#6,32g6,16f6,16a#.,32f6,32f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c6,a#,16a6,16d.6,16a#6,32a6,32a6,32g6,32f#6,32a6,8g6,16g6,16c.6,32a6,32a6,32g6,32g6,32f6,32e6,32g6,8f6,16f6,16a#.,16a#6,32g6,16f6,16a#.,16f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c.6,32d6,32d#6,32f6,16a#,16c.6,32d6,32d#6,32f6,16a#6,16c7,8a#.6";
const char song16[] = "Jeopardy:d=4,o=6,b=125:c,f,c,f5,c,f,2c,c,f,c,f,a.,8g,8f,8e,8d,8c#,c,f,c,f5,c,f,2c,f.,8d,c,a#5,a5,g5,f5,p,d#,g#,d#,g#5,d#,g#,2d#,d#,g#,d#,g#,c.7,8a#,8g#,8g,8f,8e,d#,g#,d#,g#5,d#,g#,2d#,g#.,8f,d#,c#,c,p,a#5,p,g#.5,d#,g#";
const char song17[] = "Gadget:d=16,o=5,b=50:32d#,32f,32f#,32g#,a#,f#,a,f,g#,f#,32d#,32f,32f#,32g#,a#,d#6,4d6,32d#,32f,32f#,32g#,a#,f#,a,f,g#,f#,8d#";
const char song18[] = "Smurfs:d=32,o=5,b=200:4c#6,16p,4f#6,p,16c#6,p,8d#6,p,8b,p,4g#,16p,4c#6,p,16a#,p,8f#,p,8a#,p,4g#,4p,g#,p,a#,p,b,p,c6,p,4c#6,16p,4f#6,p,16c#6,p,8d#6,p,8b,p,4g#,16p,4c#6,p,16a#,p,8b,p,8f,p,4f#";
const char song19[] = "MahnaMahna:d=16,o=6,b=125:c#,c.,b5,8a#.5,8f.,4g#,a#,g.,4d#,8p,c#,c.,b5,8a#.5,8f.,g#.,8a#.,4g,8p,c#,c.,b5,8a#.5,8f.,4g#,f,g.,8d#.,f,g.,8d#.,f,8g,8d#.,f,8g,d#,8c,a#5,8d#.,8d#.,4d#,8d#.";
const char song20[] = "LeisureSuit:d=16,o=6,b=56:f.5,f#.5,g.5,g#5,32a#5,f5,g#.5,a#.5,32f5,g#5,32a#5,g#5,8c#.,a#5,32c#,a5,a#.5,c#.,32a5,a#5,32c#,d#,8e,c#.,f.,f.,f.,f.,f,32e,d#,8d,a#.5,e,32f,e,32f,c#,d#.,c#";
const char song21[] = "MissionImp:d=16,o=6,b=95:32d,32d#,32d,32d#,32d,32d#,32d,32d#,32d,32d,32d#,32e,32f,32f#,32g,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,a#,g,2d,32p,a#,g,2c#,32p,a#,g,2c,a#5,8c,2p,32p,a#5,g5,2f#,32p,a#5,g5,2f,32p,a#5,g5,2e,d#,8d";

const char *songs[] = {
	song1,
	song2,
	song3,
	song4,
	song5,
	song6,
	song7,
	song8,
	song9,
	song10,
	song11,
	song12,
	song13,
	song14,
	song15,
	song16,
	song17,
	song18,
	song19,
	song20,
	song21,
};	
	

#define isdigit(n) (n >= '0' && n <= '9')


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
	const char *song;
	const struct buzzer_sound *tone;
	uint16_t freq;
	uint8_t	cnt;
} buzzer_state;

#if 1

#define OCTAVE_OFFSET -1

static void USER_FUNC buzzer_timer_handler(hftimer_handle_t timer)
{
	static const char *p;
	static uint8_t state = 0;

  static uint8_t default_dur = 4;
  static uint8_t default_oct = 6;
  static int bpm = 63;
  int num;
  static long wholenote;
  long duration;
  uint8_t note;
  uint8_t scale;
	
u_printf(".%d.", state);
	switch (state) {
		case 0:
			/* init */
			u_printf("init\r\n");
			p = buzzer_state.song;
			
			if (p == NULL)
				return;
			
			while (*p != ':')
				p++;
			p++;
			
			// get default duration
			if(*p == 'd') {
				p++; p++;              // skip "d="
				num = 0;
				while (isdigit(*p)) {
					num = (num * 10) + (*p++ - '0');
				}
				if(num > 0)
					default_dur = num;
				p++;                   // skip comma
			}

			// get default octave
			if(*p == 'o') {
				p++; p++;              // skip "o="
				num = *p++ - '0';
				if(num >= 3 && num <=7)
					default_oct = num;
				p++;                   // skip comma
			}

			// get BPM
			if(*p == 'b')
			{
				p++; p++;              // skip "b="
				num = 0;
				while(isdigit(*p)) {
					num = (num * 10) + (*p++ - '0');
				}
				bpm = num;
				p++;                   // skip colon
			}

			// BPM usually expresses the number of quarter notes per minute
			wholenote = (60 * 1000L / bpm) * 4;  // this is the time for whole note (in milliseconds)

			u_printf("starting tune %d %d\r\n", bpm, wholenote);
			
			state = 1;
			hftimer_start(buzzer_timer);
			break;
		case 1:
			hfgpio_pwm_disable(GPIO_BUZZER);
			hfgpio_fset_out_low(GPIO_BUZZER);
			u_printf("p");

			if (*p == '\0') {
				state = 2;
				hftimer_change_period(timer, 10); // start(buzzer_timer);
				break;
			}
			
			// first, get note duration, if available
			num = 0;
			while (isdigit(*p))
				num = (num * 10) + (*p++ - '0');
    
			if (num)
				duration = wholenote / num;
			else
				duration = wholenote / default_dur;  // we will need to check if we are a dotted note after

			// now get the note
			note = 0;

			switch(*p)
			{
				case 'c':
					note = 1;
					break;
				case 'd':
					note = 3;
					break;
				case 'e':
					note = 5;
					break;
				case 'f':
					note = 6;
					break;
				case 'g':
					note = 8;
					break;
				case 'a':
					note = 10;
					break;
				case 'b':
					note = 12;
					break;
				case 'p':
				default:
					note = 0;
			}
			p++;

			// now, get optional '#' sharp
			if(*p == '#') {
				note++;
				p++;
			}

			// now, get optional '.' dotted note
			if(*p == '.') {
				duration += duration/2;
				p++;
			}
  
			// now, get scale
			if (isdigit(*p)) {
				scale = *p - '0';
				p++;
			} else {
				scale = default_oct;
			}

			scale += OCTAVE_OFFSET;
//const char song11[] = "StarWars:d=4,o=5,b=45:32p,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#.6,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#6";

			if(*p == ',')
				p++;       // skip comma for next note (or we may be at the end)

			// now play the note
			if (note) {
				uint16_t freq = (uint16_t) notes[(scale - 4) * 12 + note];
				hfgpio_pwm_enable(GPIO_BUZZER, freq, 1);
			} else {
				hfgpio_pwm_disable(GPIO_BUZZER);
				hfgpio_fset_out_low(GPIO_BUZZER);
			}
			u_printf("duraton=%d\r\n", duration);
			hftimer_change_period(timer, duration); // start(buzzer_timer);
			break;
		default:
		case 2:
			u_printf("end\r\n");
			/* end */
			hfgpio_pwm_disable(GPIO_BUZZER);
			hfgpio_fset_out_low(GPIO_BUZZER);
			state = 0;
			p = NULL;
			break;
	}
}
#else
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
#endif

void USER_FUNC buzzer_play(uint8_t tone)
{
	hftimer_stop(buzzer_timer);
	hfgpio_pwm_disable(GPIO_BUZZER);
	hfgpio_fset_out_low(GPIO_BUZZER);

	buzzer_state.cnt = 0;
	buzzer_state.tone = buzzer_tones[tone];

	buzzer_state.song = songs[tone];

	hftimer_change_period(buzzer_timer, 100);
}

void USER_FUNC buzzer_init(void)
{
	hfgpio_configure_fpin(GPIO_BUZZER, HFM_IO_OUTPUT_0);
	buzzer_timer = hftimer_create("buzzer", 10, false, HFTIMER_ID_BUZZER, buzzer_timer_handler, 0);
}
#else
void USER_FUNC buzzer_play(uint8_t tone) {}
void USER_FUNC buzzer_init(void) {}
#endif

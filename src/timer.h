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

#ifndef _TIMER_H_
#define _TIMER_H_

#include <hsf.h>


typedef struct {
    uint8_t seconds[8];
    uint8_t minutes[8];
    uint8_t hours[3];
    uint8_t days_of_week[1];
    uint8_t days_of_month[4];
    uint8_t months[2];
} cron_expr;

//struct hfeasy_timer;

struct hfeasy_timer {
	cron_expr cron;
	//time_t    time;
	uint16_t  flags;
	struct hfeasy_timer *next;
};

#define TIMER_DISABLED     0x0000
#define TIMER_TURN_ON      0x1000
#define TIMER_TURN_OFF     0x2000
#define TIMER_TOGGLE       0x4000
#define TIMER_ACTIVE       0x8000

#define TIMER_VALUE_MASK   0x00ff


void USER_FUNC timer_init(void);

#endif

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


struct hfeasy_timer {
	time_t    time;
	uint16_t  flags;
};

#define TIMER_TURN_ON      (1 << 0)
#define TIMER_TURN_OFF     (1 << 1)

#define TIMER_WEEK_MON     (1 << 2)
#define TIMER_WEEK_TUE     (1 << 3)
#define TIMER_WEEK_WED     (1 << 4)
#define TIMER_WEEK_THU     (1 << 5)
#define TIMER_WEEK_FRI     (1 << 6)
#define TIMER_WEEK_SAT     (1 << 7)
#define TIMER_WEEK_SUN     (1 << 8)

#define TIMER_DAYLY        (1 << 9)
#define TIMER_WEEKLY       (1 << 9)
#define TIMER_MONTHLY      (1 << 10)


void USER_FUNC timer_init(void);

#endif

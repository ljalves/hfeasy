/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef RDA_WDT_API_H
#define RDA_WDT_API_H

#include <stdint.h>
#include "device.h"

/** WDT HAL structure
 */
typedef struct wdt_s wdt_t;

#ifdef __cplusplus
extern "C" {
#endif

/** Initialize the wdt module
 *
 * @param obj The WDT object
 * @param to  The timeout value (0 ~ 15) to be set in units of second
 */
void rda_wdt_init(wdt_t *obj, uint8_t to);

/** Start the wdt module
 *
 * @param obj The WDT object
 */
void rda_wdt_start(wdt_t *obj);

/** Stop the wdt module
 *
 * @param obj The WDT object
 */
void rda_wdt_stop(wdt_t *obj);

/** Feed the watchdog
 *
 * @param obj The WDT object
 */
void rda_wdt_feed(wdt_t *obj);

/** Software reset using watchdog
 *
 */
void rda_wdt_softreset(void);

/** System software reset
 *
 */
void rda_sys_softreset(void);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

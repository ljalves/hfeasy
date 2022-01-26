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
#ifndef RDA_GPADC_API_H
#define RDA_GPADC_API_H

#include <stdint.h>
#include "device.h"
#include "gpio_irq_api.h"
#include "analogin_api.h"

#if (DEVICE_INTERRUPTIN && DEVICE_ANALOGIN)
/** GPADC HAL enum
 */
typedef enum {
    GP_NONE,
    GP_RISE,
    GP_FALL
} gpadc_event;

/** GPADC HAL structure
 */
typedef struct {
    struct gpadc_s gpadc;
    gpio_irq_t irq;
    gpio_t gpio;
    analogin_t ain;
} gpadc_t;
typedef void (*gpadc_handler)(uint8_t ch, uint16_t val, gpadc_event event);

#ifdef __cplusplus
extern "C" {
#endif

/** Initialize the gpadc module
 *
 */
void rda_gpadc_init(gpadc_t *obj, PinName pin, gpadc_handler handler);

/** Set the gpadc event
 *
 */
void rda_gpadc_set(gpadc_t *obj, gpadc_event event, uint32_t enable);

/** Free the gpadc module
 *
 */
void rda_gpadc_free(gpadc_t *obj);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* DEVICE_INTERRUPTIN && DEVICE_ANALOGIN */

#endif /* RDA_GPADC_API_H */

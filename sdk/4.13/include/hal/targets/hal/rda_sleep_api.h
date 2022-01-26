/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#ifndef RDA_SLEEP_API_H
#define RDA_SLEEP_API_H

#if DEVICE_SLEEP

typedef enum {
    SLEEP_NORMAL    = 0,
    SLEEP_DEEP      = 1,
    SLEEP_POWERDOWN = 2,
    SLEEP_LEVEL_NUM
} SLEEP_LEVEL_T;

typedef enum {
    WAKESRC_UART    = (0x01UL << 0),
    WAKESRC_GPIO    = (0x01UL << 1),
    WAKESRC_TIMER   = (0x01UL << 2)
} WAKEUP_SOURCE_T;

typedef struct {
    void (*set_slp_mode) (unsigned char is_slp);
    int  (*is_slp_allow) (void);
    void (*indic_wakeup) (void);
} sleep_entry_t;

typedef void (*sys_wakeup_handler)(void);

#ifdef __cplusplus
extern "C" {
#endif

void sleep_entry_register(sleep_entry_t *entry);
void sleep_management(void);

/** Set system sleep level
 *
 * @param level The sleep level to be set, NORMAL/DEEP/POWERDOWN supported
 */
void sleep_set_level(SLEEP_LEVEL_T level);

/** Initialize the user sleep module
 *
 */
void user_sleep_init(void);

/** Set user sleep flag
 *
 * @param yes The flag value to be set, non-zero for allowing sleep operation
 */
void user_sleep_allow(int yes);

/** Set user wakeup source
 *
 * @param src    The wakeup source type, UART/GPIO/TIMER supported
 * @param enable The operation to be set, non-zero for enable
 * @param arg    The arguments to be set, meaningful for GPIO(GPIO Index)
 *               or TIMER(time interval in units of 10ms)
 */
void user_sleep_wakesrc_set(WAKEUP_SOURCE_T src, unsigned int enable, unsigned int arg);

/** Set user boot address of powerdown mode
 *
 * @param boot_addr The running address to be set, valid only for powerdown mode
 */
void user_sleep_bootaddr_set(unsigned int boot_addr);

#ifdef __cplusplus
}
#endif

#endif

#endif


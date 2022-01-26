#ifndef _WLAND_FLASH_WP_H_
#define _WLAND_FLASH_WP_H_

#ifdef __cplusplus
extern "C" {
#endif
extern void flash_wrtie_protect_all();
extern void flash_wrtie_protect_none();
extern void flash_wrtie_protect(unsigned int offset);
extern void rda5981_flash_init();
#ifdef __cplusplus
}
#endif

#endif


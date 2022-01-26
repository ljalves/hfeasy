#ifndef _WLAND_RF_H_
#define _WLAND_RF_H_

#include "wland_types.h"

#define WLAND_TXP_NUM       2
#define WLAND_CHANNEL_NUM   14
#define WLAND_TX_POWER_PHY_GN_REG  0x11F
#define WLAND_TX_POWER_PHY_B_REG   0x120
#define WLAND_TX_POWER_RF_REG      0x8A
#define WLAND_XTAL_CAL_REG         0xDA

#define MAKE_WORD16(lsb, msb)           (((u16)(msb) << 8)  & 0xFF00) | (lsb)
#define MAKE_WORD32(lsw, msw)           (((u32)(msw) << 16) & 0xFFFF0000) | (lsw)

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif


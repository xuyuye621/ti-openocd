/*
 * Reconstructed from MSPM0G1X0X_G3X0X_MAIN_128KB.FLM DevDscr section.
 * The decoded fields match the installed TI DFP 1.3.1 image.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "FlashOS.h"

struct FlashDevice const FlashDevice = {
    FLASH_DRV_VERS,
    "MSPM0G MAIN 128KB",
    ONCHIP,
    0x00000000,
    0x00020000,
    1024,
    0,
    0xFF,
    500,
    3000,
    { { 0x00000400, 0x00000000 }, { SECTOR_END } }
};

/*
 * Reconstructed TI MSPM0 Keil FLM source.
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * This is not an official TI source release. It was recovered from:
 *   MSPM0G1X0X_G3X0X_DFP 1.3.1
 *   MSPM0G1X0X_G3X0X_MAIN_128KB.FLM
 * using its embedded DWARF debug information, symbol table, section data,
 * and disassembly, then mapped onto TI DriverLib headers.
 *
 * The original FLM records these source paths:
 *   FlashPrg.c
 *   FlashDev.c
 *   source/ti/driverlib/dl_flashctl.c
 *   source/ti/driverlib/m0p/sysctl/dl_sysctl_mspm0g1x0x_g3x0x.c
 */

#include <stdbool.h>

#include "FlashOS.h"

#include "ti/driverlib/dl_common.h"
#include "ti/driverlib/dl_flashctl.h"
#include "ti/driverlib/m0p/dl_core.h"
#include "ti/driverlib/m0p/dl_factoryregion.h"
#include "ti/driverlib/m0p/sysctl/dl_sysctl_mspm0g1x0x_g3x0x.h"
#include "ti/devices/msp/m0p/mspm0g350x.h"

typedef enum {
    MAIN_REGION = 0,
    NONMAIN_REGION = 1
} Region_Type;

static bool determineMemoryRegion(uint32_t addr, Region_Type *region)
{
    bool success = false;

    if (addr < ((uint32_t)DL_FactoryRegion_getMAINFlashSize() * 1024U)) {
        *region = MAIN_REGION;
        success = true;
    }

    return success;
}

static void programMemoryBlocking64WithECCGenerated(
    FLASHCTL_Regs *flashctl, uint32_t address, uint32_t *data,
    uint32_t dataSize, DL_FLASHCTL_REGION_SELECT regionSelect)
{
    bool success = true;

    while ((dataSize != 0U) && success) {
        DL_FlashCTL_unprotectSector(flashctl, address, regionSelect);

        while ((flashctl->GEN.STATCMD & FLASHCTL_STATCMD_CMDINPROGRESS_MASK) !=
               0U) {
            ;
        }

        DL_FlashCTL_programMemory64WithECCGenerated(flashctl, address, data);

        dataSize -= 2U;
        data += 2U;
        address += 8U;

        success = DL_FlashCTL_waitForCmdDone(flashctl);
    }
}

int Init(unsigned long adr, unsigned long clk, unsigned long fnc)
{
    (void)adr;
    (void)clk;
    (void)fnc;

    DL_CORE_configInstruction(DL_CORE_CACHE_DISABLED,
        DL_CORE_PREFETCH_DISABLED, DL_CORE_LITERAL_CACHE_DISABLED);

    DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);

    if (DL_SYSCTL_getMCLKSource() == DL_SYSCTL_MCLK_SOURCE_HSCLK) {
        DL_SYSCTL_switchMCLKfromHSCLKtoSYSOSC();
    } else if (DL_SYSCTL_getMCLKSource() == DL_SYSCTL_MCLK_SOURCE_LFCLK) {
        DL_SYSCTL_switchMCLKfromLFCLKtoSYSOSC();
    }

    DL_SYSCTL_setMCLKDivider(DL_SYSCTL_MCLK_DIVIDER_DISABLE);
    DL_SYSCTL_setPowerPolicyRUN0SLEEP0();

    return 0;
}

int UnInit(unsigned long fnc)
{
    (void)fnc;
    return 0;
}

int EraseChip(void)
{
    bool success = DL_FlashCTL_massErase(FLASHCTL);
    int result = success ? 0 : 1;

    return result;
}

int EraseSector(unsigned long adr)
{
    bool success;
    Region_Type region;
    uint32_t result = 0;
    DL_FLASHCTL_REGION_SELECT regionSelect;

    success = determineMemoryRegion(adr, &region);
    if (!success) {
        result = 1;
    } else {
        regionSelect = (region == MAIN_REGION) ?
            DL_FLASHCTL_REGION_SELECT_MAIN :
            DL_FLASHCTL_REGION_SELECT_NONMAIN;

        for (uint32_t eraseRepeats = 0; eraseRepeats < 5U; eraseRepeats++) {
            DL_FlashCTL_unprotectSector(FLASHCTL, adr, regionSelect);
            DL_FlashCTL_eraseMemory(FLASHCTL, adr,
                DL_FLASHCTL_COMMAND_SIZE_SECTOR);

            if (DL_FlashCTL_waitForCmdDone(FLASHCTL)) {
                break;
            }
        }
    }

    return (int)result;
}

int ProgramPage(unsigned long adr, unsigned long sz, unsigned char *buf)
{
    Region_Type region;
    DL_FLASHCTL_REGION_SELECT regionSelect;

    if (!determineMemoryRegion(adr, &region)) {
        return 1;
    }

    if ((sz == 0U) || ((sz & 0x7U) != 0U)) {
        return 1;
    }

    regionSelect = (region == MAIN_REGION) ?
        DL_FLASHCTL_REGION_SELECT_MAIN :
        DL_FLASHCTL_REGION_SELECT_NONMAIN;

    programMemoryBlocking64WithECCGenerated(FLASHCTL, adr,
        (uint32_t *)buf, sz / sizeof(uint32_t), regionSelect);

    return 0;
}

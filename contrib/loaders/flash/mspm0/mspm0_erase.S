/* SPDX-License-Identifier: GPL-2.0-or-later */

/*
 * TI MSPM0 flash sector erase algorithm for OpenOCD RAM loader.
 *
 * Parameters:
 *   r0 = flash address
 *   r1 = protect register address
 *   r2 = sector protect mask
 *
 * Return in r0:
 *   0 = success
 *   1 = command failed
 */

.syntax unified
.cpu cortex-m0
.thumb
.thumb_func
.global mspm0_erase_sector

#define FCTL_CMDTYPE   0x400CE104
#define FCTL_CMDADDR   0x400CE120
#define FCTL_CMDEXEC   0x400CE100
#define FCTL_STATCMD   0x400CE3D0

mspm0_erase_sector:
    bl mspm0_prepare_clock
    ldr r3, [r1]
    mvns r4, r2
    ands r3, r4
    str r3, [r1]

    ldr r3, =FCTL_CMDTYPE
    movs r4, #0x42
    str r4, [r3]

    ldr r3, =FCTL_CMDADDR
    str r0, [r3]

    ldr r3, =FCTL_CMDEXEC
    movs r4, #1
    str r4, [r3]

    ldr r4, =FCTL_STATCMD
erase_poll:
    ldr r3, [r4]
    movs r5, #1
    ands r5, r3
    beq erase_poll

    movs r5, #2
    ands r5, r3
    beq erase_failed

    movs r0, #0
    bkpt #0

erase_failed:
    movs r0, #1
    bkpt #0

mspm0_prepare_clock:
    push {r0-r4, lr}
    ldr r3, =0x40401300
    ldr r0, [r3]
    movs r1, #7
    bics r0, r1
    str r0, [r3]

    ldr r3, =0x400B0100
    ldr r0, [r3, #0]
    movs r1, #3
    bics r0, r1
    str r0, [r3, #0]
    ldr r0, [r3, #4]
    ldr r1, =0x0011000F
    bics r0, r1
    str r0, [r3, #4]

    ldr r3, =0x400B1204
prepare_poll:
    ldr r0, [r3]
    movs r1, #0x10
    movs r2, r0
    ands r2, r1
    bne prepare_poll
    movs r1, #0xC0
    movs r2, r0
    ands r2, r1
    bne prepare_poll

    ldr r3, =0xE000ED10
    ldr r0, [r3]
    movs r1, #4
    bics r0, r1
    str r0, [r3]
    pop {r0-r4, pc}

.pool

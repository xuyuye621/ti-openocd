/* SPDX-License-Identifier: GPL-2.0-or-later */

/*
 * TI MSPM0 flash block-write algorithm for OpenOCD RAM loader.
 *
 * This algorithm writes 128-bit flash words from a source buffer in SRAM.
 * The caller must provide a count that is a multiple of 16 bytes.
 *
 * Parameters:
 *   r0 = flash address
 *   r1 = source buffer address
 *   r2 = byte count
 *   r3 = protect register address
 *   r4 = sector protect mask
 *
 * Return in r0:
 *   0 = success
 *   1 = command failed
 */

.syntax unified
.cpu cortex-m0
.thumb
.thumb_func
.global mspm0_write_block

#define FCTL_CMDTYPE   0x400CE104
#define FCTL_CMDADDR   0x400CE120
#define FCTL_CMDBYTEN  0x400CE124
#define FCTL_CMDDATA0  0x400CE130
#define FCTL_CMDEXEC   0x400CE100
#define FCTL_STATCMD   0x400CE3D0

mspm0_write_block:
    bl mspm0_prepare_clock
    cmp r2, #0
    beq write_done

    ldr r5, =0xFFFFFFFF
    cmp r4, r5
    beq write_skip_unprotect
    ldr r5, [r3]
    mvns r6, r4
    ands r5, r6
    str r5, [r3]

write_skip_unprotect:
    ldr r5, =FCTL_CMDTYPE
    movs r6, #1
    str r6, [r5]

write_loop:
    cmp r2, #0
    beq write_done

    ldr r5, =FCTL_CMDBYTEN
    ldr r6, =0x000001FF
    str r6, [r5]

    ldr r5, =FCTL_CMDADDR
    str r0, [r5]

    ldr r5, =FCTL_CMDDATA0
    ldr r6, [r1]
    str r6, [r5]
    ldr r6, [r1, #4]
    str r6, [r5, #4]

    ldr r5, =FCTL_CMDEXEC
    movs r6, #1
    str r6, [r5]

    ldr r7, =FCTL_STATCMD
write_poll:
    ldr r5, [r7]
    movs r6, #1
    ands r6, r5
    beq write_poll

    movs r6, #2
    ands r6, r5
    beq write_failed

    adds r0, r0, #8
    adds r1, r1, #8
    subs r2, r2, #8
    b write_loop

write_failed:
    mov r0, r5
    bkpt #0

write_done:
    movs r0, #0
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

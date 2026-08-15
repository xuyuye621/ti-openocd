# Reconstructed TI MSPM0 Keil FLM source

This directory contains a source-level reconstruction of the TI MSPM0 MAIN
flash programming algorithm used by Keil. It is not an official TI source
release.

## Source image

The reconstruction is based on:

`TexasInstruments.MSPM0G1X0X_G3X0X_DFP 1.3.1`
`MSPM0G1X0X_G3X0X_MAIN_128KB.FLM`

The FLM contains:

- `PrgCode`
- `PrgData`
- `DevDscr`
- full DWARF debug information
- a symbol table

## Original paths recorded in DWARF

The FLM records a source checkout under `keil/Flash/MSPM0` and these source
files:

```text
FlashPrg.c
FlashDev.c
source\ti\driverlib\dl_flashctl.c
source\ti\driverlib\m0p\sysctl\dl_sysctl_mspm0g1x0x_g3x0x.c
```

## Recovery method

1. Read ELF section headers, symbols, and strings with GNU Arm `readelf`,
   `nm`, `objcopy`, and `objdump`.
2. Disassembled `PrgCode` with source line mapping.
3. Decoded `DevDscr` as the standard Keil `struct FlashDevice`.
4. Mapped the recovered calls and constants onto the local TI DriverLib
   headers supplied by the consuming project.

## Files

- `FlashOS.h`: minimal generic Keil FlashOS definitions.
- `FlashDev.c`: device description decoded from `DevDscr`.
- `FlashPrg.c`: recovered programming functions.

## Important caveat

The public `TexasInstruments/mspm0-sdk` repository and the installed DFP do
not contain `FlashPrg.c` or `FlashDev.c`. The exact original TI repository
`msp-iar-keil-sp` was not found in public searches. Therefore this code is a
best-effort equivalent reconstruction, not the unmodified original source.

## License

This component is provided under the BSD-3-Clause terms in `LICENSE`, with
copyright notice retained from the TI Device Family Pack. It is a
reverse-engineered reconstruction, not an official TI source release.

Additional search results:

- A full `git ls-tree` scan of `TexasInstruments/mspm0-sdk` on `main` and all
  19 published SDK tags did not find `FlashPrg.c`, `FlashDev.c`, or a matching
  FLM source path.
- `https://git.ti.com/git/msp-iar-keil-sp.git` and several related public
  cgit/git paths returned 404 or repository-not-found.

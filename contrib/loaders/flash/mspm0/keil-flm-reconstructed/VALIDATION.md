# Minimum validation report

Scope: minimum verification of the reconstructed Keil FLM source against the
original TI FLM binary.

## Result

- Rebuilt `FlashDevice` data matches the original FLM `DevDscr` byte-for-byte:
  4256 bytes, 0 differences.
- `FlashPrg.c` and `FlashDev.c` compile with local ARMCLANG 6.21.
- `UnInit` and `EraseChip` disassemble to the same size and instructions as
  the original.
- `Init` is semantically equivalent; register constants match the original
  when compiled with `-Os`.
- `EraseSector` and `ProgramPage` match the original control flow and calls.

## Verified differences

- Local ARMCLANG is 6.21 while the FLM was built with 6.19, so instruction
  scheduling and constant pools can differ.
- The local DriverLib version of `DL_FactoryRegion_getMAINFlashSize()` saves
  and restores the CPUSS cache configuration, which adds a few instructions in
  `EraseSector` and `ProgramPage` compared with the older FLM-internal
  implementation.
- The reconstruction no longer accepts NONMAIN addresses in this MAIN FLM
  build, matching the original MAIN image.

## Artifacts

- `flm_DevDscr.bin`, `flm_PrgCode.bin`: extracted from the original FLM.
- `flm_orig_disasm.txt`: original `PrgCode` disassembly.
- `rebuilt_Os_disasm.txt`: rebuilt `FlashPrg.c` disassembly compiled with
  `-Os`.
- `flashprg.o`, `flashdev.o`: ARMCLANG object files.

These artifacts are generated during local validation and are intentionally
not committed to the repository.

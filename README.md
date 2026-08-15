# ti-keil OpenOCD

OpenOCD build for TI MSPM0 microcontrollers that uses TI's Keil FLM algorithm
for flash programming. It is tested with the nanoDAP-wireless CMSIS-DAP
adapter.

简体中文：[README.zh-CN.md](README.zh-CN.md)

Latest release: [nanodap-wireless-0.3.1](https://github.com/xuyuye621/ti-openocd/releases/tag/nanodap-wireless-0.3.1)

## Why This Fork

On wireless CMSIS-DAP bridges, a normal register-by-register flash path can be
slow or unstable. This build embeds TI's Keil FLM algorithm instead, so the
erase and program sequence runs on the MCU itself, closer to how Keil MDK
programs flash.

## What Is TI Keil FLM?

Keil FLM is the flash programming algorithm used by Keil MDK. OpenOCD loads it
into the target's SRAM, and the MSPM0 then configures its FlashCtl controller,
erases/programs flash, and waits for command completion locally. This reduces
the number of slow debugger round trips and behaves more like Keil flashing.

## Supported Hardware

- MCU: TI MSPM0 G1x0x / G3x0x series. Validated on MSPM0G3507.
- Probe: CMSIS-DAP interface. Validated on nanoDAP-wireless; other CMSIS-DAP
  probes are not officially tested.

## Quick Start

Requirements:

- Windows with PowerShell
- Extracted release zip
- Connected CMSIS-DAP probe

Run:

```powershell
powershell -File .\flash-mspm0.ps1 -ElfPath path\to\your.elf -Verify
```

Options:

| Option | Default | Description |
| --- | --- | --- |
| `-ElfPath` | required | firmware file to program |
| `-Verify` | off | verify flash after programming |
| `-SpeedKHz` | `5000` | SWD speed in kHz |

Firmware notes:

- `.elf` and `.hex` are recommended.
- `.bin` requires a base address and is not auto-detected by the script.

Success looks like:

```text
Programming Finished
Verified OK
```

## Troubleshooting

- `unable to find a matching CMSIS-DAP device`: check the USB/wireless probe
  connection and close other debug tools that may hold the probe.
- `Verify failed` or flash hangs: reconnect the probe and retry at a lower
  speed, for example `-SpeedKHz 500`.
- Long timeout: check the wireless link, reduce speed, or retry after
  reconnecting.

## Build

```bash
git clone -b ti-release https://github.com/xuyuye621/ti-openocd.git
cd ti-openocd
./configure --disable-buspirate CFLAGS="-O0 -g"
make -j$(nproc)
```

The executable is `src/openocd.exe`. A release package also needs the required
DLLs and the OpenOCD script directory.

`flash-mspm0.ps1` is included in the release zip. A source-only clone does not
contain it.

## Debug

```powershell
& .\bin\openocd.exe -f interface/cmsis-dap.cfg -f target/ti_mspm0.cfg -c "adapter speed 5000" -c "init"
```

Connect `arm-none-eabi-gdb` to port `3333`, then `monitor reset halt`, `load`,
and `continue`.

## Origin

- Based on [TexasInstruments/ti-openocd](https://github.com/TexasInstruments/ti-openocd).
- The Keil FLM reconstruction in
  `contrib/loaders/flash/mspm0/keil-flm-reconstructed/` is reverse-engineered
  from a TI Device Family Pack FLM. It is not official TI source and is
  distributed under the BSD-3-Clause terms in its `LICENSE` file.

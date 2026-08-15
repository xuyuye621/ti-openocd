# ti-keil OpenOCD

OpenOCD build for TI MSPM0 microcontrollers that uses TI's Keil FLM algorithm
for flashing. It is tested with the nanoDAP-wireless CMSIS-DAP adapter.

简体中文：[README.zh-CN.md](README.zh-CN.md)

Latest release: [nanodap-wireless-0.3.1](https://github.com/xuyuye621/ti-openocd/releases/tag/nanodap-wireless-0.3.1)

## Quick Start

Download and extract the latest release zip, then run:

```powershell
powershell -File .\flash-mspm0.ps1 -ElfPath path\to\your.elf -Verify
```

Options:

| Option | Default | Description |
| --- | --- | --- |
| `-ElfPath` | required | firmware file to program |
| `-Verify` | off | verify flash after programming |
| `-SpeedKHz` | `5000` | SWD speed in kHz |

## What It Does

- Uses TI Keil FLM for flash erase and programming.
- Does not use a RAM loader or register fallback path.
- Defaults to 5 MHz SWD.

## Build

```bash
git clone -b ti-release https://github.com/xuyuye621/ti-openocd.git
cd ti-openocd
./configure --disable-buspirate CFLAGS="-O0 -g"
make -j$(nproc)
```

The executable is `src/openocd.exe`. A release package also needs the required
DLLs and the OpenOCD script directory.

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

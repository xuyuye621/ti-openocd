# ti-keil OpenOCD

TI OpenOCD source build with a Keil FLM flash path for nanoDAP-wireless.

简体中文说明：[README.zh-CN.md](README.zh-CN.md)

Latest release: [nanodap-wireless-0.3.1](https://github.com/xuyuye621/ti-openocd/releases/tag/nanodap-wireless-0.3.1)

Source: https://github.com/TexasInstruments/ti-openocd
Fork: https://github.com/xuyuye621/ti-openocd
Source branch: `ti-release`
Build: MSYS2 MinGW64, `-O0`, `--disable-buspirate`

Layout:

```text
bin/openocd.exe
bin/*.dll
share/openocd/scripts
flash-mspm0.ps1
README.md
README.zh-CN.md
```

## What Changed

1. MSPM0 flash uses the TI Keil FLM algorithm only.
2. RAM loader fallback and register programming fallback are removed.
3. Default SWD speed is 5 MHz.

## Keil FLM Reconstruction

`contrib/loaders/flash/mspm0/keil-flm-reconstructed/` contains a
reverse-engineered source-level reconstruction of the TI Keil FLM algorithm
used by this build. It is not official TI source and is distributed under the
BSD-3-Clause terms in its `LICENSE` file.

## Flash

`-ElfPath` is required. The script has no machine-specific default path.

Flash without verify:

```powershell
powershell -File .\flash-mspm0.ps1 -ElfPath .\build\MSPM0.elf
```

Flash with verify:

```powershell
powershell -File .\flash-mspm0.ps1 -ElfPath .\build\MSPM0.elf -Verify
```

Example with verify:

```powershell
powershell -File .\flash-mspm0.ps1 -ElfPath .\build\MSPM0.elf -Verify -SpeedKHz 5000
```

### Parameters

| Parameter | Default | Description |
| --- | --- | --- |
| `-ElfPath` | required | elf file to program |
| `-Verify` | off | Verify flash after programming |
| `-SpeedKHz` | `5000` | SWD speed in kHz |

## Build from Source

This binary is built from the `ti-release` branch in the fork.

In an MSYS2 MinGW64 shell:

```bash
git clone -b ti-release https://github.com/xuyuye621/ti-openocd.git
cd ti-openocd
./configure --disable-buspirate CFLAGS="-O0 -g"
make -j$(nproc)
```

The resulting executable is `src/openocd.exe` on Windows. Copy `src/openocd.exe`, the required DLLs, and the OpenOCD script directory into a release directory.

## Debug

Start OpenOCD from the extracted release directory:

```powershell
& .\bin\openocd.exe -f interface/cmsis-dap.cfg -f target/ti_mspm0.cfg -c "adapter speed 5000" -c "init"
```

Then connect with `arm-none-eabi-gdb` on port 3333:

```gdb
target extended-remote :3333
monitor reset halt
load
continue
```

## Verified

- Small SDK example: programming and Verify OK
- 85KB CMake firmware: Programming and Verify OK at 500 kHz and 5 MHz
- Small SDK example and 85KB CMake firmware: Keil FLM programming and Verify OK

## Upstream Documentation

The original upstream documentation remains in [README](README).

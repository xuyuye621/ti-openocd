# ti-keil OpenOCD

TI OpenOCD source build with a Keil-style HID pacing patch for nanoDAP-wireless.

简体中文说明：[README.zh-CN.md](README.zh-CN.md)

Source: https://github.com/TexasInstruments/ti-openocd
Fork: https://github.com/xuyuye621/ti-openocd
Source branch: `nanoDAP-wireless-hid-pacing`
Source commit: `4a5ae73` (base `cb22a31`)
Build: MSYS2 MinGW64, `-O0`, `--disable-buspirate`
Patch: `cmsis_dap_keil_pacing.patch`

Layout:

```text
bin/openocd.exe
bin/*.dll
share/openocd/scripts
flash-mspm0.ps1
README.md
README.zh-CN.md
cmsis_dap_keil_pacing.patch
```

## What Changed

1. Configurable HID pacing: `cmsis-dap hiddelay <us>`, default 200 us, Keil-like behavior.
2. HID read timeout retry, up to 5 attempts.
3. Default SWD speed is 5 MHz; lower the speed or increase the delay if the wireless link is unstable.

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

Example with 5 MHz and 200 us HID pacing:

```powershell
powershell -File .\flash-mspm0.ps1 -ElfPath .\build\MSPM0.elf -Verify -SpeedKHz 5000 -DelayUs 200
```

### Parameters

| Parameter | Default | Description |
| --- | --- | --- |
| `-ElfPath` | required | elf file to program |
| `-Verify` | off | Verify flash after programming |
| `-SpeedKHz` | `5000` | SWD speed in kHz |
| `-DelayUs` | `200` | HID read/write pacing in microseconds |

## Build from Source

This binary is built from the `nanoDAP-wireless-hid-pacing` branch in the fork. Source commit: `4a5ae73`.

In an MSYS2 MinGW64 shell:

```bash
git clone -b ti-release https://github.com/xuyuye621/ti-openocd.git
cd ti-openocd
git checkout nanoDAP-wireless-hid-pacing
./configure --disable-buspirate CFLAGS="-O0 -g"
make -j$(nproc)
```

The resulting executable is `src/openocd.exe` on Windows. Copy `src/openocd.exe`, the required DLLs, and the OpenOCD script directory into a release directory.

## Debug

Start OpenOCD from the extracted release directory:

```powershell
& .\bin\openocd.exe -f interface/cmsis-dap.cfg -f target/ti_mspm0.cfg -c "cmsis-dap hiddelay 200" -c "adapter speed 5000" -c "init"
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
- Verify uses HID read retry for better stability; verify is off by default for speed

## Upstream Documentation

The original upstream documentation remains in [README](README).

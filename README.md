# TI OpenOCD Fork - nanoDAP-wireless HID Pacing Build

This fork of [TexasInstruments/ti-openocd](https://github.com/TexasInstruments/ti-openocd) adds a Keil-style HID pacing patch for wireless CMSIS-DAP adapters such as nanoDAP-wireless.

- Custom branch: `nanoDAP-wireless-hid-pacing`
- Source patch: `cmsis_dap_keil_pacing.patch`
- Windows binary release: [nanodap-wireless-0.1](https://github.com/xuyuye621/ti-openocd/releases/tag/nanodap-wireless-0.1)
- Default SWD speed: 5 MHz
- Default HID delay: 200 us

## What Changed

1. HID reads and writes are paced with a configurable `cmsis-dap hiddelay <us>` command, defaulting to 200 us.
2. HID read timeouts retry up to 5 times.
3. This reduces packet loss and timeouts over lossy wireless CMSIS-DAP links.

## Usage

Download the Windows zip from the release, extract it, then run:

```powershell
powershell -File flash-mspm0.ps1 -Verify -SpeedKHz 5000 -DelayUs 200
```

## 中文说明

本 fork 针对 nanoDAP-wireless 这类无线 CMSIS-DAP 调试器，加入 Keil 风格的 HID 读写节奏控制：

1. 支持 `cmsis-dap hiddelay <us>`，默认 200 微秒。
2. HID 读取超时自动重试，最多 5 次。
3. Windows 可执行压缩包见 [Release](https://github.com/xuyuye621/ti-openocd/releases/tag/nanodap-wireless-0.1)。

## Upstream Documentation

The original upstream documentation remains in [README](README).

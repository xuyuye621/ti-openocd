# ti-keil OpenOCD（nanoDAP-wireless 专用版）

English: [README.md](README.md)

最新 Release：[nanodap-wireless-0.2](https://github.com/xuyuye621/ti-openocd/releases/tag/nanodap-wireless-0.2)

针对 TI MSPM0 的 OpenOCD 预编译版本。基于 TI 官方 OpenOCD 源码构建，并加入 Keil 风格的 HID 节奏控制补丁，用于改善 nanoDAP-wireless 这类无线 CMSIS-DAP 调试器在烧录时的丢包、超时和失败问题。

- 源码：https://github.com/TexasInstruments/ti-openocd
- Fork：https://github.com/xuyuye621/ti-openocd
- 源码分支：`nanoDAP-wireless-hid-pacing`
- 源码提交：`4a5ae73`（基线 `cb22a31`）
- 构建环境：MSYS2 MinGW64，`-O0`，`--disable-buspirate`
- 补丁：`cmsis_dap_keil_pacing.patch`

## 目录结构

```text
bin/openocd.exe
bin/*.dll
share/openocd/scripts
flash-mspm0.ps1
README.md
README.zh-CN.md
cmsis_dap_keil_pacing.patch
```

## 这个版本做了什么

1. HID 读写后增加可配置延迟：`cmsis-dap hiddelay <us>`，默认 200 微秒，行为接近 Keil 的 DAP 驱动。
2. HID 读取超时时自动重试，最多 5 次，减少无线桥接偶发丢包导致的失败。
3. 默认 SWD 频率 5 MHz；如果无线链路不稳，可降低频率或增大延迟。

## 烧录

`-ElfPath` 是必填参数，脚本不内置任何本机默认路径。

不带校验烧录：

```powershell
powershell -File .\flash-mspm0.ps1 -ElfPath .\build\MSPM0.elf
```

带校验烧录：

```powershell
powershell -File .\flash-mspm0.ps1 -ElfPath .\build\MSPM0.elf -Verify
```

指定 5 MHz、200 微秒 HID 延迟：

```powershell
powershell -File .\flash-mspm0.ps1 -ElfPath .\build\MSPM0.elf -Verify -SpeedKHz 5000 -DelayUs 200
```

### 参数说明

| 参数 | 默认值 | 说明 |
| --- | --- | --- |
| `-ElfPath` | 必填 | 要烧录的 elf 文件路径 |
| `-Verify` | 关闭 | 烧录后校验 Flash |
| `-SpeedKHz` | `5000` | SWD 频率，单位 kHz |
| `-DelayUs` | `200` | HID 读写节奏延迟，单位微秒 |

## 从源码构建

本压缩包由 fork 的 `nanoDAP-wireless-hid-pacing` 分支构建，源码提交为 `4a5ae73`。

在 MSYS2 MinGW64 终端中执行：

```bash
git clone -b ti-release https://github.com/xuyuye621/ti-openocd.git
cd ti-openocd
git checkout nanoDAP-wireless-hid-pacing
./configure --disable-buspirate CFLAGS="-O0 -g"
make -j$(nproc)
```

Windows 下的构建产物为 `src/openocd.exe`。发布时需要同时带上所需 DLL 和 OpenOCD 脚本目录。

## 直接调试

在解压目录中启动 OpenOCD：

```powershell
& .\bin\openocd.exe -f interface/cmsis-dap.cfg -f target/ti_mspm0.cfg -c "cmsis-dap hiddelay 200" -c "adapter speed 5000" -c "init"
```

再用 `arm-none-eabi-gdb` 连接 3333 端口：

```gdb
target extended-remote :3333
monitor reset halt
load
continue
```

## 已验证

- 小体积 SDK 例程：烧录和 Verify 通过。
- 85 KB CMake 固件：500 kHz 与 5 MHz 下烧录和 Verify 通过。
- 校验使用 HID 读重试，稳定性更好；默认关闭校验以加快烧录。

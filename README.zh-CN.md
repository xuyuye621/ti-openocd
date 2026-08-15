# ti-keil OpenOCD（nanoDAP-wireless 专用版）

English: [README.md](README.md)

最新 Release：[nanodap-wireless-0.3.1](https://github.com/xuyuye621/ti-openocd/releases/tag/nanodap-wireless-0.3.1)

针对 TI MSPM0 的 OpenOCD 预编译版本。基于 TI 官方 OpenOCD 源码构建，烧录路径只使用 TI Keil FLM 算法。

- 源码：https://github.com/TexasInstruments/ti-openocd
- Fork：https://github.com/xuyuye621/ti-openocd
- 源码分支：`ti-release`
- 构建环境：MSYS2 MinGW64，`-O0`，`--disable-buspirate`

## 目录结构

```text
bin/openocd.exe
bin/*.dll
share/openocd/scripts
flash-mspm0.ps1
README.md
README.zh-CN.md
```

## 这个版本做了什么

1. MSPM0 烧录只使用 TI Keil FLM 算法。
2. 已移除 RAM loader 回退和寄存器编程回退。
3. 默认 SWD 频率 5 MHz。

## Keil FLM 重建源码

`contrib/loaders/flash/mspm0/keil-flm-reconstructed/` 存放本版本使用的 TI
Keil FLM 算法的逆向重建源码。它不是 TI 官方源码，按 `LICENSE` 中的
BSD-3-Clause 条款发布。

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

指定 5 MHz 并校验：

```powershell
powershell -File .\flash-mspm0.ps1 -ElfPath .\build\MSPM0.elf -Verify -SpeedKHz 5000
```

### 参数说明

| 参数 | 默认值 | 说明 |
| --- | --- | --- |
| `-ElfPath` | 必填 | 要烧录的 elf 文件路径 |
| `-Verify` | 关闭 | 烧录后校验 Flash |
| `-SpeedKHz` | `5000` | SWD 频率，单位 kHz |

## 从源码构建

本压缩包由 fork 的 `ti-release` 分支构建。

在 MSYS2 MinGW64 终端中执行：

```bash
git clone -b ti-release https://github.com/xuyuye621/ti-openocd.git
cd ti-openocd
./configure --disable-buspirate CFLAGS="-O0 -g"
make -j$(nproc)
```

Windows 下的构建产物为 `src/openocd.exe`。发布时需要同时带上所需 DLL 和 OpenOCD 脚本目录。

## 直接调试

在解压目录中启动 OpenOCD：

```powershell
& .\bin\openocd.exe -f interface/cmsis-dap.cfg -f target/ti_mspm0.cfg -c "adapter speed 5000" -c "init"
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

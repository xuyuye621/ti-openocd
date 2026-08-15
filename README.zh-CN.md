# ti-keil OpenOCD

面向 TI MSPM0 的 OpenOCD 构建版本，烧录时使用 TI Keil FLM 算法。已在
nanoDAP-wireless CMSIS-DAP 无线调试器上测试。

English：[README.md](README.md)

最新 Release：[nanodap-wireless-0.3.1](https://github.com/xuyuye621/ti-openocd/releases/tag/nanodap-wireless-0.3.1)

## 快速开始

下载并解压最新 Release，然后运行：

```powershell
powershell -File .\flash-mspm0.ps1 -ElfPath 你的固件路径.elf -Verify
```

参数：

| 参数 | 默认值 | 说明 |
| --- | --- | --- |
| `-ElfPath` | 必填 | 要烧录的固件文件 |
| `-Verify` | 关闭 | 烧录后校验 Flash |
| `-SpeedKHz` | `5000` | SWD 频率，单位 kHz |

## 特性

- 擦除和编程只使用 TI Keil FLM 算法。
- 不使用 RAM loader，也没有寄存器回退路径。
- 默认 SWD 频率 5 MHz。

## 从源码构建

```bash
git clone -b ti-release https://github.com/xuyuye621/ti-openocd.git
cd ti-openocd
./configure --disable-buspirate CFLAGS="-O0 -g"
make -j$(nproc)
```

构建产物为 `src/openocd.exe`。发布包还需要所需 DLL 和 OpenOCD 脚本目录。

## 调试

```powershell
& .\bin\openocd.exe -f interface/cmsis-dap.cfg -f target/ti_mspm0.cfg -c "adapter speed 5000" -c "init"
```

再用 `arm-none-eabi-gdb` 连接 3333 端口，执行 `monitor reset halt`、`load`、
`continue`。

## 来源说明

- 基于 [TexasInstruments/ti-openocd](https://github.com/TexasInstruments/ti-openocd)。
- `contrib/loaders/flash/mspm0/keil-flm-reconstructed/` 中的 Keil FLM 重建源码
  是从 TI Device Family Pack FLM 逆向恢复的，不是 TI 官方源码，按 `LICENSE`
  中的 BSD-3-Clause 条款发布。

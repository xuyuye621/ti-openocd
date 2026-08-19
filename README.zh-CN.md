# ti-keil OpenOCD

面向 TI MSPM0 的 OpenOCD 构建版本，烧录时使用 TI Keil FLM 算法。已在
nanoDAP-wireless CMSIS-DAP 无线调试器上测试。

English：[README.md](README.md)

最新 Release：[nanodap-wireless-0.3.2](https://github.com/xuyuye621/ti-openocd/releases/tag/nanodap-wireless-0.3.2)

## 为什么用这个版本

在无线 CMSIS-DAP 桥接器上，普通的逐个寄存器烧录路径可能又慢又不稳定。
这个版本内置 TI Keil FLM 算法，擦除和编程序列在芯片内部执行，行为更接近
Keil MDK 的烧录方式。

0.3.2 改用与 TI 官方 OpenOCD 一致的 GCC 14.1.0 工具链构建，修复 Windows
下配置文件偶发加载失败的问题。

## TI Keil FLM 是什么

Keil FLM 是 Keil MDK 使用的 Flash 编程算法。OpenOCD 会把它加载到目标芯片的
SRAM，然后由 MSPM0 自己配置 FlashCtl、执行擦除/编程并等待命令完成，减少
调试器往返次数，烧录更接近 Keil 的行为。

## 支持范围

- 芯片：TI MSPM0 G1x0x / G3x0x 系列，已在 MSPM0G3507 上验证。
- 调试器：CMSIS-DAP 接口，已在 nanoDAP-wireless 上验证；其他 CMSIS-DAP
  调试器未正式测试。

## 快速开始

环境要求：

- Windows + PowerShell
- 已解压的 Release 包
- 已连接的 CMSIS-DAP 调试器

运行：

```powershell
powershell -File .\flash-mspm0.ps1 -ElfPath 你的固件路径.elf -Verify
```

参数：

| 参数 | 默认值 | 说明 |
| --- | --- | --- |
| `-ElfPath` | 必填 | 要烧录的固件文件 |
| `-Verify` | 关闭 | 烧录后校验 Flash |
| `-SpeedKHz` | `5000` | SWD 频率，单位 kHz |

固件格式说明：

- 推荐使用 `.elf` 或 `.hex`。
- `.bin` 需要基地址，当前脚本不会自动推断。

成功时会看到：

```text
Programming Finished
Verified OK
```

## 常见问题

- `unable to find a matching CMSIS-DAP device`：检查 USB/无线调试器连接，
  并关闭其他占用调试器的工具。
- `Verify failed` 或烧录卡住：重新连接调试器，并用更低频率重试，例如
  `-SpeedKHz 500`。
- 长时间超时：检查无线链路，降低频率，或重新连接后重试。

## 从源码构建

```bash
git clone -b ti-release https://github.com/xuyuye621/ti-openocd.git
cd ti-openocd
./configure --disable-buspirate CFLAGS="-O0 -g"
make -j$(nproc)
```

构建产物为 `src/openocd.exe`。发布包还需要所需 DLL 和 OpenOCD 脚本目录。

`flash-mspm0.ps1` 只随 Release 包提供，源码 clone 中不包含。

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

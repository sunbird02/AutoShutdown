# CODEBUDDY.md This file provides guidance to CodeBuddy when working with code in this repository.

## 项目概述

这是一个 Windows 平台的自动关机助手应用程序，使用 Win32 API 和 C/C++ 开发。项目包含多个 UI 风格的版本：经典版、现代化深色主题版和赛博朋克终端风格版。

## 常用命令

### 编译项目

```powershell
# 使用 TCC 编译经典版（推荐，体积小）
tcc -o AutoShutdown.exe AutoShutdown.c -luser32 -lkernel32 -lgdi32

# 使用 TCC 编译现代化版本
tcc -o AutoShutdown.exe AutoShutdown_modern.c -luser32 -lkernel32 -lgdi32 -lcomctl32 -lshell32

# 使用 TCC 编译管理员权限版
tcc -o AutoShutdown.exe AutoShutdown_管理员.c -luser32 -lkernel32 -lgdi32 -lshell32

# 使用 MinGW/g++ 编译赛博朋克版本（需要 GDI+）
g++ -o AutoShutdown_Cyberpunk.exe AutoShutdown_Cyberpunk.cpp -mwindows -lcomctl32 -lgdiplus -lgdi32 -luser32 -lkernel32
```

### 一键编译

```powershell
# 运行批处理脚本（自动检测 TCC 路径并编译）
.\一键编译.bat
```

### 修复待机功能

```powershell
# 以管理员身份运行修复脚本
.\修复待机.bat
```

## 代码架构

### 文件结构

- `AutoShutdown.c` - 经典版，使用原生 Win32 API，简洁稳定
- `AutoShutdown_modern.c` - 现代化深色主题版，使用 Unicode 和自定义绘制
- `AutoShutdown_Cyberpunk.cpp` - 赛博朋克终端风格版，使用 GDI+ 和现代 C++
- `AutoShutdown_管理员.c` - 管理员权限版，处理 UAC 和待机功能
- `一键编译.bat` - TCC 一键编译脚本
- `修复待机.bat` - 修复系统待机/休眠功能的工具脚本

### 核心功能模块

所有版本共享相同的底层架构：

1. **窗口过程 (WndProc)** - 处理 Windows 消息循环，响应用户交互
2. **任务设置 (OnSet)** - 解析时间输入，计算倒计时，调用 `shutdown` 或 `rundll32 powrprof.dll` 执行操作
3. **任务取消 (OnCancel)** - 调用 `shutdown /a` 取消关机计划，结束 `timeout.exe` 进程
4. **倒计时更新 (UpdateCountdown)** - 每秒更新一次界面上的倒计时显示

### 时间处理逻辑

- **定时模式**: 解析 HH:MM 格式，若设定时间已过则自动设置为次日
- **倒计时模式**: 将分钟转换为秒，立即计算目标时间戳
- 使用 `time()` 和 `mktime()` 进行时间计算，跨天时自动处理日期递增

### 系统操作实现

| 操作 | 命令 |
|------|------|
| 关机 | `shutdown /s /t [秒数]` |
| 重启 | `shutdown /r /t [秒数]` |
| 待机/休眠 | `rundll32 powrprof.dll,SetSuspendState 0,1,0` |
| 取消任务 | `shutdown /a` |

### UI 风格差异

- **经典版**: 使用系统默认控件样式，依赖 `COLOR_WINDOW` 背景
- **现代版**: 自定义绘制按钮、渐变背景、深色主题配色，使用 `SetWindowSubclass` 实现自绘按钮
- **赛博朋克版**: 使用 GDI+ 实现渐变、发光边框、噪点纹理效果，霓虹配色方案

### 依赖关系

- **TCC (Tiny C Compiler)**: 轻量级编译器，推荐安装在 `C:\tcc`
- **Windows SDK**: 需要 `windows.h`, `commctrl.h`, `shellapi.h`, `gdiplus.h`
- **库文件**: `user32.lib`, `kernel32.lib`, `gdi32.lib`, `comctl32.lib`, `shell32.lib`, `gdiplus.lib`

### 注意事项

- 待机功能可能被系统驱动或程序阻止，运行 `修复待机.bat` 可尝试修复
- 管理员权限版本使用 `ShellExecuteEx` 的 `runas` 动词请求提升权限
- 赛博朋克版本需要 GDI+ 初始化 (`GdiplusStartup`) 和清理 (`GdiplusShutdown`)
- 所有版本都使用 `CREATE_NO_WINDOW` 标志隐藏命令行窗口

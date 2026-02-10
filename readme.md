# AutoShutdown - Windows 自动关机助手

一个简洁、轻量的 Windows 自动关机工具，支持多种 UI 风格和定时模式。

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Platform](https://img.shields.io/badge/platform-Windows-green.svg)
![Language](https://img.shields.io/badge/language-C%2FC%2B%2B-orange.svg)

## 功能特点

- **多种定时模式**：支持定时关机、倒计时关机
- **多种操作类型**：关机、重启、待机/休眠
- **多种 UI 风格**：经典版、现代深色主题版、赛博朋克终端风格版
- **轻量免安装**：单文件可执行程序，无需安装
- **管理员权限支持**：完整支持系统待机和休眠功能

## 下载

前往 [Releases](https://github.com/sunbird02/AutoShutdown/releases) 页面下载最新版本。

## 使用方法

1. 下载 `AutoShutdown.exe`
2. 双击运行程序
3. 选择操作类型（关机/重启/待机）
4. 设置时间：
   - **定时模式**：输入目标时间（如 `23:00`）
   - **倒计时模式**：输入倒计时分钟数
5. 点击「设置任务」启动定时器
6. 可随时点击「取消任务」中止

## 截图

> 截图待添加

## 编译

### 环境要求

- Windows 操作系统
- [TCC (Tiny C Compiler)](https://bellard.org/tcc/) 或 MinGW/g++

### 使用 TCC 编译

```powershell
# 经典版（推荐，体积小）
tcc -o AutoShutdown.exe AutoShutdown.c -luser32 -lkernel32 -lgdi32

# 现代深色主题版
tcc -o AutoShutdown.exe AutoShutdown_modern.c -luser32 -lkernel32 -lgdi32 -lcomctl32 -lshell32

# 管理员权限版
tcc -o AutoShutdown.exe AutoShutdown_管理员.c -luser32 -lkernel32 -lgdi32 -lshell32
```

### 使用 MinGW/g++ 编译赛博朋克版

```powershell
g++ -o AutoShutdown_Cyberpunk.exe AutoShutdown_Cyberpunk.cpp -mwindows -lcomctl32 -lgdiplus -lgdi32 -luser32 -lkernel32
```

### 一键编译

运行 `一键编译.bat` 自动检测 TCC 路径并编译：

```powershell
.\一键编译.bat
```

## 项目结构

```
AutoShutdown/
├── AutoShutdown.c              # 经典版 - 原生 Win32 API
├── AutoShutdown_modern.c       # 现代版 - 深色主题
├── AutoShutdown_Cyberpunk.cpp  # 赛博朋克版 - GDI+
├── AutoShutdown_管理员.c       # 管理员权限版
├── 一键编译.bat               # 一键编译脚本
├── 修复待机.bat               # 待机功能修复工具
└── readme.md                   # 本文件
```

## 系统操作实现

| 操作 | 命令 |
|------|------|
| 关机 | `shutdown /s /t [秒数]` |
| 重启 | `shutdown /r /t [秒数]` |
| 待机/休眠 | `rundll32 powrprof.dll,SetSuspendState 0,1,0` |
| 取消任务 | `shutdown /a` |

## 注意事项

- **待机功能问题**：部分系统可能阻止待机，运行 `修复待机.bat` 尝试修复
- **权限问题**：待机和休眠功能需要管理员权限，建议使用管理员权限版本
- **跨天时间**：定时模式会自动处理跨天情况

## 技术栈

- **语言**: C / C++
- **API**: Win32 API, GDI+
- **编译器**: TCC, MinGW/g++

## 许可证

[MIT License](LICENSE)

## 贡献

欢迎提交 Issue 和 Pull Request！

---

**提示**: 这是一个个人学习项目，代码可能存在不完善之处，欢迎提出建议。

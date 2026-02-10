# AutoShutdown - Windows 自动关机助手

一个简洁、轻量的 Windows 自动关机工具，支持定时关机和倒计时关机。

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Platform](https://img.shields.io/badge/platform-Windows-green.svg)
![Language](https://img.shields.io/badge/language-C-orange.svg)

## 功能特点

- **定时关机**：指定具体时间（如 23:00）自动关机
- **倒计时关机**：设定分钟数后自动关机
- **多种操作**：支持关机、重启、待机/休眠
- **轻量免安装**：单文件可执行程序，无需安装
- **原生 Win32**：使用 Windows API 开发，体积小巧

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

### 编译命令

```powershell
# 使用 TCC 编译
tcc -o AutoShutdown.exe AutoShutdown.c -luser32 -lkernel32 -lgdi32

# 或使用 MinGW/gcc
gcc -o AutoShutdown.exe AutoShutdown.c -luser32 -lkernel32 -lgdi32
```

### 一键编译

运行 `一键编译.bat` 自动检测 TCC 路径并编译：

```powershell
.\一键编译.bat
```

## 项目结构

```
AutoShutdown/
├── AutoShutdown.c      # 主程序源码
├── AutoShutdown.exe    # 编译后的可执行文件
├── 一键编译.bat       # 编译脚本
└── readme.md          # 本文件
```

## 系统操作实现

| 操作 | 命令 |
|------|------|
| 关机 | `shutdown /s /t [秒数]` |
| 重启 | `shutdown /r /t [秒数]` |
| 待机/休眠 | `rundll32 powrprof.dll,SetSuspendState 0,1,0` |
| 取消任务 | `shutdown /a` |

## 注意事项

- **跨天时间**：定时模式会自动处理跨天情况
- **权限问题**：待机和休眠功能可能需要管理员权限

## 技术栈

- **语言**: C
- **API**: Win32 API
- **编译器**: TCC, MinGW/gcc

## 许可证

[MIT License](LICENSE)

## 贡献

欢迎提交 Issue 和 Pull Request！

---

**提示**: 这是一个个人学习项目，代码可能存在不完善之处，欢迎提出建议。

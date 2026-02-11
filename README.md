# AutoShutdown

Windows 10/11 自动待机和关机工具，支持倒计时功能。

## 功能特性

- ⏱️ **倒计时显示** - 大字体实时显示剩余时间
- 🛏️ **待机功能** - 将系统进入睡眠模式
- 🔌 **关机功能** - 关闭系统
- ⏸️ **取消功能** - 中止已计划的操作
- 📦 **轻量级** - 单个 exe 文件，无需安装
- 💾 **低资源占用** - 内存占用 < 20MB

## 系统要求

- Windows 10 或 Windows 11
- .NET Framework 4.8（Windows 10/11 自带）

## 使用方法

### 输入格式

时间输入格式为 `时:分:秒`，例如：
- `0:0:0` - 立即执行
- `0:5:30` - 5 分 30 秒后执行
- `1:30:0` - 1 小时 30 分钟后执行

### 操作步骤

1. 运行 `ShutdownTool.exe`
2. 在输入框中输入延迟时间（格式：时:分:秒）
3. 点击 **待机** 或 **关机** 按钮
4. 倒计时开始显示
5. 点击 **取消** 可中止操作

## 编译

### 前置要求

- 安装 .NET SDK 6.0 或更新版本
- 已配置 csc 编译器到 PATH

### 编译命令

```powershell
dotnet build -c Release
```

编译后的 exe 文件位于：`bin\Release\net48\ShutdownTool.exe`

### 发布单个 exe

```powershell
dotnet publish -c Release -o publish
```

## 版本历史

### v2.0.0
- ✨ 新增倒计时显示功能
- 🎨 改进界面设计
- ⏰ 支持时:分:秒格式输入
- 🔧 优化系统调用

## 许可证

MIT License

## 贡献

欢迎提交 Issue 和 Pull Request！

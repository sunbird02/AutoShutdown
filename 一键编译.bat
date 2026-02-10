@echo off
chcp 65001 >nul
echo ===================================
echo  自动关机助手 - TCC 精简版编译
echo ===================================
echo.

set "TCC_PATH=C:\tcc"

if not exist "%TCC_PATH%\tcc.exe" (
    echo [错误] 未找到 TCC！
    echo 请解压到 C:\tcc
    pause
    exit /b 1
)

echo [1/2] 编译中...
cd /d "%~dp0"

:: 结束旧进程
taskkill /f /im AutoShutdown.exe 2>nul

"%TCC_PATH%\tcc.exe" -o AutoShutdown.exe AutoShutdown.c -luser32 -lkernel32 -lgdi32 -DUNICODE -D_UNICODE

if %errorlevel% neq 0 (
    echo [错误] 编译失败！
    pause
    exit /b 1
)

echo [2/2] 编译成功！
echo.
for %%I in (AutoShutdown.exe) do echo 文件大小: %%~zI 字节
echo.
pause
start AutoShutdown.exe

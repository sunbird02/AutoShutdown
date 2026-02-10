/* 自动关机助手 - 极简风格 */
/* 编译: tcc -o AutoShutdown.exe AutoShutdown.c -luser32 -lkernel32 -lgdi32 */

#include <windows.h>
#include <stdio.h>
#include <time.h>

#define ID_TIME     1001
#define ID_SHUTDOWN 1002
#define ID_STANDBY  1003
#define ID_RESTART  1004
#define ID_SET      1005
#define ID_CANCEL   1006
#define ID_MINCHK   1007
#define ID_MINEDIT  1008
#define ID_TIMER    1

// 极简配色
#define COLOR_BG        RGB(250, 250, 250)    // 近白背景
#define COLOR_TEXT      RGB(50, 50, 50)       // 深灰文字
#define COLOR_TEXT_LIGHT RGB(120, 120, 120)   // 浅灰文字
#define COLOR_ACCENT    RGB(30, 130, 230)     // 蓝灰强调
#define COLOR_LINE      RGB(220, 220, 220)    // 分割线

HWND hTime, hShutdown, hStandby, hRestart, hSet, hCancel;
HWND hMinChk, hMinEdit, hStatus, hCountdown;
int isTimerSet = 0;
time_t targetTime = 0;
HFONT fontTitle, fontBody, fontNum;
HBRUSH hbrBg;

void EnableControls(int enable) {
    EnableWindow(hSet, enable);
    EnableWindow(hCancel, !enable);
    EnableWindow(hTime, enable);
    EnableWindow(hMinEdit, enable && SendMessageW(hMinChk, BM_GETCHECK, 0, 0));
    EnableWindow(hMinChk, enable);
    EnableWindow(hShutdown, enable);
    EnableWindow(hStandby, enable);
    EnableWindow(hRestart, enable);
}

void UpdateCountdown() {
    wchar_t buf[32];
    time_t now;
    int sec, h, m, s;
    
    if (!isTimerSet) return;
    
    time(&now);
    sec = (int)(targetTime - now);
    
    if (sec <= 0) {
        SetWindowTextW(hCountdown, L"执行中");
        return;
    }
    
    h = sec / 3600;
    m = (sec % 3600) / 60;
    s = sec % 60;
    swprintf(buf, L"%02d:%02d:%02d", h, m, s);
    SetWindowTextW(hCountdown, buf);
}

void OnSet(HWND hwnd) {
    wchar_t buf[256];
    int seconds = 0, hour = 0, minute = 0;
    int isShutdown = 0, isStandby = 1;
    wchar_t cmd[512];
    
    if (SendMessageW(hShutdown, BM_GETCHECK, 0, 0)) {
        isShutdown = 1;
        isStandby = 0;
    } else if (SendMessageW(hRestart, BM_GETCHECK, 0, 0)) {
        isShutdown = 0;
        isStandby = 0;
    }
    
    if (SendMessageW(hMinChk, BM_GETCHECK, 0, 0)) {
        GetWindowTextW(hMinEdit, buf, 256);
        for (int i = 0; buf[i]; i++) {
            if (buf[i] >= L'0' && buf[i] <= L'9') seconds = seconds * 10 + (buf[i] - L'0');
        }
        seconds *= 60;
        if (seconds <= 0) seconds = 60;
        time(&targetTime);
        targetTime += seconds;
    } else {
        GetWindowTextW(hTime, buf, 256);
        int state = 0;
        for (int i = 0; buf[i]; i++) {
            if (buf[i] >= L'0' && buf[i] <= L'9') {
                if (!state) hour = hour * 10 + (buf[i] - L'0');
                else minute = minute * 10 + (buf[i] - L'0');
            } else if (buf[i] == L':') state = 1;
        }
        
        if (hour > 23 || minute > 59) {
            MessageBoxW(hwnd, L"时间格式错误，请使用 HH:MM 格式", L"提示", MB_OK);
            return;
        }
        
        time_t now;
        struct tm tm;
        time(&now);
        tm = *localtime(&now);
        tm.tm_hour = hour;
        tm.tm_min = minute;
        tm.tm_sec = 0;
        
        if (mktime(&tm) <= now) tm.tm_mday++;
        targetTime = mktime(&tm);
        seconds = (int)(targetTime - now);
    }
    
    STARTUPINFOW si = {sizeof(si)};
    PROCESS_INFORMATION pi;
    
    if (isStandby) {
        swprintf(cmd, L"cmd /c timeout /t %d /nobreak && rundll32 powrprof.dll,SetSuspendState 0,1,0", seconds);
        if (CreateProcessW(NULL, cmd, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
            CloseHandle(pi.hProcess); CloseHandle(pi.hThread);
            isTimerSet = 1;
            EnableControls(0);
            SetWindowTextW(hStatus, L"已设置：待机");
        }
    } else {
        swprintf(cmd, L"shutdown %s /t %d", isShutdown ? L"/s" : L"/r", seconds);
        if (CreateProcessW(NULL, cmd, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
            CloseHandle(pi.hProcess); CloseHandle(pi.hThread);
            isTimerSet = 1;
            EnableControls(0);
            SetWindowTextW(hStatus, isShutdown ? L"已设置：关机" : L"已设置：重启");
        }
    }
}

void OnCancel(HWND hwnd) {
    STARTUPINFOW si = {sizeof(si)};
    PROCESS_INFORMATION pi;
    CreateProcessW(NULL, L"shutdown /a", NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
    CreateProcessW(NULL, L"taskkill /f /im timeout.exe", NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
    
    isTimerSet = 0;
    EnableControls(1);
    SetWindowTextW(hStatus, L"就绪");
    SetWindowTextW(hCountdown, L"00:00:00");
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l) {
    switch (msg) {
        case WM_CREATE: {
            // 极简字体
            fontTitle = CreateFontW(18, 0, 0, 0, FW_LIGHT, 0, 0, 0, DEFAULT_CHARSET, 
                                   0, 0, CLEARTYPE_QUALITY, 0, L"Segoe UI");
            fontBody = CreateFontW(12, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, 
                                  0, 0, CLEARTYPE_QUALITY, 0, L"Segoe UI");
            fontNum = CreateFontW(22, 0, 0, 0, FW_LIGHT, 0, 0, 0, DEFAULT_CHARSET, 
                                 0, 0, CLEARTYPE_QUALITY, 0, L"Segoe UI");
            
            hbrBg = CreateSolidBrush(COLOR_BG);
            
            // 标题 - 居中大字体
            HWND hTitle = CreateWindowW(L"STATIC", L"自动关机", 
                                       WS_VISIBLE|WS_CHILD|SS_CENTER, 0, 25, 360, 30, hwnd, NULL, NULL, NULL);
            SendMessageW(hTitle, WM_SETFONT, (WPARAM)fontTitle, 1);
            
            // 第一组：时间
            CreateWindowW(L"STATIC", L"设定时间", WS_VISIBLE|WS_CHILD, 40, 75, 60, 18, hwnd, NULL, NULL, NULL);
            hTime = CreateWindowW(L"EDIT", L"21:30", WS_VISIBLE|WS_CHILD|WS_BORDER|ES_CENTER, 
                                 40, 95, 100, 28, hwnd, (HMENU)ID_TIME, NULL, NULL);
            SendMessageW(hTime, WM_SETFONT, (WPARAM)fontNum, 1);
            
            // 倒计时选项
            hMinChk = CreateWindowW(L"BUTTON", L"倒计时", WS_VISIBLE|WS_CHILD|BS_AUTOCHECKBOX, 
                                   160, 78, 70, 18, hwnd, (HMENU)ID_MINCHK, NULL, NULL);
            hMinEdit = CreateWindowW(L"EDIT", L"30", WS_VISIBLE|WS_CHILD|WS_BORDER|ES_NUMBER|ES_CENTER, 
                                    160, 95, 50, 28, hwnd, (HMENU)ID_MINEDIT, NULL, NULL);
            SendMessageW(hMinEdit, WM_SETFONT, (WPARAM)fontNum, 1);
            
            CreateWindowW(L"STATIC", L"分钟", WS_VISIBLE|WS_CHILD, 215, 100, 35, 18, hwnd, NULL, NULL, NULL);
            EnableWindow(hMinEdit, 0);
            
            // 第二组：操作类型
            CreateWindowW(L"STATIC", L"操作", WS_VISIBLE|WS_CHILD, 40, 145, 40, 18, hwnd, NULL, NULL, NULL);
            
            hStandby = CreateWindowW(L"BUTTON", L"待机", WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON|WS_GROUP, 
                                    40, 168, 60, 20, hwnd, (HMENU)ID_STANDBY, NULL, NULL);
            hShutdown = CreateWindowW(L"BUTTON", L"关机", WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON, 
                                     110, 168, 60, 20, hwnd, (HMENU)ID_SHUTDOWN, NULL, NULL);
            hRestart = CreateWindowW(L"BUTTON", L"重启", WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON, 
                                    180, 168, 60, 20, hwnd, (HMENU)ID_RESTART, NULL, NULL);
            SendMessageW(hStandby, BM_SETCHECK, 1, 0);
            
            // 按钮
            hSet = CreateWindowW(L"BUTTON", L"开始", WS_VISIBLE|WS_CHILD|BS_FLAT, 
                                40, 220, 110, 36, hwnd, (HMENU)ID_SET, NULL, NULL);
            hCancel = CreateWindowW(L"BUTTON", L"取消", WS_VISIBLE|WS_CHILD|BS_FLAT|WS_DISABLED, 
                                   170, 220, 110, 36, hwnd, (HMENU)ID_CANCEL, NULL, NULL);
            
            // 第三组：状态
            hStatus = CreateWindowW(L"STATIC", L"就绪", WS_VISIBLE|WS_CHILD|SS_CENTER, 
                                   40, 280, 240, 20, hwnd, NULL, NULL, NULL);
            SendMessageW(hStatus, WM_SETFONT, (WPARAM)fontBody, 1);
            
            hCountdown = CreateWindowW(L"STATIC", L"00:00:00", WS_VISIBLE|WS_CHILD|SS_CENTER, 
                                      40, 305, 240, 35, hwnd, NULL, NULL, NULL);
            SendMessageW(hCountdown, WM_SETFONT, (WPARAM)fontNum, 1);
            
            SetTimer(hwnd, ID_TIMER, 1000, NULL);
            return 0;
        }
        
        case WM_CTLCOLORSTATIC: {
            HDC hdc = (HDC)w;
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, COLOR_TEXT);
            return (LRESULT)hbrBg;
        }
        
        case WM_CTLCOLOREDIT: {
            HDC hdc = (HDC)w;
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, COLOR_TEXT);
            return (LRESULT)hbrBg;
        }
        
        case WM_ERASEBKGND: {
            HDC hdc = (HDC)w;
            RECT rc;
            GetClientRect(hwnd, &rc);
            FillRect(hdc, &rc, hbrBg);
            return 1;
        }
        
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            
            // 绘制分隔线
            HPEN pen = CreatePen(PS_SOLID, 1, COLOR_LINE);
            HPEN oldPen = SelectObject(hdc, pen);
            
            MoveToEx(hdc, 40, 60, NULL);
            LineTo(hdc, 320, 60);
            MoveToEx(hdc, 40, 205, NULL);
            LineTo(hdc, 320, 205);
            MoveToEx(hdc, 40, 270, NULL);
            LineTo(hdc, 320, 270);
            
            SelectObject(hdc, oldPen);
            DeleteObject(pen);
            
            EndPaint(hwnd, &ps);
            return 0;
        }
        
        case WM_COMMAND:
            if (LOWORD(w) == ID_SET) OnSet(hwnd);
            else if (LOWORD(w) == ID_CANCEL) OnCancel(hwnd);
            else if (LOWORD(w) == ID_MINCHK && HIWORD(w) == BN_CLICKED)
                EnableWindow(hMinEdit, SendMessageW(hMinChk, BM_GETCHECK, 0, 0));
            return 0;
        
        case WM_TIMER:
            if (w == ID_TIMER) UpdateCountdown();
            return 0;
        
        case WM_CLOSE:
            if (isTimerSet && MessageBoxW(hwnd, L"任务运行中，退出不取消任务。确定退出？", L"确认", MB_YESNO) == IDNO) return 0;
            DestroyWindow(hwnd);
            return 0;
        
        case WM_DESTROY:
            DeleteObject(fontTitle);
            DeleteObject(fontBody);
            DeleteObject(fontNum);
            DeleteObject(hbrBg);
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProcW(hwnd, msg, w, l);
}

int WINAPI WinMain(HINSTANCE hi, HINSTANCE hp, LPSTR cl, int show) {
    WNDCLASSEXW wc = {sizeof(wc)};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hi;
    wc.lpszClassName = L"AutoShutdownMinimal";
    wc.hbrBackground = hbrBg;
    wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
    RegisterClassExW(&wc);
    
    HWND hwnd = CreateWindowExW(0, L"AutoShutdownMinimal", L"自动关机", 
                               WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX,
                               CW_USEDEFAULT, CW_USEDEFAULT, 380, 420,
                               NULL, NULL, hi, NULL);
    ShowWindow(hwnd, show);
    UpdateWindow(hwnd);
    
    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return (int)msg.wParam;
}

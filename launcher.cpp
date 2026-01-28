// Nuitka Shim Launcher (Dynamic Version)
// 自动寻找 _internal 下与自己同名的 exe 进行启动
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>

// 配置：内部文件夹名称
#define INTERNAL_FOLDER_NAME _T("_internal")

void ErrorMessage(const TCHAR* msg) {
    MessageBox(NULL, msg, _T("Launcher Error"), MB_OK | MB_ICONERROR);
}

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
    // 1. 获取当前启动器的完整路径 (例如 C:\App\MyTool.exe)
    TCHAR szSelfPath[MAX_PATH];
    if (GetModuleFileName(NULL, szSelfPath, MAX_PATH) == 0) {
        return 1;
    }

    // 2. 分离 目录 和 文件名
    TCHAR szDir[MAX_PATH];    // C:\App
    TCHAR szName[MAX_PATH];   // MyTool.exe
    
    StringCchCopy(szDir, MAX_PATH, szSelfPath);
    LPTSTR pSlash = _tcsrchr(szDir, _T('\\'));
    
    if (pSlash) {
        // 复制文件名部分
        StringCchCopy(szName, MAX_PATH, pSlash + 1);
        // 截断路径得到目录
        *pSlash = _T('\0');
    } else {
        // 理论上不会发生，除非在根目录等极端情况
        StringCchCopy(szName, MAX_PATH, szSelfPath);
        szDir[0] = _T('\0'); 
    }

    // 3. 拼接目标路径: current_dir/_internal/SameName.exe
    TCHAR szTargetExe[MAX_PATH];
    // 格式：C:\App\_internal\MyTool.exe
    StringCchPrintf(szTargetExe, MAX_PATH, _T("%s\\%s\\%s"), szDir, INTERNAL_FOLDER_NAME, szName);

    // 4. 检查目标是否存在
    DWORD dwAttrib = GetFileAttributes(szTargetExe);
    if (dwAttrib == INVALID_FILE_ATTRIBUTES || (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)) {
        TCHAR msg[1024];
        StringCchPrintf(msg, 1024, 
            _T("Target executable not found!\n\n")
            _T("Launcher: %s\n")
            _T("Looking for: %s"), 
            szName, szTargetExe);
        ErrorMessage(msg);
        return 1;
    }

    // 5. 构造命令行参数
    // 需要处理参数转发，格式: "path\to\target.exe" arg1 arg2 ...
    size_t lenTarget = _tcslen(szTargetExe);
    size_t lenCmd = _tcslen(lpCmdLine);
    size_t totalLen = lenTarget + lenCmd + 10; 
    
    LPTSTR szFinalCmd = (LPTSTR)LocalAlloc(LPTR, totalLen * sizeof(TCHAR));
    if (!szFinalCmd) return 1;

    // 这里的 lpCmdLine 是不包含程序名的纯参数部分
    // 注意：如果是从 cmd 启动，lpCmdLine 可能会有差异，但在 WinMain 中通常是除去程序名后的部分
    if (_tcslen(lpCmdLine) > 0) {
        StringCchPrintf(szFinalCmd, totalLen, _T("\"%s\" %s"), szTargetExe, lpCmdLine);
    } else {
        StringCchPrintf(szFinalCmd, totalLen, _T("\"%s\""), szTargetExe);
    }

    // 6. 启动进程
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = nCmdShow; 

    BOOL bRet = CreateProcess(
        NULL,           
        szFinalCmd,     
        NULL,           
        NULL,           
        FALSE,          
        0,              
        NULL,           
        NULL,           // 工作目录默认继承，这样程序读取相对路径配置文件不会错
        &si,
        &pi
    );

    if (!bRet) {
        ErrorMessage(_T("Failed to launch internal application."));
        LocalFree(szFinalCmd);
        return 1;
    }

    // 7. 等待子进程 (可选，如果只是启动器，建议不等待直接退出，除非你需要捕获退出码)
    // WaitForSingleObject(pi.hProcess, INFINITE); 
    // 上面这行注释掉，启动器点完就消失，不会占后台内存

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    LocalFree(szFinalCmd);

    return 0;
}
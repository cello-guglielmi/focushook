#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>

DWORD FindProcessId(const char *name) {
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe = { sizeof(pe) };
    if (Process32First(snap, &pe)) {
        do {
            if (_stricmp(pe.szExeFile, name) == 0) {
                CloseHandle(snap);
                return pe.th32ProcessID;
            }
        } while (Process32Next(snap, &pe));
    }
    CloseHandle(snap);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: injector.exe <process.exe> <full\\path\\to\\focus_hook.dll>\n");
        return 1;
    }

    DWORD pid = FindProcessId(argv[1]);
    if (!pid) { printf("Process not found: %s\n", argv[1]); return 1; }

    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProc) { printf("Failed to open process. Try running as Administrator.\n"); return 1; }

    size_t len = strlen(argv[2]) + 1;
    LPVOID mem = VirtualAllocEx(hProc, NULL, len, MEM_COMMIT, PAGE_READWRITE);
    WriteProcessMemory(hProc, mem, argv[2], len, NULL);

    HANDLE hThread = CreateRemoteThread(hProc, NULL, 0,
        (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA"),
        mem, 0, NULL);

    if (!hThread) { printf("Injection failed.\n"); return 1; }

    WaitForSingleObject(hThread, 3000);
    CloseHandle(hThread);
    CloseHandle(hProc);
    printf("Done! Focus hook injected.\n");
    return 0;
}
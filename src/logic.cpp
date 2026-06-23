#include <Windows.h>
#include <vector>
#include <string>
#include "..\headers\KiwiMapper.h"
#pragma comment(lib, "ntdll.lib")

std::string OpenFileDialog() {
    OPENFILENAMEA ofn;
    char szFile[260] = { 0 };

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "Dynamic Link Library (*.dll)\0*.dll\0All Files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileNameA(&ofn)) {
        return std::string(szFile);
    }
    return "";
}

void findpid(Procs* List) {

    List->pids.clear();
    List->names.clear();

    ULONG retLen = 0;
    static PVOID buf = VirtualAlloc(NULL, 5242880, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!buf) return;

    if (NtQuerySystemInformation((SYSTEM_INFORMATION_CLASS)5, buf, 5242880, &retLen) >= 0) {
        auto procs = (PSYSTEM_PROCESS_INFORMATION)buf;
        size_t index = 0;

        while (1) {
            if (procs->SessionId > 0) {
                if (index >= List->pids.size()) {
                    List->pids.push_back((DWORD)(DWORD_PTR)procs->UniqueProcessId);
                    List->names.emplace_back();
                }
                else List->pids[index] = (DWORD)(DWORD_PTR)procs->UniqueProcessId;

                std::wstring& current_name = List->names[index];
                if (procs->ImageName.Buffer && procs->ImageName.Length > 0) {
                    current_name.assign(procs->ImageName.Buffer, procs->ImageName.Length / sizeof(wchar_t));
                    for (auto& c : current_name) if (c >= L'A' && c <= L'Z') c += 32;
                }
                else current_name = L"unknown";

                index++;
            }
            if (!procs->NextEntryOffset) break;
            procs = (PSYSTEM_PROCESS_INFORMATION)((PBYTE)procs + procs->NextEntryOffset);
        }

    }
}

DWORD Inject(DWORD pid, std::string path, int method, int count) {
    std::string s = "#" + std::to_string(count) + " PID:" + std::to_string(pid);
    HANDLE hProc = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ | PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION, FALSE, pid);

    // Лог хэндла пишется только для самой первой DLL
    if (count == 0) {
        if (hProc) Logs.push_back("[+] " + s + " Handle OK");
        else { Logs.push_back("[×] " + s + " Handle err"); return false; }
    }
    else if (!hProc) return false;

    if (method == 0) {
        LPVOID pMem = VirtualAllocEx(hProc, NULL, strlen(path.c_str()) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        if (!pMem) { Logs.push_back("[×] " + s + " Alloc err"); CloseHandle(hProc); return false; }
        Logs.push_back("[+] " + s + " Alloc OK");

        if (!WriteProcessMemory(hProc, pMem, path.c_str(), strlen(path.c_str()) + 1, NULL)) { Logs.push_back("[×] " + s + " Write err"); VirtualFreeEx(hProc, pMem, 0, MEM_RELEASE); CloseHandle(hProc); return false; }
        Logs.push_back("[+] " + s + " Write OK");

        LPVOID pLoadLib = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
        HANDLE hThread = CreateRemoteThread(hProc, NULL, 0, (LPTHREAD_START_ROUTINE)pLoadLib, pMem, 0, NULL);

        if (hThread) {
            Logs.push_back("[+] " + s + " Thread OK");
            WaitForSingleObject(hThread, INFINITE);
            CloseHandle(hThread);
            Logs.push_back("[+] " + s + " Success");
        }
        else Logs.push_back("[×] " + s + " Thread err");

        VirtualFreeEx(hProc, pMem, 0, MEM_RELEASE);
        CloseHandle(hProc);
        return hThread != NULL;
    }
    return 0;
}







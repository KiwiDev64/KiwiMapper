#pragma once
#define PHNT_VERSION PHNT_THRESHOLD
#include <phnt_windows.h>
#include <phnt.h>
#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <d3d11.h>
#include <tchar.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_impl_win32.h>
#include <dwmapi.h>
#include <imgui_impl_dx11.h>

// структуры

struct Procs {
	std::vector<DWORD> pids;
	std::vector<std::wstring> names;
};
extern Procs list;

// Прототипы функций
extern bool CreateDeviceD3D(HWND hWnd);
extern void CleanupDeviceD3D();
extern void CreateRenderTarget();
extern void CleanupRenderTarget();
extern std::string OpenFileDialog();
extern LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
extern void findpid(Procs* List);
DWORD Inject(DWORD pid, std::string path, int method, int count);

// GUI
extern HWND hwnd;
extern void gui();

// GUI: указатели

extern ID3D11Device* g_pd3dDevice;
extern ID3D11DeviceContext* g_pd3dDeviceContext;
extern ID3D11RenderTargetView* g_mainRenderTargetView;
extern IDXGISwapChain* g_pSwapChain;
extern WNDCLASSEXW wc;

// logic: переменные

extern DWORD pid;
extern std::vector<std::string> Logs;
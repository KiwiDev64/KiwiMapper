#include "..\headers\KiwiMapper.h"
#include <filesystem>
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
using namespace ImGui;

void style_s(float* clr, float* clr1, float* clr2, float* childround, float* frameround) {
	PushStyleColor(ImGuiCol_WindowBg, ImVec4(clr[0], clr[1], clr[2], 0.1f));
	PushStyleColor(ImGuiCol_TitleBg, ImVec4(clr1[0] - 0.1, clr1[1] - 0.1, clr1[2] - 0.1, 1));
	PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(clr1[0], clr1[1], clr1[2], 1));
	PushStyleColor(ImGuiCol_Button, ImVec4(clr2[0], clr2[1], clr2[2], clr2[3]));
	PushStyleVar(ImGuiStyleVar_ScrollbarSize, 0.0f);
	PushStyleVar(ImGuiStyleVar_ChildRounding, childround[0]);
	PushStyleVar(ImGuiStyleVar_FrameRounding, frameround[0]);
	PushStyleVar(ImGuiStyleVar_WindowRounding, childround[0]);
}

void style_e() {
	PopStyleVar(4);
	PopStyleColor(4);
}

void setts(bool* set, float* clr, float* clr1, float* clr2, float* childround, float* frameround, float* speed, const char** methods, int* method, int methods_count) {
	SetNextWindowSize(ImVec2(255, 125), ImGuiCond_Always);
	PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 1));
	Begin("Settings Menu", set, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	SetNextItemWidth(43.0f);
	DragFloat("FrameRound", frameround, 0.05, 0.0f, 12.5f, "%.2f");
	SameLine(146.0f);
	ColorEdit3("BGColor", clr, ImGuiColorEditFlags_NoInputs);

	SetNextItemWidth(43.0f);
	DragFloat("WindowRound", childround, 0.05, 0.0f, 12.5f, "%.2f");
	SameLine(146.0f);
	ColorEdit3("TitleColor", clr1, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);

	SetNextItemWidth(43.0f);
	DragFloat("AnimSpeed", speed, 0.05, 1.0f, 50.0f, "%.2f");
	SameLine(146.0f);
	ColorEdit4("ButtonColor", clr2, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);

	SetNextItemWidth(240.0f);
	Combo("##InjectionMethod", method, methods, methods_count);

	End();

	PopStyleColor();
}

struct DLLs {
	std::string path;
	std::string name;
	bool enabled;
	DLLs(std::string p, bool e = true) : path(p), enabled(e) {
		name = std::filesystem::path(p).filename().string();
	}
};
std::vector<DLLs> dllList;
std::vector<std::string> Logs;
Procs list;

void dlls(bool remove) {
	if (remove) {
		for (int i = 0; i < dllList.size(); i++) {
			if (dllList[i].enabled == true) {
				Logs.push_back("[-] " + dllList[i].name);
				dllList.erase(dllList.begin() + i);
				i--;
			}
		}
		return;
	}

	BeginChild("DLLs", ImVec2(296, 135), true);
	for (int i = 0; i < dllList.size(); i++) {
		ImGui::Checkbox(("##check" + std::to_string(i)).c_str(), &dllList[i].enabled);
		ImGui::SameLine();
		ImGui::Text(dllList[i].name.c_str());
	}
	EndChild();
}

void loglist(const std::vector<std::string>& logs, float h) {
	BeginChild("LogRegion", ImVec2(296, h), true);

	PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.0f));
	PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
	if (Button("Clear", ImVec2(-1.0f, 10.0f))) Logs.clear();
	PopStyleVar(2);

	for (const auto& log_line : logs) {
		TextUnformatted(log_line.c_str());
	}

	EndChild();
}

void choiceprocess(bool* done, char* proc_buffer, DWORD& out_pid, bool& ping, float h, float w) {
	if (!ping) {
		findpid(&list);
		ping = true;
	}

	float h1 = (h > 181) ? (h + 4.0f) : h;

	if (ImGui::BeginChild("SelectProc", ImVec2(w, h1), true)) {
		for (size_t i = 0; i < list.names.size(); i++) {
			if (list.names[i].empty()) continue;

			std::wstring wName = list.names[i];
			int size_needed = WideCharToMultiByte(CP_UTF8, 0, wName.c_str(), (int)wName.length(), NULL, 0, NULL, NULL);
			std::string sName(size_needed, 0);
			WideCharToMultiByte(CP_UTF8, 0, wName.c_str(), (int)wName.length(), &sName[0], size_needed, NULL, NULL);

			std::string rowText = sName + " (PID: " + std::to_string(list.pids[i]) + ")##" + std::to_string(i);

			if (ImGui::Selectable(rowText.c_str(), false, 0, ImVec2(0, 15))) {
				strncpy_s(proc_buffer, 256, sName.c_str(), _TRUNCATE);
				out_pid = list.pids[i];
				*done = false;
			}
		}
		ImGui::EndChild();
	}
}

void gui() {
	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = NULL;

	static ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;
	static float animProgress = 0.0f;
	static float animProgressw = 0.0f;
	static bool selectproc = false;
	static char process[128] = ""; // НАЗВАНИЕ ПРОЦЕССА
	static DWORD procpid; // ЕГО ПИД
	static bool pidspinged = false; // чтобы один раз проверял пиды

	float clr[] = { 0.0f,0.0f,0.0f };
	float clr1[] = { 0.2f,0.2f,0.2f };
	float clr2[] = { 0.2f,0.2f,0.2f,0.8f };
	float roundwindow[] = { 2.0f };
	float roundframe[] = { 2.0f };
	float speed[] = { 35.0f };

	bool done = true;
	bool settings = 0;
	bool showLogs = false;

	int Height = 216;
	int Widgth = 312;
	static int WidgthConst = Widgth;
	static int HeightConst = Height;

	const char* methods[] = { "Standard Injection" };
	static int current_method = 0;

	while (done) {

		MSG msg;
		while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT) break;
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		NewFrame();

		{
			SetNextWindowSize(ImVec2(Widgth, Height), ImGuiCond_Always);
			style_s(clr, clr1, clr2, roundwindow, roundframe);

			if (settings) flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoInputs;
			else { flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize; }


			Begin("Kiwi v1.0 (🥝)", &done, flags);

			PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(7.0f, 4.0f));
			SetNextItemWidth(201);
			InputTextWithHint("##procname", "process.exe", process, IM_ARRAYSIZE(process), ImGuiInputTextFlags_ReadOnly);
			SameLine(); SetNextItemWidth(30); Text("PID:");

			PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 3));
			SameLine(); SetNextItemWidth(28);
			InputScalar("##procpid", ImGuiDataType_U32, &procpid, nullptr, nullptr, "%u", ImGuiInputTextFlags_ReadOnly);
			SameLine();
			PopStyleVar();

			PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 3));
			if (Button("+##ChooseProcess")) selectproc = !selectproc;
			PopStyleVar(2);
			SameLine(); ImVec2 pos = GetCursorPos(); NewLine();

			PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6.0f, 4.0f));

			if (Button("Inject")) {
				Logs.push_back("[#########################]");
				int index = 0;
				for (const auto& dll : dllList) { Inject(procpid, dll.path, current_method, index++); }
				index = 0;
				Logs.push_back("[#########################]");
				Logs.push_back("\n");
				//MessageBoxA(NULL, "Injection Successfully!", "OK", MB_ICONINFORMATION);
			}


			SetCursorPosX(100); SameLine();

			if (Button("Settings")) settings = !settings; SameLine();

			if (Button("Show Logs")) {
				showLogs = !showLogs;
			} SameLine();

			if (Button("Add")) {
				flags |= ImGuiWindowFlags_NoInputs; std::string path = OpenFileDialog(); flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
				if (path != "") {
					Logs.push_back("[+] " + std::filesystem::path(path).filename().string());
					dllList.push_back(path);
				};

			}SameLine();

			if (Button("Remove")) dlls(true);
			ImVec2 dianimcur = GetCursorPos();

			PopStyleVar();

			// анимашке
			static float currentLogHeight = 0.0f;
			float direction = showLogs ? 1.0f : -1.0f;
			animProgress += direction * (speed[0] / 10.0f) * ImGui::GetIO().DeltaTime;
			if (animProgress > 1.0f) animProgress = 1.0f;
			if (animProgress < 0.0f) animProgress = 0.0f;
			float smoothFactor = -(cosf(animProgress * 3.141592f) - 1.0f) / 2.0f;
			Height = HeightConst + (239.0f * smoothFactor);

			if (Height > HeightConst) {
				currentLogHeight = 235.0f * smoothFactor;
				loglist(Logs, currentLogHeight);
				dianimcur = GetCursorPos();
			}
			else {
				currentLogHeight = 0.0f;
				Height = HeightConst;
			}

			float directionw = selectproc ? 1.0f : -1.0f;
			animProgressw += directionw * (speed[0] / 10.0f) * ImGui::GetIO().DeltaTime;
			if (animProgressw > 1.0f) animProgressw = 1.0f;
			if (animProgressw < 0.0f) animProgressw = 0.0f;
			float smoothFactorw = -(cosf(animProgressw * 3.141592f) - 1.0f) / 2.0f;
			Widgth = WidgthConst + (314.0f * smoothFactorw);
			ImVec2 enanimcur = dianimcur;
			if (Widgth > WidgthConst) {
				enanimcur = GetCursorPos();
				SetCursorPos(pos);
				choiceprocess(&selectproc, process, procpid, pidspinged, 181.0f + currentLogHeight, 304.0f * smoothFactorw);
			}
			else { Widgth = WidgthConst;  pidspinged = 0; }

			// настройки
			if (settings) setts(&settings, clr, clr1, clr2, roundwindow, roundframe, speed, methods, &current_method, IM_ARRAYSIZE(methods));

			// чилды
			SetCursorPos(enanimcur);
			dlls(false);

			End(); style_e();

		}


		ImGui::Render();
		g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
		g_pSwapChain->Present(1, 0);
	}
}
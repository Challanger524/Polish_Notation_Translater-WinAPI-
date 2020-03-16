#include "Precompiled.h"
#include "API.h"
#include "Translator.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance[[maybe_unused]], LPSTR lpCmdLine[[maybe_unused]], int nCmdShow[[maybe_unused]])
{
	Craete_Console();	

	Timer t;
	MSG msg;
	Main Window("Notation transleter", GetSystemMetrics(SM_CXSCREEN) / 2 - 400, GetSystemMetrics(SM_CYSCREEN) / 2 - 250, 800, 500, hInstance);
	Manual Dialog("Manual", 0, 0, 200, 300);

	hWndMain = static_cast<HWND>(Window);
	if (!hWndMain) {
		PrintError("Main window creation failed");
		return 1;
	}
	hDialog = static_cast<HWND>(Dialog);
	if (!hDialog) {
		PrintError("Dialog creation failed");
		return 2;
	}

	if (thread::hardware_concurrency() > 1u) Terminal = Terminal_Double_Thread;
	else Terminal = Terminal_Single_Thread;	
	cout.precision(4);

	while (GetMessage(&msg, NULL, 0, 0)) {
		if (msg.wParam == VK_RETURN) {//for keyboard 'Enter' hooking
			if (msg.message == WM_KEYUP) SendMessage(hWndMain, VK_RETURN, 0, 0);
			continue;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	cout.precision(2);
	cout << "\nTermination...\nOperating time: " << static_cast<std::chrono::duration<float>>(t.get()).count() << " sec.\n";
	if (IsWindowVisible(GetConsoleWindow())) system("pause");

	return msg.wParam;//exit
}
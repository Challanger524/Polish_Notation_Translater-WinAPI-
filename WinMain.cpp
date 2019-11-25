#include "Precompiled.h"
#include "API.h"
#include "Translator.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Craete_Console();

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

	if (thread::hardware_concurrency() > 1) Terminal = Terminal_Double_Thread;
	else Terminal = Terminal_Single_Thread;	

	ShowWindow(hWndMain, SW_SHOWNORMAL);

	while (GetMessage(&msg, NULL, 0, 0)) {
		if (msg.wParam == VK_RETURN) {//for keyboard 'Enter' hooking
			if (msg.message == WM_KEYUP) SendMessage(hWndMain, VK_RETURN, 0, 0);
			continue;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	//return 0;
	return msg.wParam;
}
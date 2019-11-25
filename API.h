#pragma once
#include "Precompiled.h"
//#include "resource.h"

enum BT {
	BT_PASTE = 3000,
	BT_COPY1,
	BT_COPY2,
	BT_TRANSL,
	BT_MANUAL
};

extern HWND hWndMain;
extern HWND hDialog;

extern bool(*Terminal)(const char *, unique_ptr<char[]> &, unique_ptr<char[]> &);

extern int OperChecker(const char Operator);
void PrintOut(const char *Buffer, const unique_ptr<char[]> &res1, const unique_ptr<char[]> &res2);

void Craete_Console();
void PrintError(const char *Message);

class Main {
	HWND handle = nullptr;
	static LRESULT CALLBACK MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);// Заголовок оконной функции
public:
	explicit operator HWND () const { return handle; }
	explicit Main(LPCSTR caption, int Pos_X, int Pos_Y, int Siz_X, int Siz_Y, HINSTANCE hInstance);
	Main(const Main&) = delete;
	Main operator =(const Main&) = delete;
private:
	Main() {}
	//~Main() {}
};
class Manual {
	HWND handle = nullptr;
	static LRESULT CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);
public:
	explicit operator HWND () const { return handle; }
	explicit Manual(LPCSTR caption, int Pos_X, int Pos_Y, int Siz_X, int Siz_Y)
	{
		const char ClassName[] = "DialogClass";
		WNDCLASS wc = {0};
		if (!handle)
		{
			memset(&wc, 0, sizeof(wc));
			wc.lpfnWndProc = (WNDPROC) DialogProc;
			wc.hInstance = (HINSTANCE) GetModuleHandle(NULL);
			wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
			//wc.hIcon = LoadIconA (GetModuleHandleA (NULL), MAKEINTRESOURCEA (ICON_TAB));
			if (wc.hIcon == NULL) wc.hIcon = LoadIconA(NULL, IDI_APPLICATION);
			wc.lpszClassName = ClassName;

			if (RegisterClass(&wc)) {
				cout << "DialogClass registerd\n";
				handle = CreateWindow(ClassName, caption, WS_CAPTION | WS_CLIPCHILDREN, Pos_X, Pos_Y, Siz_X, Siz_Y, NULL, NULL, (HINSTANCE) GetModuleHandle(NULL), NULL);
			}
			else {
				cout << "FAIL: to register DialogClass\n";
				terminate();
			}
			if (handle) cout << "Dialog Box created\n";
			else {
				cout << "FAIL: to create Dialog Box\n";
				terminate();
			}
		}
		else {
			PrintError("Only one instance of the window is allowed!");
			terminate();
		}
	}
	Manual(const Manual&) = delete;
	Manual operator =(const Manual&) = delete;
private:
	Manual() {}
	//~Manual() {}
};

#if 0
LRESULT CALLBACK Main::MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_CREATE: {
		GetClientRect(hWnd, &Mrc);
		hEdit1 = CreateWindow("Edit", "a+b", WS_BORDER | WS_CHILD | ES_AUTOHSCROLL | WS_VISIBLE, (Mrc.right - Mrc.left) / 6, (Mrc.bottom - Mrc.top) / 6, int((Mrc.right - Mrc.left)*0.6), 22, hWnd, NULL, NULL, NULL);
		hEdit2 = CreateWindow("Edit", "Here will be the answer", WS_BORDER | WS_CHILD | ES_READONLY | WS_VISIBLE, (Mrc.right - Mrc.left) / 6, (Mrc.bottom - Mrc.top) / 2, int((Mrc.right - Mrc.left)*0.6), 22, hWnd, NULL, NULL, NULL);
		hEdit3 = CreateWindow("Edit", "Here will be the answer", WS_BORDER | WS_CHILD | ES_READONLY | WS_VISIBLE, (Mrc.right - Mrc.left) / 6, int((Mrc.bottom - Mrc.top) / 1.5), int((Mrc.right - Mrc.left)*0.6), 22, hWnd, NULL, NULL, NULL);

		hEdit11 = CreateWindow("Edit", "Expression type detector", WS_CHILD | WS_VISIBLE | WS_DISABLED, (Mrc.right - Mrc.left) / 6 + 2, (Mrc.bottom - Mrc.top) / 6 + 22, 190, 22, hWnd, NULL, NULL, NULL);
		hEdit21 = CreateWindow("Edit", "Result:", WS_CHILD | ES_READONLY | WS_VISIBLE | WS_DISABLED, (Mrc.right - Mrc.left) / 6 - 50, int((Mrc.bottom - Mrc.top) / 2), 50, 22, hWnd, NULL, NULL, NULL);
		hEdit31 = CreateWindow("Edit", "Result:", WS_CHILD | ES_READONLY | WS_VISIBLE | WS_DISABLED, (Mrc.right - Mrc.left) / 6 - 50, int((Mrc.bottom - Mrc.top) / 1.5), 50, 22, hWnd, NULL, NULL, NULL);

		HWND hButton12 = CreateWindow("Button", "Paste", WS_BORDER | WS_CHILD | WS_VISIBLE, (Mrc.right - Mrc.left) / 6 + int((Mrc.right - Mrc.left)*0.6), (Mrc.bottom - Mrc.top) / 6, 42, 22, hWnd, (HMENU) BT_PASTE, NULL, NULL);
		HWND hButton22 = CreateWindow("Button", "Copy", WS_BORDER | WS_CHILD | WS_VISIBLE, (Mrc.right - Mrc.left) / 6 + int((Mrc.right - Mrc.left)*0.6), (Mrc.bottom - Mrc.top) / 2, 42, 22, hWnd, (HMENU) BT_COPY1, NULL, NULL);
		HWND hButton32 = CreateWindow("Button", "Copy", WS_BORDER | WS_CHILD | WS_VISIBLE, (Mrc.right - Mrc.left) / 6 + int((Mrc.right - Mrc.left)*0.6), int((Mrc.bottom - Mrc.top) / 1.5), 42, 22, hWnd, (HMENU) BT_COPY2, NULL, NULL);

		HWND hButtTrans = CreateWindow("Button", "Translate", WS_BORDER | WS_CHILD | WS_VISIBLE | BS_VCENTER | BS_CENTER, (Mrc.right - Mrc.left) / 2 - 60, ((Mrc.bottom - Mrc.top) / 6 + (Mrc.bottom - Mrc.top) / 2) / 2, 100, 50, hWnd, (HMENU) BT_TRANSL, NULL, NULL);
		HWND hButtManual = CreateWindow("Button", "Manual", WS_CHILD | WS_VISIBLE | BS_VCENTER | BS_CENTER, 0, 0, 60, 25, hWnd, (HMENU) BT_MANUAL, NULL, NULL);

		SendMessage(hEdit1, EM_SETLIMITTEXT, G_SIZER - 1, 0);
		SetFocus(hEdit1);
		break;
	}
	case WM_PAINT:
		char Limit[32];
		PAINTSTRUCT ps;
		HDC hDC = BeginPaint(hWnd, &ps);		

		wsprintf(Limit, "Characters: %d / %d\0", SendMessage(hEdit1, EM_LINELENGTH, 0, 0), G_SIZER - 1);
		TextOut(hDC, (Mrc.right - Mrc.left) / 6, (Mrc.bottom - Mrc.top) / 6 - 20, Limit, strlen(Limit));

		EndPaint(hWnd, &ps);
		break;

	case WM_COMMAND:
	{
		if (HIWORD(wParam) == EN_UPDATE) InvalidateRect(hWnd, NULL, TRUE);//actualize char limiter
		else if (HIWORD(wParam) == EN_SETFOCUS) {
			cout << "Active hEdit:";
			if (hEdit1 == HWND(lParam)) {
				puts("1");
				if (GetWindowTextLengthA(hEdit1) == 20) {
					char buf[22];
					GetWindowTextA(hEdit1, buf, 21);
					if (strcmp(buf, "Type expression here") == 0) SetWindowTextA(hEdit1, "");
				}
			}
			else if (hEdit2 == HWND(lParam)) puts("2");
			else if (hEdit3 == HWND(lParam)) puts("3");
		}
		else if (HIWORD(wParam) == EN_KILLFOCUS) {
			cout << "Not Active hEdit:";
			if (hEdit1 == HWND(lParam)) {
				puts("1");
				if (GetWindowTextLengthA(hEdit1) == 0) SetWindowTextA(hEdit1, "Type expression here");
			}
			else if (hEdit2 == HWND(lParam)) puts("2");
			else if (hEdit3 == HWND(lParam)) puts("3");
		}
		else if (LOWORD(wParam) == BT_PASTE) {//Paste button push
			puts("Paste");
			if (IsClipboardFormatAvailable(CF_TEXT) == 0) break;
			if (OpenClipboard(hWndMain) == 0) break;
			HGLOBAL hglb = GetClipboardData(CF_TEXT);
			SetWindowText(hEdit1, (char*) GlobalLock(hglb));
			GlobalUnlock(hglb);
			CloseClipboard();
			SetFocus(hEdit1);
			SendMessage(hEdit1, EM_SETSEL, 0, 128);
		}
		if (LOWORD(wParam) == BT_COPY1) {//Copy 1 button push
			puts("Copy 1");
			if (OpenClipboard(hWndMain) == 0) break;
			EmptyClipboard();
			HGLOBAL hglb = GlobalAlloc(GMEM_FIXED, GetWindowTextLength(hEdit2) + 1);
			GetWindowText(hEdit2, (char*) GlobalLock(hglb), GetWindowTextLength(hEdit2) + 1);//Putting retrieved text into Clipboard memory obtained by locking it.(with one line of code)
			GlobalUnlock(hglb);
			SetClipboardData(CF_TEXT, hglb);
			CloseClipboard();
		}
		if (LOWORD(wParam) == BT_COPY2) {//Copy 2 button push
			puts("Copy 2");
			if (OpenClipboard(hWndMain) == 0) break;
			EmptyClipboard();
			HGLOBAL hglb = GlobalAlloc(GMEM_FIXED, GetWindowTextLength(hEdit3) + 1);
			GetWindowText(hEdit3, (char*) GlobalLock(hglb), GetWindowTextLength(hEdit3) + 1);//Putting retrieved text into Clipboard memory obtained by locking it.(with one line of code)
			GlobalUnlock(hglb);
			SetClipboardData(CF_TEXT, hglb);
			CloseClipboard();
		}
		else if (LOWORD(wParam) == BT_TRANSL) {//Translete
			puts("Translate");
			SendMessage(hWndMain, VK_RETURN, 0, 0);
		}
		else if (LOWORD(wParam) == BT_MANUAL) {
			puts("Manual");
			if (IsWindowVisible(hDialog)) ShowWindow(hDialog, SW_HIDE);
			else {
				RECT rc;
				GetWindowRect(hWndMain, &rc);
				SetWindowPos(hDialog, NULL, rc.left - 199, rc.top, NULL, NULL, SWP_NOSIZE | SWP_SHOWWINDOW);
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
		SetFocus(hWnd);
		break;

	case VK_RETURN:
	{
		cout << "Enter\n";
		/*if (GetWindowTextLengthA(hEdit1) < 3) break;

		char buf[22];
		GetWindowTextA(hEdit1, buf, 21);

		if (strcmp(Buffer, buf) == 0) break;
		if (GetWindowTextLengthA(hEdit1) == 20) if (strcmp(buf, "Type expression here") == 0) break;

		GetWindowTextA(hEdit1, Buffer, G_SIZER);
		Terminal(Buffer);*/
		break;
	}
	case WM_KEYDOWN:

		if (GetAsyncKeyState(VK_CONTROL) & GetAsyncKeyState(VK_TAB)) {//Console showing 
			cout << "Ctrl+Tab\n";
			if (IsWindowVisible(GetConsoleWindow())) ShowWindow(GetConsoleWindow(), SW_HIDE);
			else ShowWindow(GetConsoleWindow(), SW_SHOW);
		}
		break;
	case WM_CTLCOLORSTATIC://Making background of Readonly and Noactive Edits transperent.
	{
		//SetTextColor((HDC)wParam, RGB(0, 0, 0));
		SetBkColor((HDC) wParam, RGB(255, 255, 255));
		//if (hbrBkgnd == NULL) hbrBkgnd = CreateSolidBrush(RGB(255, 255, 255));
		return (INT_PTR) hbrBkgnd;
		break;
	}
	case WM_DESTROY:
		DeleteObject(hbrBkgnd);
		DestroyWindow(hDialog);//Destroing Dialog info box.
		PostQuitMessage(0);
		break;
	default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}
LRESULT CALLBACK Manual::DialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)//Tab Contol's message manager
{
	switch (msg) {
	case WM_CREATE:
	{
		char Label1[] = "I/O Data";
		char Label2[] = "Keys";
		char Label3[] = "Close";
		char Text1[] = "-Letters(A-z) and \r\nDigits(1-9) are allowed\r\n-Must be a spacebar ' ' between two numbers\r\n-Operators: +, -, *, /, \r\n^(power), %(mod) and unar '!' are allowed\r\n"
			"-Wrong Input cause wrong Output or Nothing:)";
		char Text2[] = "-Enter is working\r\n-Ctrl+Tab to show Console\r\n";
		TCITEMA TabItem;
		RECT rc;

		GetClientRect(hWnd, &rc);
		hTab = CreateWindow(WC_TABCONTROL, "Tab Conrol Panel", WS_CHILD | WS_VISIBLE, 0, 0, rc.right + 1, rc.bottom + 1, hWnd, NULL, NULL, NULL);

		TabItem.mask = TCIF_TEXT;
		TabItem.pszText = Label1;
		SendMessageA(hTab, TCM_INSERTITEM, 0, (LPARAM) &TabItem);
		TabItem.pszText = Label2;
		SendMessageA(hTab, TCM_INSERTITEM, 1, (LPARAM) &TabItem);
		TabItem.pszText = Label3;
		SendMessageA(hTab, TCM_INSERTITEM, 2, (LPARAM) &TabItem);

		SendMessageA(hTab, TCM_GETITEMRECT, 0, LPARAM(&rc));
		int y = rc.bottom;
		GetClientRect(hWnd, &rc);
		rc.top += y;
		rc.bottom -= y;

		Tab[0] = CreateWindow("Edit", Text1, ES_LEFT | ES_MULTILINE | ES_READONLY | WS_BORDER | WS_CHILD | WS_VISIBLE, rc.left + 10, rc.top + 10, rc.right - 20, rc.bottom - 20, hTab, NULL, (HINSTANCE) GetModuleHandle(NULL), 0);
		Tab[1] = CreateWindow("Edit", Text2, ES_LEFT | ES_MULTILINE | ES_READONLY | WS_BORDER | WS_CHILD, rc.left + 10, rc.top + 10, rc.right - 20, rc.bottom - 20, hTab, NULL, (HINSTANCE) GetModuleHandle(NULL), 0);
		break;
	}
	case WM_NOTIFY:
	{
		LPNMHDR	tc = (LPNMHDR) lParam;
		if (tc != NULL && tc->code == TCN_SELCHANGING) {
			int a = SendMessageA(hTab, TCM_GETCURSEL, 0, 0);
			if (a < 2)ShowWindow(Tab[a], SW_HIDE);
			return FALSE;//allow tab changing
		}
		else if (tc->code == TCN_SELCHANGE) {
			int a = SendMessageA(hTab, TCM_GETCURSEL, 0, 0);
			if (a == 2) {
				SendMessageA(hTab, TCM_SETCURFOCUS, 0, NULL);
				ShowWindow(hDialog, SW_HIDE);
			}
			else ShowWindow(Tab[a], SW_SHOW);
		}
		break;
	}
	case WM_KEYDOWN:
		if (GetAsyncKeyState(VK_CONTROL) & GetAsyncKeyState(VK_TAB)) {
			if (IsWindowVisible(GetConsoleWindow())) ShowWindow(GetConsoleWindow(), SW_HIDE);
			else ShowWindow(GetConsoleWindow(), SW_SHOW);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}
#endif
#include "Precompiled.h"
#include "API.h"
#include "resource.h"

HBRUSH hbrBkgnd = NULL;

HWND hWndMain,
hEdit1, // Input/Output Edits
hEdit2,
hEdit3,
hEdit11,// Additional left/below no active edits with info.
hEdit21,
hEdit31;
HWND hCombo;

HWND hDialog, hTab;
array<HWND, 2> Tab;

array<char[24], 6 > examples = {
	"(a+b!!)*12!", "((a!+b)!*15)^2!",  //infix
	"!*!+!a!1 23", "!^!/*!+a!b1!23!4",//prefix
	"a!1 23-+", "ab+c*3 1 2-/^",     //postfix
};

function<void(string_view, unique_ptr<char[]> &, unique_ptr<char[]> &)> Terminal;

void PrintError(const char *Message) { MessageBox(0, Message, "Error", MB_OK | MB_ICONWARNING | MB_DEFBUTTON2); }
void ShowResult(const char *input, const unique_ptr<char[]> &res1, const unique_ptr<char[]> &res2) {

	if (OperChecker(input[0])) {//if Prefix

		SetWindowText(hEdit11, "Prefix expression accepted");
		SetWindowText(hEdit21, "    Infix:");
		SetWindowText(hEdit31, "Postfix:");
	}	
	else {
		size_t pos = strlen(input) - 1;
		if (input[pos] == ' ' || OperChecker(input[pos]) < 0) while (--pos != 0 && (input[pos] == ' ' || OperChecker(input[pos]) < 0));//if unar operators at the end

		if (OperChecker(input[pos]) > 0) {//if Postfix
			SetWindowText(hEdit11, "Postfix expression accepted");
			SetWindowText(hEdit21, " Prefix:");
			SetWindowText(hEdit31, "  Infix:");
		}
		else if (isalnum(input[pos]) || input[pos] == ')') {//if Infix
			SetWindowText(hEdit11, "Infix expression accepted");
			SetWindowText(hEdit21, "  Prefix:");
			SetWindowText(hEdit31, "Postfix:");
		}
		else cout << "Wrong expression detected.";//if wtf
	}
	SetWindowText(hEdit2, res1.get());
	SetWindowText(hEdit3, res2.get());
}
void Craete_Console() 
{
	FILE *stream;
	AllocConsole();
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	SetConsoleTitle("Console");
	SetWindowPos(GetConsoleWindow(), HWND_TOP, 0, 0, 300, 200, NULL);
	//freopen_s(&stream, "CONIN$",  "r", stdin);
	//freopen_s(&stream, "CONOUT$", "w", stderr);
	freopen_s(&stream, "CONOUT$", "w", stdout);
	cout << "Console creaed\n";
}
void Edit_Paste(HWND hEdit) {
		if (IsClipboardFormatAvailable(CF_TEXT) == 0) return;
		if (OpenClipboard(hWndMain) == 0) return;
		HGLOBAL hglb = GetClipboardData(CF_TEXT);
		SetWindowText(hEdit, (char*) GlobalLock(hglb));
		GlobalUnlock(hglb);
		CloseClipboard();
		SetFocus(hEdit);
		SendMessage(hEdit, EM_SETSEL, 0, -1);//0, -1 means all text sellection.
}
void Edit_Copy(HWND hEdit) {
		if (OpenClipboard(hWndMain) == 0) return;
		EmptyClipboard();
		HGLOBAL hglb = GlobalAlloc(GMEM_FIXED, GetWindowTextLength(hEdit) + 1);
		GetWindowText(hEdit, (char*) GlobalLock(hglb), GetWindowTextLength(hEdit) + 1);//Putting retrieved text into Clipboard memory obtained by locking it.(with one line of code)
		GlobalUnlock(hglb);
		SetClipboardData(CF_TEXT, hglb);
		CloseClipboard();
}

Main::Main(LPCSTR caption, int Pos_X, int Pos_Y, int Siz_X, int Siz_Y, HINSTANCE hInstance)
{
	WNDCLASS wc = {0};
	if (!handle)
	{
		memset(&wc, 0, sizeof(wc));
		wc.lpszClassName = "main_window";
		wc.lpfnWndProc = (WNDPROC) MainProc;
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.hInstance = hInstance;//from point of enterance
	   wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(ICON_EXE));
		if (wc.hIcon == NULL) wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursorA(NULL, IDC_CROSS);
		wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
		wc.lpszMenuName = NULL;

		if (RegisterClass(&wc)) {//(old)if (GetClassInfo((HINSTANCE) GetModuleHandle(NULL), "main_window", &wc) != 0)
			cout << "Main Class registerd\n";
			handle = CreateWindow("main_window", caption, WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX, Pos_X, Pos_Y, Siz_X, Siz_Y, NULL, NULL, /*(HINSTANCE) GetModuleHandle*/(NULL), NULL);
		}
		else {
			cout << "FAIL: to register Main Class\n";
			terminate();
		}
		if (handle) cout << "Main window created\n";
		else {
			cout << "FAIL: to create Main window\n";
			terminate();
		}
	}
	else {
		PrintError("Only one instance of the window is allowed!");
		terminate();
	}
}
Manual::Manual(LPCSTR caption, int Pos_X, int Pos_Y, int Siz_X, int Siz_Y)
{
	const char ClassName[] = "DialogClass";
	WNDCLASS wc = {0};
	if (!handle)
	{
		memset(&wc, 0, sizeof(wc));
		wc.lpfnWndProc = (WNDPROC) DialogProc;
		wc.hInstance = (HINSTANCE) GetModuleHandle(NULL);
		wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
		wc.hIcon = LoadIcon (GetModuleHandle (NULL), MAKEINTRESOURCE (ICON_TAB));
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

LRESULT CALLBACK Main::MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static RECT rect;

	switch (msg) {
	case WM_CREATE:
	{
		GetClientRect(hWnd, &rect);
		hbrBkgnd = CreateSolidBrush(RGB(255, 255, 255));

		hEdit1 = CreateWindow("Edit", "a+b", WS_BORDER | WS_CHILD | ES_AUTOHSCROLL | WS_VISIBLE, (rect.right - rect.left) / 6, (rect.bottom - rect.top) / 6, int((rect.right - rect.left)*0.6), 22, hWnd, NULL, NULL, NULL);
		hEdit2 = CreateWindow("Edit", "Here will be the answer", WS_BORDER | WS_CHILD | ES_READONLY | WS_VISIBLE, (rect.right - rect.left) / 6, (rect.bottom - rect.top) / 2, int((rect.right - rect.left)*0.6), 22, hWnd, NULL, NULL, NULL);
		hEdit3 = CreateWindow("Edit", "Here will be the answer", WS_BORDER | WS_CHILD | ES_READONLY | WS_VISIBLE, (rect.right - rect.left) / 6, int((rect.bottom - rect.top) / 1.5), int((rect.right - rect.left)*0.6), 22, hWnd, NULL, NULL, NULL);

		hEdit11 = CreateWindow("Edit", "Expression type detector", WS_CHILD | WS_VISIBLE | WS_DISABLED, (rect.right - rect.left) / 6 + 2, (rect.bottom - rect.top) / 6 + 22, 190, 22, hWnd, NULL, NULL, NULL);
		hEdit21 = CreateWindow("Edit", "Result:", WS_CHILD | ES_READONLY | WS_VISIBLE | WS_DISABLED, (rect.right - rect.left) / 6 - 50, int((rect.bottom - rect.top) / 2), 50, 22, hWnd, NULL, NULL, NULL);
		hEdit31 = CreateWindow("Edit", "Result:", WS_CHILD | ES_READONLY | WS_VISIBLE | WS_DISABLED, (rect.right - rect.left) / 6 - 50, int((rect.bottom - rect.top) / 1.5), 50, 22, hWnd, NULL, NULL, NULL);

		CreateWindow("Button", "Paste", WS_BORDER | WS_CHILD | WS_VISIBLE, (rect.right - rect.left) / 6 + int((rect.right - rect.left)*0.6), (rect.bottom - rect.top) / 6, 42, 22, hWnd, (HMENU) BT_PASTE, NULL, NULL);
		CreateWindow("Button", "Copy", WS_BORDER | WS_CHILD | WS_VISIBLE, (rect.right - rect.left) / 6 + int((rect.right - rect.left)*0.6), (rect.bottom - rect.top) / 2, 42, 22, hWnd, (HMENU) BT_COPY1, NULL, NULL);
		CreateWindow("Button", "Copy", WS_BORDER | WS_CHILD | WS_VISIBLE, (rect.right - rect.left) / 6 + int((rect.right - rect.left)*0.6), int((rect.bottom - rect.top) / 1.5), 42, 22, hWnd, (HMENU) BT_COPY2, NULL, NULL);

		CreateWindow("Button", "Translate", WS_BORDER | WS_CHILD | WS_VISIBLE | BS_VCENTER | BS_CENTER, (rect.right - rect.left) / 2 - 60, ((rect.bottom - rect.top) / 6 + (rect.bottom - rect.top) / 2) / 2, 100, 50, hWnd, (HMENU) BT_TRANSL, NULL, NULL);
		CreateWindow("Button", "Manual", WS_CHILD | WS_VISIBLE | BS_VCENTER | BS_CENTER, 0, 0, 60, 25, hWnd, (HMENU) BT_MANUAL, NULL, NULL);

		hCombo = CreateWindow(WC_COMBOBOX, TEXT("Examples"), CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 0, (rect.bottom - rect.top) / 6 - 1, 100, rect.bottom, hWnd, NULL, NULL, NULL);
		SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"Examples:");
		SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"Example 1");
		SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"Example 2");
		SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"Example 3");
		SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"Example 4");
		SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"Example 5");
		SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"Example 6");

		SendMessage(hCombo, CB_SETCURSEL, (WPARAM) 0, (LPARAM) 0);

		rect.top = (rect.bottom - rect.top) / 6 - 20;
		rect.left = (rect.right - rect.left) / 6;
		rect.bottom = rect.top + 20;
		rect.right = rect.left + 150;
		
		SendMessage(hEdit1, EM_SETLIMITTEXT, G_SIZER - 1, 0);
		SendMessage(hEdit1, EM_SETSEL, 0, -1);//0, -1 means all text sellection.
		SetFocus(hEdit1);

		ShowWindow(hWnd, SW_SHOWNORMAL);

		break;
	}
	case WM_PAINT:	
		char limit_4_display[32];
		PAINTSTRUCT ps;
		HDC hDC;

		hDC = BeginPaint(hWnd, &ps);
		wsprintf(limit_4_display, "Characters: %d / %d\0", SendMessage(hEdit1, EM_LINELENGTH, 0, 0), G_SIZER - 1);
		TextOut(hDC, rect.left, rect.top, limit_4_display, strlen(limit_4_display));

		EndPaint(hWnd, &ps);
		break;
	case WM_COMMAND:
	{
		if (HIWORD(wParam) == EN_UPDATE) InvalidateRect(hWnd, &rect, TRUE);//actualize char limiter

		else if (HIWORD(wParam) == EN_SETFOCUS) {
			if (hEdit1 == HWND(lParam)) if (GetWindowTextLength(hEdit1) == 20) {
				char buf[22];
				GetWindowText(hEdit1, buf, 21);
				if (strcmp(buf, "Type expression here") == 0) SetWindowText(hEdit1, "");
			}
		}
		else if (HIWORD(wParam) == EN_KILLFOCUS) {
			if (hEdit1 == HWND(lParam)) if (GetWindowTextLength(hEdit1) == 0) SetWindowText(hEdit1, "Type expression here");
		}
		else if (LOWORD(wParam) == BT_PASTE) {//Paste button push
			puts("Paste");
			Edit_Paste(hEdit1);
		}
		if (LOWORD(wParam) == BT_COPY1) {//Copy 1 button push
			puts("Copy 1");
			Edit_Copy(hEdit2);
		}
		if (LOWORD(wParam) == BT_COPY2) {//Copy 2 button push
			puts("Copy 2");
			Edit_Copy(hEdit3);
		}
		else if (LOWORD(wParam) == BT_TRANSL) {//Translete
			puts("BT_Translate");
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
		else if (HIWORD(wParam) == CBN_SELCHANGE) {//combo box maintanence
			cout << "CBN_SELCHANGE: ";
			size_t i = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
			if (i != 0 && i <= examples.size()) SetWindowText(hEdit1, examples[i - 1]);
			cout << i << endl;
		}
		break;
	}
	case WM_LBUTTONDOWN:
		SetFocus(hWnd);
		break;

	case VK_RETURN://Point of entrance to the Translation
	{
		static char Buffer[G_SIZER];
		static char buf[22];

		if (GetWindowTextLength(hEdit1) < 1) break;
		
		GetWindowText(hEdit1, buf, 22);

		if (strcmp(Buffer, buf) == 0) break;
		if (strlen(buf) == 20) if (strcmp(buf, "Type expression here") == 0) break;

		GetWindowText(hEdit1, Buffer, G_SIZER);

		unique_ptr<char[]> res1(nullptr);
		unique_ptr<char[]> res2(nullptr);

		Terminal(Buffer, res1, res2);

		if (res1 &&res2) ShowResult(Buffer, res1, res2);
		else SetWindowText(hEdit11, "Wrong expression detected");

		break;
	}
	case WM_KEYDOWN:
		if (GetAsyncKeyState(VK_CONTROL) & GetAsyncKeyState(VK_TAB)) {//Console showing 
			HWND hConsole = GetConsoleWindow();
			cout << "Ctrl+Tab\n";
			if (IsWindowVisible(hConsole)) ShowWindow(hConsole, SW_HIDE);
			else ShowWindow(hConsole, SW_SHOW);
		}
		break;
	case WM_CTLCOLORSTATIC://Making background of Readonly and Noactive Edits transperent.
	{
		//SetTextColor((HDC)wParam, RGB(0, 0, 0));
		//SetBkColor((HDC)wParam, RGB(255, 255, 255));
		//if (hbrBkgnd == NULL) hbrBkgnd = CreateSolidBrush(RGB(255, 255, 255));
		return (INT_PTR)hbrBkgnd;
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
		char Text1[] = "-Letters(A-z) and \r\nDigits(1-9) are allowed.\r\n-Must be a spacebar ' ' between two numbers.\r\n"
			"-Operators: +, -, *, /, \r\n^(power), %(mod) and unar '!' are allowed.\r\n"
			"-Same Input cause Nothing.\r\n-Wrong input cause appropriate message to be shown.";
		char Text2[] = "-Enter is working.\r\n-Ctrl+Tab to show Console.\r\n-Drop down Dialog Box contain predefined expressions.";
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
#pragma once
#include "Precompiled.h"

enum BT {
	BT_PASTE = 3000,
	BT_COPY1,
	BT_COPY2,
	BT_TRANSL,
	BT_MANUAL
};

extern HWND hWndMain;
extern HWND hDialog;

extern function<void(string_view, unique_ptr<char[]> &, unique_ptr<char[]> &)> Terminal;
extern int OperChecker(const char Operator);//For inside API.cpp usage

void PrintError(const char *message);
void Craete_Console();

class Main {
	HWND handle = nullptr;
	static LRESULT CALLBACK MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);//Main window procedure function declaration
public:
	explicit Main(LPCSTR caption, int Pos_X, int Pos_Y, int Siz_X, int Siz_Y, HINSTANCE hInstance);
	explicit operator HWND () const { return handle; }
	Main(const Main&) = delete;
	Main operator =(const Main&) = delete;
private:
	Main() {}
	//~Main() {}
};
//HWND Main::handle = nullptr;
class Manual {
	HWND handle = nullptr;
	static LRESULT CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);
public:
	explicit operator HWND () const { return handle; }
	explicit Manual(LPCSTR caption, int Pos_X, int Pos_Y, int Siz_X, int Siz_Y);

	Manual(const Manual&) = delete;
	Manual operator =(const Manual&) = delete;
private:
	Manual() {}
	//~Manual() {}
};

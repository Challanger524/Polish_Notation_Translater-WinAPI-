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

extern bool(*Terminal)(const char *, unique_ptr<char[]> &, unique_ptr<char[]> &);//For inside WinMain usage.
extern int OperChecker(const char Operator);//For inside API.cpp usage

void PrintError(const char *message);
void Craete_Console();
//void Edit_Paste(HWND hEdit, char *str);
//void Edit_Copy(HWND hEdit, char *str);


class Main {
	HWND handle = nullptr;
	static LRESULT CALLBACK MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);//Main window procedure function declaration
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
	explicit Manual(LPCSTR caption, int Pos_X, int Pos_Y, int Siz_X, int Siz_Y);

	Manual(const Manual&) = delete;
	Manual operator =(const Manual&) = delete;
private:
	Manual() {}
	//~Manual() {}
};

#pragma once
#include <Windows.h>
#include <CommCtrl.h>
#include <iostream>

#include <string_view>
#include <vector>
#include <array>
#include <stack>
#include <regex>

#include <thread>
#include <future>
#include <memory>
#include <cassert>

//for visual styles enabling
/*
#pragma comment(linker, \
  "\"/manifestdependency:type='Win32' "\
  "name='Microsoft.Windows.Common-Controls' "\
  "version='6.0.0.0' "\
  "processorArchitecture='*' "\
  "publicKeyToken='6595b64144ccf1df' "\
  "language='*'\"")

#pragma comment(lib, "ComCtl32.lib")
*/

using namespace std;

constexpr unsigned int G_SIZER = 128;

struct Timer {
	Timer() : start{std::chrono::steady_clock::now()} {}
	Timer(const Timer&) = delete;
	Timer operator = (const Timer&) = delete;
	~Timer() { cout << "\nTimer : " << static_cast<std::chrono::duration<float>>(std::chrono::steady_clock::now() - start).count() * 1000 << "ms\n"; }

	std::chrono::duration<float> get() { return std::chrono::steady_clock::now() - start; }//sec
	operator std::chrono::duration<float>() const { return std::chrono::steady_clock::now() - start; }
	void Lap() { cout << "\nLap : " << static_cast<std::chrono::duration<float>>(std::chrono::steady_clock::now() - start).count() * 1000 << "ms\n"; }
private:
	std::chrono::time_point<std::chrono::steady_clock> start;
};
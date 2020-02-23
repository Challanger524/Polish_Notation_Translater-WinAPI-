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
//#include <cassert>

/*//for visual styles enabling
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
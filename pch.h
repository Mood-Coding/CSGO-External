// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#define NOMINMAX

#pragma comment(lib, "d3d9.lib") 
#pragma comment(lib, "d3dx9.lib")
#pragma warning( push )
#pragma warning( disable : 26495)
#include <d3d9.h>
#include <d3dx9.h>
#pragma warning( pop ) 

//#pragma comment(lib, "dinput8.lib")
//#pragma comment(lib, "dxguid.lib")
//#include <dinput.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "Dwmapi.lib")
#pragma comment(lib, "ntdll.lib")

#include <Windows.h>
#include <Dwmapi.h>

#include <string>
#include <iostream>
#include <chrono>
#include <vector>
#include <thread>

#endif //PCH_H

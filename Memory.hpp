#pragma once

#include <cstdint>
#include <iostream>
#include <Windows.h>
#include <Psapi.h>
#include <TlHelp32.h>

class Memory
{
public:
	HANDLE hProcess;
	DWORD ProcessID;

	bool Init(DWORD ProcessID)
	{
		this->ProcessID = ProcessID;
		this->hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessID);

		if (this->hProcess)
		{
			std::cout << "<OpenProcess> Open a handle to " << this->ProcessID << " success!\n";
			return true;
		}
		else
		{
			std::cout << "<OpenProcess> Error " << GetLastError() << '\n';
			return false;
		}
	}

	~Memory()
	{
		if (CloseHandle(hProcess))
		{
			std::cout << "<CloseHandle> Success!\n";
		}
	}

	bool Read(uintptr_t ReadAddress, PVOID ReadBuffer, SIZE_T ReadSize)
	{
		if (!ReadProcessMemory(hProcess, (LPCVOID)ReadAddress, ReadBuffer, ReadSize, NULL))
		{
			std::cout << "<ReadProcessMemory> Error " << std::hex << GetLastError() << '\n';
			return false;
		}

		return true;
	}

	bool Write(uintptr_t WriteAddress, LPCVOID WriteBuffer, SIZE_T WriteSize)
	{
		if (WriteProcessMemory(hProcess, (LPVOID)WriteAddress, WriteBuffer, WriteSize, NULL))
		{
			std::cout << "<WriteProcessMemory> Error " << GetLastError() << '\n';
			return false;
		}

		return true;
	}

	uintptr_t GetProcessModuleBaseAddress(const wchar_t* ModuleName, DWORD ProcessID)
	{
		HANDLE hSnapshot;
		MODULEENTRY32 ME32;
		uintptr_t BaseAddress = 0;

		hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, ProcessID);
		if (hSnapshot == INVALID_HANDLE_VALUE)
		{
			std::cout << "<CreateToolhelp32Snapshot> Error " << GetLastError() << '\n';
			return 0;
		}

		ME32.dwSize = sizeof(MODULEENTRY32);

		if (!Module32First(hSnapshot, &ME32))
		{
			std::cout << "<Module32First> Error " << GetLastError() << '\n';
			CloseHandle(hSnapshot);
			return 0;
		}

		do
		{
			// A zero value indicates that both are considered equal
			if (wcscmp(ME32.szModule, ModuleName) == 0)
			{
				BaseAddress = (uintptr_t)ME32.modBaseAddr;
				break;
			}
		} while (Module32Next(hSnapshot, &ME32));

		CloseHandle(hSnapshot);
		return BaseAddress;
	}

	DWORD GetProcessID(const wchar_t* processName)
	{
		DWORD PID = 0;
		HANDLE hProcessSnapshot;
		PROCESSENTRY32 PE32;

		// Take a snapshot of all processes in the system.
		hProcessSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		if (hProcessSnapshot == INVALID_HANDLE_VALUE)
		{
			std::cout << "<CreateToolhelp32Snapshot> Invalid handle";
			return 0;
		}

		// Set the size of the structure before using it.
		PE32.dwSize = sizeof(PROCESSENTRY32);

		// Retrieves information about the first process and exit if unsuccessful
		if (!Process32First(hProcessSnapshot, &PE32))
		{
			std::cout << "<Process32First> Error " << GetLastError() << '\n';
			CloseHandle(hProcessSnapshot);
			return 0;
		}

		// Now walk the snapshot of processes,
		// and find the right process then get its PID
		do
		{
			// Returns 0 value indicates that both wchar_t* string are equal
			if (wcscmp(processName, PE32.szExeFile) == 0)
			{
				PID = PE32.th32ProcessID;
				break;
			}
		} while (Process32Next(hProcessSnapshot, &PE32));

		CloseHandle(hProcessSnapshot);
		return PID;
	}
};


#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <TlHelp32.h>

#include <cstdint>
#include <string_view>
#include <locale>
#include <codecvt>

class Memory
{
private:
	std::uintptr_t processId = 0;
	void* processHandle = nullptr;

public:
	int pid = 0;
	// Constructor that finds the process id
	// and opens a handle
	Memory(const std::string_view processName) noexcept
	{
		::PROCESSENTRY32 entry = { };
		entry.dwSize = sizeof(::PROCESSENTRY32);

		const auto snapShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		while (::Process32Next(snapShot, &entry))
		{
			std::wstring wstr(entry.szExeFile);
			std::string str(wstr.begin(), wstr.end());
			if (!std::string_view(processName).compare(str))
			{
				pid = processId = entry.th32ProcessID;
				processHandle = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
				break;
			}

		}

		// Free handle
		if (snapShot)
			::CloseHandle(snapShot);
	}

	// Destructor that frees the opened handle
	~Memory()
	{
		if (processHandle)
			::CloseHandle(processHandle);
	}

	// Returns the base address of a module by name
	const std::uintptr_t GetModuleAddress(const std::string_view moduleName) const noexcept
	{
		::MODULEENTRY32 entry = { };
		entry.dwSize = sizeof(::MODULEENTRY32);

		const auto snapShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processId);

		std::uintptr_t result = 0;

		while (::Module32Next(snapShot, &entry))
		{
			std::wstring wstr(entry.szModule);
			std::string str(wstr.begin(), wstr.end());

			if (std::string_view(str).compare(moduleName) == 0)
			{
				result = reinterpret_cast<std::uintptr_t>(entry.modBaseAddr);
				break;
			}





		}

		if (snapShot)
			::CloseHandle(snapShot);

		return result;
	}
	bool UpdatePid() {
		DWORD exitCode;
		if (!GetExitCodeProcess(processHandle, &exitCode)) {
			// Если процесс не существует, GetExitCodeProcess вернет ошибку.
			pid = 0;
			processId = 0;
			return false;
		}
		else if (exitCode != STILL_ACTIVE) {
			// Если процесс завершился, GetExitCodeProcess вернет его код выхода, который не равен STILL_ACTIVE.
			pid = 0;
			processId = 0;
			return false;
		}
		return true;
		// В противном случае процесс все еще активен, и pid остается без изменений.
	}

	// Read process memory
	template <typename T>
	constexpr const T Read(const std::uintptr_t& address) const noexcept
	{
		T value = { };
		::ReadProcessMemory(processHandle, reinterpret_cast<const void*>(address), &value, sizeof(T), NULL);
		return value;
	}

	// Write process memory
	template <typename T>
	constexpr void Write(const std::uintptr_t& address, const T& value) const noexcept
	{
		::WriteProcessMemory(processHandle, reinterpret_cast<void*>(address), &value, sizeof(T), NULL);
	}
};

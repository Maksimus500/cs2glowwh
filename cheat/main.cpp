#include "memory.h"
#include <thread>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <clocale>
#include <array>

constexpr std::ptrdiff_t m_hMyWeapons = 0x40;
constexpr std::ptrdiff_t m_nFallbackPaintKit = 0x1548;
constexpr std::ptrdiff_t m_nFallbackSeed = 0x154C; // int32
constexpr std::ptrdiff_t m_flFallbackWear = 0x1550; // float32
constexpr std::ptrdiff_t m_nFallbackStatTrak = 0x1554; // int32
constexpr std::ptrdiff_t m_iItemDefinitionIndex = 0x1BA; // uint16
constexpr std::ptrdiff_t m_iItemIDHigh = 0x1D0; // uint32
constexpr std::ptrdiff_t m_iEntityQuality = 0x1BC; // int32
constexpr std::ptrdiff_t m_iAccountID = 0x1D8; // uint32
constexpr std::ptrdiff_t m_OriginalOwnerXuidLow = 0x1600; // uint32
constexpr std::ptrdiff_t dwEntityList = 0x18C1EA8;
constexpr std::ptrdiff_t m_iPlayerState = 0x1304; // CSPlayerState
constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x173A3C8;



void openProcess(const char* path) {
	char command[1024];
	strcpy_s(command, sizeof(command), "start \"\" \"");
	strcat_s(command, sizeof(command), path);
	strcat_s(command, sizeof(command), "\"");
	system(command);
}

int main(int argc, char* argv[])
{

	setlocale(LC_ALL, "Russian");
	const auto em = Memory("cs2.exe");
	const int Standing = 65665;
	const int pjump = 65537;
	const int mjump = 256;
	int pid = em.pid;
	setlocale(LC_ALL, "Russian");
	if (pid == 0) {
		const char* path = "steam://rungameid/730";
		openProcess(path);
		
		while (pid == 0) {
			Sleep(200);
			Memory em = Memory("cs2.exe");
			pid = em.pid;
		}
		std::string command = "start ";
		command += argv[0];
		system(command.c_str());
		ShowWindow(GetConsoleWindow(), HIDE_WINDOW);
		Sleep(5000);
		quick_exit(0);

	}

	std::cout << "initializing";
	Sleep(10000);
	auto mem = Memory("cs2.exe");
	const auto client = mem.GetModuleAddress("client.dll");
	uint16_t old = mem.Read<uint16_t>(client + 0x8263F2);
	


	bool flag = false;

	MessageBoxA(NULL, "F8 to toggle wallhack, insert to unhook", "important information", MB_OK);
	while (true) {
		if (mem.UpdatePid() == 0) {
			quick_exit(0);
		}
		if (GetAsyncKeyState(VK_F8) and flag == true) {
			while (GetAsyncKeyState(VK_F8)) {
				mem.Write<uint16_t>(client + 0x8263F2, 37008);
				flag = false;
			}
		}
		else if (GetAsyncKeyState(VK_F8)) {
			while (GetAsyncKeyState(VK_F8)) {
				mem.Write<uint16_t>(client + 0x8263F2, old);
				flag = true;
			}
		}
		if (GetAsyncKeyState(VK_INSERT)) {
			quick_exit(0);
		}
	}
	return 0;
}

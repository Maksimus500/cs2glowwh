#include "memory.h"
#include <thread>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <clocale>

int m_iFOV = 0x210;
int dwLocalPlayerPawn = 0x17361E8;
int m_pCameraServices = 0x1138;
int m_bIsScoped = 0x1400;

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

	Sleep(10000);
	std::cout << "initializing";
	auto mem = Memory("cs2.exe");
	const auto client = mem.GetModuleAddress("client.dll");
	uint16_t old = mem.Read<uint16_t>(client + 0x821D88);
	bool flag = false;
	ShowWindow(GetConsoleWindow(), HIDE_WINDOW);
	MessageBoxA(NULL, "F8 to toggle wallhack, insert to unhook", "important information", MB_OK);
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		if (mem.UpdatePid() == 0) {
			quick_exit(0);
		}
		if (GetAsyncKeyState(VK_F8) and flag == true) {
			while (GetAsyncKeyState(VK_F8)) {
				mem.Write<uint16_t>(client + 0x821D88, 37008);
				flag = false;
			}
		}
		else if (GetAsyncKeyState(VK_F8)) {
			while (GetAsyncKeyState(VK_F8)) {
				mem.Write<uint16_t>(client + 0x821D88, old);
				flag = true;
			}
		}
		if (GetAsyncKeyState(VK_INSERT)) {
			quick_exit(0);
		}
	}
	return 0;
}

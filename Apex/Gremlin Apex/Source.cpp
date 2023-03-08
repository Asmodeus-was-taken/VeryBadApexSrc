#include <iostream>
#include "driver.hpp"
#include <windows.h>
#include <algorithm>
#include "overlay.hpp"
#include "auth/antidump.h"
#include "auth/anti_debugger.h"
#include "auth/auth.hpp"
#include "auth/bytes.h"
#include "mapper/kdmapper.hpp"
#include "glob.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;
int language = 3;


void antiDebug() {
	searchDbg();
	anti_dump();
	if (remotepresent())
		BSOD;
}

bool status = false;
int s2;

void login() {
	KeyAuth::api KeyAuthApp(_("Apex").c_str(),
		_("6Rolek0rSb").c_str(),
		_("b3d66592a9f5c7aaac86a80afdd0c2e327e2791619c7eb715456a41aa67dda07").c_str(),
		_("1.0").c_str(),
		_("https://keyauth.win/api/1.2/").c_str());

	KeyAuthApp.init();
	std::string key;

	std::ifstream MyReadFile("C:\\key.txt");
	if (MyReadFile) {
		getline(MyReadFile, key);
		MyReadFile.close();
	}
	else {
		MyReadFile.close();
		std::cout << _("Enter Key: ").c_str();
		std::cin >> key;
		std::ofstream file("C:\\key.txt");
		file << key;
		file.close();
	}

	KeyAuthApp.license(key);
	if (KeyAuthApp.data.success) {
		KeyAuthApp.log("Apex Gremlins");
		std::cout << _("Login Success\n").c_str();
		Sleep(1000);
		s2 = 3;
		status = true;
	}

	else {
		std::cout << _("Failed\n").c_str();
		remove("C:\\key.txt");
		Sleep(5000);
		exit(1);
	}

}
int logIn = 1;
int reG = 2;

void dmpSearch() {
	std::string path = _("C:\\").c_str();
	for (const auto& entry : fs::directory_iterator(path)) {
		if (entry.path().string().substr(entry.path().string().find_last_of((_(".").c_str())) + 1) == _("DMP").c_str()) {
			remove(entry.path().string().c_str());
			BSOD();
		}
	}
}

int main() {

	SetConsoleTitleA("Gremlin Apex External");

	hidethread();
	std::thread(antiDebug).detach();


	login();


	if (!status || s2 != 3) {
		BSOD();
	}


	if (!mem::find_driver()) {
		mem::find_process(_("r5apex.exe").c_str());
		if (mem::process_id) {
			MessageBoxA(NULL, _("Please have the game closed before you launch").c_str(), _("Gremlin Apex").c_str(), 0);
			exit(2);
		}
	}
	ShowWindow(GetConsoleWindow(), SW_HIDE);


	if (!mem::find_driver()) {
		HANDLE iqvw64e_device_handle = intel_driver::Load();
		if (!iqvw64e_device_handle || iqvw64e_device_handle == INVALID_HANDLE_VALUE) {
			MessageBoxA(NULL, _("Please Run As Admin or Downgrade ur Windows").c_str(), _("Gremlin Apex").c_str(), 0);
			exit(3);
		}
		kdmapper::MapDriver(iqvw64e_device_handle, goat);
		intel_driver::Unload(iqvw64e_device_handle);
	}
	dmpSearch();
	Sleep(2000);
	dmpSearch();
	system("cls");
	Sleep(1000);
	ShowWindow(GetConsoleWindow(), SW_SHOW);

	if (!mem::find_driver()) std::cout << _("[-] Driver Is Not Mapped or Is Not Mapped Properly\n").c_str();
	else std::cout << _("[+] Driver Mapped | Loaded\n").c_str();
	std::cout << _("Please Launch Apex\n").c_str();
	while (mem::process_id == NULL) {
		mem::find_process(_("r5apex.exe").c_str());
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	printf(_("Found Game | PID %u\nPlease press insert once you are in the game's menu\nPlease make sure the game is not fullscreened").c_str());
	while (!GetAsyncKeyState(VK_INSERT)) {

	}


	ShowWindow(GetConsoleWindow(), SW_HIDE);
	system("cls");
	std::cout << "Please close once your finished!\n";
	std::thread(aimbot).detach();
	std::thread(m_rand::genRand).detach();
	//std::thread(rainbow::run).detach();
	std::thread(visable::updateTime).detach();
	std::thread(printPlayerInfo).detach();
	std::thread(itemGlow).detach();
	std::thread(NoSpread).detach();
	draw::Initialize();
}
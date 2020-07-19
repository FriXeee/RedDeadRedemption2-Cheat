#include "stdafx.h"

bool Cheat::Controls::isMenuEnabled = false;
bool Cheat::Controls::optionpress = false;
bool Cheat::Controls::leftpress = false;
bool Cheat::Controls::rightpress = false;
bool Cheat::Controls::uppress = false;
bool Cheat::Controls::downpress = false;
int Cheat::Controls::optioncount;
int Cheat::Controls::currentoption = 1;
int menulevel = 0;
int Delay = GetTickCount();

char* currentmenu[100];
char* actualmenu = (char*)"mainmenu";
int lastoption[100];

void Cheat::Controls::changeMenu(const char* menuname) {
	currentmenu[menulevel] = actualmenu;
	lastoption[menulevel] = currentoption;
	menulevel++;
	actualmenu = (char*)menuname;
	currentoption = 1;
}

bool Cheat::Controls::currentMenu(const char* menuname) {
	if (menuname == actualmenu) return true;
	else return false;
}

void backMenu() {
	if (Cheat::Controls::currentMenu("mainmenu")) { Cheat::Controls::isMenuEnabled = false; }
	menulevel--;
	actualmenu = currentmenu[menulevel];
	Cheat::Controls::currentoption = lastoption[menulevel];
}

void Cheat::Controls::CheckKeys() {
	optionpress = false;

	if (GetTickCount64() - Delay > 150) {
		if (GetAsyncKeyState(VK_MULTIPLY) & 1 && Cheat::CheatFunctions::IsGameWindowFocussed() || GetAsyncKeyState(VK_F4) & 1 && Cheat::CheatFunctions::IsGameWindowFocussed() || PAD::IS_CONTROL_PRESSED(0, INPUT_FRONTEND_RB)) {
			AUDIO::PLAY_SOUND_FRONTEND("SELECT", "HUD_SHOP_SOUNDSET", true, 0);
			if (menulevel == 0) changeMenu("mainmenu");
			else if (menulevel == 1) backMenu();
			Delay = GetTickCount64();
		}
		if (GetAsyncKeyState(VK_NUMPAD0) & 1 && Cheat::CheatFunctions::IsGameWindowFocussed() || PAD::IS_CONTROL_PRESSED(0, INPUT_FRONTEND_CANCEL)) {
			if (menulevel > 0) { backMenu(); AUDIO::PLAY_SOUND_FRONTEND("BACK", "HUD_SHOP_SOUNDSET", true, 0); }
			Delay = GetTickCount64();
		}
		if (GetAsyncKeyState(VK_NUMPAD5) & 1 && Cheat::CheatFunctions::IsGameWindowFocussed() || GetAsyncKeyState(VK_RETURN) && Cheat::CheatFunctions::IsGameWindowFocussed() || PAD::IS_CONTROL_PRESSED(0, INPUT_SCRIPT_SELECT)) {
			optionpress = true;
			AUDIO::PLAY_SOUND_FRONTEND("SELECT", "HUD_SHOP_SOUNDSET", true, 0);
			Delay = GetTickCount64();
		}
		if (GetAsyncKeyState(VK_NUMPAD2) & 1 && Cheat::CheatFunctions::IsGameWindowFocussed() || PAD::IS_CONTROL_PRESSED(0, INPUT_FRONTEND_DOWN)) {
			if (currentoption < optioncount) { currentoption++; } else { currentoption = 1; }
			AUDIO::PLAY_SOUND_FRONTEND("BACK", "HUD_SHOP_SOUNDSET", true, 0);
			Delay = GetTickCount64();
			downpress = true;
		}
		if (GetAsyncKeyState(VK_NUMPAD8) & 1 && Cheat::CheatFunctions::IsGameWindowFocussed() || PAD::IS_CONTROL_PRESSED(0, INPUT_FRONTEND_UP)) {
			if (currentoption > 1) { currentoption--; } else { currentoption = optioncount; }
			AUDIO::PLAY_SOUND_FRONTEND("BACK", "HUD_SHOP_SOUNDSET", true, 0);
			Delay = GetTickCount64();
			uppress = true;
		}
		if (GetAsyncKeyState(VK_NUMPAD4) & 1 && Cheat::CheatFunctions::IsGameWindowFocussed() || PAD::IS_CONTROL_PRESSED(0, INPUT_FRONTEND_LEFT)) {
			leftpress = true;
			AUDIO::PLAY_SOUND_FRONTEND("BACK", "HUD_SHOP_SOUNDSET", true, 0);
			Delay = GetTickCount64();
		}
		if (GetAsyncKeyState(VK_NUMPAD6) & 1 && Cheat::CheatFunctions::IsGameWindowFocussed() || PAD::IS_CONTROL_PRESSED(0, INPUT_FRONTEND_RIGHT)) {
			rightpress = true;
			AUDIO::PLAY_SOUND_FRONTEND("BACK", "HUD_SHOP_SOUNDSET", true, 0);
			Delay = GetTickCount64();
		}
	}
}
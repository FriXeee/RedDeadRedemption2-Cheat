#include "stdafx.h"

bool Cheat::Controls::optionpress = false;
bool Cheat::Controls::leftpress = false;
bool Cheat::Controls::rightpress = false;
bool Cheat::Controls::uppress = false;
bool Cheat::Controls::downpress = false;

int Cheat::Controls::previousOption = 0;
int Cheat::Controls::optionCount;
int Cheat::Controls::currentOption = 1;
int Cheat::Controls::menuLevel = 0;
SubMenus Cheat::Controls::PreviousMenu = NOMENU;
int Cheat::Controls::PreviousMenuLevel;
int Cheat::Controls::optionsArray[1000];
SubMenus Cheat::Controls::menusArray[1000];
SubMenus Cheat::Controls::currentMenu;
bool Cheat::Controls::RestorePreviousSubmenu = true;
int Delay = GetTickCount();

void Cheat::Controls::MoveMenu(SubMenus menu) 
{
	Controls::menusArray[Controls::menuLevel] = Controls::currentMenu;
	Controls::optionsArray[Controls::menuLevel] = Controls::currentOption;
	Controls::menuLevel++;
	Controls::currentMenu = menu;
	Controls::currentOption = 1;
}

void Cheat::Controls::BackMenu()
{
	Controls::PreviousMenu = Controls::currentMenu;
	Controls::PreviousMenuLevel = Controls::menuLevel;
	Controls::previousOption = Controls::currentOption;
	Controls::menuLevel--;
	Controls::currentMenu = Controls::menusArray[Controls::menuLevel];
	Controls::currentOption = Controls::optionsArray[Controls::menuLevel];
}

void Cheat::Controls::CloseMenu()
{
	Controls::PreviousMenu = Cheat::Controls::currentMenu;
	Controls::PreviousMenuLevel = Controls::menuLevel;
	Controls::previousOption = Controls::currentOption;
	Controls::menuLevel = 0;
	Controls::currentMenu = Controls::menusArray[Controls::menuLevel];
	Controls::currentOption = Controls::optionsArray[Controls::menuLevel];
}

void Cheat::Controls::CheckKeys() {
	optionpress = false;

	if (GetTickCount64() - Delay > 150) {
		if (GetAsyncKeyState(VK_MULTIPLY) & 1 && Cheat::CheatFunctions::IsGameWindowFocussed() || GetAsyncKeyState(VK_F4) & 1 && Cheat::CheatFunctions::IsGameWindowFocussed() || PAD::IS_CONTROL_PRESSED(0, INPUT_FRONTEND_RB)) {
			AUDIO::PLAY_SOUND_FRONTEND("SELECT", "HUD_SHOP_SOUNDSET", true, 0);	
			if (Controls::menuLevel == 0)
			{
				if (Controls::PreviousMenu != NOMENU && Cheat::Controls::RestorePreviousSubmenu) { Controls::MoveMenu(Controls::PreviousMenu); Controls::menuLevel = Controls::PreviousMenuLevel; Controls::currentOption = Controls::previousOption; }
				else { Controls::MoveMenu(SubMenus::MainMenu); }
			}
			else
			{
				Controls::CloseMenu();
			}
			Delay = GetTickCount64();
		}
		if (GetAsyncKeyState(VK_NUMPAD0) & 1 && Cheat::CheatFunctions::IsGameWindowFocussed() || PAD::IS_CONTROL_PRESSED(0, INPUT_FRONTEND_CANCEL)) {
			if (Controls::menuLevel > 0) { Controls::BackMenu(); AUDIO::PLAY_SOUND_FRONTEND("BACK", "HUD_SHOP_SOUNDSET", true, 0); }
			Delay = GetTickCount64();
		}
		if (GetAsyncKeyState(VK_NUMPAD5) & 1 && Cheat::CheatFunctions::IsGameWindowFocussed() || GetAsyncKeyState(VK_RETURN) && Cheat::CheatFunctions::IsGameWindowFocussed() || PAD::IS_CONTROL_PRESSED(0, INPUT_SCRIPT_SELECT)) {
			optionpress = true;
			AUDIO::PLAY_SOUND_FRONTEND("SELECT", "HUD_SHOP_SOUNDSET", true, 0);
			Delay = GetTickCount64();
		}
		if (GetAsyncKeyState(VK_NUMPAD2) & 1 && Cheat::CheatFunctions::IsGameWindowFocussed() || PAD::IS_CONTROL_PRESSED(0, INPUT_FRONTEND_DOWN)) {
			Controls::currentOption < Controls::optionCount ? Controls::currentOption++ : Controls::currentOption = 1;
			AUDIO::PLAY_SOUND_FRONTEND("BACK", "HUD_SHOP_SOUNDSET", true, 0);
			Delay = GetTickCount64();
			downpress = true;
		}
		if (GetAsyncKeyState(VK_NUMPAD8) & 1 && Cheat::CheatFunctions::IsGameWindowFocussed() || PAD::IS_CONTROL_PRESSED(0, INPUT_FRONTEND_UP)) {
			Controls::currentOption > 1 ? Controls::currentOption-- : Controls::currentOption = Controls::optionCount;
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
	Controls::optionCount = 0;
}
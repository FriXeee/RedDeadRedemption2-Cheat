#pragma once
#include "stdafx.h"

float Cheat::GUI::guiX				= 330.f;
rgba titleRGB						= { 255, 255, 255, 255 };
rgba optionRectRGB					= { 255, 0, 0, 200 };
rgba optionRGB						= { 255, 255, 255, 255 };
rgba scrollerRGB					= { 0, 0, 255, 255 };
std::string InformationTextVar;

void drawRect(float x, float y, float w, float h, rgba rgba) 
{
	int ResolutionX;
	int ResolutionY;
	GRAPHICS::GET_SCREEN_RESOLUTION(&ResolutionX, &ResolutionY);
	x /= ResolutionX; w /= ResolutionX;
	y /= ResolutionY; h /= ResolutionY;
	GRAPHICS::DRAW_RECT(x, y, w, h, rgba.r, rgba.g, rgba.b, rgba.a, 0, 0);
}
void Cheat::GUI::drawText(std::string text, float font_size, float x, float y, rgba rgba, bool centered = false) 
{
	int ResolutionX;
	int ResolutionY;
	GRAPHICS::GET_SCREEN_RESOLUTION(&ResolutionX, &ResolutionY);
	x /= ResolutionX; y /= ResolutionY;
	font_size /= 100.f;
	HUD::SET_TEXT_SCALE(font_size, font_size);
	HUD::_SET_TEXT_COLOR(rgba.r, rgba.g, rgba.b, rgba.a);
	HUD::SET_TEXT_CENTRE(centered);
	auto str = MISC::_CREATE_VAR_STRING(10, "LITERAL_STRING", text.c_str());
	HUD::_DISPLAY_TEXT(str, x, y);
}
void Cheat::GUI::Title(std::string title)
{
	drawRect(guiX, 42.f, 350.f, 40.f, { 0, 0, 255, 255 });
	drawText(title, 73.f, guiX, 22.f, titleRGB, true);

}
bool Cheat::GUI::Option(std::string option, std::string InformationText)
{
	Cheat::GUI::optionCount++;

	bool thisOption = false;
	if (Cheat::GUI::currentOption == Cheat::GUI::optionCount) thisOption = true;

	if (Cheat::GUI::currentOption <= GUI::maxVisOptions && Cheat::GUI::optionCount <= GUI::maxVisOptions) {
		drawText(option, 45.f, guiX - 170.f, ((Cheat::GUI::optionCount * 30.f) + 36.5f), optionRGB);
		if (thisOption) { drawRect(guiX, ((Cheat::GUI::optionCount * 30.22f) + 46.6f), 350.f, 30.22f, scrollerRGB); }
		else { drawRect(guiX, ((Cheat::GUI::optionCount * 30.22f) + 46.6f), 350.f, 30.22f, { 0, 0, 0, 150 }); }
	}

	else if ((Cheat::GUI::optionCount > (Cheat::GUI::currentOption - GUI::maxVisOptions)) && Cheat::GUI::optionCount <= Cheat::GUI::currentOption)
	{
		drawText(option, 45.f, guiX - 170.f, (((Cheat::GUI::optionCount - (Cheat::GUI::currentOption - GUI::maxVisOptions)) * 30.f) + 36.5f), optionRGB);
		if (thisOption) { drawRect(guiX, (((Cheat::GUI::optionCount - (Cheat::GUI::currentOption - GUI::maxVisOptions)) * 30.22f) + 46.6f), 350.f, 30.22f, scrollerRGB); }
		else { drawRect(guiX, (((Cheat::GUI::optionCount - (Cheat::GUI::currentOption - GUI::maxVisOptions)) * 30.22f) + 46.6f), 350.f, 30.22f, { 0, 0, 0, 150 }); }
	}

	if (thisOption) { InformationTextVar = InformationText; }

	if (Cheat::GUI::OptionSelect && Cheat::GUI::currentOption == Cheat::GUI::optionCount)
	{
		if (Cheat::GUI::OptionSelect)
		{
			return true;
		}
	}
	else return false;
}
bool Cheat::GUI::MenuOption(std::string option, SubMenus menu)
{
	Option(option, "");

	if (Cheat::GUI::OptionSelect && Cheat::GUI::currentOption == Cheat::GUI::optionCount)
	{
		Cheat::GUI::OptionSelect = false;
		Cheat::GUI::MoveMenu(menu);
		return true;
	}
	else return false;
}
bool Cheat::GUI::BoolOption(std::string option, bool* isEnabled, std::string InformationText)
{
	Option(option, InformationText);

	if (Cheat::GUI::currentOption <= GUI::maxVisOptions && Cheat::GUI::optionCount <= GUI::maxVisOptions) {
		drawText(*isEnabled ? "~COLOR_GREEN~On" : "~COLOR_RED~Off", 45.f, guiX + 135.f, ((Cheat::GUI::optionCount * 30.f) + 36.5f), optionRGB);
	}
	else if ((Cheat::GUI::optionCount > (Cheat::GUI::currentOption - 16)) && Cheat::GUI::optionCount <= Cheat::GUI::currentOption)
	{
		drawText(*isEnabled ? "~COLOR_GREEN~On" : "~COLOR_RED~Off", 45.f, guiX + 135.f, ((Cheat::GUI::optionCount * 30.f) + 36.5f), optionRGB);
	}

	if (Cheat::GUI::OptionSelect && Cheat::GUI::currentOption == Cheat::GUI::optionCount) {
		*isEnabled ^= 1;
		return true;
	}
	return false;
}
bool Cheat::GUI::IntOption(std::string option, int* var, int min, int max, int step, std::string InformationText)
{
	Option(option, InformationText);

	if (Cheat::GUI::currentOption <= GUI::maxVisOptions && Cheat::GUI::optionCount <= GUI::maxVisOptions) {
		drawText("<" + std::to_string(*var) + ">", 45.f, guiX + 130.f, (Cheat::GUI::optionCount * 30.f) + 36.5f, optionRGB);
	}
	else if ((Cheat::GUI::optionCount > (Cheat::GUI::currentOption - 16)) && Cheat::GUI::optionCount <= Cheat::GUI::currentOption)
	{
		drawText("<" + std::to_string(*var) + ">", 45.f, guiX + 130.f, (((Cheat::GUI::optionCount - (Cheat::GUI::currentOption - GUI::maxVisOptions)) * 30.f) + 36.5f), optionRGB);
	}

	if (Cheat::GUI::currentOption == Cheat::GUI::optionCount) {
		if (Cheat::GUI::leftpress) {
			if (*var <= min) *var = max;
			else *var -= step;
			Cheat::GUI::leftpress = false;
			return true;
		}
		if (*var < min) *var = max;
		if (Cheat::GUI::rightpress) {
			if (*var >= max) *var = min;
			else *var += step;
			Cheat::GUI::rightpress = false;
			return true;
		}
		if (*var > max) *var = min;
	}

	if (Cheat::GUI::OptionSelect && Cheat::GUI::currentOption == Cheat::GUI::optionCount)
	{
		return true;
	}
	else return false;
}
bool Cheat::GUI::FloatOption(std::string option, float* var, float min, float max, float step, std::string InformationText)
{
	Option(option, InformationText);

	char buf[100];
	_snprintf_s(buf, sizeof(buf), "%.2f"), *var;

	if (Cheat::GUI::currentOption <= GUI::maxVisOptions && Cheat::GUI::optionCount <= GUI::maxVisOptions) {
		drawText("<" + (std::string)buf + ">", 45.f, guiX + 85.f, (Cheat::GUI::optionCount * 30.f) + 37.f, optionRGB);
	}
	else if ((Cheat::GUI::optionCount > (Cheat::GUI::currentOption - 16)) && Cheat::GUI::optionCount <= Cheat::GUI::currentOption)
	{
		drawText("<" + (std::string)buf + ">", 45.f, guiX + 85.f, (((Cheat::GUI::optionCount - (Cheat::GUI::currentOption - GUI::maxVisOptions)) * 30.f) + 37.f), optionRGB);
	}


	if (Cheat::GUI::currentOption == Cheat::GUI::currentOption) {
		if (Cheat::GUI::leftpress) {
			if (*var <= min) *var = max;
			else *var -= step;
			Cheat::GUI::leftpress = false;
			return true;
		}
		if (*var < min) *var = max;

		if (Cheat::GUI::rightpress) {
			if (*var >= max) *var = min;
			else *var += step;
			Cheat::GUI::rightpress = false;
			return true;
		}
		if (*var > max) *var = min;
	}

	if (Cheat::GUI::OptionSelect && Cheat::GUI::currentOption == Cheat::GUI::optionCount)
		return true;
	else return false;
}
void Cheat::GUI::End()
{
	if (Cheat::GUI::currentMenu != 0)
	{
		if (Cheat::GUI::optionCount <= GUI::maxVisOptions)
		{
			drawRect(guiX, 30.22f * Cheat::GUI::optionCount + 62.f, 350.f, 1.f, { 0, 0, 255, 255 });  //Menu bottom line
			drawRect(guiX, 30.22f * Cheat::GUI::optionCount + 77.5f, 350.f, 30.f, { 0, 0, 0, 255 });  //Menu bottom draw
			drawText(Cheat::CheatFunctions::ReturnCheatBuildAsString(), 40.f, guiX + 115.f, 30.22f * Cheat::GUI::optionCount + 67.f, { 255, 255, 255, 255 }, false); //Cheat build number draw
			drawText(std::to_string(Cheat::GUI::currentOption) + " / " + std::to_string(Cheat::GUI::optionCount), 40.f, guiX - 170.f, 30.22f * Cheat::GUI::optionCount + 67.f, { 255, 255, 255, 255 }, false); //Option count draw at menu bottom
			if (InformationTextVar != "")
			{
				drawRect(guiX, 30.22f * Cheat::GUI::optionCount + 103.f, 350.f, 25.f, { 0, 0, 0, 255 });
				drawText(InformationTextVar, 40.f, guiX - 170.f, 30.22f * Cheat::GUI::optionCount + 90.f, { 255, 255, 255, 255 }, false);
			}
		}
		else
		{
			drawRect(guiX, 30.22f * GUI::maxVisOptions + 62.f, 350.f, 1.f, { 0, 0, 255, 255 });   //Menu bottom line
			drawRect(guiX, 30.22f * GUI::maxVisOptions + 77.5f, 350.f, 30.f, { 0, 0, 0, 255 });   //Menu bottom draw
			drawText(Cheat::CheatFunctions::ReturnCheatBuildAsString(), 40.f, guiX + 115.f, 30.22f * GUI::maxVisOptions + 67.f, { 255, 255, 255, 255 }, false); //Cheat build number draw
			drawText(std::to_string(Cheat::GUI::currentOption) + " / " + std::to_string(Cheat::GUI::optionCount), 40.f, guiX - 170.f, 30.22f * GUI::maxVisOptions + 67.f, { 255, 255, 255, 255 }, false); //Option count draw at menu bottom
			if (InformationTextVar != "")
			{
				drawRect(guiX, 30.22f * GUI::maxVisOptions + 103.f, 350.f, 25.f, { 0, 0, 0, 255 });
				drawText(InformationTextVar, 40.f, guiX - 170.f, 30.22f * GUI::maxVisOptions + 90.f, { 255, 255, 255, 255 }, false);
			}
		}
	}
}

bool Cheat::GUI::OptionSelect		= false;
bool Cheat::GUI::leftpress			= false;
bool Cheat::GUI::rightpress			= false;
bool Cheat::GUI::uppress			= false;
bool Cheat::GUI::downpress			= false;
int Cheat::GUI::openKey				= VK_F5;
int Cheat::GUI::previousOption		= 0;
int Cheat::GUI::optionCount;
int Cheat::GUI::currentOption		= 0;
int Cheat::GUI::menuLevel			= 0;
SubMenus Cheat::GUI::PreviousMenu	= NOMENU;
int Cheat::GUI::PreviousMenuLevel;
int Cheat::GUI::optionsArray		[1000];
SubMenus Cheat::GUI::menusArray		[1000];
SubMenus Cheat::GUI::currentMenu;
bool Cheat::GUI::RestorePreviousSubmenu = true;
int Cheat::GUI::maxVisOptions		= 10;
int Delay							= GetTickCount64();

void Cheat::GUI::MoveMenu(SubMenus menu)
{
	GUI::menusArray[GUI::menuLevel] = GUI::currentMenu;
	GUI::optionsArray[GUI::menuLevel] = GUI::currentOption;
	GUI::menuLevel++;
	GUI::currentMenu = menu;
	GUI::currentOption = 1;
}

void Cheat::GUI::BackMenu()
{
	GUI::PreviousMenu = GUI::currentMenu;
	GUI::PreviousMenuLevel = GUI::menuLevel;
	GUI::previousOption = GUI::currentOption;
	GUI::menuLevel--;
	GUI::currentMenu = GUI::menusArray[GUI::menuLevel];
	GUI::currentOption = GUI::optionsArray[GUI::menuLevel];
}

void Cheat::GUI::CloseMenu()
{
	GUI::PreviousMenu = Cheat::GUI::currentMenu;
	GUI::PreviousMenuLevel = GUI::menuLevel;
	GUI::previousOption = GUI::currentOption;
	GUI::menuLevel = 0;
	GUI::currentMenu = GUI::menusArray[GUI::menuLevel];
	GUI::currentOption = GUI::optionsArray[GUI::menuLevel];
}

void Cheat::GUI::ControlsLoop() 
{
	OptionSelect = false;
	if (GetTickCount64() - Delay > 150) 
	{
		if (GetAsyncKeyState(GUI::openKey) & 0x8000 && Cheat::CheatFunctions::IsGameWindowFocussed())
		{
			AUDIO::PLAY_SOUND_FRONTEND("SELECT", "HUD_SHOP_SOUNDSET", true, 0);
			if (GUI::menuLevel == 0)
			{
				if (GUI::PreviousMenu != NOMENU && Cheat::GUI::RestorePreviousSubmenu)
				{ 
					GUI::MoveMenu(GUI::PreviousMenu); GUI::menuLevel = GUI::PreviousMenuLevel; GUI::currentOption = GUI::previousOption; 
				}
				else 
				{ 
					GUI::MoveMenu(MainMenu); 
				}
			}
			else
			{
				GUI::CloseMenu();
			}
			Delay = GetTickCount64();
		}
		if (GetAsyncKeyState(VK_NUMPAD0) & 0x8000 && Cheat::CheatFunctions::IsGameWindowFocussed())
		{
			if (GUI::menuLevel > 0) { GUI::BackMenu(); AUDIO::PLAY_SOUND_FRONTEND("BACK", "HUD_SHOP_SOUNDSET", true, 0); }
			Delay = GetTickCount64();
		}
		if (GetAsyncKeyState(VK_NUMPAD5) & 0x8000 && Cheat::CheatFunctions::IsGameWindowFocussed() || GetAsyncKeyState(VK_RETURN) && Cheat::CheatFunctions::IsGameWindowFocussed())
		{
			OptionSelect = true;
			AUDIO::PLAY_SOUND_FRONTEND("SELECT", "HUD_SHOP_SOUNDSET", true, 0);
			Delay = GetTickCount64();
		}
		if (GetAsyncKeyState(VK_NUMPAD2) & 0x8000 && Cheat::CheatFunctions::IsGameWindowFocussed())
		{
			GUI::currentOption < GUI::optionCount ? GUI::currentOption++ : GUI::currentOption = 1;
			AUDIO::PLAY_SOUND_FRONTEND("BACK", "HUD_SHOP_SOUNDSET", true, 0);
			Delay = GetTickCount64();
			downpress = true;
		}
		if (GetAsyncKeyState(VK_NUMPAD8) & 0x8000 && Cheat::CheatFunctions::IsGameWindowFocussed())
		{
			GUI::currentOption > 1 ? GUI::currentOption-- : GUI::currentOption = GUI::optionCount;
			AUDIO::PLAY_SOUND_FRONTEND("BACK", "HUD_SHOP_SOUNDSET", true, 0);
			Delay = GetTickCount64();
			uppress = true;
		}
		if (GetAsyncKeyState(VK_NUMPAD4) & 0x8000 && Cheat::CheatFunctions::IsGameWindowFocussed())
		{
			leftpress = true;
			AUDIO::PLAY_SOUND_FRONTEND("BACK", "HUD_SHOP_SOUNDSET", true, 0);
			Delay = GetTickCount64();
		}
		if (GetAsyncKeyState(VK_NUMPAD6) & 0x8000 && Cheat::CheatFunctions::IsGameWindowFocussed())
		{
			rightpress = true;
			AUDIO::PLAY_SOUND_FRONTEND("BACK", "HUD_SHOP_SOUNDSET", true, 0);
			Delay = GetTickCount64();
		}
	}
	GUI::optionCount = 0;
}
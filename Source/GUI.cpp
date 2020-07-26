#pragma once
#include "stdafx.h"

float Cheat::GUI::MenuXFloat = 330.f;
float MenuOptionsTextSize = 45.f;
rgba titleRGB = { 255, 255, 255, 255 };
rgba optionRectRGB{ 255, 0, 0, 200 };
rgba optionRGB = { 255, 255, 255, 255 };
rgba scrollerRGB = { 0, 0, 255, 255 };
const char* InformationTextVar;

void drawRect(float x, float y, float w, float h, rgba rgba) {
	int ResolutionX;
	int ResolutionY;
	GRAPHICS::GET_SCREEN_RESOLUTION(&ResolutionX, &ResolutionY);
	x /= ResolutionX; w /= ResolutionX;
	y /= ResolutionY; h /= ResolutionY;

	GRAPHICS::DRAW_RECT(x, y, w, h, rgba.r, rgba.g, rgba.b, rgba.a, 0, 0);
}
void Cheat::GUI::drawText(std::string text, float font_size, float x, float y, rgba rgba, bool centered = false) {
	int ResolutionX;
	int ResolutionY;
	GRAPHICS::GET_SCREEN_RESOLUTION(&ResolutionX, &ResolutionY);
	x /= ResolutionX; y /= ResolutionY;
	font_size /= 100.f;
	HUD::SET_TEXT_SCALE(font_size, font_size);
	HUD::_SET_TEXT_COLOR(rgba.r, rgba.g, rgba.b, rgba.a);
	HUD::SET_TEXT_CENTRE(centered);
	auto str = MISC::_CREATE_VAR_STRING(10, xorstr_("LITERAL_STRING"), text.c_str());
	HUD::_DRAW_TEXT(str, x, y);
}
void Cheat::GUI::Title(const char* title) {
	drawRect(MenuXFloat, 42.f, 350.f, 40.f, { 0, 0, 255, 255 });
	drawText(title, 73.f, MenuXFloat, 22.f, titleRGB, true);
}
bool Cheat::GUI::Option(const char* option, const char* InformationText) {
	Cheat::Controls::optionCount++;

	bool thisOption = false;
	if (Cheat::Controls::currentOption == Cheat::Controls::optionCount) thisOption = true;

	if (Cheat::Controls::currentOption <= Controls::maxVisOptions && Cheat::Controls::optionCount <= Controls::maxVisOptions) {
		drawText(option, MenuOptionsTextSize, MenuXFloat - 170.f, ((Cheat::Controls::optionCount * 30.f) + 36.5f), optionRGB);
		if (thisOption) { drawRect(MenuXFloat, ((Cheat::Controls::optionCount * 30.22f) + 46.6f), 350.f, 30.22f, scrollerRGB); }
		else { drawRect(MenuXFloat, ((Cheat::Controls::optionCount * 30.22f) + 46.6f), 350.f, 30.22f, { 0, 0, 0, 150 }); }
	}

	else if ((Cheat::Controls::optionCount > (Cheat::Controls::currentOption - Controls::maxVisOptions)) && Cheat::Controls::optionCount <= Cheat::Controls::currentOption)
	{
		drawText(option, MenuOptionsTextSize, MenuXFloat - 170.f, (((Cheat::Controls::optionCount - (Cheat::Controls::currentOption - Controls::maxVisOptions)) * 30.f) + 36.5f), optionRGB);
		if (thisOption) { drawRect(MenuXFloat, (((Cheat::Controls::optionCount - (Cheat::Controls::currentOption - Controls::maxVisOptions)) * 30.22f) + 46.6f), 350.f, 30.22f, scrollerRGB); }
		else { drawRect(MenuXFloat, (((Cheat::Controls::optionCount - (Cheat::Controls::currentOption - Controls::maxVisOptions)) * 30.22f) + 46.6f), 350.f, 30.22f, { 0, 0, 0, 150 }); }
	}

	if (thisOption) { InformationTextVar = InformationText; }

	if (Cheat::Controls::optionpress && Cheat::Controls::currentOption == Cheat::Controls::optionCount)
	{
		if (Cheat::Controls::optionpress)
		{
			return true;
		}
	}
	else return false;
}
bool Cheat::GUI::MenuOption(const char* option, SubMenus menu) {
	Option(option, "");

	if (Cheat::Controls::optionpress && Cheat::Controls::currentOption == Cheat::Controls::optionCount) {
		Cheat::Controls::optionpress = false;
		Cheat::Controls::MoveMenu(menu);
		return true;
	}
	else return false;
}

bool Cheat::GUI::BoolOption(const char* option, bool* isEnabled, const char* InformationText) {
	Option(option, InformationText);

	if (Cheat::Controls::currentOption <= Controls::maxVisOptions && Cheat::Controls::optionCount <= Controls::maxVisOptions) {
		drawText(*isEnabled ? xorstr_("~COLOR_GREEN~On") : xorstr_("~COLOR_RED~Off"), MenuOptionsTextSize, MenuXFloat + 135.f, ((Cheat::Controls::optionCount * 30.f) + 36.5f), optionRGB);
	}
	else if ((Cheat::Controls::optionCount > (Cheat::Controls::currentOption - 16)) && Cheat::Controls::optionCount <= Cheat::Controls::currentOption)
	{
		drawText(*isEnabled ? xorstr_("~COLOR_GREEN~On") : xorstr_("~COLOR_RED~Off"), MenuOptionsTextSize, MenuXFloat + 135.f, ((Cheat::Controls::optionCount * 30.f) + 36.5f), optionRGB);
	}

	if (Cheat::Controls::optionpress && Cheat::Controls::currentOption == Cheat::Controls::optionCount) {
		*isEnabled ^= 1;
		return true;
	}
	return false;
}

bool Cheat::GUI::IntOption(const char* option, int* var, int min, int max, int step, const char* InformationText) {
	Option(option, InformationText);

	if (Cheat::Controls::currentOption <= Controls::maxVisOptions && Cheat::Controls::optionCount <= Controls::maxVisOptions) {
		drawText("<" + std::to_string(*var) + ">", MenuOptionsTextSize, MenuXFloat + 130.f, (Cheat::Controls::optionCount * 30.f) + 36.5f, optionRGB);
	}
	else if ((Cheat::Controls::optionCount > (Cheat::Controls::currentOption - 16)) && Cheat::Controls::optionCount <= Cheat::Controls::currentOption)
	{
		drawText("<" + std::to_string(*var) + ">", MenuOptionsTextSize, MenuXFloat + 130.f, (((Cheat::Controls::optionCount - (Cheat::Controls::currentOption - Controls::maxVisOptions)) * 30.f) + 36.5f), optionRGB);
	}

	if (Cheat::Controls::currentOption == Cheat::Controls::optionCount) {
		if (Cheat::Controls::leftpress) {
			if (*var <= min) *var = max;
			else *var -= step;
			Cheat::Controls::leftpress = false;
			return true;
		}
		if (*var < min) *var = max;
		if (Cheat::Controls::rightpress) {
			if (*var >= max) *var = min;
			else *var += step;
			Cheat::Controls::rightpress = false;
			return true;
		}
		if (*var > max) *var = min;
	}

	if (Cheat::Controls::optionpress && Cheat::Controls::currentOption == Cheat::Controls::optionCount)
	{
		return true;
	}
	else return false;
}

bool Cheat::GUI::FloatOption(const char* option, float* var, float min, float max, float step, const char* InformationText) {
	Option(option, InformationText);

	char buf[100];
	_snprintf_s(buf, sizeof(buf), xorstr_("%.2f"), *var);

	if (Cheat::Controls::currentOption <= Controls::maxVisOptions && Cheat::Controls::optionCount <= Controls::maxVisOptions) {
		drawText("<" + (std::string)buf + ">", MenuOptionsTextSize, MenuXFloat + 85.f, (Cheat::Controls::optionCount * 30.f) + 37.f, optionRGB);
	}
	else if ((Cheat::Controls::optionCount > (Cheat::Controls::currentOption - 16)) && Cheat::Controls::optionCount <= Cheat::Controls::currentOption)
	{
		drawText("<" + (std::string)buf + ">", MenuOptionsTextSize, MenuXFloat + 85.f, (((Cheat::Controls::optionCount - (Cheat::Controls::currentOption - Controls::maxVisOptions)) * 30.f) + 37.f), optionRGB);
	}


	if (Cheat::Controls::currentOption == Cheat::Controls::currentOption) {
		if (Cheat::Controls::leftpress) {
			if (*var <= min) *var = max;
			else *var -= step;
			Cheat::Controls::leftpress = false;
			return true;
		}
		if (*var < min) *var = max;

		if (Cheat::Controls::rightpress) {
			if (*var >= max) *var = min;
			else *var += step;
			Cheat::Controls::rightpress = false;
			return true;
		}
		if (*var > max) *var = min;
	}

	if (Cheat::Controls::optionpress && Cheat::Controls::currentOption == Cheat::Controls::optionCount)
		return true;
	else return false;
}
void Cheat::GUI::End()
{
	if (Cheat::Controls::currentMenu != 0)
	{
		if (Cheat::Controls::optionCount <= Controls::maxVisOptions)
		{
			drawRect(MenuXFloat, 30.22f * Cheat::Controls::optionCount + 62.f, 350.f, 1.f, { 0, 0, 255, 255 });  //Menu bottom line
			drawRect(MenuXFloat, 30.22f * Cheat::Controls::optionCount + 77.5f, 350.f, 30.f, { 0, 0, 0, 255 });  //Menu bottom draw
			drawText(Cheat::CheatFunctions::ReturnCheatBuildAsString(), 40.f, MenuXFloat + 115.f, 30.22f * Cheat::Controls::optionCount + 67.f, { 255, 255, 255, 255 }, false); //Cheat build number draw
			drawText(std::to_string(Cheat::Controls::currentOption) + xorstr_(" / ") + std::to_string(Cheat::Controls::optionCount), 40.f, MenuXFloat - 170.f, 30.22f * Cheat::Controls::optionCount + 67.f, { 255, 255, 255, 255 }, false); //Option count draw at menu bottom
			if (InformationTextVar != "")
			{
				drawRect(MenuXFloat, 30.22f * Cheat::Controls::optionCount + 103.f, 350.f, 25.f, { 0, 0, 0, 255 });
				drawText(InformationTextVar, 40.f, MenuXFloat - 170.f, 30.22f * Cheat::Controls::optionCount + 90.f, { 255, 255, 255, 255 }, false);
			}
		}
		else
		{
			drawRect(MenuXFloat, 30.22f * Controls::maxVisOptions + 62.f, 350.f, 1.f, { 0, 0, 255, 255 });   //Menu bottom line
			drawRect(MenuXFloat, 30.22f * Controls::maxVisOptions + 77.5f, 350.f, 30.f, { 0, 0, 0, 255 });   //Menu bottom draw
			drawText(Cheat::CheatFunctions::ReturnCheatBuildAsString(), 40.f, MenuXFloat + 115.f, 30.22f * Controls::maxVisOptions + 67.f, { 255, 255, 255, 255 }, false); //Cheat build number draw
			drawText(std::to_string(Cheat::Controls::currentOption) + xorstr_(" / ") + std::to_string(Cheat::Controls::optionCount), 40.f, MenuXFloat - 170.f, 30.22f * Controls::maxVisOptions + 67.f, { 255, 255, 255, 255 }, false); //Option count draw at menu bottom
			if (InformationTextVar != "")
			{
				drawRect(MenuXFloat, 30.22f * Controls::maxVisOptions + 103.f, 350.f, 25.f, { 0, 0, 0, 255 });
				drawText(InformationTextVar, 40.f, MenuXFloat - 170.f, 30.22f * Controls::maxVisOptions + 90.f, { 255, 255, 255, 255 }, false);
			}
		}
	}
}
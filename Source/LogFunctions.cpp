#pragma once
#include "stdafx.h"

void Cheat::LogFunctions::Message(char* Message)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	std::cout << Cheat::CheatFunctions::ReturnDateAndTimeAsString() << " [Info] " << Message << std::endl;
}

void Cheat::LogFunctions::MessageCustomCategory(std::string CategoryName, std::string Message)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	std::cout << Cheat::CheatFunctions::ReturnDateAndTimeAsString() << " [" << CategoryName << "] " << Message << std::endl;
}

void Cheat::LogFunctions::DebugMessage(char* Message)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
	std::cout << Cheat::CheatFunctions::ReturnDateAndTimeAsString() << " [Debug] " << Message << std::endl;
}

void Cheat::LogFunctions::Error(char* Message, bool ShowMessageBox)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
	std::cout << Cheat::CheatFunctions::ReturnDateAndTimeAsString() << " [Error] " << Message << " " << std::endl;
	if (ShowMessageBox)
	{
		MessageBoxA(NULL, Message, "RDR2 Cheat Error", MB_TOPMOST | MB_ICONERROR | MB_SETFOREGROUND);
	}
}

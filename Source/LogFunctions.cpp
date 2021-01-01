#pragma once
#include "stdafx.h"

void Cheat::LogFunctions::Init()
{
	std::cout << Cheat::CheatFunctions::ReturnDateAndTimeAsString() << " [Init] " << "Initializing RDR2 Cheat" << " | Github Repository: https://github.com/HowYouDoinMate/RedDeadRedemption2-Cheat" << std::endl;
}

void Cheat::LogFunctions::Message(char* Message)
{
	std::cout << Cheat::CheatFunctions::ReturnDateAndTimeAsString() << " [Info] " << Message << std::endl;
}

void Cheat::LogFunctions::DebugMessage(char* Message)
{
	std::cout << Cheat::CheatFunctions::ReturnDateAndTimeAsString() << " [Debug] " << Message << std::endl;
}

void Cheat::LogFunctions::Error(char* Message)
{
	std::cout << Cheat::CheatFunctions::ReturnDateAndTimeAsString() << " [Error] " << Message << " " << std::endl;
	MessageBoxA(NULL, Message, "RDR2 Error", MB_TOPMOST | MB_ICONERROR | MB_SETFOREGROUND);
}

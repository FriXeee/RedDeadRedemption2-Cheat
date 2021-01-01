#include "stdafx.h"

DWORD										wakeAt;
static HANDLE								MainFiber;
static DWORD								WakeUpTime;
typedef int(__fastcall* get_player_ped_fn)	(Player player);
get_player_ped_fn							GetPlayerPedOriginal;

void ScriptFunction(LPVOID lpParameter)
{
	srand(GetTickCount64());
	Cheat::Main();
}

int GetPlayedPedHooked(Player PlayerHandle) 
{
	if (!MainFiber) { MainFiber = ConvertThreadToFiber(nullptr); }
	if (WakeUpTime > timeGetTime()) { goto ReturnOriginal; }
	static HANDLE MainFiberHandle = nullptr;
	if (MainFiberHandle) { SwitchToFiber(MainFiberHandle); }
	else { MainFiberHandle = CreateFiber(NULL, ScriptFunction, nullptr); }

ReturnOriginal:
	return GetPlayerPedOriginal(PlayerHandle);
}

void Cheat::GameHooking::Init() 
{
	Cheat::LogFunctions::Message("Hooking Game Function & Creating Main Fiber");
	if (MH_Initialize() != MH_OK) { Cheat::LogFunctions::Error("Failed to initialize MinHook", true); std::exit(EXIT_SUCCESS); }
	Cheat::LogFunctions::DebugMessage("Initialized MinHook");
	Cheat::LogFunctions::DebugMessage("Hooking 'GET_PLAYER_PED'");
	uintptr_t GetPlayerPedPointer = Memory::find_signature(NULL, "\x40\x53\x48\x83\xEC\x20\x33\xDB\x81\xF9", "xxxxxxxxxx");
	if (MH_CreateHook((PVOID)GetPlayerPedPointer, GetPlayedPedHooked, reinterpret_cast<void**>(&GetPlayerPedOriginal)) != MH_OK) { Cheat::LogFunctions::Error("Failed to hook GET_PLAYER_PED", true); std::exit(EXIT_SUCCESS); }
	if (MH_EnableHook((PVOID)GetPlayerPedPointer)) { Cheat::LogFunctions::Error("Failed to enable GET_PLAYER_PED hook", true); std::exit(EXIT_SUCCESS); }
}

void Cheat::GameHooking::FiberWait(DWORD ms, bool ShowMessage)
{
	if (ShowMessage) { Cheat::GUI::drawText("One moment please", 80.f, 150.f, 600.f, { 255, 255, 255 }, true); }
	WakeUpTime = timeGetTime() + ms;
	SwitchToFiber(MainFiber);
}
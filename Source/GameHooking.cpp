#pragma once
#include "stdafx.h"
#include "invoker.h"
#include "natives.h"


HANDLE mainFiber;
DWORD wakeAt;
void OnFiberThreadTick();

static HANDLE MainFiber;
static DWORD time_to_wake_up;

typedef int(__fastcall* get_player_ped_fn)(Player player);
get_player_ped_fn o_get_player_ped;

int __fastcall get_player_ped_hook(Player player) {
	OnFiberThreadTick();
	return o_get_player_ped(player);
}

void Cheat::GameHooking::Init() 
{
	Cheat::LogFunctions::Message(xorstr_("Hooking Game Functions"));
	auto status = MH_Initialize();
	auto get_player_ped = Memory::find_signature(0, xorstr_("\x40\x53\x48\x83\xEC\x20\x33\xDB\x81\xF9"), xorstr_("xxxxxxxxxx"));
	status = MH_CreateHook((PVOID)get_player_ped, get_player_ped_hook, reinterpret_cast<void**>(&o_get_player_ped));
	status = MH_EnableHook((PVOID)get_player_ped);
	if (status != MH_OK) { Cheat::LogFunctions::Error(xorstr_("Failed to hook game functions")); std::exit(EXIT_SUCCESS); }
}

void WAIT(DWORD ms) {
	time_to_wake_up = timeGetTime() + ms;
	SwitchToFiber(MainFiber);
}

void __stdcall fiber_thread(LPVOID params) {
	srand(GetTickCount64());
	Cheat::Main();

	/*
	while (true) 
	{
		Cheat::CheatFeatures::Loop();
		Cheat::GameFunctions::GUILoop();
		WAIT(0);
	}
	*/
}

void OnFiberThreadTick() {
	if (!MainFiber) { MainFiber = ConvertThreadToFiber(nullptr); }
	if (time_to_wake_up > timeGetTime()) { return; }
	static HANDLE fiber_handle = nullptr;
	if (fiber_handle) { SwitchToFiber(fiber_handle); }
	else { fiber_handle = CreateFiber(NULL, fiber_thread, nullptr); }
}

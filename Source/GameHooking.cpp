#pragma once
#include "stdafx.h"
#include "invoker.h"
#include "natives.h"


DWORD										wakeAt;
void										OnFiberThreadTick();
static HANDLE								MainFiber;
static DWORD								WakeUpTime;
typedef int(__fastcall* get_player_ped_fn)	(Player player);
get_player_ped_fn							o_get_player_ped;
void __stdcall								FiberThread(LPVOID params);

int __fastcall get_player_ped_hook(Player PlayerHandle) 
{
	OnFiberThreadTick();
	return o_get_player_ped(PlayerHandle);
}

void Cheat::GameHooking::Init() 
{
	Cheat::LogFunctions::Message(xorstr_("Hooking Game Function & Creating Main Cheat Fiber"));
	if (MH_Initialize() != MH_OK) { Cheat::LogFunctions::Error("Failed to initialize MinHook"); std::exit(EXIT_SUCCESS); }
	uintptr_t GetPlayerPedPointer = Memory::find_signature(0, xorstr_("\x40\x53\x48\x83\xEC\x20\x33\xDB\x81\xF9"), xorstr_("xxxxxxxxxx"));
	if (MH_CreateHook((PVOID)GetPlayerPedPointer, get_player_ped_hook, reinterpret_cast<void**>(&o_get_player_ped)) != MH_OK) { Cheat::LogFunctions::Error("Failed to hook GET_PLAYER_PED"); std::exit(EXIT_SUCCESS); }
	if (MH_EnableHook((PVOID)GetPlayerPedPointer)) { Cheat::LogFunctions::Error("Failed to enable GET_PLAYER_PED hook"); std::exit(EXIT_SUCCESS); }
}

void Cheat::GameHooking::FiberWait(DWORD ms, bool ShowMessage)
{
	if (ShowMessage) { Cheat::GUI::drawText(xorstr_("One moment please"), 80.f, 500.f, 450.f, { 255, 255, 255 }, true); }
	WakeUpTime = timeGetTime() + ms;
	SwitchToFiber(MainFiber);
}

void __stdcall FiberThread(LPVOID params) 
{
	srand(GetTickCount64());
	Cheat::Main();
}

void OnFiberThreadTick() 
{
	if (!MainFiber) { MainFiber = ConvertThreadToFiber(nullptr); }
	if (WakeUpTime > timeGetTime()) { return; }
	static HANDLE fiber_handle = nullptr;
	if (fiber_handle) { SwitchToFiber(fiber_handle); }
	else { fiber_handle = CreateFiber(NULL, FiberThread, nullptr); }
}
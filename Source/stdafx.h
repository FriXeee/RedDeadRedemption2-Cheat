#pragma once

#include <Windows.h>
#include <string>
#include <stdio.h>
#include <direct.h>
#include <iostream>
#include <sstream>
#include <timeapi.h>
#include <ctime>
#include <vector>
#include <iomanip>
#include <time.h>
#include <map>
#include <fstream>
#include <atlstr.h>
#include <corecrt_io.h>
#include <filesystem>
#include <fcntl.h>
#include <algorithm>
#include <psapi.h>
#include <stdint.h>
#include <tlhelp32.h>
#pragma comment(lib, "winmm.lib")


//XORSTR
#include "ThirdParty/XORSTR/xorstr.hpp"

//MinHook
#include "ThirdParty/MinHook/MinHook.h"
#pragma comment(lib, "ThirdParty/MinHook/libMinHook-x64-v141-md.lib")


#include "invoker.h"
#include "natives.h"
#include "controls.h"


namespace Cheat 
{
	namespace CheatFunctions 
	{
		std::string ReturnCheatBuildAsString();
		const std::string ReturnCustomTeleportsIniFilePath();
		std::string ReturnConfigFilePath();
		std::string ReturnCheatModuleDirectoryPath();
		bool IsIntegerInRange(unsigned low, unsigned high, unsigned x);
		bool DoesDirectoryExists(const std::string& dirName_in);
		void CreateNewDirectory(std::string Path);
		std::string ReturnDateAndTimeAsString();
		void CreateConsoleWindow();
		char* CombineTwoStrings(char* string1, char* string2);
		void CheatInitialization();
		int MessageBoxWithAutoClose(HWND hWnd, const WCHAR* sText, const WCHAR* sCaption, UINT uType, DWORD dwMilliseconds);
		std::string ReadStringFromIni(std::string file, std::string app, std::string key);
		bool IsGameWindowFocussed();
		void LoadConfig(bool StartUp);
		void WriteBoolToIni(bool b00l, std::string file, std::string app, std::string key);
		void WriteStringToIni(std::string string, std::string file, std::string app, std::string key);
		void SaveConfig();
		void WriteFloatToIni(float floatValue, std::string file, std::string app, std::string key);
		void SaveCustomTeleport(char* Name);
		void DeleteCustomTeleport(char* Name);
		void DoCustomLocationTeleport(char* Name);
		bool isWhitespace(std::string s);
		std::string GetLastErrorAsString();
	}
	namespace CheatFeatures
	{
		extern bool AutoSaveConfigBool;
		extern bool TriggerbotShootPlayersBool;
		extern int OnlineSelectedPlayer;
		extern bool SpawnVehicleInvincibleBool;
		extern bool SpawnPedInVehicleBool;
		extern bool DeleteCurrentVehicleBool;
		extern bool SpawnPedDeadBool;

		void Loop();
		extern bool GodmodeBool;
		extern void Godmode(bool toggle);
		extern bool SuperJumpBool;
		extern void SuperJump();
		extern bool NoRagdollBool;
		extern void NoRagdoll(bool toggle);
		extern bool IgnoredByNPCBool;
		extern void IgnoredByNPC(bool toggle);
		extern bool TriggerbotBool;
		extern void Triggerbot();
		extern bool SuperRunBool;
		extern void SuperRun();
		extern bool NeverWantedBool;
		extern void NeverWanted(bool toggle);
		extern bool UnlimitedDeadEyeBool;
		extern void UnlimitedDeadEye();
		extern bool InvisibleBool;
		extern void Invisible(bool toggle);
		extern bool InfiniteStaminaBool;
		extern void InfiniteStamina();
		extern bool InfiniteAmmoBool;
		extern void InfiniteAmmo();
		extern bool NoClipBool;
		extern void NoClip();
		extern bool AutoTeleportToWaypointBool;
		extern void AutoTeleportToWaypoint();
		extern bool HideHUDBool;
		extern void HideHUD(bool toggle);
		extern bool SpectateSelectedPlayerBool;
		extern void SpectateSelectedPlayer(bool toggle);
		extern bool FreezeSelectedPlayerBool;
		extern void FreezeSelectedPlayer();
		extern bool WeaponDamageMultiplierBool;
		extern void WeaponDamageMultiplier(bool toggle);
		extern bool TeleportGunBool;
		extern void TeleportGun();
		extern bool ExplosiveAmmoBool;
		extern void ExplosiveAmmo();
		extern bool VehicleGodmodeBool;
		extern void VehicleGodmode(bool toggle);
		extern bool VehicleInvisibleBool;
		extern void VehicleInvisible(bool toggle);	
		extern bool UnlimitedHorseStaminaBool;
		extern void UnlimitedHorseStamina();
	}
	namespace GameFunctions 
	{
		void SpawnVehicleCustomInput();
		void GiveAllWeapons(Ped Player);
		void TPto(Vector3 Coords);
		void GUILoop();
		void RequestControlOfId(Entity netid);
		void RequestControlOfEnt(Entity entity);
		void PrintSubtitle(const char* Text);
		void TeleportToWaypoint();
		void SpawnVehicle(const char* ModelHash);
		void SpawnPed(const char* ModelHash, Ped PlayerPed);
		void DeleteCurrentVehicleHorse();
		void ShowPlayerInformationBox(Player Player);
		void GetCameraDirection(float* dirX, float* dirY, float* dirZ);
	}
	namespace GameArrays 
	{
		extern const std::vector<std::string> VehicleModels;
		extern const std::vector<std::string> PedModels;
	}
	namespace LogFunctions 
	{
		void Init();
		void Message(char* Message);
		void Error(char* Message);
		void DebugMessage(char* Message);
	}
	namespace GameHooking 
	{
		void Init();
		namespace Memory {
			static uintptr_t find_signature(const char* module, const char* pattern_, const char* mask) {
				const auto compare = [](const uint8_t* data, const uint8_t* pattern, const char* mask_) {
					for (; *mask_; ++mask_, ++data, ++pattern)
						if (*mask_ == 'x' && *data != *pattern)
							return false;

					return (*mask_) == 0;
				};


				MODULEINFO module_info = {};
				GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(module), &module_info, sizeof MODULEINFO);

				auto module_start = uintptr_t(module_info.lpBaseOfDll);
				const uint8_t* pattern = reinterpret_cast<const uint8_t*>(pattern_);
				for (size_t i = 0; i < module_info.SizeOfImage; i++)
					if (compare(reinterpret_cast<uint8_t*>(module_start + i), pattern, mask))
						return module_start + i;

				return 0;
			}
		}

	}
	namespace Controls
	{
		extern bool isMenuEnabled;
		extern int optioncount;
		extern int currentoption;
		extern bool optionpress;
		extern bool leftpress;
		extern bool rightpress;
		extern bool uppress;
		extern bool downpress;
		extern void CheckKeys();
		extern bool currentMenu(const char* menuname);
		extern void changeMenu(const char* menuname);
	}
	namespace GUI 
	{
		extern void Title(const char* title);
		extern void End();
		extern bool Option(const char* option, const char* InformationText);
		extern bool MenuOption(const char* option, const char* men);
		extern bool BoolOption(const char* option, bool* isEnabled, const char* InformationText);
		extern bool IntOption(const char* option, int* var, int min, int max, int step, const char* InformationText);
		extern bool FloatOption(const char* option, float* var, float min, float max, float step, const char* InformationText);
		extern float MenuXFloat;
	}
}

struct rgba {
	int r, g, b, a;
};

extern HINSTANCE ModuleHModule;
extern void WAIT(DWORD ms);

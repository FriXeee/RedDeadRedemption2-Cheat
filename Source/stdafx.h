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
#include <array>
#pragma comment(lib, "winmm.lib")


//MinHook
#include "ThirdParty/MinHook/MinHook.h"
#pragma comment(lib, "ThirdParty/MinHook/libMinHook-x64-v141-md.lib")


#include "Invoker.h"
#include "Natives.h"
#include "Enums.h"
#include "GUI.h"


namespace Cheat 
{
	void Main();
	namespace CheatFunctions 
	{
		void LoopedFunctions();
		std::string ReturnCheatBuildAsString();
		const std::string ReturnCustomTeleportsIniFilePath();
		std::string ReturnConfigFilePath();
		std::string ReturnCheatModuleDirectoryPath();
		bool IsIntegerInRange(unsigned low, unsigned high, unsigned x);
		bool DoesDirectoryExists(const std::string& dirName_in);
		void CreateNewDirectory(std::string Path);
		std::string ReturnDateAndTimeAsString();
		void CreateConsole();
		char* CombineTwoStrings(char* string1, char* string2);
		std::string ReadStringFromIni(std::string file, std::string app, std::string key);
		bool IsGameWindowFocussed();
		void WriteBoolToIni(bool b00l, std::string file, std::string app, std::string key);
		void WriteStringToIni(std::string string, std::string file, std::string app, std::string key);
		void WriteFloatToIni(float floatValue, std::string file, std::string app, std::string key);
		void SaveCustomTeleport(char* Name);
		void DeleteCustomTeleport(char* Name);
		void DoCustomLocationTeleport(char* Name);
		bool isWhitespace(std::string s);
		std::string GetLastErrorAsString();
		void PostInitCheat();
		std::string VirtualKeyCodeToString(UCHAR virtualKey);
		bool ReturnPressedKey(int& PressedKey);
	}
	namespace CheatFeatures
	{
		extern bool TriggerbotShootPlayersBool;
		extern int SelectedPlayer;
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
		extern bool NoClipWasEnabled;
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
		extern bool HorseGodmodeBool;
		extern void HorseGodmode(bool toggle);
		extern bool VehicleInvisibleBool;
		extern void VehicleInvisible(bool toggle);	
		extern bool HorseInvisibleBool;
		extern void HorseInvisible(bool toggle);
		extern bool UnlimitedHorseStaminaBool;
		extern void UnlimitedHorseStamina();
	}
	namespace GameFunctions 
	{
		extern Player PlayerID;
		extern Ped PlayerPedID;
		void GiveAllWeapons(Ped Player);
		void TeleportToCoords(Entity e, Vector3 coords, bool AutoCorrectGroundHeight, bool IgnoreCurrentPedVehicleOrHorse);
		void RequestNetworkControlOfEntity(Entity entity);
		void PrintSubtitle(std::string Text);
		void TeleportToWaypoint();
		void SpawnVehicle(const char* ModelHash);
		void SpawnPed(const char* ModelHash, Ped PlayerPed);
		void DeleteCurrentVehicle();
		void DeleteCurrentMount();
		void GetCameraDirection(float* dirX, float* dirY, float* dirZ);
	}
	namespace GameArrays 
	{
		extern const std::vector<std::string> WeaponHashes;
		extern const std::vector<std::string> VehicleModels;
		extern const std::vector<std::string> PedModels;
	}
	namespace LogFunctions 
	{
		void Message(char* Message);
		void MessageCustomCategory(std::string CategoryName, std::string Message);
		void Error(char* Message, bool ShowMessageBox);
		void DebugMessage(char* Message);
	}
	namespace GameHooking 
	{
		extern void FiberWait(DWORD ms, bool ShowMessage = true);
		void Init();
		namespace Memory 
		{
			static uintptr_t find_signature(const char* module, const char* pattern_, const char* mask) 
			{
				const auto compare = [](const uint8_t* data, const uint8_t* pattern, const char* mask_) 
				{
					for (; *mask_; ++mask_, ++data, ++pattern)
					{
						if (*mask_ == 'x' && *data != *pattern)
						{
							return false;
						}
					}
					return (*mask_) == 0;
				};


				MODULEINFO module_info = {};
				if (GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(module), &module_info, sizeof(MODULEINFO)))
				{
					auto module_start = uintptr_t(module_info.lpBaseOfDll);
					const uint8_t* pattern = reinterpret_cast<const uint8_t*>(pattern_);
					for (size_t i = 0; i < module_info.SizeOfImage; i++)
					{
						if (compare(reinterpret_cast<uint8_t*>(module_start + i), pattern, mask)) 
						{
							return module_start + i;
						}
					}
				}
				return 0;
			}
		}
	}
	namespace GUI
	{
		extern int openKey;
		extern int maxVisOptions;
		extern void MoveMenu(SubMenus menu);
		extern void CloseMenu();
		extern int previousOption;
		extern void BackMenu();
		extern int PreviousMenuLevel;
		extern SubMenus PreviousMenu;
		extern int menuLevel;
		extern int optionsArray[1000];
		extern SubMenus menusArray[1000];
		extern SubMenus currentMenu;
		extern int optionCount;
		extern int currentOption;
		extern bool OptionSelect;
		extern bool leftpress;
		extern bool rightpress;
		extern bool uppress;
		extern bool downpress;
		extern void ControlsLoop();
		extern bool RestorePreviousSubmenu;
		extern void Title(std::string title);
		extern void End();
		extern bool Option(std::string option, std::string InformationText);
		extern bool MenuOption(std::string option, SubMenus menu);
		extern bool BoolOption(std::string option, bool* isEnabled, std::string InformationText);
		extern bool IntOption(std::string option, int* var, int min, int max, int step, std::string InformationText);
		extern bool FloatOption(std::string option, float* var, float min, float max, float step, std::string InformationText);
		extern void drawText(std::string text, float font_size, float x, float y, rgba rgba, bool centered);
		extern float guiX;
	}
}

extern HINSTANCE ModuleHModule;

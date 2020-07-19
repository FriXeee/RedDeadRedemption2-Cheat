#pragma once
#include "stdafx.h"

std::string Cheat::CheatFunctions::ReturnCheatBuildAsString()
{
	return xorstr_("1.0.0.0");
}

const std::string Cheat::CheatFunctions::ReturnCustomTeleportsIniFilePath()
{
	return ReturnCheatModuleDirectoryPath() + (std::string)xorstr_("\\rdr2\\CustomTeleports.ini");
}

std::string Cheat::CheatFunctions::ReturnConfigFilePath()
{
	return ReturnCheatModuleDirectoryPath() + (std::string)xorstr_("\\rdr2\\config.ini");
}

std::string Cheat::CheatFunctions::ReturnCheatModuleDirectoryPath()
{
	char CheatModuleFilePath[MAX_PATH];
	GetModuleFileNameA(ModuleHModule, CheatModuleFilePath, MAX_PATH);
	return std::filesystem::path{ CheatModuleFilePath }.parent_path().string();
}

bool Cheat::CheatFunctions::DoesDirectoryExists(const std::string& dirName_in)
{
	DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES) { return false; }
	if (ftyp & FILE_ATTRIBUTE_DIRECTORY) { return true; }
	return false;
}

void Cheat::CheatFunctions::CreateNewDirectory(std::string Path)
{
	if (!std::filesystem::create_directory(Path))
	{
		std::string String = xorstr_("Failed to create directory '") + Path + xorstr_("' Error: ") + Cheat::CheatFunctions::GetLastErrorAsString();
		Cheat::LogFunctions::DebugMessage((char*)String.c_str());
	}
	else
	{
		std::string String = xorstr_("Created directory '") + Path + xorstr_("'");
		Cheat::LogFunctions::DebugMessage((char*)String.c_str());
	}
}

std::string Cheat::CheatFunctions::ReturnDateAndTimeAsString()
{
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);

	std::ostringstream oss;
	oss << std::put_time(&tm, xorstr_("[%d-%m-%Y - %H:%M:%S]"));
	auto str = oss.str();

	return str;
}


void Cheat::CheatFunctions::CreateConsoleWindow()
{
	AllocConsole();			
	SetConsoleTitleA(xorstr_("RDR2 Cheat Console"));

	// Set Console Dimensions so all text is properly visible
	HWND ConsoleWindowHandle = GetConsoleWindow();
	RECT CurrentRect;
	GetWindowRect(ConsoleWindowHandle, &CurrentRect);
	MoveWindow(ConsoleWindowHandle, CurrentRect.left, CurrentRect.top, 1200, 500, TRUE);
	CloseHandle(ConsoleWindowHandle);

	//Disable Close Button Off Console Window And Set Max Window Size
	HWND hwnd = ::GetConsoleWindow();
	if (hwnd != NULL)
	{
		SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

		HMENU hMenu = ::GetSystemMenu(hwnd, FALSE);
		if (hMenu != NULL) DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);


		//Disable Console Quick Edit Mode
		HANDLE stdIn = GetStdHandle(STD_INPUT_HANDLE);
		if (stdIn != INVALID_HANDLE_VALUE) {
			DWORD dwMode = 0;
			if (GetConsoleMode(stdIn, &dwMode)) {
				dwMode &= ~ENABLE_QUICK_EDIT_MODE;
				SetConsoleMode(stdIn, dwMode | ENABLE_EXTENDED_FLAGS);
			}
		}
	}

	//Redirect Std Outputs to Console
	HANDLE ConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	int SystemOutput = _open_osfhandle(intptr_t(ConsoleOutput), _O_TEXT);
	FILE* COutputHandle = _fdopen(SystemOutput, xorstr_("w"));
	HANDLE ConsoleError = GetStdHandle(STD_ERROR_HANDLE);
	int SystemError = _open_osfhandle(intptr_t(ConsoleError), _O_TEXT);
	FILE* CErrorHandle = _fdopen(SystemError, xorstr_("w"));
	HANDLE ConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
	int SystemInput = _open_osfhandle(intptr_t(ConsoleInput), _O_TEXT);
	FILE* CInputHandle = _fdopen(SystemInput, xorstr_("r"));
	std::ios::sync_with_stdio(true);
	freopen_s(&CInputHandle, xorstr_("CONIN$"), xorstr_("r"), stdin);
	freopen_s(&COutputHandle, xorstr_("CONOUT$"), xorstr_("w"), stdout);
	freopen_s(&CErrorHandle, xorstr_("CONOUT$"), xorstr_("w"), stderr);
	std::wcout.clear();
	std::cout.clear();
	std::wcerr.clear();
	std::cerr.clear();
	std::wcin.clear();
	std::cin.clear();

	std::cout << xorstr_("Build: ") << Cheat::CheatFunctions::ReturnCheatBuildAsString() << std::endl;
}


char str[200];
char str2[128];
char* Cheat::CheatFunctions::CombineTwoStrings(char* string1, char* string2)
{
	strcpy_s(str2, "");
	sprintf_s(str2, xorstr_("%s %s"), string1, string2);
	return str2;
}



int Cheat::CheatFunctions::MessageBoxWithAutoClose(HWND hWnd, const WCHAR* sText, const WCHAR* sCaption, UINT uType, DWORD dwMilliseconds)
{
	typedef int(__stdcall* MSGBOXWAPI)(IN HWND hWnd, IN LPCWSTR lpText, IN LPCWSTR lpCaption, IN UINT uType, IN WORD wLanguageId, IN DWORD dwMilliseconds);

	int iResult;

	HMODULE hUser32 = LoadLibraryA(xorstr_("user32.dll"));
	if (hUser32)
	{
		auto MessageBoxTimeoutW = (MSGBOXWAPI)GetProcAddress(hUser32, xorstr_("MessageBoxTimeoutW"));

		iResult = MessageBoxTimeoutW(hWnd, sText, sCaption, uType, 0, dwMilliseconds);

		FreeLibrary(hUser32);
	}
	else
	{
		FreeLibrary(hUser32);
		return 0;
	}

	return iResult;
}


void Cheat::CheatFunctions::WriteFloatToIni(float floatValue, std::string file, std::string app, std::string key)
{
	WriteStringToIni((std::to_string(floatValue)), file, app, key);
}

std::string Cheat::CheatFunctions::ReadStringFromIni(std::string file, std::string app, std::string key)
{
	char buf[100];
	GetPrivateProfileStringA(app.c_str(), key.c_str(), "", buf, 100, file.c_str());
	return (std::string)buf;
}
void Cheat::CheatFunctions::WriteBoolToIni(bool b00l, std::string file, std::string app, std::string key)
{
	WriteStringToIni(b00l ? xorstr_("true") : xorstr_("false"), file, app, key);
}
void Cheat::CheatFunctions::WriteStringToIni(std::string string, std::string file, std::string app, std::string key)
{
	WritePrivateProfileStringA(app.c_str(), key.c_str(), string.c_str(), file.c_str());
}


bool Cheat::CheatFunctions::IsGameWindowFocussed()
{
	HWND GameWindowHandle = FindWindowA(0, xorstr_("Red Dead Redemption 2"));
	HWND HandleProcessWithKeyboardFocus = GetForegroundWindow();
	if (GameWindowHandle == HandleProcessWithKeyboardFocus) { return true; } else { return false; }
}


bool Cheat::CheatFunctions::IsIntegerInRange(unsigned low, unsigned high, unsigned x)
{
	return  ((x - low) <= (high - low));
} 

void Cheat::CheatFunctions::SaveCustomTeleport(char* Name)
{
	std::string ValidationStr = Name;
	if (ValidationStr.empty() || isWhitespace(ValidationStr)) { Cheat::GameFunctions::PrintSubtitle("~COLOR_RED~You must enter a valid Custom Teleport name"); return; }
	if (ValidationStr.length() > 50) { Cheat::GameFunctions::PrintSubtitle("~COLOR_RED~The Custom Teleport name cannot be longer than 50 characters"); return; }
	if (ValidationStr.length() < 3) { Cheat::GameFunctions::PrintSubtitle("~COLOR_RED~The Custom Teleport name must be at least 3 characters long"); return; }
	if (std::any_of(ValidationStr.begin(), ValidationStr.end(), ::isdigit)) { Cheat::GameFunctions::PrintSubtitle("~COLOR_RED~The Custom Teleport name cannot contain any digits"); return; }

	Vector3 CurrentLocalPlayerCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), false, false);
	Cheat::CheatFunctions::WriteStringToIni(std::to_string(CurrentLocalPlayerCoords.x), ReturnCustomTeleportsIniFilePath(), Name, xorstr_("x"));
	Cheat::CheatFunctions::WriteStringToIni(std::to_string(CurrentLocalPlayerCoords.y), ReturnCustomTeleportsIniFilePath(), Name, xorstr_("y"));
	Cheat::CheatFunctions::WriteStringToIni(std::to_string(CurrentLocalPlayerCoords.z), ReturnCustomTeleportsIniFilePath(), Name, xorstr_("z"));
	Cheat::GameFunctions::PrintSubtitle("~COLOR_GREEN~Custom Teleport Location Saved");
}

void Cheat::CheatFunctions::DeleteCustomTeleport(char* Name)
{
	if (WritePrivateProfileStringA(Name, NULL, "", ReturnCustomTeleportsIniFilePath().c_str()))
	{
		std::string Message = "~COLOR_GREEN~The Custom Teleport Entry with the name " + (std::string)Name + " has been successfully removed";
		Cheat::GameFunctions::PrintSubtitle(Message.c_str());
	}
	else
	{
		Cheat::GameFunctions::PrintSubtitle("~COLOR_RED~Failed to remove Custom Teleport Location");
	}
}

void Cheat::CheatFunctions::DoCustomLocationTeleport(char* Name)
{
	std::string XCoord = Cheat::CheatFunctions::ReadStringFromIni(ReturnCustomTeleportsIniFilePath(), Name, xorstr_("x"));
	std::string YCoord = Cheat::CheatFunctions::ReadStringFromIni(ReturnCustomTeleportsIniFilePath(), Name, xorstr_("y"));
	std::string ZCoord = Cheat::CheatFunctions::ReadStringFromIni(ReturnCustomTeleportsIniFilePath(), Name, xorstr_("z"));

	try
	{
		Vector3 TPCoords;
		TPCoords.x = std::stof(XCoord);
		TPCoords.y = std::stof(YCoord);
		TPCoords.z = std::stof(ZCoord); 
		Cheat::GameFunctions::TPto(TPCoords);
	}
	catch (std::exception& ia)
	{
		Cheat::GameFunctions::PrintSubtitle("~COLOR_RED~Invalid Custom Teleport Entry - teleporting failed");
		return;
	}
}



bool Cheat::CheatFunctions::isWhitespace(std::string s) {
	for (int index = 0; index < s.length(); index++) {
		if (!std::isspace(s[index]))
			return false;
	}
	return true;
}


std::string Cheat::CheatFunctions::GetLastErrorAsString()
{
	//Get the error message, if any.
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0) { return std::string(); }

	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	std::string message(messageBuffer, size);

	//Free the buffer.
	LocalFree(messageBuffer);

	return message;
}


void Cheat::CheatFunctions::SaveConfig()
{
	Cheat::CheatFunctions::WriteBoolToIni(Cheat::CheatFeatures::GodmodeBool, ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("godmode"));
	Cheat::CheatFunctions::WriteBoolToIni(Cheat::CheatFeatures::SuperJumpBool, ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("superjump"));
	Cheat::CheatFunctions::WriteBoolToIni(Cheat::CheatFeatures::NoRagdollBool, ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("noragdoll"));
	Cheat::CheatFunctions::WriteBoolToIni(Cheat::CheatFeatures::NeverWantedBool, ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("neverwanted"));
	Cheat::CheatFunctions::WriteBoolToIni(Cheat::CheatFeatures::WeaponDamageMultiplierBool, ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("weapon_damage_multiplier"));
	Cheat::CheatFunctions::WriteBoolToIni(Cheat::CheatFeatures::HideHUDBool, ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("hide_hud"));
	Cheat::CheatFunctions::WriteBoolToIni(Cheat::CheatFeatures::InfiniteStaminaBool, ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("infiniteammo"));
	Cheat::CheatFunctions::WriteBoolToIni(Cheat::CheatFeatures::InfiniteAmmoBool, ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("infiniteammo"));
	Cheat::CheatFunctions::WriteBoolToIni(Cheat::CheatFeatures::InvisibleBool, ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("player_invisible"));
	Cheat::CheatFunctions::WriteBoolToIni(Cheat::CheatFeatures::VehicleGodmodeBool, ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("vehicle_godmode"));
	Cheat::CheatFunctions::WriteBoolToIni(Cheat::CheatFeatures::VehicleInvisibleBool, ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("vehicle_invisible"));
	Cheat::CheatFunctions::WriteBoolToIni(Cheat::CheatFeatures::AutoSaveConfigBool, ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("auto_save_config"));
	Cheat::CheatFunctions::WriteBoolToIni(Cheat::CheatFeatures::UnlimitedDeadEyeBool, ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("unlimited_dead_eye"));
	Cheat::CheatFunctions::WriteBoolToIni(Cheat::CheatFeatures::InfiniteStaminaBool, ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("infinite_stamina"));
	Cheat::CheatFunctions::WriteBoolToIni(Cheat::CheatFeatures::IgnoredByNPCBool, ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("ignored_by_npcs"));
	Cheat::CheatFunctions::WriteBoolToIni(Cheat::CheatFeatures::SuperRunBool, ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("super_run"));
	Cheat::CheatFunctions::WriteBoolToIni(Cheat::CheatFeatures::TriggerbotBool, ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("trigger_bot"));
	Cheat::CheatFunctions::WriteBoolToIni(Cheat::CheatFeatures::TriggerbotShootPlayersBool, ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("trigger_bot_shoot_players"));
	Cheat::CheatFunctions::WriteBoolToIni(Cheat::CheatFeatures::SpawnVehicleInvincibleBool, ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("spawn_vehicle_invincible"));
	Cheat::CheatFunctions::WriteBoolToIni(Cheat::CheatFeatures::SpawnPedInVehicleBool, ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("spawn_in_vehicle"));
	Cheat::CheatFunctions::WriteBoolToIni(Cheat::CheatFeatures::DeleteCurrentVehicleBool, ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("delete_current_vehicle"));
	Cheat::CheatFunctions::WriteBoolToIni(Cheat::CheatFeatures::TeleportGunBool, ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("teleport_gun"));
	Cheat::CheatFunctions::WriteBoolToIni(Cheat::CheatFeatures::ExplosiveAmmoBool, ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("explosive_ammo"));
	Cheat::CheatFunctions::WriteBoolToIni(Cheat::CheatFeatures::SpawnPedDeadBool, ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("ped_spawner_spawn_dead"));
	Cheat::CheatFunctions::WriteBoolToIni(Cheat::CheatFeatures::AutoTeleportToWaypointBool, ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("auto_teleport_to_waypoint"));
	Cheat::CheatFunctions::WriteFloatToIni(Cheat::GUI::MenuXFloat, ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("menu_x"));
	Cheat::CheatFunctions::WriteBoolToIni(Cheat::CheatFeatures::UnlimitedHorseStaminaBool, ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("unlimited_horse_stamina"));
}

void Cheat::CheatFunctions::LoadConfig(bool StartUp)
{
	if (StartUp) { Cheat::LogFunctions::Message("Loading Config"); }
	else { Cheat::LogFunctions::Message("Reloading Config"); }
	if (ReadStringFromIni(ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("godmode")) == xorstr_("true")) { Cheat::CheatFeatures::GodmodeBool = true; }
	if (ReadStringFromIni(ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("superjump")) == xorstr_("true")) { Cheat::CheatFeatures::SuperJumpBool = true; }
	if (ReadStringFromIni(ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("noragdoll")) == xorstr_("true")) { Cheat::CheatFeatures::NoRagdollBool = true; }
	if (ReadStringFromIni(ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("neverwanted")) == xorstr_("true")) { Cheat::CheatFeatures::NeverWantedBool = true; }
	if (ReadStringFromIni(ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("weapon_damage_multiplier")) == xorstr_("true")) { Cheat::CheatFeatures::WeaponDamageMultiplierBool = true; }
	if (ReadStringFromIni(ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("hide_hud")) == xorstr_("true")) { Cheat::CheatFeatures::HideHUDBool = true; }
	if (ReadStringFromIni(ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("unlimited_dead_eye")) == xorstr_("true")) { Cheat::CheatFeatures::UnlimitedDeadEyeBool = true; }
	if (ReadStringFromIni(ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("infinitestamina")) == xorstr_("true")) { Cheat::CheatFeatures::InfiniteStaminaBool = true; }
	if (ReadStringFromIni(ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("infiniteammo")) == xorstr_("true")) { Cheat::CheatFeatures::InfiniteAmmoBool = true; }
	if (ReadStringFromIni(ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("player_invisible")) == xorstr_("true")) { Cheat::CheatFeatures::InvisibleBool = true; }
	if (ReadStringFromIni(ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("vehicle_godmode")) == xorstr_("true")) { Cheat::CheatFeatures::VehicleGodmodeBool = true; }
	if (ReadStringFromIni(ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("vehicle_invisible")) == xorstr_("true")) { Cheat::CheatFeatures::VehicleInvisibleBool = true; }
	if (ReadStringFromIni(ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("infinite_stamina")) == xorstr_("true")) { Cheat::CheatFeatures::InfiniteStaminaBool = true; }
	if (ReadStringFromIni(ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("ignored_by_npcs")) == xorstr_("true")) { Cheat::CheatFeatures::IgnoredByNPCBool = true; }
	if (ReadStringFromIni(ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("super_run")) == xorstr_("true")) { Cheat::CheatFeatures::SuperRunBool = true; }
	if (ReadStringFromIni(ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("trigger_bot")) == xorstr_("true")) { Cheat::CheatFeatures::TriggerbotBool = true; }
	if (ReadStringFromIni(ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("trigger_bot_shoot_players")) == xorstr_("true")) { Cheat::CheatFeatures::TriggerbotShootPlayersBool = true; }
	if (ReadStringFromIni(ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("spawn_vehicle_invincible")) == xorstr_("true")) { Cheat::CheatFeatures::SpawnVehicleInvincibleBool = true; }
	if (ReadStringFromIni(ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("spawn_in_vehicle")) == xorstr_("true")) { Cheat::CheatFeatures::SpawnPedInVehicleBool = true; }
	if (ReadStringFromIni(ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("delete_current_vehicle")) == xorstr_("true")) { Cheat::CheatFeatures::DeleteCurrentVehicleBool = true; }
	if (ReadStringFromIni(ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("teleport_gun")) == xorstr_("true")) { Cheat::CheatFeatures::TeleportGunBool = true; }
	if (ReadStringFromIni(ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("explosive_ammo")) == xorstr_("true")) { Cheat::CheatFeatures::ExplosiveAmmoBool = true; }
	if (ReadStringFromIni(ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("ped_spawner_spawn_dead")) == xorstr_("true")) { Cheat::CheatFeatures::SpawnPedDeadBool = true; }
	if (ReadStringFromIni(ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("unlimited_horse_stamina")) == xorstr_("true")) { Cheat::CheatFeatures::UnlimitedHorseStaminaBool = true; }
	if (ReadStringFromIni(ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("auto_teleport_to_waypoint")) == xorstr_("true")) { Cheat::CheatFeatures::AutoTeleportToWaypointBool = true; }
	if (ReadStringFromIni(ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("auto_save_config")) == xorstr_("true")) { Cheat::CheatFeatures::AutoSaveConfigBool = true; }
	else { if (ReadStringFromIni(ReturnConfigFilePath().c_str(), "CHEAT", "auto_save_config") == "false") { Cheat::CheatFeatures::AutoSaveConfigBool = false; } }
	std::string MenuXSetting = Cheat::CheatFunctions::ReadStringFromIni(ReturnConfigFilePath().c_str(), xorstr_("CHEAT"), xorstr_("menu_x")); if (MenuXSetting != "" && Cheat::CheatFunctions::IsIntegerInRange(180.00, 1100.00, std::stod(MenuXSetting))) { double MenuXDouble = std::stod(MenuXSetting); Cheat::GUI::MenuXFloat = MenuXDouble; }
}


void Cheat::CheatFunctions::CheatInitialization()
{
	Cheat::LogFunctions::Message(xorstr_("Allocating Console"));
	Cheat::CheatFunctions::CreateConsoleWindow();
	Cheat::LogFunctions::Init();
	if (!GetModuleHandleA("RDR2.exe")) { Cheat::LogFunctions::Error(xorstr_("Invalid module")); ::exit(0); }
	Cheat::LogFunctions::Message(xorstr_("Creating Main Cheat Fiber"));
	Cheat::GameHooking::Init();
	if (!PLAYER::IS_PLAYER_PLAYING(PLAYER::PLAYER_ID() || DLC::GET_IS_LOADING_SCREEN_ACTIVE())) { Cheat::LogFunctions::Message(xorstr_("Waiting until the game finished loading")); }
	while (!PLAYER::IS_PLAYER_PLAYING(PLAYER::PLAYER_ID() || DLC::GET_IS_LOADING_SCREEN_ACTIVE())) { Sleep(200); }
	Cheat::CheatFunctions::LoadConfig(true);
	Cheat::Controls::changeMenu("mainmenu");
	Cheat::Controls::isMenuEnabled = true;
	Cheat::LogFunctions::Message(xorstr_("RDR2 Cheat Initialization Completed"));
}
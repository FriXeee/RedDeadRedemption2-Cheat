#pragma once
#include "stdafx.h"

std::string Cheat::CheatFunctions::ReturnCheatBuildAsString()
{
	return xorstr_("1.0.0.1");
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


void Cheat::CheatFunctions::CreateConsole()
{
	Cheat::LogFunctions::Message(xorstr_("Allocating Console"));
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
	catch (...)
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


void Cheat::CheatFunctions::PostInitCheat()
{
	//Wait until the game is done loading
	if (!PLAYER::IS_PLAYER_PLAYING(PLAYER::PLAYER_ID() || DLC::GET_IS_LOADING_SCREEN_ACTIVE())) { Cheat::LogFunctions::Message(xorstr_("Waiting until the game has finished loading")); }
	while (!PLAYER::IS_PLAYER_PLAYING(PLAYER::PLAYER_ID() || DLC::GET_IS_LOADING_SCREEN_ACTIVE())) { Cheat::GameHooking::FiberWait(0); }

	//Do post init actions
	Cheat::CheatFunctions::LoadConfig(true);
	Cheat::Controls::MoveMenu(MainMenu);
	Cheat::LogFunctions::Message(xorstr_("RDR2 Cheat Initialization Completed"));
}

std::string Cheat::CheatFunctions::VirtualKeyCodeToString(UCHAR virtualKey)
{
	UINT scanCode = MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC);

	CHAR szName[128];
	int result = 0;
	switch (virtualKey)
	{
	case VK_LEFT:		case VK_UP: case VK_RIGHT: case VK_DOWN:
	case VK_RCONTROL:	case VK_RMENU:
	case VK_LWIN:		case VK_RWIN: case VK_APPS:
	case VK_PRIOR:		case VK_NEXT:
	case VK_END:		case VK_HOME:
	case VK_INSERT:		case VK_DELETE:
	case VK_DIVIDE:
	case VK_NUMLOCK:
		scanCode |= KF_EXTENDED;
	default:
		result = GetKeyNameTextA(scanCode << 16, szName, 128);
	}
	if (result == 0) { return xorstr_("Unknown"); }
	return szName;
}


bool Cheat::CheatFunctions::ReturnPressedKey(int& PressedKey)
{
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) { PressedKey = VK_LBUTTON; return true; }
	else if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) { PressedKey = VK_RBUTTON; return true; }
	else if (GetAsyncKeyState(VK_MBUTTON) & 0x8000) { PressedKey = VK_MBUTTON;  return true; }
	else if (GetAsyncKeyState(VK_BACK) & 0x8000) { PressedKey = VK_BACK; return true; }
	else if (GetAsyncKeyState(VK_TAB) & 0x8000) { PressedKey = VK_TAB; return true; }
	else if (GetAsyncKeyState(VK_MENU) & 0x8000) { PressedKey = VK_MENU; return true; }
	else if (GetAsyncKeyState(VK_CAPITAL) & 0x8000) { PressedKey = VK_CAPITAL; return true; }
	else if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) { PressedKey = VK_ESCAPE; return true; }
	else if (GetAsyncKeyState(VK_SPACE) & 0x8000) { PressedKey = VK_SPACE; return true; }
	else if (GetAsyncKeyState(VK_PRIOR) & 0x8000) { PressedKey = VK_PRIOR; return true; }
	else if (GetAsyncKeyState(VK_NEXT) & 0x8000) { PressedKey = VK_NEXT; return true; }
	else if (GetAsyncKeyState(VK_END) & 0x8000) { PressedKey = VK_END; return true; }
	else if (GetAsyncKeyState(VK_HOME) & 0x8000) { PressedKey = VK_HOME; return true; }
	else if (GetAsyncKeyState(VK_LEFT) & 0x8000) { PressedKey = VK_LEFT; return true; }
	else if (GetAsyncKeyState(VK_UP) & 0x8000) { PressedKey = VK_UP; return true; }
	else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) { PressedKey = VK_RIGHT; return true; }
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000) { PressedKey = VK_DOWN; return true; }
	else if (GetAsyncKeyState(VK_SELECT) & 0x8000) { PressedKey = VK_SELECT; return true; }
	else if (GetAsyncKeyState(VK_PRINT) & 0x8000) { PressedKey = VK_PRINT; return true; }
	else if (GetAsyncKeyState(VK_SNAPSHOT) & 0x8000) { PressedKey = VK_SNAPSHOT; return true; }
	else if (GetAsyncKeyState(VK_INSERT) & 0x8000) { PressedKey = VK_INSERT; return true; }
	else if (GetAsyncKeyState(VK_DELETE) & 0x8000) { PressedKey = VK_DELETE; return true; }
	else if (GetAsyncKeyState(0x30) & 0x8000) { PressedKey = 0x30; return true; }
	else if (GetAsyncKeyState(0x31) & 0x8000) { PressedKey = 0x31; return true; }
	else if (GetAsyncKeyState(0x32) & 0x8000) { PressedKey = 0x32; return true; }
	else if (GetAsyncKeyState(0x32) & 0x8000) { PressedKey = 0x32; return true; }
	else if (GetAsyncKeyState(0x33) & 0x8000) { PressedKey = 0x33; return true; }
	else if (GetAsyncKeyState(0x34) & 0x8000) { PressedKey = 0x34; return true; }
	else if (GetAsyncKeyState(0x36) & 0x8000) { PressedKey = 0x36; return true; }
	else if (GetAsyncKeyState(0x37) & 0x8000) { PressedKey = 0x37; return true; }
	else if (GetAsyncKeyState(0x38) & 0x8000) { PressedKey = 0x38; return true; }
	else if (GetAsyncKeyState(0x39) & 0x8000) { PressedKey = 0x39; return true; }
	else if (GetAsyncKeyState(0x41) & 0x8000) { PressedKey = 0x41; return true; }
	else if (GetAsyncKeyState(0x42) & 0x8000) { PressedKey = 0x42; return true; }
	else if (GetAsyncKeyState(0x43) & 0x8000) { PressedKey = 0x43; return true; }
	else if (GetAsyncKeyState(0x44) & 0x8000) { PressedKey = 0x44; return true; }
	else if (GetAsyncKeyState(0x45) & 0x8000) { PressedKey = 0x45; return true; }
	else if (GetAsyncKeyState(0x46) & 0x8000) { PressedKey = 0x46; return true; }
	else if (GetAsyncKeyState(0x47) & 0x8000) { PressedKey = 0x47; return true; }
	else if (GetAsyncKeyState(0x48) & 0x8000) { PressedKey = 0x48; return true; }
	else if (GetAsyncKeyState(0x49) & 0x8000) { PressedKey = 0x49; return true; }
	else if (GetAsyncKeyState(0x4A) & 0x8000) { PressedKey = 0x4A; return true; }
	else if (GetAsyncKeyState(0x4B) & 0x8000) { PressedKey = 0x4B; return true; }
	else if (GetAsyncKeyState(0x4C) & 0x8000) { PressedKey = 0x4C; return true; }
	else if (GetAsyncKeyState(0x4D) & 0x8000) { PressedKey = 0x4D; return true; }
	else if (GetAsyncKeyState(0x4E) & 0x8000) { PressedKey = 0x4E; return true; }
	else if (GetAsyncKeyState(0x4F) & 0x8000) { PressedKey = 0x4F; return true; }
	else if (GetAsyncKeyState(0x50) & 0x8000) { PressedKey = 0x50; return true; }
	else if (GetAsyncKeyState(0x51) & 0x8000) { PressedKey = 0x51; return true; }
	else if (GetAsyncKeyState(0x52) & 0x8000) { PressedKey = 0x52; return true; }
	else if (GetAsyncKeyState(0x53) & 0x8000) { PressedKey = 0x53; return true; }
	else if (GetAsyncKeyState(0x54) & 0x8000) { PressedKey = 0x54; return true; }
	else if (GetAsyncKeyState(0x55) & 0x8000) { PressedKey = 0x55; return true; }
	else if (GetAsyncKeyState(0x56) & 0x8000) { PressedKey = 0x56; return true; }
	else if (GetAsyncKeyState(0x57) & 0x8000) { PressedKey = 0x57; return true; }
	else if (GetAsyncKeyState(0x58) & 0x8000) { PressedKey = 0x58; return true; }
	else if (GetAsyncKeyState(0x59) & 0x8000) { PressedKey = 0x59; return true; }
	else if (GetAsyncKeyState(0x5A) & 0x8000) { PressedKey = 0x5A; return true; }
	else if (GetAsyncKeyState(VK_LWIN) & 0x8000) { PressedKey = VK_LWIN; return true; }
	else if (GetAsyncKeyState(VK_RWIN) & 0x8000) { PressedKey = VK_RWIN; return true; }
	else if (GetAsyncKeyState(VK_NUMPAD0) & 0x8000) { PressedKey = VK_NUMPAD0; return true; }
	else if (GetAsyncKeyState(VK_NUMPAD1) & 0x8000) { PressedKey = VK_NUMPAD1; return true; }
	else if (GetAsyncKeyState(VK_NUMPAD2) & 0x8000) { PressedKey = VK_NUMPAD2; return true; }
	else if (GetAsyncKeyState(VK_NUMPAD3) & 0x8000) { PressedKey = VK_NUMPAD3; return true; }
	else if (GetAsyncKeyState(VK_NUMPAD4) & 0x8000) { PressedKey = VK_NUMPAD4; return true; }
	else if (GetAsyncKeyState(VK_NUMPAD6) & 0x8000) { PressedKey = VK_NUMPAD6; return true; }
	else if (GetAsyncKeyState(VK_NUMPAD7) & 0x8000) { PressedKey = VK_NUMPAD7; return true; }
	else if (GetAsyncKeyState(VK_NUMPAD8) & 0x8000) { PressedKey = VK_NUMPAD8; return true; }
	else if (GetAsyncKeyState(VK_NUMPAD9) & 0x8000) { PressedKey = VK_NUMPAD9; return true; }
	else if (GetAsyncKeyState(VK_MULTIPLY) & 0x8000) { PressedKey = VK_MULTIPLY; return true; }
	else if (GetAsyncKeyState(VK_ADD) & 0x8000) { PressedKey = VK_ADD; return true; }
	else if (GetAsyncKeyState(VK_SEPARATOR) & 0x8000) { PressedKey = VK_SEPARATOR; return true; }
	else if (GetAsyncKeyState(VK_SUBTRACT) & 0x8000) { PressedKey = VK_SUBTRACT; return true; }
	else if (GetAsyncKeyState(VK_DECIMAL) & 0x8000) { PressedKey = VK_DECIMAL; return true; }
	else if (GetAsyncKeyState(VK_DIVIDE) & 0x8000) { PressedKey = VK_DIVIDE; return true; }
	else if (GetAsyncKeyState(VK_F1) & 0x8000) { PressedKey = VK_F1; return true; }
	else if (GetAsyncKeyState(VK_F2) & 0x8000) { PressedKey = VK_F2; return true; }
	else if (GetAsyncKeyState(VK_F3) & 0x8000) { PressedKey = VK_F3; return true; }
	else if (GetAsyncKeyState(VK_F4) & 0x8000) { PressedKey = VK_F4; return true; }
	else if (GetAsyncKeyState(VK_F5) & 0x8000) { PressedKey = VK_F5; return true; }
	else if (GetAsyncKeyState(VK_F6) & 0x8000) { PressedKey = VK_F6; return true; }
	else if (GetAsyncKeyState(VK_F7) & 0x8000) { PressedKey = VK_F7; return true; }
	else if (GetAsyncKeyState(VK_F8) & 0x8000) { PressedKey = VK_F8; return true; }
	else if (GetAsyncKeyState(VK_F9) & 0x8000) { PressedKey = VK_F9; return true; }
	else if (GetAsyncKeyState(VK_F10) & 0x8000) { PressedKey = VK_F10; return true; }
	else if (GetAsyncKeyState(VK_F11) & 0x8000) { PressedKey = VK_F11; return true; }
	else if (GetAsyncKeyState(VK_F12) & 0x8000) { PressedKey = VK_F12; return true; }
	else if (GetAsyncKeyState(VK_F13) & 0x8000) { PressedKey = VK_F13; return true; }
	else if (GetAsyncKeyState(VK_F14) & 0x8000) { PressedKey = VK_F14; return true; }
	else if (GetAsyncKeyState(VK_F15) & 0x8000) { PressedKey = VK_F15; return true; }
	else if (GetAsyncKeyState(VK_F16) & 0x8000) { PressedKey = VK_F16; return true; }
	else if (GetAsyncKeyState(VK_F17) & 0x8000) { PressedKey = VK_F17; return true; }
	else if (GetAsyncKeyState(VK_F18) & 0x8000) { PressedKey = VK_F18; return true; }
	else if (GetAsyncKeyState(VK_F19) & 0x8000) { PressedKey = VK_F19; return true; }
	else if (GetAsyncKeyState(VK_F20) & 0x8000) { PressedKey = VK_F20; return true; }
	else if (GetAsyncKeyState(VK_F21) & 0x8000) { PressedKey = VK_F21; return true; }
	else if (GetAsyncKeyState(VK_F22) & 0x8000) { PressedKey = VK_F22; return true; }
	else if (GetAsyncKeyState(VK_F23) & 0x8000) { PressedKey = VK_F23; return true; }
	else if (GetAsyncKeyState(VK_F24) & 0x8000) { PressedKey = VK_F24; return true; }
	else if (GetAsyncKeyState(VK_NUMLOCK) & 0x8000) { PressedKey = VK_NUMLOCK; return true; }
	else if (GetAsyncKeyState(VK_SCROLL) & 0x8000) { PressedKey = VK_SCROLL; return true; }
	else if (GetAsyncKeyState(VK_LSHIFT) & 0x8000) { PressedKey = VK_LSHIFT; return true; }
	else if (GetAsyncKeyState(VK_RSHIFT) & 0x8000) { PressedKey = VK_RSHIFT; return true; }
	else if (GetAsyncKeyState(VK_LCONTROL) & 0x8000) { PressedKey = VK_LCONTROL; return true; }
	else if (GetAsyncKeyState(VK_OEM_CLEAR) & 0x8000) { PressedKey = VK_OEM_CLEAR; return true; }
	return false;
}
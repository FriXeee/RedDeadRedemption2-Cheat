#include "stdafx.h"
int WorldTimeHour;
int WorldTimeMinutes;
int WorldTimeSeconds;;


void Cheat::Main()
{
	Cheat::CheatFunctions::PostInitCheat();

	while (true) 
	{
		Cheat::CheatFunctions::LoopedFunctions();
	
		switch (Cheat::GUI::currentMenu){
		case MainMenu:
		{
			GUI::Title("RDR2 Cheat");
			GUI::MenuOption("Self Options >", SelfMenu);
			GUI::MenuOption("Network Options >", NetworkingOptionsMenu);
			GUI::MenuOption("Vehicle Options >", VehicleOptionsMenu);
			GUI::MenuOption("Horse Options >", HorseOptionsMenu);
			GUI::MenuOption("Weapon Options >", WeaponOptionsMenu);
			GUI::MenuOption("Teleport Options >", TeleportOptionsMenu);
			GUI::MenuOption("World Options >", WorldOptionsMenu);
			GUI::MenuOption("Misc Options >", MiscOptionsMenu);
			GUI::MenuOption("Settings >", CheatSettingsMenu);
		}
		break;
		case SelfMenu:
		{
			GUI::Title("Self Options");
			GUI::MenuOption("Model Changer >", ModelChangerMenu);
			GUI::BoolOption("Godmode", &Cheat::CheatFeatures::GodmodeBool, "Makes your character invincible");
			GUI::BoolOption("No Ragdoll", &Cheat::CheatFeatures::NoRagdollBool, "Disables ragdoll for your character model");
			GUI::BoolOption("Super Jump", &Cheat::CheatFeatures::SuperJumpBool, "Your character will jump very high");
			GUI::BoolOption("Never Wanted", &Cheat::CheatFeatures::NeverWantedBool, "Never get a wanted level");
			GUI::BoolOption("Ignored by NPC's", &Cheat::CheatFeatures::IgnoredByNPCBool, "NPC's will (mostly) ignore you");
			GUI::BoolOption("Super Run", &Cheat::CheatFeatures::SuperRunBool, "Run very fast");
			GUI::BoolOption("Invisible", &Cheat::CheatFeatures::InvisibleBool, "Makes your character model invisible");
			GUI::BoolOption("Unlimited Dead Eye", &Cheat::CheatFeatures::UnlimitedDeadEyeBool, "");
			GUI::BoolOption("Infinite Stamina", &Cheat::CheatFeatures::InfiniteStaminaBool, "");
			if (GUI::Option("Clean Player", "Remove any damage from your character")) { PED::CLEAR_PED_BLOOD_DAMAGE(Cheat::GameFunctions::PlayerPedID); }
			if (GUI::Option("Suicide", "Kill your player")) { ENTITY::_SET_ENTITY_HEALTH(Cheat::GameFunctions::PlayerPedID, 0, 0); }
		}
		break;
		case ModelChangerMenu:
		{
			GUI::Title("Model Changer");
			if (GUI::Option("Custom Input", "Enter custom ped name"))
			{
				MISC::DISPLAY_ONSCREEN_KEYBOARD(true, "", "", "", "", "", "", 50);
				while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) Cheat::GameHooking::FiberWait(0, false);
				const char* ModelChangerCustomInput = MISC::GET_ONSCREEN_KEYBOARD_RESULT();
				if (MISC::GET_ONSCREEN_KEYBOARD_RESULT())
				{
					Hash model = MISC::GET_HASH_KEY(ModelChangerCustomInput);
					if (!STREAMING::IS_MODEL_A_PED(model))
					{
						Cheat::GameFunctions::PrintSubtitle("~COLOR_RED~That is not a valid Ped Model");
					}
					else
					{
						STREAMING::REQUEST_MODEL(model, 0);
						while (!STREAMING::HAS_MODEL_LOADED(model)) { Cheat::GameHooking::FiberWait(0); }
						PLAYER::SET_PLAYER_MODEL(Cheat::GameFunctions::PlayerID, model, true);
						PED::SET_PED_RANDOM_COMPONENT_VARIATION(Cheat::GameFunctions::PlayerPedID, false);
						PED::_SET_RANDOM_OUTFIT_VARIATION(Cheat::GameFunctions::PlayerPedID, true);
						ENTITY::_SET_ENTITY_HEALTH(Cheat::GameFunctions::PlayerPedID, PED::GET_PED_MAX_HEALTH(Cheat::GameFunctions::PlayerPedID), 0);
						STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
					}
				}
			}
			for (auto const& i : Cheat::GameArrays::PedModels)
			{
				if (GUI::Option(i.c_str(), ""))
				{
					Hash model = MISC::GET_HASH_KEY(i.c_str());
					if (STREAMING::IS_MODEL_A_PED(model)) {
						STREAMING::REQUEST_MODEL(model, 0);
						while (!STREAMING::HAS_MODEL_LOADED(model)) { Cheat::GameHooking::FiberWait(0); }
						PLAYER::SET_PLAYER_MODEL(Cheat::GameFunctions::PlayerID, model, true);
						PED::SET_PED_RANDOM_COMPONENT_VARIATION(Cheat::GameFunctions::PlayerPedID, false);
						PED::_SET_RANDOM_OUTFIT_VARIATION(Cheat::GameFunctions::PlayerPedID, true);
						ENTITY::_SET_ENTITY_HEALTH(Cheat::GameFunctions::PlayerPedID, PED::GET_PED_MAX_HEALTH(Cheat::GameFunctions::PlayerPedID), 0);
						STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
					}
				}
			}
		}
		break;
		case NetworkingOptionsMenu:
		{
			GUI::Title("Network Options");
			GUI::MenuOption("Player List >", PlayerlistMenu);
		}
		break;
		case PlayerlistMenu:
		{
			GUI::Title("Player List");
			for (int i = 0; i < 32; ++i) 
			{
				std::string PlayernameString = PLAYER::GET_PLAYER_NAME(i);
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i))) 
				{
					if (Cheat::GameFunctions::PlayerID == i) { PlayernameString.append(" ~COLOR_GREEN~[You]"); }

		
					GUI::MenuOption(PlayernameString, SelectedPlayerMenu) ? Cheat::CheatFeatures::SelectedPlayer = i : NULL;
				}
			}
		}
		break;
		case SelectedPlayerMenu:
		{
			GUI::Title(PLAYER::GET_PLAYER_NAME(Cheat::CheatFeatures::SelectedPlayer));
			if (GUI::BoolOption("Spectate", &Cheat::CheatFeatures::SpectateSelectedPlayerBool, "Spectate selected player"));
			if (GUI::Option("Teleport To", "Teleport to selected player coords")) 
			{
				Cheat::GameFunctions::TeleportToCoords(Cheat::GameFunctions::PlayerPedID, 
													   ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(Cheat::CheatFeatures::SelectedPlayer), false, false), 
													   false, false);
			}
			GUI::MenuOption("Troll Options >", SelectedPlayerTrollMenu);
			GUI::MenuOption("Spawn Options >", SelectedPlayerSpawnOptionsMenu);
		}
		break;
		case SelectedPlayerTrollMenu:
		{
			GUI::Title("Troll Options");
			GUI::BoolOption("Freeze", &Cheat::CheatFeatures::FreezeSelectedPlayerBool, "Freeze selected player LOL");
			if (GUI::Option("Clone", "Clone selected player ped")) { PED::CLONE_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(Cheat::CheatFeatures::SelectedPlayer), ENTITY::GET_ENTITY_HEADING(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(Cheat::CheatFeatures::SelectedPlayer)), 1, 1); }
			if (GUI::Option("Attach to player", "")) { if (PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(Cheat::CheatFeatures::SelectedPlayer) != Cheat::GameFunctions::PlayerPedID) { ENTITY::ATTACH_ENTITY_TO_ENTITY(Cheat::GameFunctions::PlayerPedID, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(Cheat::CheatFeatures::SelectedPlayer), 0, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, true, true, false, true, 2, true, NULL, NULL); } }
			if (GUI::Option("Detach from player", "")) { ENTITY::DETACH_ENTITY(Cheat::GameFunctions::PlayerPedID, true, true); }
			if (GUI::Option("Explode Player", "")) { Vector3 coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(Cheat::CheatFeatures::SelectedPlayer), false, false); FIRE::ADD_EXPLOSION(coords.x, coords.y, coords.z, 22, 1000.f, true, false, false); }
		}
		break;
		case SelectedPlayerSpawnOptionsMenu:
		{
			GUI::Title("Spawn Options");
			GUI::MenuOption("Ped Spawner >", SelectedPlayerPedSpawnerMenu);
		}
		break;
		case SelectedPlayerPedSpawnerMenu:
		{
			GUI::Title("Ped Spawner");
			GUI::BoolOption("Spawn Ped Dead", &Cheat::CheatFeatures::SpawnPedDeadBool, "Usefull for selling");
			if (GUI::Option("Custom Input", "")) { MISC::DISPLAY_ONSCREEN_KEYBOARD(true, "", "", "", "", "", "", 50); while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) Cheat::GameHooking::FiberWait(0, false); 
			const char* SpawnPed = MISC::GET_ONSCREEN_KEYBOARD_RESULT(); if (MISC::GET_ONSCREEN_KEYBOARD_RESULT()) { Cheat::GameFunctions::SpawnPed(SpawnPed, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(Cheat::CheatFeatures::SelectedPlayer)); } }
			for (auto const& i : Cheat::GameArrays::PedModels) { if (GUI::Option(i.c_str(), "")) { Cheat::GameFunctions::SpawnPed(i.c_str(), PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(Cheat::CheatFeatures::SelectedPlayer)); } }
		}
		break;
		case HorseOptionsMenu:
		{
			GUI::Title("Horse Options");
			if (GUI::Option("Delete Current Horse", "")) { Cheat::GameFunctions::DeleteCurrentMount(); }
			GUI::BoolOption("Horse Godmode", &Cheat::CheatFeatures::HorseGodmodeBool, "Makes your current horse invincible");
			GUI::BoolOption("Horse Invisible", &Cheat::CheatFeatures::HorseInvisibleBool, "Makes your current horse invisible");
			GUI::BoolOption("Unlimited Horse Stamina", &Cheat::CheatFeatures::UnlimitedHorseStaminaBool, "Your horse will always have 100% stamina");
		}
		break;
		case VehicleOptionsMenu:
		{
			GUI::Title("Vehicle Options");
			GUI::MenuOption("Vehicle Spawner >", VehicleSpawnerMenu);
			if (GUI::Option("Delete Current Vehicle", "")) { Cheat::GameFunctions::DeleteCurrentVehicle(); }
			GUI::BoolOption("Vehicle Godmode", &Cheat::CheatFeatures::VehicleGodmodeBool, "Makes your vehicle invincible");
			GUI::BoolOption("Vehicle Invisible", &Cheat::CheatFeatures::VehicleInvisibleBool, "Makes your vehicle invisible");
		}
		break;
		case VehicleSpawnerMenu:
		{
			GUI::Title("Vehicle Spawner");
			GUI::MenuOption("Spawn Settings >", VehicleSpawnSettingsMenu);
			if (GUI::Option("Custom Input", ""))
			{
				MISC::DISPLAY_ONSCREEN_KEYBOARD(true, "", "", "", "", "", "", 50);
				while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) Cheat::GameHooking::FiberWait(0, false);
				const char* SpawnVehicle = MISC::GET_ONSCREEN_KEYBOARD_RESULT();
				if (MISC::GET_ONSCREEN_KEYBOARD_RESULT()) { Cheat::GameFunctions::SpawnVehicle(SpawnVehicle); }
			}
			for (auto const& i : Cheat::GameArrays::VehicleModels)
			{
				if (GUI::Option(i.c_str(), ""))
				{
					Cheat::GameFunctions::SpawnVehicle(i.c_str());
				}
			}
		}
		break;
		case VehicleSpawnSettingsMenu:
		{
			GUI::Title("Vehicle Spawn Settings");
			GUI::BoolOption("Spawn Invincible", &Cheat::CheatFeatures::SpawnVehicleInvincibleBool, "");
			GUI::BoolOption("Spawn In Vehicle", &Cheat::CheatFeatures::SpawnPedInVehicleBool, "Player gets placed in vehicle");
			GUI::BoolOption("Delete Current Vehicle", &Cheat::CheatFeatures::DeleteCurrentVehicleBool, "");
		}
		break;
		case WeaponOptionsMenu:
		{
			GUI::Title("Weapon Options");
			if (GUI::Option("Get All Weapons", "")) { Cheat::GameFunctions::GiveAllWeapons(Cheat::GameFunctions::PlayerPedID); }
			if (GUI::Option("Remove All Weapons", "")) { WEAPON::REMOVE_ALL_PED_WEAPONS(Cheat::GameFunctions::PlayerPedID, true, true); }
			GUI::MenuOption("Triggerbot >", TriggerBotMenu);
			GUI::BoolOption("Infinite Ammo & No Reload", &Cheat::CheatFeatures::InfiniteAmmoBool, "Never run out of bullets & never reload again!");
			GUI::BoolOption("Weapon Damage Multiplier", &Cheat::CheatFeatures::WeaponDamageMultiplierBool, "Multiplies bullet damage");
			GUI::BoolOption("Teleport Gun", &Cheat::CheatFeatures::TeleportGunBool, "Teleport to where you shoot");
			GUI::BoolOption("Explosive Ammo", &Cheat::CheatFeatures::ExplosiveAmmoBool, "Explosion at the location you shoot");
		}
		break;
		case TriggerBotMenu:
		{
			GUI::Title("Triggerbot");
			GUI::BoolOption("Toggle", &Cheat::CheatFeatures::TriggerbotBool, "Toggles triggerbot on/off");
			GUI::BoolOption("Shoot Players", &Cheat::CheatFeatures::TriggerbotShootPlayersBool, "When disabled only shoots NPC's");
		}
		break;
		case TeleportOptionsMenu:
		{
			GUI::Title("Teleport Options");
			if (GUI::Option("Teleport to Waypoint", "")) { Cheat::GameFunctions::TeleportToWaypoint(); }
			if (GUI::Option("Teleport Forward (2m)", ""))
			{
				Vector3 Coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(Cheat::GameFunctions::PlayerPedID, 0.0, 2.0, 0.0);
				int Handle = Cheat::GameFunctions::PlayerPedID;
				if (PED::IS_PED_IN_ANY_VEHICLE(Handle, 0))
				{
					Handle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0);
				}
				else if (PED::IS_PED_ON_MOUNT(Handle))
				{
					Handle = PED::GET_MOUNT(PLAYER::PLAYER_PED_ID());
				}

				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(Handle, Coords.x, Coords.y, Coords.z, 0, 0, 1);
			}
			GUI::MenuOption("Common Locations >", CommonTeleportsMenu);
			GUI::MenuOption("Custom Teleports >", CustomTeleportLocationsMenu);
		}
		break;
		case CommonTeleportsMenu:
		{
			GUI::Title("Common Locations");
			if (GUI::Option("Emerald Ranch Fence", ""))
			{
				Vector3 Coords;
				Coords.x = 1417.818; Coords.y = 268.0298; Coords.z = 89.61942;
				Cheat::GameFunctions::TeleportToCoords(Cheat::GameFunctions::PlayerPedID, Coords, false, false);
			}
			if (GUI::Option("UFO Cult House", ""))
			{
				Vector3 Coords;
				Coords.x = 1469.529; Coords.y = 803.9516; Coords.z = 100.2565;
				Cheat::GameFunctions::TeleportToCoords(Cheat::GameFunctions::PlayerPedID, Coords, false, false);
			}
			if (GUI::Option("Rhodes", ""))
			{
				Vector3 Coords;
				Coords.x = 1232.205; Coords.y = -1251.088; Coords.z = 73.67763;
				Cheat::GameFunctions::TeleportToCoords(Cheat::GameFunctions::PlayerPedID, Coords, false, false);
			}
			if (GUI::Option("Rhodes Sheriff Station", ""))
			{
				Vector3 Coords;
				Coords.x = 1359.575; Coords.y = -1301.451; Coords.z = 77.76775;
				Cheat::GameFunctions::TeleportToCoords(Cheat::GameFunctions::PlayerPedID, Coords, false, false);
			}
			if (GUI::Option("Braithwaite Manor", ""))
			{
				Vector3 Coords;
				Coords.x = 1010.883; Coords.y = -1741.42; Coords.z = 46.57271;
				Cheat::GameFunctions::TeleportToCoords(Cheat::GameFunctions::PlayerPedID, Coords, false, false);
			}
			if (GUI::Option("Caliga Hall", ""))
			{
				Vector3 Coords;
				Coords.x = 1748.701; Coords.y = -1373.015; Coords.z = 44.07471;
				Cheat::GameFunctions::TeleportToCoords(Cheat::GameFunctions::PlayerPedID, Coords, false, false);
			}
			if (GUI::Option("Saint Denis Entrance", ""))
			{
				Vector3 Coords;
				Coords.x = 2209.557; Coords.y = -1346.319; Coords.z = 45.27962;
				Cheat::GameFunctions::TeleportToCoords(Cheat::GameFunctions::PlayerPedID, Coords, false, false);
			}
			if (GUI::Option("Saint Denis Police Dept. HQ", ""))
			{
				Vector3 Coords;
				Coords.x = 2519.439; Coords.y = -1309.522; Coords.z = 48.98456;
				Cheat::GameFunctions::TeleportToCoords(Cheat::GameFunctions::PlayerPedID, Coords, false, false);
			}
			if (GUI::Option("Cornwall Freight", ""))
			{
				Vector3 Coords;
				Coords.x = 2327.491; Coords.y = -1502.421; Coords.z = 46.15683;
				Cheat::GameFunctions::TeleportToCoords(Cheat::GameFunctions::PlayerPedID, Coords, false, false);
			}
			if (GUI::Option("Saint Denis Paperboy", ""))
			{
				Vector3 Coords;
				Coords.x = 2683.755; Coords.y = -1401.467; Coords.z = 46.36121;
				Cheat::GameFunctions::TeleportToCoords(Cheat::GameFunctions::PlayerPedID, Coords, false, false);
			}
			if (GUI::Option("Saint Denis Post Office", ""))
			{
				Vector3 Coords;
				Coords.x = 2747.493; Coords.y = -1403.775; Coords.z = 46.19331;
				Cheat::GameFunctions::TeleportToCoords(Cheat::GameFunctions::PlayerPedID, Coords, false, false);
			}
			if (GUI::Option("Schiffer Brewery", ""))
			{
				Vector3 Coords;
				Coords.x = 2626.123; Coords.y = -1219.239; Coords.z = 53.24075;
				Cheat::GameFunctions::TeleportToCoords(Cheat::GameFunctions::PlayerPedID, Coords, false, false);
			}
			if (GUI::Option("Holdern Barber", ""))
			{
				Vector3 Coords;
				Coords.x = 2663.265; Coords.y = -1182.402; Coords.z = 53.18868;
				Cheat::GameFunctions::TeleportToCoords(Cheat::GameFunctions::PlayerPedID, Coords, false, false);
			}
			if (GUI::Option("Theatre Raleur", ""))
			{
				Vector3 Coords;
				Coords.x = 2531.462; Coords.y = -1271.88; Coords.z = 49.19459;
				Cheat::GameFunctions::TeleportToCoords(Cheat::GameFunctions::PlayerPedID, Coords, false, false);
			}
			if (GUI::Option("Park Gazebo (Dominos)", ""))
			{
				Vector3 Coords;
				Coords.x = 2520.084; Coords.y = -1250.082; Coords.z = 50.04138;
				Cheat::GameFunctions::TeleportToCoords(Cheat::GameFunctions::PlayerPedID, Coords, false, false);
			}
			if (GUI::Option("Milliners (Clothes)", ""))
			{
				Vector3 Coords;
				Coords.x = 2552.014; Coords.y = -1176.577; Coords.z = 53.31112;
				Cheat::GameFunctions::TeleportToCoords(Cheat::GameFunctions::PlayerPedID, Coords, false, false);
			}
			if (GUI::Option("Dr. Joseph R Barnes MD (Doctor)", ""))
			{
				Vector3 Coords;
				Coords.x = 2725.046; Coords.y = -1240.167; Coords.z = 49.9267;
				Cheat::GameFunctions::TeleportToCoords(Cheat::GameFunctions::PlayerPedID, Coords, false, false);
			}
			if (GUI::Option("Kuo Chao and Co (Guns)", ""))
			{
				Vector3 Coords;
				Coords.x = 2724.051; Coords.y = -1283.323; Coords.z = 49.40885;
				Cheat::GameFunctions::TeleportToCoords(Cheat::GameFunctions::PlayerPedID, Coords, false, false);
			}
			if (GUI::Option("Horner & Co General Store", ""))
			{
				Vector3 Coords;
				Coords.x = 2819.6; Coords.y = -1311.311; Coords.z = 46.73164;
				Cheat::GameFunctions::TeleportToCoords(Cheat::GameFunctions::PlayerPedID, Coords, false, false);
			}
			if (GUI::Option("Saint Denis Trapper", ""))
			{
				Vector3 Coords;
				Coords.x = 2831.333; Coords.y = -1227.615; Coords.z = 47.64871;
				Cheat::GameFunctions::TeleportToCoords(Cheat::GameFunctions::PlayerPedID, Coords, false, false);
			}
			if (GUI::Option("Saint Denis Fence", ""))
			{
				Vector3 Coords;
				Coords.x = 2849.29; Coords.y = -1203.049; Coords.z = 47.69022;
				Cheat::GameFunctions::TeleportToCoords(Cheat::GameFunctions::PlayerPedID, Coords, false, false);
			}
		}
		break;
		case CustomTeleportLocationsMenu:
		{
			GUI::Title("Custom Teleports");
			if (GUI::Option("Create New Custom Teleport", "Uses current player ped coordinates"))
			{
				MISC::DISPLAY_ONSCREEN_KEYBOARD(true, "", "", "", "", "", "", 50);
				while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) Cheat::GameHooking::FiberWait(0, false);
				const char* CustomTeleportName = MISC::GET_ONSCREEN_KEYBOARD_RESULT();
				if (MISC::GET_ONSCREEN_KEYBOARD_RESULT())
				{
					Cheat::CheatFunctions::SaveCustomTeleport((char*)CustomTeleportName);
				}
			}
			if (GUI::Option("Delete Custom Teleport", "Delete Custom Teleport by name"))
			{
				MISC::DISPLAY_ONSCREEN_KEYBOARD(true, "", "", "", "", "", "", 20);
				while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) Cheat::GameHooking::FiberWait(0, false);
				const char* CustomTeleportName = MISC::GET_ONSCREEN_KEYBOARD_RESULT();
				if (MISC::GET_ONSCREEN_KEYBOARD_RESULT())
				{
					Cheat::CheatFunctions::DeleteCustomTeleport((char*)CustomTeleportName);
				}
			}

			std::vector<std::string> ListOfIniSectionNames;
			char CustomTeleportsBuffer[1024] = { 0 };
			GetPrivateProfileSectionNamesA(CustomTeleportsBuffer, 1024, Cheat::CheatFunctions::ReturnCustomTeleportsIniFilePath().c_str());

			char* p = (char*)CustomTeleportsBuffer;
			size_t length = 0;
			int nCnt = 0;
			while (*p)
			{
				length = strlen(p);
				ListOfIniSectionNames.push_back(p);
				p += length;
				p++;
			}

			if (!ListOfIniSectionNames.empty())
			{
				for (int i = 0; i < ListOfIniSectionNames.size(); ++i)
				{
					if (ListOfIniSectionNames[i].length() <= 50)
					{
						if (GUI::Option(ListOfIniSectionNames[i].c_str(), "Select to teleport"))
						{
							Cheat::CheatFunctions::DoCustomLocationTeleport((char*)ListOfIniSectionNames[i].c_str());
						}
					}
				}
			}
			else
			{
				GUI::Option("No Custom Teleport Locations Available", "Created Custom Teleports will appear here");
			}
		}
		break;
		case WorldOptionsMenu:
		{
			GUI::Title("World Options");
			GUI::MenuOption("Ped Spawner >", PedSpawnerMenu);
			GUI::MenuOption("Time Options >", TimeSettingsMenu);
			GUI::MenuOption("Weather Options >", WeatherOptionsMenu);
		}
		break;
		case WeatherOptionsMenu:
		{
			GUI::Title("Weather Options");
			if (GUI::Option("Sunny", "")) { MISC::_SET_WEATHER_TYPE(0x614A1F91, true, true, true, 0, true); }
			if (GUI::Option("Overcast", "")) { MISC::_SET_WEATHER_TYPE(0xBB898D2D, true, true, true, 0, true); }
			if (GUI::Option("Blizzard", "")) { MISC::_SET_WEATHER_TYPE(0x27EA2814, true, true, true, 0, true); }
			if (GUI::Option("Clouds", "")) { MISC::_SET_WEATHER_TYPE(0x30FDAF5C, true, true, true, 0, true); }
			if (GUI::Option("Drizzle", "")) { MISC::_SET_WEATHER_TYPE(0x995C7F44, true, true, true, 0, true); }
			if (GUI::Option("Fog", "")) { MISC::_SET_WEATHER_TYPE(0xD61BDE01, true, true, true, 0, true); }
			if (GUI::Option("Ground Blizzard", "")) { MISC::_SET_WEATHER_TYPE(0x7F622122, true, true, true, 0, true); }
			if (GUI::Option("Hail", "")) { MISC::_SET_WEATHER_TYPE(0x75A9E268, true, true, true, 0, true); }
			if (GUI::Option("High Pressure", "")) { MISC::_SET_WEATHER_TYPE(0xF5A87B65, true, true, true, 0, true); }
			if (GUI::Option("Hurricane", "")) { MISC::_SET_WEATHER_TYPE(0x320D0951, true, true, true, 0, true); }
			if (GUI::Option("Misty", "")) { MISC::_SET_WEATHER_TYPE(0x5974E8E5, true, true, true, 0, true); }
			if (GUI::Option("Overcast Dark", "")) { MISC::_SET_WEATHER_TYPE(0x19D4F1D9, true, true, true, 0, true); }
			if (GUI::Option("Rain", "")) { MISC::_SET_WEATHER_TYPE(0x54A69840, true, true, true, 0, true); }
			if (GUI::Option("Sandstorm", "")) { MISC::_SET_WEATHER_TYPE(0xB17F6111, true, true, true, 0, true); }
			if (GUI::Option("Shower", "")) { MISC::_SET_WEATHER_TYPE(0xE72679D5, true, true, true, 0, true); }
			if (GUI::Option("Sleet", "")) { MISC::_SET_WEATHER_TYPE(0xCA71D7C, true, true, true, 0, true); }
			if (GUI::Option("Snow", "")) { MISC::_SET_WEATHER_TYPE(0xEFB6EFF6, true, true, true, 0, true); }
			if (GUI::Option("Snow Clearing", "")) { MISC::_SET_WEATHER_TYPE(0x641DFC11, true, true, true, 0, true); }
			if (GUI::Option("Snow Light", "")) { MISC::_SET_WEATHER_TYPE(0x23FB812B, true, true, true, 0, true); }
			if (GUI::Option("Sunny", "")) { MISC::_SET_WEATHER_TYPE(0x614A1F91, true, true, true, 0, true); }
			if (GUI::Option("Thunder", "")) { MISC::_SET_WEATHER_TYPE(0xB677829F, true, true, true, 0, true); }
			if (GUI::Option("Thunderstorm", "")) { MISC::_SET_WEATHER_TYPE(0x7C1C4A13, true, true, true, 0, true); }
			if (GUI::Option("White Out", "")) { MISC::_SET_WEATHER_TYPE(0x2B402288, true, true, true, 0, true); }
		}
		break;
		case TimeSettingsMenu:
		{
			GUI::Title("Time Options");
			GUI::IntOption("Hours", &WorldTimeHour, 0, 23, 1, "");
			GUI::IntOption("Minutes", &WorldTimeMinutes, 0, 59, 1, "");
			GUI::IntOption("Seconds", &WorldTimeSeconds, 0, 59, 1, "");
			if (GUI::Option("Set Time", "")) { NETWORK::_NETWORK_CLOCK_TIME_OVERRIDE(WorldTimeHour, WorldTimeMinutes, WorldTimeSeconds, 0, false); }
		}
		break;
		case PedSpawnerMenu:
		{
			GUI::Title("Ped Spawner");
			GUI::BoolOption("Spawn Ped Dead", &Cheat::CheatFeatures::SpawnPedDeadBool, "Usefull for selling");
			if (GUI::Option("Custom Input", ""))
			{
				MISC::DISPLAY_ONSCREEN_KEYBOARD(true, "", "", "", "", "", "", 50);
				while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) Cheat::GameHooking::FiberWait(0, false);
				const char* SpawnPed = MISC::GET_ONSCREEN_KEYBOARD_RESULT();
				if (MISC::GET_ONSCREEN_KEYBOARD_RESULT()) { Cheat::GameFunctions::SpawnPed(SpawnPed, Cheat::GameFunctions::PlayerPedID); }
			}
			for (auto const& i : Cheat::GameArrays::PedModels)
			{
				if (GUI::Option(i.c_str(), ""))
				{
					Cheat::GameFunctions::SpawnPed(i.c_str(), Cheat::GameFunctions::PlayerPedID);
				}
			}
		}
		break;
		case MiscOptionsMenu:
		{
			GUI::Title("Misc Options");
			GUI::MenuOption("HUD Options >", HudOptionsMenu);
			GUI::BoolOption("No-Clip", &Cheat::CheatFeatures::NoClipBool, "");
			GUI::BoolOption("Auto Teleport To Waypoint", &Cheat::CheatFeatures::AutoTeleportToWaypointBool, "Auto teleport when setting waypoint");
		}
		break;
		case HudOptionsMenu:
		{
			GUI::Title("HUD Options");
			GUI::BoolOption("Hide HUD", &Cheat::CheatFeatures::HideHUDBool, "Hide all HUD elements");
		}
		break;
		case CheatSettingsMenu:
		{
			GUI::Title("Cheat Settings");
			GUI::MenuOption("GUI Settings >", GUISettingsMenu);
		}
		break;
		case GUISettingsMenu:
		{
			GUI::Title("GUI Settings");
			GUI::IntOption("Max Visible Menu Options", &Cheat::GUI::maxVisOptions, 5, 16, 1, "Set Max Visible Menu Options");
			GUI::BoolOption("Restore To Previous Submenu", &Cheat::GUI::RestorePreviousSubmenu, "When opening restores previous submenu");
			GUI::FloatOption("X-Axis", &Cheat::GUI::guiX, 180.f, 1100.f, 10, "Changes GUI X-Axis Position");
			std::string OpenKeyString = "Open Key: " + Cheat::CheatFunctions::VirtualKeyCodeToString(Cheat::GUI::openKey);
			if (GUI::Option(OpenKeyString.c_str(), "Select to change"))
			{
				int PressedKey;
				while (!Cheat::CheatFunctions::ReturnPressedKey(PressedKey)) { Cheat::GUI::drawText("Press any key to set Open Key, press Escape to cancel", 150.f, 625.f, 40.f, { 255, 255, 255, 255 }, true); Cheat::GameHooking::FiberWait(0); }
				if (PressedKey == 27) { Cheat::GameFunctions::PrintSubtitle("Canceled Setting Open Key"); break; }
				if (PressedKey != 0) { Cheat::GUI::openKey = PressedKey; Cheat::GameFunctions::PrintSubtitle("Open Key has been set"); }
			}
		}
		break;
		}
		GUI::End();
		Cheat::GameHooking::FiberWait(0, false);
	}
}

DWORD WINAPI InitThread(LPVOID lpParam)
{
	Cheat::CheatFunctions::CreateConsole();
	std::cout << Cheat::CheatFunctions::ReturnDateAndTimeAsString() << " [Init] " << "Initializing RDR2 Cheat" << " | Github Repository: https://github.com/HowYouDoinMate/RedDeadRedemption2-Cheat" << std::endl;
	Cheat::GameHooking::Init();
	//Hook created - this thread is no longer needed
	return 0;
}

HINSTANCE ModuleHModule;
BOOL __stdcall DllMain(HINSTANCE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		ModuleHModule = hModule;
		//Create 'rdr2' folder
		if (!Cheat::CheatFunctions::DoesDirectoryExists(Cheat::CheatFunctions::ReturnCheatModuleDirectoryPath() + (std::string)"\\rdr2")) { Cheat::CheatFunctions::CreateNewDirectory(Cheat::CheatFunctions::ReturnCheatModuleDirectoryPath() + (std::string)"\\rdr2"); }
		//Continue cheat loading
		CreateThread(NULL, NULL, InitThread, hModule, NULL, NULL);
		break;
	}
	return TRUE;
}

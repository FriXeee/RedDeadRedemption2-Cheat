#pragma once
#include "stdafx.h"


void Cheat::GameFunctions::GetCameraDirection(float* dirX, float* dirY, float* dirZ)
{
	float tX, tZ, num;
	Vector3 rot = CAM::GET_GAMEPLAY_CAM_ROT(0);

	tZ = rot.z * 0.0174532924f;
	tX = rot.x * 0.0174532924f;
	num = abs(cos(tX));

	*dirX = (-sin(tZ)) * num;
	*dirY = (cos(tZ)) * num;
	*dirZ = sin(tX);
}


void Cheat::GameFunctions::GiveAllWeapons(Ped Player)
{
	for (auto const& i : Cheat::GameArrays::WeaponHashes)
	{
		WEAPON::_GIVE_WEAPON_TO_PED(Player, MISC::GET_HASH_KEY(i.c_str()), 9999, true, true, false, 0.0);
	}
}

Vector3 TPCoords;
void Cheat::GameFunctions::TPto(Vector3 Coords)
{
	int Handle = PLAYER::PLAYER_PED_ID();
	if (PED::IS_PED_IN_ANY_VEHICLE(Handle, 0))
	{
		ENTITY::SET_ENTITY_COORDS(PED::GET_VEHICLE_PED_IS_IN(Handle, false), Coords.x, Coords.y, Coords.z, 0, 0, 0, 1);
	}
	else  
	{
		if (PED::IS_PED_ON_MOUNT(Handle)) {
			ENTITY::SET_ENTITY_COORDS(PED::GET_MOUNT(Handle), Coords.x, Coords.y, Coords.z, 0, 0, 0, 1);
		}
		else
		{
			ENTITY::SET_ENTITY_COORDS(Handle, Coords.x, Coords.y, Coords.z, 0, 0, 0, 1);
		}
	}
}

void Cheat::GameFunctions::RequestControlOfId(Entity netid)
{
	int Tick = 0;
	while (!NETWORK::NETWORK_HAS_CONTROL_OF_NETWORK_ID(netid) && Tick <= 25)
	{
		NETWORK::NETWORK_REQUEST_CONTROL_OF_NETWORK_ID(netid);
		Tick++;
	}
}

void Cheat::GameFunctions::RequestControlOfEnt(Entity entity)
{
	int Tick = 0;
	while (!NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(entity) && Tick <= 25)
	{
		NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(entity);
		Tick++;
	}
	if (NETWORK::NETWORK_IS_SESSION_STARTED())
	{
		int netID = NETWORK::NETWORK_GET_NETWORK_ID_FROM_ENTITY(entity);
		RequestControlOfId(netID);
	}
}

void Cheat::GameFunctions::PrintSubtitle(const char* Text)
{
	UILOG::_LOG_SET_CACHED_OBJECTIVE(MISC::_CREATE_VAR_STRING(10, "LITERAL_STRING", Text));
	UILOG::_LOG_PRINT_CACHED_OBJECTIVE();
	UILOG::_LOG_CLEAR_CACHED_OBJECTIVE();
}


void Cheat::GameFunctions::TeleportToWaypoint()
{
	if (!MAP::IS_WAYPOINT_ACTIVE()) { Cheat::GameFunctions::PrintSubtitle(xorstr_("~COLOR_RED~No waypoint has been set")); return; }
	
	Vector3 coords = MAP::_GET_WAYPOINT_COORDS();
	Entity e = PLAYER::PLAYER_PED_ID();
	if (PED::IS_PED_IN_ANY_VEHICLE(e, 0))
	{
		e = PED::GET_VEHICLE_PED_IS_USING(e);
	}
	else if (PED::IS_PED_ON_MOUNT(e))
	{
		e = PED::GET_MOUNT(e);
	}

	bool groundFound = false;
	static float groundCheckHeight[] =
	{ 100.0, 150.0, 50.0, 0.0, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 750.0, 800.0, 850.0, 900.0, 950.0, 1000.0, 1050.0 };
	for (int i = 0; i < sizeof(groundCheckHeight) / sizeof(float); i++)
	{
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(e, coords.x, coords.y, groundCheckHeight[i], 0, 0, 1);
		Cheat::GameHooking::FiberWait(100);
		if (MISC::GET_GROUND_Z_FOR_3D_COORD(coords.x, coords.y, groundCheckHeight[i], &coords.z, 0))
		{
			groundFound = true;
			coords.z += 3.0;
			break;
		}
	}

	if (!groundFound) { coords.z = 1000.0; }

	Cheat::GameFunctions::TPto(coords);
}

bool Cheat::CheatFeatures::SpawnVehicleInvincibleBool = false;
bool Cheat::CheatFeatures::SpawnPedInVehicleBool = false;
bool Cheat::CheatFeatures::DeleteCurrentVehicleBool = false;
void Cheat::GameFunctions::SpawnVehicle(const char* ModelHash)
{
	DWORD model = MISC::GET_HASH_KEY(ModelHash);
	if (!STREAMING::IS_MODEL_A_VEHICLE(model) || !STREAMING::IS_MODEL_IN_CDIMAGE(model)) { PrintSubtitle(xorstr_("~COLOR_RED~That is not a valid vehicle model")); return; }
	STREAMING::REQUEST_MODEL(model, 0);
	Vector3 pCoords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0, -10.0, 0.0);
	while (!STREAMING::HAS_MODEL_LOADED(model)) { Cheat::GameHooking::FiberWait(0); }
	Vector3 coords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1, 0);
	int veh = VEHICLE::CREATE_VEHICLE(model, coords.x, coords.y, coords.z, 0, 0, 0, 0, 1);
	Cheat::GameFunctions::RequestControlOfEnt(veh);
	VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(veh, 0);
	ENTITY::SET_ENTITY_VISIBLE(veh, true);
	NETWORK::NETWORK_REGISTER_ENTITY_AS_NETWORKED(veh);
	NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(NETWORK::NET_TO_VEH(veh), true);
	DECORATOR::DECOR_SET_BOOL(veh, xorstr_("wagon_block_honor"), true);
	ENTITY::SET_ENTITY_AS_MISSION_ENTITY(veh, 0, 0);
	ENTITY::_SET_ENTITY_SOMETHING(veh, true);
	if (Cheat::CheatFeatures::DeleteCurrentVehicleBool) { if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0)) { DeleteCurrentVehicleHorse(); } }
	if (Cheat::CheatFeatures::SpawnVehicleInvincibleBool) { ENTITY::SET_ENTITY_INVINCIBLE(veh, true); }
	if (Cheat::CheatFeatures::SpawnPedInVehicleBool) { PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), veh, -1); }
	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);  
}

bool Cheat::CheatFeatures::SpawnPedDeadBool = false;
void Cheat::GameFunctions::SpawnPed(const char* ModelHash, Ped PlayerPed)
{
	Hash ped = MISC::GET_HASH_KEY(ModelHash);
	if (!STREAMING::IS_MODEL_A_PED(ped)) { GameFunctions::PrintSubtitle(xorstr_("~COLOR_RED~That is not a valid Ped model")); return; }
	STREAMING::REQUEST_MODEL(ped, 0);
	while (!STREAMING::HAS_MODEL_LOADED(ped)) { Cheat::GameHooking::FiberWait(0); }
	Entity player = PlayerPed;
	Vector3 playerCor = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(player, 0, 5, 0);
	Entity spawn = PED::CREATE_PED(ped, playerCor.x, playerCor.y, playerCor.z, 0.f, false, false, false, false, true, true);
	Cheat::GameFunctions::RequestControlOfEnt(spawn);
	ENTITY::SET_ENTITY_VISIBLE(spawn, true);
	NETWORK::NETWORK_REGISTER_ENTITY_AS_NETWORKED(spawn);
	NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(NETWORK::NET_TO_PED(spawn), true);
	ENTITY::SET_ENTITY_ALPHA(spawn, 255, false);
	if (Cheat::CheatFeatures::SpawnPedDeadBool) { ENTITY::SET_ENTITY_MAX_HEALTH(spawn, 0); }
	PED::SET_PED_VISIBLE(spawn, true);
	ENTITY::SET_ENTITY_AS_MISSION_ENTITY(spawn, 1, 1);
	ENTITY::_SET_ENTITY_SOMETHING(spawn, true);
	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(ped);
}

void Cheat::GameFunctions::DeleteCurrentVehicleHorse()
{
	if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false)) 
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
		if (ENTITY::DOES_ENTITY_EXIST(veh))
		{
			int netID = NETWORK::NETWORK_GET_NETWORK_ID_FROM_ENTITY(veh);
			Cheat::GameFunctions::RequestControlOfId(netID);
			ENTITY::SET_ENTITY_AS_MISSION_ENTITY(veh, 1, 1);
			VEHICLE::DELETE_VEHICLE(&veh);
		}
	}
	else if (PED::IS_PED_ON_MOUNT(PLAYER::PLAYER_PED_ID())) 
	{
		Ped Horse = PED::GET_MOUNT(PLAYER::PLAYER_PED_ID());
		if (ENTITY::DOES_ENTITY_EXIST(Horse))
		{
			int netID = NETWORK::NETWORK_GET_NETWORK_ID_FROM_ENTITY(Horse);
			Cheat::GameFunctions::RequestControlOfId(netID);
			ENTITY::SET_ENTITY_AS_MISSION_ENTITY(Horse, 1, 1);
			ENTITY::DELETE_ENTITY(&Horse);
		}
	}
	else
	{
		Cheat::GameFunctions::PrintSubtitle(xorstr_("~COLOR_RED~You are not in a vehicle or on a horse"));
		return;
	}
}

void Cheat::GameFunctions::ShowPlayerInformationBox(Player Player)
{

}

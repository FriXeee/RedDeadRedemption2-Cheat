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
		WEAPON::_GIVE_WEAPON_TO_PED_2(Player, MISC::GET_HASH_KEY(i.c_str()), 9999, true, true, 0, false, 0.5f, 1.0f, false, false, 0.f, 0.f);
	}
}

/*
Description: Teleport target entity to target coords
Note(s): If AutoCorrectGroundHeight is set, the function automatically searches for nearby ground
		 If IgnoreCurrentPedVehicle is set, the vehicle a entity may be in will be ignored and only the ped will be teleported
*/
void Cheat::GameFunctions::TeleportToCoords(Entity e, Vector3 coords, bool AutoCorrectGroundHeight, bool IgnoreCurrentPedVehicleOrHorse)
{
	Entity TargetEntity = e;

	if (ENTITY::IS_ENTITY_A_PED(TargetEntity))
	{
		if (PED::IS_PED_IN_ANY_VEHICLE(TargetEntity, false) && !IgnoreCurrentPedVehicleOrHorse)
		{
			TargetEntity = PED::GET_VEHICLE_PED_IS_USING(TargetEntity);
		}
		else if (PED::IS_PED_ON_MOUNT(TargetEntity))
		{
			TargetEntity = PED::GET_MOUNT(TargetEntity);
		}
	}
	if (ENTITY::IS_ENTITY_A_VEHICLE(TargetEntity) || ENTITY::IS_ENTITY_A_PED(TargetEntity))
	{
		Cheat::GameFunctions::RequestNetworkControlOfEntity(TargetEntity);
	}

	if (!AutoCorrectGroundHeight)
	{
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(TargetEntity, coords.x, coords.y, coords.z, false, false, true);
	}
	else
	{
		bool groundFound = false;
		static std::array<float, 21> groundCheckHeight = { 0.0, 50.0, 100.0, 150.0, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 750.0, 800.0, 850.0, 900.0, 950.0, 1000.00 };

		for (const float& CurrentHeight : groundCheckHeight)
		{
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(TargetEntity, coords.x, coords.y, CurrentHeight, false, false, true);
			GameHooking::FiberWait(50);
			if (MISC::GET_GROUND_Z_FOR_3D_COORD(coords.x, coords.y, CurrentHeight, &coords.z, false))
			{
				groundFound = true;
				coords.z += 3.0;
				break;
			}
		}

		if (!groundFound)
		{
			Vector3 ClosestRoadCoord;
			if (PATHFIND::GET_CLOSEST_ROAD(coords.x, coords.y, coords.z, 1.f, 1,
				&ClosestRoadCoord, &ClosestRoadCoord, NULL, NULL, NULL, 0))
			{
				coords = ClosestRoadCoord;
			}
			GameFunctions::PrintSubtitle("Ground not found; teleported to nearby road");
		}
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(TargetEntity, coords.x, coords.y, coords.z, false, false, true);
	}
}

void Cheat::GameFunctions::RequestNetworkControlOfEntity(Entity entity)
{
	int EntityTick = 0;
	while (!NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(entity) && EntityTick <= 25)
	{
		NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(entity);
		EntityTick++;
	}
	if (NETWORK::NETWORK_IS_SESSION_STARTED())
	{
		int netID = NETWORK::NETWORK_GET_NETWORK_ID_FROM_ENTITY(entity);
		int NetTick = 0;
		while (!NETWORK::NETWORK_HAS_CONTROL_OF_NETWORK_ID(netID) && NetTick <= 25)
		{
			NETWORK::NETWORK_REQUEST_CONTROL_OF_NETWORK_ID(netID);
			NetTick++;
		}
	}
}

void Cheat::GameFunctions::PrintSubtitle(std::string Text)
{
	UILOG::_UILOG_SET_CACHED_OBJECTIVE(MISC::_CREATE_VAR_STRING(10, "LITERAL_STRING", Text.c_str()));
	UILOG::_UILOG_PRINT_CACHED_OBJECTIVE();
	UILOG::_UILOG_CLEAR_CACHED_OBJECTIVE();
}

void Cheat::GameFunctions::TeleportToWaypoint()
{
	if (MAP::IS_WAYPOINT_ACTIVE()) 
	{ 
		Cheat::GameFunctions::TeleportToCoords(PlayerPedID, MAP::_GET_WAYPOINT_COORDS(), true, false);
		return;
	}
	Cheat::GameFunctions::PrintSubtitle("~COLOR_RED~No waypoint has been set");
}

bool Cheat::CheatFeatures::SpawnVehicleInvincibleBool = false;
bool Cheat::CheatFeatures::SpawnPedInVehicleBool = false;
bool Cheat::CheatFeatures::DeleteCurrentVehicleBool = false;
void Cheat::GameFunctions::SpawnVehicle(const char* ModelHash)
{
	DWORD model = MISC::GET_HASH_KEY(ModelHash);
	if (!STREAMING::IS_MODEL_A_VEHICLE(model) || !STREAMING::IS_MODEL_IN_CDIMAGE(model)) { PrintSubtitle("~COLOR_RED~That is not a valid vehicle model"); return; }
	STREAMING::REQUEST_MODEL(model, 0);
	Vector3 pCoords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(Cheat::GameFunctions::PlayerPedID, 0.0, -10.0, 0.0);
	while (!STREAMING::HAS_MODEL_LOADED(model)) { Cheat::GameHooking::FiberWait(0); }
	Vector3 coords = ENTITY::GET_ENTITY_COORDS(Cheat::GameFunctions::PlayerPedID, 1, 0);
	int veh = VEHICLE::CREATE_VEHICLE(model, coords.x, coords.y, coords.z, 0, 0, 0, 0, 1);
	Cheat::GameFunctions::RequestNetworkControlOfEntity(veh);
	VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(veh, 0);
	ENTITY::SET_ENTITY_VISIBLE(veh, true);
	NETWORK::NETWORK_REGISTER_ENTITY_AS_NETWORKED(veh);
	NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(NETWORK::NET_TO_VEH(veh), true);
	DECORATOR::DECOR_SET_BOOL(veh, "wagon_block_honor", true);
	ENTITY::SET_ENTITY_AS_MISSION_ENTITY(veh, 0, 0);
	ENTITY::_SET_ENTITY_SOMETHING(veh, true);
	if (Cheat::CheatFeatures::DeleteCurrentVehicleBool) { if (PED::IS_PED_IN_ANY_VEHICLE(Cheat::GameFunctions::PlayerPedID, 0)) { DeleteCurrentVehicle(); } }
	if (Cheat::CheatFeatures::SpawnVehicleInvincibleBool) { ENTITY::SET_ENTITY_INVINCIBLE(veh, true); }
	if (Cheat::CheatFeatures::SpawnPedInVehicleBool) { PED::SET_PED_INTO_VEHICLE(Cheat::GameFunctions::PlayerPedID, veh, -1); }
	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);  
}

bool Cheat::CheatFeatures::SpawnPedDeadBool = false;
void Cheat::GameFunctions::SpawnPed(const char* ModelHash, Ped PlayerPed)
{
	Hash ped = MISC::GET_HASH_KEY(ModelHash);
	if (!STREAMING::IS_MODEL_A_PED(ped)) { GameFunctions::PrintSubtitle("~COLOR_RED~That is not a valid Ped model"); return; }
	STREAMING::REQUEST_MODEL(ped, 0);
	while (!STREAMING::HAS_MODEL_LOADED(ped)) { Cheat::GameHooking::FiberWait(0); }
	Entity player = PlayerPed;
	Vector3 playerCor = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(player, 0, 5, 0);
	Entity spawn = PED::CREATE_PED(ped, playerCor.x, playerCor.y, playerCor.z, 0.f, true, false, false, false);
	Cheat::GameFunctions::RequestNetworkControlOfEntity(spawn);
	ENTITY::SET_ENTITY_VISIBLE(spawn, true);
	NETWORK::NETWORK_REGISTER_ENTITY_AS_NETWORKED(spawn);
	NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(NETWORK::NET_TO_PED(spawn), true);
	ENTITY::SET_ENTITY_ALPHA(spawn, 255, false);
	if (Cheat::CheatFeatures::SpawnPedDeadBool) { ENTITY::SET_ENTITY_MAX_HEALTH(spawn, 0); }
	PED::_SET_RANDOM_OUTFIT_VARIATION(spawn, true);
	ENTITY::SET_ENTITY_AS_MISSION_ENTITY(spawn, 1, 1);
	ENTITY::_SET_ENTITY_SOMETHING(spawn, true);
	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(ped);
}


/*
Description: Delete the vehicle the local player is using
Note(s): None
*/
void Cheat::GameFunctions::DeleteCurrentVehicle()
{
	if (PED::IS_PED_IN_ANY_VEHICLE(PlayerPedID, false))
	{
		Vehicle VehicleHandle = PED::GET_VEHICLE_PED_IS_USING(PlayerPedID);
		if (ENTITY::DOES_ENTITY_EXIST(VehicleHandle))
		{
			RequestNetworkControlOfEntity(VehicleHandle);
			ENTITY::SET_ENTITY_AS_MISSION_ENTITY(VehicleHandle, true, true);
			VEHICLE::DELETE_VEHICLE(&VehicleHandle);
			return;
		}
	}
	Cheat::GameFunctions::PrintSubtitle("~COLOR_RED~You are not in a vehicle");
}

void Cheat::GameFunctions::DeleteCurrentMount()
{
	if (PED::IS_PED_ON_MOUNT(PlayerPedID))
	{
		Entity MountHandle = PED::GET_MOUNT(PlayerPedID);
		if (ENTITY::DOES_ENTITY_EXIST(MountHandle))
		{
			RequestNetworkControlOfEntity(MountHandle);
			ENTITY::SET_ENTITY_AS_MISSION_ENTITY(MountHandle, true, true);
			ENTITY::DELETE_ENTITY(&MountHandle);
			return;
		}
	}
	Cheat::GameFunctions::PrintSubtitle("~COLOR_RED~You are not on a horse");
}
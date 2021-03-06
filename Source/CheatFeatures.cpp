#include "stdafx.h"

bool Cheat::CheatFeatures::TriggerbotShootPlayersBool = false;
int Cheat::CheatFeatures::SelectedPlayer;

void Cheat::CheatFeatures::Loop()
{
	//Features
	GodmodeBool ? Godmode(true) : Godmode(false);
	SuperJumpBool ? SuperJump() : NULL;
	NoRagdollBool ? NoRagdoll(true) : NoRagdoll(false);
	IgnoredByNPCBool ? IgnoredByNPC(true) : IgnoredByNPC(false);
	TriggerbotBool ? Triggerbot() : NULL;
	SuperRunBool ? SuperRun() : NULL;
	NeverWantedBool ? NeverWanted(true) : NeverWanted(false);
	UnlimitedDeadEyeBool ? UnlimitedDeadEye() : NULL;
	InvisibleBool ? Invisible(true) : Invisible(false);
	InfiniteStaminaBool ? InfiniteStamina() : NULL;
	InfiniteAmmoBool ? InfiniteAmmo() : NULL;
	NoClipBool ? NoClip() : NoClipWasEnabled ? ENTITY::SET_ENTITY_COLLISION(PLAYER::PLAYER_PED_ID(), true, true), NoClipWasEnabled = false : NULL;
	AutoTeleportToWaypointBool ? AutoTeleportToWaypoint() : NULL;
	HideHUDBool ? HideHUD(true) : HideHUD(false);
	SpectateSelectedPlayerBool ? SpectateSelectedPlayer(true) : SpectateSelectedPlayer(false);
	FreezeSelectedPlayerBool ? FreezeSelectedPlayer() : NULL;
	WeaponDamageMultiplierBool ? WeaponDamageMultiplier(true) : WeaponDamageMultiplier(false);
	TeleportGunBool ? TeleportGun() : NULL;
	ExplosiveAmmoBool ? ExplosiveAmmo() : NULL;
	VehicleGodmodeBool ? VehicleGodmode(true) : VehicleGodmode(false);
	VehicleInvisibleBool ? VehicleInvisible(true) : VehicleInvisible(false);
	HorseGodmodeBool ? HorseGodmode(true) : HorseGodmode(false);
	HorseInvisibleBool ? HorseInvisible(true) : HorseInvisible(false);
	UnlimitedHorseStaminaBool ? UnlimitedHorseStamina() : NULL;
}



bool Cheat::CheatFeatures::GodmodeBool = false;
void Cheat::CheatFeatures::Godmode(bool toggle)
{
	ENTITY::SET_ENTITY_INVINCIBLE(Cheat::GameFunctions::PlayerPedID, toggle);
}

bool Cheat::CheatFeatures::SuperJumpBool = false;
void Cheat::CheatFeatures::SuperJump()
{
	MISC::SET_SUPER_JUMP_THIS_FRAME(Cheat::GameFunctions::PlayerID);
}


bool Cheat::CheatFeatures::NoRagdollBool = false;
void Cheat::CheatFeatures::NoRagdoll(bool toggle)
{
	if (toggle)
	{
		PED::SET_PED_CAN_RAGDOLL(Cheat::GameFunctions::PlayerPedID, false);
		PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(Cheat::GameFunctions::PlayerPedID, false);
		PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(Cheat::GameFunctions::PlayerPedID, false);
	}
	else
	{
		PED::SET_PED_CAN_RAGDOLL(Cheat::GameFunctions::PlayerPedID, true);
		PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(Cheat::GameFunctions::PlayerPedID, true);
		PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(Cheat::GameFunctions::PlayerPedID, true);
	}
}

bool Cheat::CheatFeatures::IgnoredByNPCBool = false;
void Cheat::CheatFeatures::IgnoredByNPC(bool toggle)
{
	PLAYER::SET_EVERYONE_IGNORE_PLAYER(Cheat::GameFunctions::PlayerID, toggle);
}


bool Cheat::CheatFeatures::TriggerbotBool = false;
void Cheat::CheatFeatures::Triggerbot()
{
	Entity AimingEntity; 
	if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(Cheat::GameFunctions::PlayerID, &AimingEntity))
	{ 
		if (TriggerbotShootPlayersBool) 
		{ 
			if (!PED::IS_PED_RELOADING(Cheat::GameFunctions::PlayerPedID) && ENTITY::IS_ENTITY_A_PED(AimingEntity) && !ENTITY::IS_ENTITY_DEAD(AimingEntity))
			{ 
				TASK::TASK_SHOOT_AT_ENTITY(Cheat::GameFunctions::PlayerPedID, AimingEntity, -1, -957453492, 1);
			} 
		} 
		else 
		{ 
			if (!PED::IS_PED_RELOADING(PLAYER::PLAYER_PED_ID()) && ENTITY::IS_ENTITY_A_PED(AimingEntity) && !ENTITY::IS_ENTITY_DEAD(AimingEntity) && !PED::IS_PED_A_PLAYER(AimingEntity)) 
			{ 
				TASK::TASK_SHOOT_AT_ENTITY(PLAYER::PLAYER_PED_ID(), AimingEntity, -1, -957453492, 1);
			} 
		} 
	}
}

bool Cheat::CheatFeatures::SuperRunBool = false;
void Cheat::CheatFeatures::SuperRun()
{
	if (PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_SPRINT)) 
	{ 
		ENTITY::APPLY_FORCE_TO_ENTITY(PLAYER::PLAYER_PED_ID(), 1, 0.0f, 1.3, 0, 0.0f, 0.0f, 0.0f, 0, 1, 1, 1, 0, 1); 
	}
}

bool Cheat::CheatFeatures::NeverWantedBool = false;
void Cheat::CheatFeatures::NeverWanted(bool toggle)
{
	if (toggle)
	{
		PLAYER::SET_WANTED_LEVEL_MULTIPLIER(0.0);
	}
	else
	{
		PLAYER::SET_WANTED_LEVEL_MULTIPLIER(1);
	}
}

bool Cheat::CheatFeatures::UnlimitedDeadEyeBool = false;
void Cheat::CheatFeatures::UnlimitedDeadEye()
{
	PLAYER::RESTORE_SPECIAL_ABILITY(Cheat::GameFunctions::PlayerID, -1, false);
}

bool Cheat::CheatFeatures::InvisibleBool = false;
void Cheat::CheatFeatures::Invisible(bool toggle)
{
	if (toggle)
	{
		ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), false); 
		NETWORK::_NETWORK_SET_ENTITY_INVISIBLE_TO_NETWORK(PLAYER::PLAYER_PED_ID(), true);
	}
	else
	{
		ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), true); 
		NETWORK::_NETWORK_SET_ENTITY_INVISIBLE_TO_NETWORK(PLAYER::PLAYER_PED_ID(), false);
	}
}

bool Cheat::CheatFeatures::InfiniteStaminaBool = false;
void Cheat::CheatFeatures::InfiniteStamina()
{
	PLAYER::RESTORE_PLAYER_STAMINA(Cheat::GameFunctions::PlayerID, 100.0f);
}


bool Cheat::CheatFeatures::InfiniteAmmoBool = false;
void Cheat::CheatFeatures::InfiniteAmmo()
{
	Hash cur;
	if (WEAPON::GET_CURRENT_PED_WEAPON(PLAYER::PLAYER_PED_ID(), &cur, true, 0, true))
	{
		if (WEAPON::IS_WEAPON_VALID(cur))
		{
			int maxAmmo;
			if (WEAPON::GET_MAX_AMMO(PLAYER::PLAYER_PED_ID(), &maxAmmo, cur))
			{
				WEAPON::SET_PED_AMMO(PLAYER::PLAYER_PED_ID(), cur, maxAmmo);

				maxAmmo = WEAPON::GET_MAX_AMMO_IN_CLIP(PLAYER::PLAYER_PED_ID(), cur, 1);
				if (maxAmmo > 0) 
				{
					WEAPON::SET_AMMO_IN_CLIP(PLAYER::PLAYER_PED_ID(), cur, maxAmmo);
				}
			}
		}
	}
}

bool Cheat::CheatFeatures::NoClipBool = false;
bool Cheat::CheatFeatures::NoClipWasEnabled = false;
void Cheat::CheatFeatures::NoClip()
{
	NoClipWasEnabled = true;
	float x, y, z;
	float d = 0.049999f;

	Cheat::GameFunctions::GetCameraDirection(&x, &y, &z);

	if (NETWORK::NETWORK_IS_SESSION_STARTED())
	{
		d = 0.049999;
	}
	else
	{
		d = 0.599999;
	}

	if (!PED::IS_PED_ON_FOOT(PLAYER::PLAYER_PED_ID()))
	{
		int entity;
		if (PED::IS_PED_ON_MOUNT(PLAYER::PLAYER_PED_ID())) { entity = PED::GET_MOUNT(PLAYER::PLAYER_PED_ID()); }
		else { entity = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false); }

		Vector3 Pos = ENTITY::GET_ENTITY_COORDS(entity, false, false);
		Vector3 rotation = CAM::GET_GAMEPLAY_CAM_ROT(0);

		ENTITY::SET_ENTITY_COLLISION(entity, true, true);
		ENTITY::SET_ENTITY_ROTATION(entity, rotation.x, rotation.y, rotation.z, 2, 1);
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(entity, Pos.x, Pos.y, Pos.z, 0, 0, 0);
		if (ENTITY::DOES_ENTITY_EXIST(entity))
		{
			if (GetAsyncKeyState(0x57) && Cheat::CheatFunctions::IsGameWindowFocussed()) //RT. Original index // was 24
			{
				ENTITY::SET_ENTITY_COLLISION(entity, false, false);
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(entity, Pos.x + (x * d), Pos.y + (y * d), Pos.z + (z * d), 0, 0, 0);
			}
		}
	}
	else
	{
		Vector3 Pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), false, false);

		Vector3 rotation = CAM::GET_GAMEPLAY_CAM_ROT(0);
		ENTITY::SET_ENTITY_COLLISION(PLAYER::PLAYER_PED_ID(), true, true);
		ENTITY::SET_ENTITY_ROTATION(PLAYER::PLAYER_PED_ID(), rotation.x, rotation.y, rotation.z, 2, 1);
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(PLAYER::PLAYER_PED_ID(), Pos.x, Pos.y, Pos.z, 0, 0, 0);
		if (GetAsyncKeyState(0x57) && Cheat::CheatFunctions::IsGameWindowFocussed())
		{
			ENTITY::SET_ENTITY_COLLISION(PLAYER::PLAYER_PED_ID(), false, false);
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(PLAYER::PLAYER_PED_ID(), Pos.x + (x * d), Pos.y + (y * d), Pos.z + (z * d), 0, 0, 0);
		}
	}
}


bool Cheat::CheatFeatures::AutoTeleportToWaypointBool = false;
void Cheat::CheatFeatures::AutoTeleportToWaypoint()
{
	if (MAP::IS_WAYPOINT_ACTIVE() && !HUD::IS_PAUSE_MENU_ACTIVE()) 
	{ 
		Cheat::GameFunctions::TeleportToWaypoint();
		MAP::SET_WAYPOINT_OFF();
	}
}


bool Cheat::CheatFeatures::HideHUDBool = false;
void Cheat::CheatFeatures::HideHUD(bool toggle)
{
	if (toggle)
	{
		HUD::DISPLAY_HUD(false);
	}
	else
	{
		HUD::DISPLAY_HUD(true);
	}
}

bool Cheat::CheatFeatures::SpectateSelectedPlayerBool = false;
void Cheat::CheatFeatures::SpectateSelectedPlayer(bool toggle)
{
	NETWORK::NETWORK_SET_IN_SPECTATOR_MODE(toggle, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(SelectedPlayer));
}

bool Cheat::CheatFeatures::FreezeSelectedPlayerBool = false;
void Cheat::CheatFeatures::FreezeSelectedPlayer()
{
	Ped SelectedPlayerHandle = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(SelectedPlayer);
	TASK::CLEAR_PED_TASKS_IMMEDIATELY(SelectedPlayerHandle, false, false);
	TASK::CLEAR_PED_TASKS(SelectedPlayerHandle, false, false);
	TASK::CLEAR_PED_SECONDARY_TASK(SelectedPlayerHandle);
}

bool Cheat::CheatFeatures::WeaponDamageMultiplierBool = false;
void Cheat::CheatFeatures::WeaponDamageMultiplier(bool toggle)
{
	if (toggle)
	{
		PLAYER::SET_PLAYER_WEAPON_DAMAGE_MODIFIER(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::PLAYER_PED_ID()), 999999);
		PLAYER::SET_PLAYER_MELEE_WEAPON_DAMAGE_MODIFIER(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::PLAYER_PED_ID()), 999999);
	}
	else
	{
		PLAYER::SET_PLAYER_WEAPON_DAMAGE_MODIFIER(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::PLAYER_PED_ID()), 1); 
		PLAYER::SET_PLAYER_MELEE_WEAPON_DAMAGE_MODIFIER(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::PLAYER_PED_ID()), 1);
	}
}

bool Cheat::CheatFeatures::TeleportGunBool = false;
void Cheat::CheatFeatures::TeleportGun()
{
	Vector3 coords; 
	if (PED::IS_PED_SHOOTING(PLAYER::PLAYER_PED_ID())) 
	{
		if (WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PLAYER::PLAYER_PED_ID(), &coords)) {
			ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), coords.x, coords.y, coords.z, 1, 0, 0, 1);
		}
	}
}

bool Cheat::CheatFeatures::ExplosiveAmmoBool = false;
void Cheat::CheatFeatures::ExplosiveAmmo()
{
	Vector3 coords;
	if (PED::IS_PED_SHOOTING(PLAYER::PLAYER_PED_ID())) 
	{
		if (WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PLAYER::PLAYER_PED_ID(), &coords)) {
			FIRE::ADD_EXPLOSION(coords.x, coords.y, coords.z, 22, 1000.f, true, false, false);
		}
	}
}

bool Cheat::CheatFeatures::VehicleGodmodeBool = false;
void Cheat::CheatFeatures::VehicleGodmode(bool toggle)
{
	if (toggle)
	{
		Vehicle Vehicle = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
		Ped Horse = PED::GET_MOUNT(PLAYER::PLAYER_PED_ID());
		ENTITY::SET_ENTITY_INVINCIBLE(Vehicle, true);
		ENTITY::SET_ENTITY_PROOFS(Vehicle, 0, 0);
		VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(Vehicle, 0);
		VEHICLE::SET_VEHICLE_WHEELS_CAN_BREAK(Vehicle, 0);
		VEHICLE::SET_VEHICLE_CAN_BE_VISIBLY_DAMAGED(Vehicle, 0);
		ENTITY::SET_ENTITY_INVINCIBLE(Horse, true);
	}
	else
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()); 
		Ped Horse = PED::GET_MOUNT(PLAYER::PLAYER_PED_ID()); 
		ENTITY::SET_ENTITY_INVINCIBLE(veh, false); 
		ENTITY::SET_ENTITY_PROOFS(veh, 0, 0); 
		VEHICLE::SET_VEHICLE_CAN_BE_VISIBLY_DAMAGED(veh, 1); 
		ENTITY::SET_ENTITY_INVINCIBLE(Horse, false);
	}
}

bool Cheat::CheatFeatures::VehicleInvisibleBool = false;
void Cheat::CheatFeatures::VehicleInvisible(bool toggle)
{
	if (toggle)
	{
		ENTITY::SET_ENTITY_VISIBLE(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), false); 
		ENTITY::SET_ENTITY_VISIBLE(PED::GET_MOUNT(PLAYER::PLAYER_PED_ID()), false);
	}
	else
	{
		ENTITY::SET_ENTITY_VISIBLE(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), true); 
		ENTITY::SET_ENTITY_VISIBLE(PED::GET_MOUNT(PLAYER::PLAYER_PED_ID()), true);
	}
}


bool Cheat::CheatFeatures::HorseGodmodeBool = false;
void Cheat::CheatFeatures::HorseGodmode(bool toggle)
{
	if (toggle)
	{
		Ped Horse = PED::GET_MOUNT(GameFunctions::PlayerPedID);
		ENTITY::SET_ENTITY_INVINCIBLE(Horse, true);
		ENTITY::SET_ENTITY_PROOFS(Horse, 0, false);
		VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(Horse, false);
		VEHICLE::SET_VEHICLE_WHEELS_CAN_BREAK(Horse, false);
		VEHICLE::SET_VEHICLE_CAN_BE_VISIBLY_DAMAGED(Horse, false);
		ENTITY::SET_ENTITY_INVINCIBLE(Horse, true);
	}
	else
	{
		Ped Horse = PED::GET_MOUNT(GameFunctions::PlayerPedID);
		ENTITY::SET_ENTITY_INVINCIBLE(Horse, false);
		ENTITY::SET_ENTITY_PROOFS(Horse, 0, false);
		VEHICLE::SET_VEHICLE_CAN_BE_VISIBLY_DAMAGED(Horse, true);
		ENTITY::SET_ENTITY_INVINCIBLE(Horse, false);
	}
}

bool Cheat::CheatFeatures::HorseInvisibleBool = false;
void Cheat::CheatFeatures::HorseInvisible(bool toggle)
{
	if (toggle)
	{
		ENTITY::SET_ENTITY_VISIBLE(PED::GET_MOUNT(GameFunctions::PlayerPedID), false);
	}
	else
	{
		ENTITY::SET_ENTITY_VISIBLE(PED::GET_MOUNT(GameFunctions::PlayerPedID), true);
	}
}



bool Cheat::CheatFeatures::UnlimitedHorseStaminaBool = false;
void Cheat::CheatFeatures::UnlimitedHorseStamina()
{
	PED::_RESTORE_PED_STAMINA(PED::GET_MOUNT(PLAYER::PLAYER_PED_ID()), 100.f);
}
#pragma once

enum SubMenus {
	NOMENU,
	MainMenu,
	SelfMenu,
	NetworkingOptionsMenu,
	VehicleOptionsMenu,
	WeaponOptionsMenu,
	TeleportOptionsMenu,
	WorldOptionsMenu,
	MiscOptionsMenu,
	CheatSettingsMenu,
	CommonTeleportsMenu,
	CustomTeleportLocationsMenu,
	GUISettingsMenu,
	HudOptionsMenu,
	PedSpawnerMenu,
	TimeSettingsMenu,
	WeatherOptionsMenu,
	ModelChangerMenu,
	PlayerlistMenu,
	SelectedPlayerMenu,
	SelectedPlayerTrollMenu,
	SelectedPlayerSpawnOptionsMenu,
	SelectedPlayerPedSpawnerMenu,
	VehicleSpawnerMenu,
	VehicleSpawnSettingsMenu,
	TriggerBotMenu
};
extern enum SubMenus;

struct rgba {
	int r, g, b, a;
};
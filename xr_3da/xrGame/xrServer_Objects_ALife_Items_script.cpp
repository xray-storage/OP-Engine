////////////////////////////////////////////////////////////////////////////
//	Module 		: xrServer_Objects_ALife_Items_script.cpp
//	Created 	: 19.09.2002
//  Modified 	: 04.06.2003
//	Author		: Dmitriy Iassenev
//	Description : Server items for ALife simulator, script export
////////////////////////////////////////////////////////////////////////////

#include "pch_script.h"
#include "xrServer_Objects_ALife_Items.h"
#include "xrServer_script_macroses.h"

using namespace luabind;

#pragma optimize("s",on)
void CSE_ALifeInventoryItem::script_register(lua_State *L)
{
	module(L)[
		class_<CSE_ALifeInventoryItem>
			("cse_alife_inventory_item")
//			.def(		constructor<LPCSTR>())
			.def_readwrite("cost", &CSE_ALifeInventoryItem::m_dwCost)
			.def_readwrite("weight", &CSE_ALifeInventoryItem::m_fMass)
			.def_readwrite("radiation", &CSE_ALifeInventoryItem::m_fRadiation)
			.def_readwrite("condition", &CSE_ALifeInventoryItem::m_fCondition)
	];
}

void CSE_ALifeItem::script_register(lua_State *L)
{
	module(L)[
//		luabind_class_item2(
		luabind_class_abstract2(
			CSE_ALifeItem,
			"cse_alife_item",
			CSE_ALifeDynamicObjectVisual,
			CSE_ALifeInventoryItem
		)
	];
}

void CSE_ALifeItemTorch::script_register(lua_State *L)
{
	module(L)[
		luabind_class_item1(
			CSE_ALifeItemTorch,
			"cse_alife_item_torch",
			CSE_ALifeItem
		)
	];
}

void CSE_ALifeItemNVDevice::script_register(lua_State *L)
{
	module(L)[
		luabind_class_item1(
			CSE_ALifeItemNVDevice,
			"cse_alife_item_nv_device",
			CSE_ALifeItem
		)
	];
}

void CSE_ALifeItemGameBox::script_register(lua_State *L)
{
	module(L)[
		luabind_class_item1(
			CSE_ALifeItemGameBox,
			"cse_alife_item_game_box",
			CSE_ALifeItem
		)
			.def("set_user_data", &CSE_ALifeItemGameBox::set_user_data)
			.def("get_user_data", &CSE_ALifeItemGameBox::get_user_data)
	];
}

void CSE_ALifeItemAmmo::script_register(lua_State *L)
{
	module(L)[
		luabind_class_item1(
			CSE_ALifeItemAmmo,
			"cse_alife_item_ammo",
			CSE_ALifeItem
		)
	];
}

void CSE_ALifeItemWeapon::script_register(lua_State *L)
{
	module(L)[
		luabind_class_item1(
			CSE_ALifeItemWeapon,
			"cse_alife_item_weapon",
			CSE_ALifeItem
		)
		.def("get_addon_state", &CSE_ALifeItemWeapon::get_addon_state)
		.def("get_weapon_state", &CSE_ALifeItemWeapon::get_weapon_state)
		.def("current_ammo_type", &CSE_ALifeItemWeapon::current_ammo_type)
	];
}

void CSE_ALifeItemWeaponShotGun::script_register(lua_State *L)
{
	module(L)[
		luabind_class_item1(
			CSE_ALifeItemWeaponShotGun,
			"cse_alife_item_weapon_shotgun",
			CSE_ALifeItemWeapon
			)
	];
}

void CSE_ALifeItemDetector::script_register(lua_State *L)
{
	module(L)[
		luabind_class_item1(
			CSE_ALifeItemDetector,
			"cse_alife_item_detector",
			CSE_ALifeItem
		)
	];
}

void CSE_ALifeItemArtefact::script_register(lua_State *L)
{
	module(L)[
		luabind_class_item1(
			CSE_ALifeItemArtefact,
			"cse_alife_item_artefact",
			CSE_ALifeItem
		)
	];
}

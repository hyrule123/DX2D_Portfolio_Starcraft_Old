#pragma once

#include "EngineInfo.h"

//================== 유닛 관련 정보를 저장하는 구조체들 ======================


enum class ESCUnit_Races
{
	Zerg,
	Terran,
	Protoss
};

//실제 레이어는 이렇게 구성되고,
enum class ESCUnit_TextureLayer : UINT8
{
	Selected = 1 << 0,
	MainWithShadow = 1 << 1,
	MainOnly = 1 << 2,
	Top = 1 << 3,
	Effect = 1 << 4,
	Attack = 1 << 5,
	Boost = 1 << 6,
	Count = (UINT8)7,
	All = 0b11111111,

};

//애니메이션에는 이렇게 겹쳐서 들어감.
enum ESCUnit_AnimLayer
{
	AnimLayer_Selected,
	AnimLayer_Shadow_Main,
	AnimLayer_Top,
	AnimLayer_Effect,
	AnimLayer_Attack_Boost,
	AnimLayer_Max
};

enum class ESCUnit_Actions
{
	None = -1,
	Birth,
	Death,
	Idle,
	Move,
	SurfaceAttack,
	AirAttack,
	Debris,
	Construction,
	ConstructionComplete,
	BuildUnit,
	End
};

//특정 애니메이션이 어느 애니메이션 레이어에 어떤 이름으로 저장되어있는지를 저장


enum class ESCUnit_Surface : UINT8
{
	Surface,
	SurfaceHover,
	Air,
	Toggle
};

enum class ESCUnit_SizeClassification : UINT8
{
	Small,
	Medium,
	Big
};

enum class ESCUnit_AttackType : UINT8
{
	Normal,
	Explosive,
	Concussive,
	Spell
};


struct SCUnit_ColliderSize
{
	UINT8 Up;
	UINT8 Down;
	UINT8 Left;
	UINT8 Right;
};

enum class ESCUnit_UnitSpecFlags : UINT32
{
	Air = 1 << 0,
	Hero = 1 << 1,
	SelfHealing = 1 << 2,
	MagicUnit = 1 << 3,
	PermanentCloaked = 1 << 4,
	Invincible = 1 << 5,
	Bionic = 1 << 6,
	Mechanic = 1 << 7,
	Robotic = 1 << 8,
	Detector = 1 << 9,
	AdditionalUnit = 1 << 10,
	ResourceGathering = 1 << 11,
	NeedPylon = 1 << 12,
	NeedCreep = 1 << 13,
	Burrowable = 1 << 14,
	Cloakable = 1 << 15,
	ReactionOnHit = 1 << 16,
	AutoAttack = 1 << 17,
	Building = 1 << 18,
	Addon = 1 << 19,
	LiftAble = 1 << 20,
	CreateUnitAble = 1 << 21

};

struct SCUnit_Anim_LayerNameBind
{
	ESCUnit_AnimLayer AnimLayer;
	std::string AnimName;
};

struct SCUnitInfo
{
	std::string UnitName;
	int MaxHP;
	int Shield;
	UINT16 DefaultArmor;
	std::string ArmorName;
	UINT8 ArmorPerUpgrade;

	UINT16 CostMineral;
	UINT16 CostGas;
	UINT16 BuildTime;

	
	ESCUnit_AttackType AttackType;

	std::string WeaponSurfaceName;
	UINT16 WeaponSurfaceDmg;
	UINT8 WeaponSurfacePerUpgrade;

	std::string WeaponAirName;
	UINT16 WeaponAirDamage;
	UINT8 WeaponAirPerUpgrade;

	//  /2를 해서 계산(저글링, 스커지 생각해서)
	UINT8 Population;
	ESCUnit_Races Race;

	ESCUnit_SizeClassification ESCUnit_SizeClassification;
	UINT8 Sight;

	ESCUnit_UnitSpecFlags UnitSpecFlags;
	
	ESCUnit_Surface isSurface;
	
	SCUnit_ColliderSize UnitCollSize;
	float MoveSpeed;
	

	//사용하는 텍스처 레이어 지정. 
	//ESCUnit_TextureLayer 참고할것
	UINT8 UsingTextureLayerFlags;
	std::string AnimationNames[(int)ESCUnit_Actions::End];

	//사운드도 추가해야함


	SCUnitInfo():
		Shield(INT_MIN)
	{}
};
//============================================================
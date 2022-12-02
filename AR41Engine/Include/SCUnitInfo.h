#pragma once

#include "EngineInfo.h"

//================== ���� ���� ������ �����ϴ� ����ü�� ======================


enum class ESCUnit_Races
{
	Zerg,
	Terran,
	Protoss
};

//���� ���̾�� �̷��� �����ǰ�,
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

//�ִϸ��̼ǿ��� �̷��� ���ļ� ��.
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

//Ư�� �ִϸ��̼��� ��� �ִϸ��̼� ���̾ � �̸����� ����Ǿ��ִ����� ����


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

	//  /2�� �ؼ� ���(���۸�, ��Ŀ�� �����ؼ�)
	UINT8 Population;
	ESCUnit_Races Race;

	ESCUnit_SizeClassification ESCUnit_SizeClassification;
	UINT8 Sight;

	ESCUnit_UnitSpecFlags UnitSpecFlags;
	
	ESCUnit_Surface isSurface;
	
	SCUnit_ColliderSize UnitCollSize;
	float MoveSpeed;
	

	//����ϴ� �ؽ�ó ���̾� ����. 
	//ESCUnit_TextureLayer �����Ұ�
	UINT8 UsingTextureLayerFlags;
	std::string AnimationNames[(int)ESCUnit_Actions::End];

	//���嵵 �߰��ؾ���


	SCUnitInfo():
		Shield(INT_MIN)
	{}
};
//============================================================
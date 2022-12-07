#pragma once

#include "EngineInfo.h"


#define SCUnitNameLenMax 64

//================== ���� ���� ������ �����ϴ� ����ü�� ======================


enum class ESCUnit_Races
{
	Zerg,
	Terran,
	Protoss
};


//���� ���̾�� �̷��� ����,
enum class ESCUnit_TextureLayerFlag : UINT8
{
	Selected = 1 << 0,
	Main = 1 << 1,
	Top = 1 << 2,
	Effect = 1 << 3,
	Booster = 1 << 4,
	UseShadowSprite = 1 << 5,
	FlagCount = 6,
	All = 0b11111111
};

//�̰� �迭�� ����� �뵵
enum class ESCUnit_TextureLayer : UINT8
{
	Selected,
	MainShadow,
	Top,
	Effect,
	Booster,
	End
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

//Only POD
struct SCUnitInfo
{
	char UnitName[SCUnitNameLenMax];
	int MaxHP;
	int Shield;
	UINT16 DefaultArmor;
	char ArmorName[SCUnitNameLenMax];
	UINT8 ArmorPerUpgrade;

	UINT16 CostMineral;
	UINT16 CostGas;
	UINT16 BuildTime;


	ESCUnit_AttackType AttackType;

	char WeaponSurfaceName[SCUnitNameLenMax];
	UINT16 WeaponSurfaceDmg;
	UINT8 WeaponSurfacePerUpgrade;

	char WeaponAirName[SCUnitNameLenMax];
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
	//ESCUnit_TextureLayerFlag �����Ұ�
	UINT8 UsingTextureLayerFlags;
	char AnimationNames[(int)ESCUnit_Actions::End][SCUnitNameLenMax];

	//���߿� ���嵵 �߰��Ұ�


	SCUnitInfo()
	{
		memset(this, 0, sizeof(*this));
	}
};
//============================================================
#include "Marine.h"

#include "Resource/ResourceManager.h"

#include "Player2D.h"
#include "Component/SpriteComponent.h"
#include "Component/CameraComponent.h"
#include "Component/TargetArm.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderSphere2D.h"
#include "Component/ColliderOBB2D.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"
#include "Device.h"
#include "Bullet.h"
#include "Resource/Material/Material.h"
#include "Animation/Animation2D.h"
#include "../Component/UnitRootComponent.h"
#include "../Component/UnitSpriteComponent.h"

CMarine::CMarine()
{
	m_ObjectTypeName = "Marine";
}

CMarine::CMarine(const CMarine& Obj) :
	CGameObject(Obj)
{
}

CMarine::~CMarine()
{
}

bool CMarine::CDOPreload()
{
	CGameObject::CDOPreload();

	m_UnitRoot = CreateComponent<CUnitRootComponent>("UnitRoot");
	SetRootComponent(m_UnitRoot);


	m_MainSprite = CreateComponent<CUnitSpriteComponent>("MainSprite");
	m_UnitRoot->AddChild((CSceneComponent*)m_MainSprite);

	m_MainSprite->SetRelativeScale(100.f, 100.f);
	m_MainSprite->SetPivot(0.5f, 0.5f);

	m_Arm = CreateComponent<CTargetArm>("Arm");
	m_UnitRoot->AddChild(m_Arm);

	Resolution RS = CDevice::GetInst()->GetResolution();

	float Width = (float)RS.Width / 2.f;
	float Height = (float)RS.Height / 2.f;

	m_Arm->SetTargetOffset(Vector3(-Width, -Height, 0.f));

	m_Camera = CreateComponent<CCameraComponent>("Camera");
	m_Arm->AddChild(m_Camera);


	CMaterial* Material = m_MainSprite->GetMaterial(0);

	m_Anim = m_MainSprite->SetAnimation<CAnimation2D>("PlayerAnim");

	CTexture* UltraTex = CResourceManager::GetInst()->FindTexture("UltraLisk");

	if (!UltraTex)
		assert(0);

	CResourceManager::GetInst()->CreateAnimationSequence2D(
		"UltraIdle", UltraTex);

	CResourceManager::GetInst()->SetAnimationSequence2DFrameAllByTileNumber("UltraIdle", 17, 16);

	CResourceManager::GetInst()->AddAnimationSequence2DFrameByTileNumber("UltraIdle", EAnimation2DType::AtlasIndexed, 17, 16, 0, 1);

	CResourceManager::GetInst()->CreateAnimationSequence2D(
		"UltraMove", UltraTex);
	CResourceManager::GetInst()->AddAnimationSequence2DFrameByTileNumber("UltraMove", EAnimation2DType::AtlasIndexed, 17, 16, 1, 9);


	CResourceManager::GetInst()->CreateAnimationSequence2D(
		"UltraAttack", UltraTex);
	CResourceManager::GetInst()->AddAnimationSequence2DFrameByTileNumber("UltraAttack", EAnimation2DType::AtlasIndexed, 17, 16, 10, 5);

	return true;
}

bool CMarine::Init()
{
	CGameObject::Init();

	m_UnitRoot = (CSceneComponent*)FindComponent("UnitRoot");
	m_MainSprite = (CUnitSpriteComponent*)FindComponent("MainSprite");
	m_Anim = m_MainSprite->GetAnimation();
	m_Camera = (CCameraComponent*)FindComponent("Camera");
	m_Arm = (CTargetArm*)FindComponent("Arm");

	m_UnitRoot->SetWorldPosition(500.f, 500.f);

	m_Anim->AddAnimation("UltraIdle", "UltraIdle");
	m_Anim->SetLoop("UltraIdle", true);
	m_Anim->SetPlayTime("UltraIdle", 30.f);

	m_Anim->AddAnimation("UltraMove", "UltraMove");
	m_Anim->SetLoop("UltraMove", true);
	m_Anim->SetPlayTime("UltraMove", 1.f);

	m_Anim->AddAnimation("UltraAttack", "UltraAttack");
	m_Anim->SetLoop("UltraAttack", true);
	m_Anim->SetPlayTime("UltraAttack", 0.5f);

	m_Anim->SetCurrentAnimation("UltraAttack");

	return true;
}

void CMarine::Start()
{
	CGameObject::Start();

	if (m_Scene)
		m_Scene->GetCameraManager()->SetCurrentCamera(m_Camera);

	CInput::GetInst()->AddBindFunction<CMarine>("Rotation", Input_Type::Push,
		this, &CMarine::RotationInv, m_Scene);
	CInput::GetInst()->AddBindFunction<CMarine>("RotationInv", Input_Type::Push,
		this, &CMarine::Rotation, m_Scene);

	CInput::GetInst()->AddBindFunction<CMarine>("MoveUp", Input_Type::Push,
		this, &CMarine::MoveUp, m_Scene);
	CInput::GetInst()->AddBindFunction<CMarine>("MoveDown", Input_Type::Push,
		this, &CMarine::MoveDown, m_Scene);

	CInput::GetInst()->AddBindFunction<CMarine>("Fire", Input_Type::Down,
		this, &CMarine::Fire, m_Scene);
}





void CMarine::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	//m_Sprite->AddRelativeRotationZ(180.f * DeltaTime);

	//m_RightChild->AddRelativeRotationZ(360.f * DeltaTime);
}

void CMarine::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CMarine* CMarine::Clone() const
{
	return new CMarine(*this);
}

void CMarine::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CMarine::Load(FILE* File)
{
	CGameObject::Load(File);
}

void CMarine::MoveUp()
{
	m_UnitRoot->AddWorldPosition(m_UnitRoot->GetWorldAxis(AXIS_Y) * 300.f * g_DeltaTime);

	m_Anim->ChangeAnimation("UltraMove");
}

void CMarine::MoveDown()
{
	m_UnitRoot->AddWorldPosition(m_UnitRoot->GetWorldAxis(AXIS_Y) * -300.f * g_DeltaTime);

	m_Anim->ChangeAnimation("UltraMove");
}

void CMarine::Rotation()
{
	m_UnitRoot->AddWorldRotationZ(360.f * g_DeltaTime);
}

void CMarine::RotationInv()
{
	m_UnitRoot->AddWorldRotationZ(-360.f * g_DeltaTime);
}

void CMarine::Fire()
{
	m_Anim->ChangeAnimation("UltraAttack");

	CBullet* Bullet = m_Scene->CreateObject<CBullet>("Bullet");

	Bullet->SetWorldPosition(GetWorldPos());
	Bullet->SetWorldRotation(GetWorldRot());
	Bullet->SetCollisionProfileName("PlayerAttack");
}

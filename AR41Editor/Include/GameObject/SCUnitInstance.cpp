#include "SCUnitInstance.h"


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
#include "Component/SCUnitRootComponent.h"
#include "../Component/SCUltraliskComponent.h"

CSCUnitInstance::CSCUnitInstance()
{
	m_ObjectTypeName = "Marine";
}

CSCUnitInstance::CSCUnitInstance(const CSCUnitInstance& Obj) :
	CGameObject(Obj)
{
}

CSCUnitInstance::~CSCUnitInstance()
{
}

bool CSCUnitInstance::CDOPreload()
{
	CGameObject::CDOPreload();

	m_UnitRoot = CreateComponent<CSCUnitRootComponent>("UnitRoot");
	SetRootComponent(m_UnitRoot);

	//일단 테스트를 위해서 UltraliskComponent를 바로 생성함
	m_MainSprite = CreateComponent<CSCUltraliskComponent>("MainSprite");
	m_UnitRoot->AddChild((CSceneComponent*)(*m_MainSprite));

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


	//CMaterial* Material = m_MainSprite->GetMaterial(0);

	//m_Anim = m_MainSprite->SetAnimation<CAnimation2D>("PlayerAnim");

	//CTexture* UltraTex = CResourceManager::GetInst()->FindTexture("UltraLisk");

	//if (!UltraTex)
	//	assert(0);

	//CResourceManager::GetInst()->CreateAnimationSequence2D(
	//	"UltraIdle", UltraTex);

	////CResourceManager::GetInst()->SetAnimationSequence2DFrameAllByTileNumber("UltraIdle", 17, 16);

	//CResourceManager::GetInst()->AddAnimationSequence2DFrameByTileNumber("UltraIdle", EAnimation2DType::AtlasIndexed, 17, 16, 0, 1);

	//CResourceManager::GetInst()->CreateAnimationSequence2D(
	//	"UltraMove", UltraTex);
	//CResourceManager::GetInst()->AddAnimationSequence2DFrameByTileNumber("UltraMove", EAnimation2DType::AtlasIndexed, 17, 16, 1, 9);

	//CResourceManager::GetInst()->CreateAnimationSequence2D(
	//	"UltraAttack", UltraTex);
	//CResourceManager::GetInst()->AddAnimationSequence2DFrameByTileNumber("UltraAttack", EAnimation2DType::AtlasIndexed, 17, 16, 10, 5);

	return true;
}

bool CSCUnitInstance::Init()
{
	CGameObject::Init();

	m_UnitRoot = (CSCUnitRootComponent*)FindComponent("UnitRoot");
	m_MainSprite = (CSCUltraliskComponent*)FindComponent("MainSprite");

	m_Camera = (CCameraComponent*)FindComponent("Camera");
	m_Arm = (CTargetArm*)FindComponent("Arm");

	m_UnitRoot->SetWorldPosition(500.f, 500.f);


	//CAnimation2D* UnitBack = m_MainSprite->GetUnitAnimLayer(0);

	//UnitBack->AddAnimation("UltraIdle", "UltraIdle");
	//UnitBack->SetLoop("UltraIdle", EAnimLoopMethod::NormalLoop);
	//UnitBack->SetPlayTime("UltraIdle", 30.f);

	//UnitBack->AddAnimation("UltraMove", "UltraMove");
	//UnitBack->SetLoop("UltraMove", EAnimLoopMethod::NormalLoop);
	//UnitBack->SetPlayTime("UltraMove", 1.f);

	//UnitBack->AddAnimation("UltraAttack", "UltraAttack");
	//UnitBack->SetLoop("UltraAttack", EAnimLoopMethod::NormalLoop);
	//UnitBack->SetPlayTime("UltraAttack", 0.5f);

	//UnitBack->SetCurrentAnimation("UltraAttack");

	return true;
}

void CSCUnitInstance::Start()
{
	CGameObject::Start();

	if (m_Scene)
		m_Scene->GetCameraManager()->SetCurrentCamera(m_Camera);

	CInput::GetInst()->AddBindFunction<CSCUnitInstance>("Rotation", Input_Type::Push,
		this, &CSCUnitInstance::RotationInv, m_Scene);
	CInput::GetInst()->AddBindFunction<CSCUnitInstance>("RotationInv", Input_Type::Push,
		this, &CSCUnitInstance::Rotation, m_Scene);

	CInput::GetInst()->AddBindFunction<CSCUnitInstance>("MoveUp", Input_Type::Push,
		this, &CSCUnitInstance::MoveUp, m_Scene);
	CInput::GetInst()->AddBindFunction<CSCUnitInstance>("MoveDown", Input_Type::Push,
		this, &CSCUnitInstance::MoveDown, m_Scene);

	CInput::GetInst()->AddBindFunction<CSCUnitInstance>("Fire", Input_Type::Down,
		this, &CSCUnitInstance::Fire, m_Scene);
}





void CSCUnitInstance::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	//m_Sprite->AddRelativeRotationZ(180.f * DeltaTime);

	//m_RightChild->AddRelativeRotationZ(360.f * DeltaTime);
}

void CSCUnitInstance::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CSCUnitInstance* CSCUnitInstance::Clone() const
{
	return new CSCUnitInstance(*this);
}

void CSCUnitInstance::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CSCUnitInstance::Load(FILE* File)
{
	CGameObject::Load(File);
}

void CSCUnitInstance::MoveUp()
{
	m_UnitRoot->AddWorldPosition(m_UnitRoot->GetWorldAxis(AXIS_Y) * 300.f * g_DeltaTime);

	m_MainSprite->ChangeAnimationByAction(ESCUnit_Actions::Move);
}

void CSCUnitInstance::MoveDown()
{
	m_UnitRoot->AddWorldPosition(m_UnitRoot->GetWorldAxis(AXIS_Y) * -300.f * g_DeltaTime);

	m_MainSprite->ChangeAnimationByAction(ESCUnit_Actions::Move);
}

void CSCUnitInstance::Rotation()
{
	m_UnitRoot->AddWorldRotationZ(360.f * g_DeltaTime);
}

void CSCUnitInstance::RotationInv()
{
	m_UnitRoot->AddWorldRotationZ(-360.f * g_DeltaTime);
}

void CSCUnitInstance::Fire()
{
	m_MainSprite->ChangeAnimationByAction(ESCUnit_Actions::SurfaceAttack);

	CBullet* Bullet = m_Scene->CreateObject<CBullet>("Bullet");

	Bullet->SetWorldPosition(GetWorldPos());
	Bullet->SetWorldRotation(GetWorldRot());
	Bullet->SetCollisionProfileName("PlayerAttack");
}

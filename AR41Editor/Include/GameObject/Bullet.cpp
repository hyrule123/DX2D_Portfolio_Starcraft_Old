
#include "Bullet.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderSphere2D.h"

CBullet::CBullet()
{
	SetTypeID<CBullet>();

	m_ObjectTypeName = "Bullet";
}

CBullet::CBullet(const CBullet& Obj) :
	CGameObject(Obj)
{
	m_Body = static_cast<CColliderSphere2D*>(FindComponent("Body"));
	m_Sprite = static_cast<CSpriteComponent*>(FindComponent("Sprite"));
}

CBullet::~CBullet()
{
}

void CBullet::SetCollisionProfileName(const std::string& Name)
{
	m_Body->SetCollisionProfile(Name);
}

bool CBullet::CDOPreload()
{
	m_Body = CreateComponent<CColliderSphere2D>("Body");
	m_Sprite = CreateComponent<CSpriteComponent>("Sprite");
	m_Body->AddChild(m_Sprite);

	return true;
}

void CBullet::Start()
{
	CGameObject::Start();
}

bool CBullet::Init()
{
	CGameObject::Init();


	m_Body->SetCollisionCallback<CBullet>(ECollision_Result::Collision, this, &CBullet::CollisionBullet);

	m_Sprite->SetPivot(0.5f, 0.5f);
	m_Sprite->SetRelativeScale(50.f, 50.f);

	return true;
}

void CBullet::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	AddWorldPosition(GetWorldAxis(AXIS_Y) * 300.f * DeltaTime);
}

void CBullet::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CBullet* CBullet::Clone() const
{
	return new CBullet(*this);
}

void CBullet::CollisionBullet(const CollisionResult& result)
{
	Destroy();
}

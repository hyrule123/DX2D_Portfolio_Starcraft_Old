#include "UnitRootComponent.h"


CUnitRootComponent::CUnitRootComponent() :
	m_DirAngleUnit(11.25f),
	m_Direction()
{
	SetTypeID<CUnitRootComponent>();

	m_ComponentType = ComponentType::Scene;

	m_ComponentTypeName = "UnitRootComponent";
}


CUnitRootComponent::CUnitRootComponent(const CUnitRootComponent& component) :
	CSceneComponent(component),
	m_DirAngleUnit(component.m_DirAngleUnit),
	m_Direction(component.m_Direction)
{
}

CUnitRootComponent::~CUnitRootComponent()
{
}



void CUnitRootComponent::Start()
{
	CSceneComponent::Start();

	m_Transform->Start();

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->Start();
	}
}

bool CUnitRootComponent::Init()
{
	if (!CSceneComponent::Init())
		return false;

	return true;
}

void CUnitRootComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);

	//현재 z축은 시계방향으로 회전 시 -가 되는 형태임.
	float RotAngle = fmodf(m_Transform->GetWorldRot().z - 5.625f, 360.f);

	if (RotAngle > 0.f)
	{
		RotAngle = 360.f - RotAngle;
	}
	else
	{
		RotAngle = -RotAngle;
	}

	m_Direction = (int)(RotAngle / 11.25f);
}

void CUnitRootComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);


}

void CUnitRootComponent::Render()
{
	CSceneComponent::Render();

}

CUnitRootComponent* CUnitRootComponent::Clone() const
{
	return new CUnitRootComponent(*this);
}

void CUnitRootComponent::Save(FILE* File)
{
	CSceneComponent::Save(File);

}

void CUnitRootComponent::Load(FILE* File)
{
	CSceneComponent::Load(File);
}



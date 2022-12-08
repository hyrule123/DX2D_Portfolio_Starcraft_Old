#include "SCUnitRootComponent.h"


CSCUnitRootComponent::CSCUnitRootComponent() :
	m_DirAngleUnit(11.25f),
	m_Direction(),
	m_SCUnitInfo()
{

	m_ComponentType = ComponentType::Scene;

	m_ComponentTypeName = "UnitRootComponent";
}


CSCUnitRootComponent::CSCUnitRootComponent(const CSCUnitRootComponent& component) :
	CSceneComponent(component),
	m_DirAngleUnit(component.m_DirAngleUnit),
	m_Direction(component.m_Direction),
	m_SCUnitInfo(component.m_SCUnitInfo)
{
}

CSCUnitRootComponent::~CSCUnitRootComponent()
{
}



void CSCUnitRootComponent::Start()
{
	CSceneComponent::Start();
}

bool CSCUnitRootComponent::Init()
{
	if (!CSceneComponent::Init())
		return false;

	return true;
}

void CSCUnitRootComponent::Update(float DeltaTime)
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

void CSCUnitRootComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);
}

void CSCUnitRootComponent::Render()
{
	CSceneComponent::Render();

}

CSCUnitRootComponent* CSCUnitRootComponent::Clone() const
{
	return new CSCUnitRootComponent(*this);
}

void CSCUnitRootComponent::Save(FILE* File)
{
	CSceneComponent::Save(File);

	//fwrite(&m_SCUnitInfo)

	////같은 유닛은 모두 동일한 유닛정보를 공유한다.(얕은 복사
	//SCUnitInfoDa* m_SCUnitInfo;
	//float m_DirAngleUnit;	//유닛 스프라이트의 방향 1단위의 각도
	//INT8 m_Direction;
	


}

void CSCUnitRootComponent::Load(FILE* File)
{
	CSceneComponent::Load(File);
}



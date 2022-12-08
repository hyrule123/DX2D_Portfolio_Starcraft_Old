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

	//���� z���� �ð�������� ȸ�� �� -�� �Ǵ� ������.
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

	////���� ������ ��� ������ ���������� �����Ѵ�.(���� ����
	//SCUnitInfoDa* m_SCUnitInfo;
	//float m_DirAngleUnit;	//���� ��������Ʈ�� ���� 1������ ����
	//INT8 m_Direction;
	


}

void CSCUnitRootComponent::Load(FILE* File)
{
	CSceneComponent::Load(File);
}



#include "ObjectComponent.h"

CObjectComponent::CObjectComponent()
{
	m_ComponentType = ComponentType::Object;

	SetTypeID<CObjectComponent>();

	m_ComponentTypeName = "ObjectComponent";
}

CObjectComponent::CObjectComponent(const CObjectComponent& Obj)	:
	CComponent(Obj)
{
}

CObjectComponent::~CObjectComponent()
{
}

void CObjectComponent::Destroy()
{
}

void CObjectComponent::Start()
{
}

bool CObjectComponent::Init()
{
	return true;
}

void CObjectComponent::Update(float DeltaTime)
{
}

void CObjectComponent::PostUpdate(float DeltaTime)
{
}

void CObjectComponent::Render()
{
}

CObjectComponent* CObjectComponent::Clone() const
{
	return nullptr;
}

void CObjectComponent::Save(FILE* File)
{
	CComponent::Save(File);
}

void CObjectComponent::Load(FILE* File)
{
	CComponent::Load(File);
}

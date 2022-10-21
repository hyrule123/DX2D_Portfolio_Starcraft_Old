#include "UnitSpriteComponent.h"


#include "Resource/Material/Material.h"
#include "Resource/ResourceManager.h"
#include "Resource/Shader/Animation2DConstantBuffer.h"
#include "UnitRootComponent.h"

CUnitSpriteComponent::CUnitSpriteComponent():
	m_UnitRoot()
{
	SetTypeID<CUnitSpriteComponent>();

	m_ComponentTypeName = "UnitSpriteComponent";
}

CUnitSpriteComponent::CUnitSpriteComponent(const CUnitSpriteComponent& component) :
	CSpriteComponent(component)
{
}

CUnitSpriteComponent::~CUnitSpriteComponent()
{
}


void CUnitSpriteComponent::Start()
{
	CSpriteComponent::Start();

	if (!m_Parent || m_Parent->GetComponentTypeName() != "UnitRootComponent")
		assert(0);

	m_UnitRoot = (CUnitRootComponent*)m_Parent;
}

bool CUnitSpriteComponent::Init()
{
	if (!CSpriteComponent::Init())
		return false;

	m_vecMaterial[0]->SetColorKeyUnsignedChar(0, 0, 0);

	return true;
}

void CUnitSpriteComponent::Update(float DeltaTime)
{
	CSpriteComponent::Update(DeltaTime);
}

void CUnitSpriteComponent::PostUpdate(float DeltaTime)
{
	CSpriteComponent::PostUpdate(DeltaTime);
}

void CUnitSpriteComponent::Render()
{
	CSpriteComponent::Render();
}

CUnitSpriteComponent* CUnitSpriteComponent::Clone() const
{
	return new CUnitSpriteComponent(*this);
}

void CUnitSpriteComponent::Save(FILE* File)
{
	CSpriteComponent::Save(File);
}

void CUnitSpriteComponent::Load(FILE* File)
{
	CSpriteComponent::Load(File);
}

int CUnitSpriteComponent::GetRowIndex() const
{
	return m_UnitRoot->GetDirection();
}



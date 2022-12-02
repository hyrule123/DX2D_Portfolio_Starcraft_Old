#include "UltraliskComponent.h"

#include "Resource/Material/Material.h"
#include "Resource/ResourceManager.h"
#include "Resource/Shader/Animation2DConstantBuffer.h"
#include "Component/SCUnitRootComponent.h"

CUltraliskComponent::CUltraliskComponent() :
	m_UnitRoot()
{
	m_ComponentTypeName = "UnitSpriteComponent";
}

CUltraliskComponent::CUltraliskComponent(const CUltraliskComponent& component) :
	CSCUnitSpriteComponent(component),
	m_UnitRoot()
{

}

CUltraliskComponent::~CUltraliskComponent()
{
}




void CUltraliskComponent::Start()
{
	CSCUnitSpriteComponent::Start();

	if (!m_Parent || !m_Parent->CheckTypeID<CSCUnitRootComponent>())
		assert(0);

	m_UnitRoot = (CSCUnitRootComponent*)m_Parent;
}

bool CUltraliskComponent::Init()
{
	if (!CSCUnitSpriteComponent::Init())
		return false;

	m_vecMaterial[0]->SetColorKeyUnsignedChar(0, 0, 0);

	return true;
}

void CUltraliskComponent::Update(float DeltaTime)
{
	CSCUnitSpriteComponent::Update(DeltaTime);
}

void CUltraliskComponent::PostUpdate(float DeltaTime)
{
	CSCUnitSpriteComponent::PostUpdate(DeltaTime);
}

void CUltraliskComponent::Render()
{
	CSCUnitSpriteComponent::Render();
}

CUltraliskComponent* CUltraliskComponent::Clone() const
{
	return new CUltraliskComponent(*this);
}

void CUltraliskComponent::Save(FILE* File)
{
	CSCUnitSpriteComponent::Save(File);
}

void CUltraliskComponent::Load(FILE* File)
{
	CSCUnitSpriteComponent::Load(File);
}

int CUltraliskComponent::GetRowIndex() const
{
	return m_UnitRoot->GetDirection();
}



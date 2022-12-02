#include "SCUltraliskComponent.h"

#include "Resource/Material/Material.h"
#include "Resource/ResourceManager.h"
#include "Resource/Shader/Animation2DConstantBuffer.h"
#include "Component/SCUnitRootComponent.h"

CSCUltraliskComponent::CSCUltraliskComponent() 
{
	m_ComponentTypeName = "UnitSpriteComponent";
}

CSCUltraliskComponent::CSCUltraliskComponent(const CSCUltraliskComponent& component) :
	CSCUnitSpriteComponent(component)
{

}

CSCUltraliskComponent::~CSCUltraliskComponent()
{
}




void CSCUltraliskComponent::Start()
{
	CSCUnitSpriteComponent::Start();

	if (!m_Parent || !m_Parent->CheckTypeID<CSCUnitRootComponent>())
		assert(0);

	m_SCUnitRoot = (CSCUnitRootComponent*)m_Parent;
}

bool CSCUltraliskComponent::CDOPreload()
{
	//여기서 자신의 정보를 미리 만들어서 등록

	CSCUnitSpriteComponent::CDOPreload();

	return true;
}

bool CSCUltraliskComponent::Init()
{
	if (!CSCUnitSpriteComponent::Init())
		return false;

	m_vecMaterial[0]->SetColorKeyUnsignedChar(0, 0, 0);

	return true;
}

void CSCUltraliskComponent::Update(float DeltaTime)
{
	CSCUnitSpriteComponent::Update(DeltaTime);
}

void CSCUltraliskComponent::PostUpdate(float DeltaTime)
{
	CSCUnitSpriteComponent::PostUpdate(DeltaTime);
}

void CSCUltraliskComponent::Render()
{
	CSCUnitSpriteComponent::Render();
}

CSCUltraliskComponent* CSCUltraliskComponent::Clone() const
{
	return new CSCUltraliskComponent(*this);
}

void CSCUltraliskComponent::Save(FILE* File)
{
	CSCUnitSpriteComponent::Save(File);
}

void CSCUltraliskComponent::Load(FILE* File)
{
	CSCUnitSpriteComponent::Load(File);
}

int CSCUltraliskComponent::GetRowIndex() const
{
	return m_SCUnitRoot->GetDirection();
}




#include "PrimitiveComponent.h"
#include "../Resource/ResourceManager.h"

#include "../Render/RenderManager.h"
#include "../Resource/Material/Material.h"
#include "../Resource/Mesh/Mesh.h"

CPrimitiveComponent::CPrimitiveComponent()
{
	m_ComponentTypeName = "PrimitiveComponent";
}

CPrimitiveComponent::CPrimitiveComponent(const CPrimitiveComponent& component)	:
	CSceneComponent(component)
{
	m_Mesh = component.m_Mesh;

	size_t	Size = component.m_vecMaterial.size();

	for (size_t i = 0; i < Size; ++i)
	{
		CMaterial* Material = component.m_vecMaterial[i]->Clone();

		m_vecMaterial.push_back(Material);
	}
}

CPrimitiveComponent::~CPrimitiveComponent()
{
}

void CPrimitiveComponent::Start()
{
	CSceneComponent::Start();

	// Scene에 배치가 되고 Start가 호출되면 출력 목록으로 지정한다.
	CRenderManager::GetInst()->AddRenderList(this);
}

bool CPrimitiveComponent::Init()
{
	if (!CSceneComponent::Init())
		return false;

	return true;
}

void CPrimitiveComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);
}

void CPrimitiveComponent::RenderInstanced()
{
}

void CPrimitiveComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);
}

void CPrimitiveComponent::Render()
{
	CSceneComponent::Render();

	int	Size = (int)m_vecMaterial.size();

	for (int i = 0; i < Size; ++i)
	{
		m_vecMaterial[i]->SetMaterial();

		m_Mesh->Render(i);

		m_vecMaterial[i]->ResetMaterial();
	}
}

CPrimitiveComponent* CPrimitiveComponent::Clone() const
{
	return new CPrimitiveComponent(*this);
}

void CPrimitiveComponent::Save(FILE* File)
{
	CSceneComponent::Save(File);

	int	Length = (int)m_Mesh->GetName().length();

	fwrite(&Length, 4, 1, File);
	fwrite(m_Mesh->GetName().c_str(), 1, Length, File);

	int	MaterialCount = (int)m_vecMaterial.size();

	fwrite(&MaterialCount, 4, 1, File);

	for (int i = 0; i < MaterialCount; ++i)
	{
		m_vecMaterial[i]->Save(File);
	}
}

void CPrimitiveComponent::Load(FILE* File)
{
	CSceneComponent::Load(File);

	int	Length = 0;
	char	MeshName[256] = {};

	fread(&Length, 4, 1, File);
	fread(MeshName, 1, Length, File);

	SetMesh(MeshName);

	int	MaterialCount = 0;

	fread(&MaterialCount, 4, 1, File);

	m_vecMaterial.clear();

	for (int i = 0; i < MaterialCount; ++i)
	{
		CMaterial* Material = m_Mesh->GetMaterial(i);

		Material = Material->Clone();

		Material->SetScene(m_Scene);

		Material->Load(File);

		m_vecMaterial.push_back(Material);
	}
}


void CPrimitiveComponent::SetMesh(const std::string& Name)
{
	m_Mesh = CResourceManager::GetInst()->FindMesh(Name);

	if (m_Mesh)
		SetMeshSize(m_Mesh->GetMeshSize());

	m_vecMaterial.clear();

	int SlotCount = m_Mesh->GetSlotCount();

	for (int i = 0; i < SlotCount; ++i)
	{
		CMaterial* Material = m_Mesh->GetMaterial(i);

		m_vecMaterial.push_back(Material->Clone());
	}
}

void CPrimitiveComponent::SetMesh(CMesh* Mesh)
{
	m_Mesh = Mesh;

	if (m_Mesh)
		SetMeshSize(m_Mesh->GetMeshSize());

	int SlotCount = m_Mesh->GetSlotCount();

	for (int i = 0; i < SlotCount; ++i)
	{
		CMaterial* Material = m_Mesh->GetMaterial(i);

		m_vecMaterial.push_back(Material->Clone());
	}
}

void CPrimitiveComponent::SetMaterial(int Slot, const std::string& Name)
{
	CMaterial* Material = nullptr;
	

	Material = CResourceManager::GetInst()->FindMaterial(Name);

	m_vecMaterial[Slot] = Material->Clone();
}

void CPrimitiveComponent::SetMaterial(int Slot, CMaterial* Material)
{
	if (Material)
		m_vecMaterial[Slot] = Material->Clone();

	else
		m_vecMaterial[Slot] = Material;
}

void CPrimitiveComponent::AddMaterial(const std::string& Name)
{
	CMaterial* Material = CResourceManager::GetInst()->FindMaterial(Name);

	m_vecMaterial.push_back(Material->Clone());
}

void CPrimitiveComponent::AddMaterial(CMaterial* Material)
{
	m_vecMaterial.push_back(Material->Clone());
}

void CPrimitiveComponent::ClearMaterial()
{
	m_vecMaterial.clear();
}


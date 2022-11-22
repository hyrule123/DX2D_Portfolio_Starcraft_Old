#include "TileMapHolder.h"

#include "Resource/ResourceManager.h"

#include "Component/TileMapComponent.h"
#include "Component/SceneComponent.h"

CTileMapHolder::CTileMapHolder():
	m_TileMap()
{
	m_ObjectTypeName = "TileMapHolder";
}

CTileMapHolder::CTileMapHolder(const CTileMapHolder& Obj) :
	CGameObject(Obj),
	m_TileMap(Obj.m_TileMap)
{
}

CTileMapHolder::~CTileMapHolder()
{
}


bool CTileMapHolder::CDOPreload()
{
	CGameObject::CDOPreload();

	return true;
}

void CTileMapHolder::Start()
{
	CGameObject::Start();
}

bool CTileMapHolder::Init()
{
	CGameObject::Init();


	CResourceManager* Mgr = CResourceManager::GetInst();
	Mgr->LoadMapDataFromFile(TEXT("(4) Python 1.4.scx"));
	
	CTileMapComponent* TM = Mgr->LoadTileMap(TEXT("(4) Python 1.4.scx"));

	if (!TM)
		return false;

	AddSceneComponent(static_cast<CSceneComponent*>(TM));
	SetWorldPosition(0.f, 0.f);
	TM->Init();
	m_TileMap = TM;

	return true;
}

void CTileMapHolder::Update(float DeltaTime)
{
	if (m_LifeTime > 0.f)
	{
		m_LifeTime -= DeltaTime;

		if (m_LifeTime <= 0.f)
		{
			Destroy();
			return;
		}
	}

	size_t	Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecObjectComponent[i]->Update(DeltaTime);
	}

	if (m_RootComponent)
		m_RootComponent->Update(DeltaTime);
}

void CTileMapHolder::PostUpdate(float DeltaTime)
{
	size_t	Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecObjectComponent[i]->PostUpdate(DeltaTime);
	}

	if (m_RootComponent)
		m_RootComponent->PostUpdate(DeltaTime);
}

CTileMapHolder* CTileMapHolder::Clone() const
{
	return new CTileMapHolder(*this);
}

void CTileMapHolder::Save(FILE* File)
{
	CRef::Save(File);

	fwrite(&m_LifeTime, 4, 1, File);

	{
		int	Length = (int)m_RootComponent->GetComponentTypeName().length();

		fwrite(&Length, 4, 1, File);
		fwrite(m_RootComponent->GetComponentTypeName().c_str(), 1, Length, File);

		m_RootComponent->Save(File);
	}

	{
		int	Count = (int)m_vecObjectComponent.size();

		fwrite(&Count, 4, 1, File);

		auto	iter = m_vecObjectComponent.begin();
		auto	iterEnd = m_vecObjectComponent.end();

		for (; iter != iterEnd; ++iter)
		{
			int	Length = (int)(*iter)->GetComponentTypeName().length();

			fwrite(&Length, 4, 1, File);
			fwrite((*iter)->GetComponentTypeName().c_str(), 1, Length, File);

			(*iter)->Save(File);
		}
	}
}

void CTileMapHolder::Load(FILE* File)
{
	CRef::Load(File);

	fread(&m_LifeTime, 4, 1, File);

	{
		int	Length = 0;
		char	TypeName[256] = {};

		fread(&Length, 4, 1, File);
		fread(TypeName, 1, Length, File);

		if (!m_RootComponent)
		{
			CComponent* CDO = CComponent::FindCDO(TypeName);

			m_RootComponent = (CSceneComponent*)CDO->Clone();

			m_RootComponent->SetOwner(this);
			m_RootComponent->SetScene(m_Scene);

			m_RootComponent->Load(File);

			m_RootComponent->AddOwner();
		}

		else
		{
			m_RootComponent->SetOwner(this);
			m_RootComponent->SetScene(m_Scene);

			m_RootComponent->Load(File);
		}
	}

	{
		int	Count = 0;

		fread(&Count, 4, 1, File);

		bool	Empty = m_vecObjectComponent.empty();

		for (int i = 0; i < Count; ++i)
		{
			int	Length = 0;
			char	TypeName[256] = {};

			fread(&Length, 4, 1, File);
			fread(TypeName, 1, Length, File);

			if (Empty)
			{
				// CDO를 얻어온다.
				CComponent* CDO = CComponent::FindCDO(TypeName);

				CComponent* Component = CDO->Clone();

				Component->SetOwner(this);
				Component->SetScene(m_Scene);
				Component->Load(File);

				m_vecObjectComponent.push_back((CObjectComponent*)Component);
			}

			else
			{
				m_vecObjectComponent[i]->SetOwner(this);
				m_vecObjectComponent[i]->SetScene(m_Scene);
				m_vecObjectComponent[i]->Load(File);
			}
		}
	}
}
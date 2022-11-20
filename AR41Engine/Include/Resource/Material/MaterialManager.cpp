
#include "MaterialManager.h"
#include "../../Scene/SceneManager.h"

CMaterialManager::CMaterialManager()
{
}

CMaterialManager::~CMaterialManager()
{
}

bool CMaterialManager::Init()
{
	CMaterial* Material = CreateMaterial<CMaterial>("DefaultColor");

	Material->SetShader("SpriteColorShader");

	Material = CreateMaterial<CMaterial>("DefaultTexture");

	Material->SetShader("SpriteShader");

	Material->AddTexture(0, (int)EShaderBufferType::Pixel, "DefaultTexture",
		TEXT("teemo.png"));

	Material = CreateMaterial<CMaterial>("DefaultTileMapBack");

	Material->SetShader("TileMapBackShader");
	Material->SetRenderState("DepthLessEqual");

	return true;
}

CMaterial* CMaterialManager::FindMaterial(const std::string& Name)
{
	auto	iter = m_mapMaterial.find(Name);

	if (iter == m_mapMaterial.end())
		return nullptr;

	return iter->second;
}

void CMaterialManager::ReleaseMaterial(const std::string& Name)
{
	auto	iter = m_mapMaterial.find(Name);

	if (iter != m_mapMaterial.end())
	{
		if (iter->second->GetRefCount() == 1)
			m_mapMaterial.erase(iter);
	}
}

void CMaterialManager::DeleteUnused()
{
	auto iter = m_mapMaterial.begin();
	auto iterEnd = m_mapMaterial.end();
	CSceneManager* SceneMgr = CSceneManager::GetInst();

	while (iter != iterEnd)
	{
		//씬에서 사용되지 않고 필수 리소스로 설정되어 있지 않을 경우 지워준다. -> RefCount == 0 이 되므로 알아서 제거
		if (iter->second->GetRefCount() == 1 && !(iter->second->GetEssential()))
		{
			iter = m_mapMaterial.erase(iter);
			continue;
		}

		SceneMgr->AddSceneResource(iter->second);
		++iter;
	}
}

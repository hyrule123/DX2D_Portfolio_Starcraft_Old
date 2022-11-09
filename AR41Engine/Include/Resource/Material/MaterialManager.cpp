
#include "MaterialManager.h"

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

	Material = CreateMaterial<CMaterial>("TileMap");

	std::vector<const TCHAR*>	vecFileNames;

	for (int i = 0; i <= 379; ++i)
	{
		TCHAR* FileName = new TCHAR[MAX_PATH];

		memset(FileName, 0, sizeof(TCHAR) * MAX_PATH);

		wsprintf(FileName, TEXT("Diablos_Lair_Floor_TRS/Diablos_Lair_Floor_%d.png"), i);

		vecFileNames.push_back(FileName);
	}

	Material->SetShader("TileMapShader");
	Material->AddTextureArray(10, (int)EShaderBufferType::Pixel, "DefaultTileIsometric",
		vecFileNames);
	Material->SetRenderState("DepthLessEqual");
	Material->SetRenderState("AlphaBlend");

	for (int i = 0; i <= 379; ++i)
	{
		SAFE_DELETE_ARRAY(vecFileNames[i]);
	}

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

	while (iter != iterEnd)
	{
		//씬에서 사용되지 않고 필수 리소스로 설정되어 있지 않을 경우 지워준다. -> RefCount == 0 이 되므로 알아서 제거
		if (iter->second->GetRefCount() == 1 && !(iter->second->GetEssential()))
		{
			m_mapMaterial.erase(iter);
			continue;
		}

		++iter;
	}
}

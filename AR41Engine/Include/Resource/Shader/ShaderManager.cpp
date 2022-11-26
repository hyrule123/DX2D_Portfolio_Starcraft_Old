
#include "ShaderManager.h"

#include "../../Scene/SceneManager.h"

//Shaders
#include "SpriteColorShader.h"
#include "SpriteShader.h"
#include "ColliderShader.h"
#include "ColliderImageShader.h"
#include "ConstantBuffer.h"
#include "ColliderConstantBuffer.h"
#include "UIShader.h"
#include "UIProgressBarShader.h"
#include "TileMapShader.h"
#include "TileMapConstantBuffer.h"
#include "TileMapBackShader.h"
#include "SCUnitShader.h"


CShaderManager::CShaderManager()
{
}

CShaderManager::~CShaderManager()
{
	SAFE_DELETE(m_ColliderCBuffer);
}



bool CShaderManager::Init()
{
	CreateShader<CSpriteColorShader>("SpriteColorShader");


	CreateShader<CSpriteShader>("SpriteShader");


	CreateShader<CColliderShader>("ColliderShader");


	CreateShader<CColliderImageShader>("ColliderImageShader");


	CreateShader<CUIShader>("UIShader");


	CreateShader<CUIProgressBarShader>("UIProgressBarShader");


	CreateShader<CTileMapShader>("TileMapShader");


	CreateShader<CTileMapBackShader>("TileMapBackShader");


	CreateShader<CSCUnitShader>("SCUnitShader");


	CreateConstantBuffer("Transform", sizeof(TransformCBuffer), 0);
	CreateConstantBuffer("Material", sizeof(MaterialCBuffer), 1);
	CreateConstantBuffer("Animation2D", sizeof(Animation2DCBuffer), 2);
	CreateConstantBuffer("Collider", sizeof(ColliderCBuffer), 10);
	CreateConstantBuffer("UI", sizeof(UICBuffer), 10);
	CreateConstantBuffer("UIProgressBar", sizeof(UIProgressBarCBuffer), 11);
	CreateConstantBuffer("TileMap", sizeof(TileMapCBuffer), 10);
	CreateConstantBuffer("SCUnit", sizeof(SCUnitCBuffer), 12);

	m_ColliderCBuffer = new CColliderConstantBuffer;

	m_ColliderCBuffer->Init();

	return true;
}

CShader* CShaderManager::FindShader(const std::string& Name)
{
	auto	iter = m_mapShader.find(Name);

	if (iter == m_mapShader.end())
		return nullptr;

	return iter->second;
}

void CShaderManager::ReleaseShader(const std::string& Name)
{
	auto	iter = m_mapShader.find(Name);

	if (iter != m_mapShader.end())
	{
		if (iter->second->GetRefCount() == 1)
			m_mapShader.erase(iter);
	}
}

CConstantBuffer* CShaderManager::CreateConstantBuffer(const std::string& Name, int Size,
	int Register, int ShaderBufferType)
{
	CConstantBuffer* Buffer = FindConstantBuffer(Name);

	if (Buffer)
		return Buffer;

	Buffer = new CConstantBuffer;

	Buffer->SetName(Name);
	Buffer->SetResourceType(EResourceType::CBuffer);

	if (!Buffer->Init(Size, Register, ShaderBufferType))
	{
		SAFE_RELEASE(Buffer);
		return nullptr;
	}

	m_mapCBuffer.insert(std::make_pair(Name, Buffer));

	return Buffer;
}

CConstantBuffer* CShaderManager::FindConstantBuffer(const std::string& Name)
{
	auto	iter = m_mapCBuffer.find(Name);

	if (iter == m_mapCBuffer.end())
		return nullptr;

	return iter->second;
}

void CShaderManager::DeleteUnused()
{
	{
		auto iter = m_mapShader.begin();
		auto iterEnd = m_mapShader.end();

		CSceneManager* SceneMgr = CSceneManager::GetInst();

		while (iter != iterEnd)
		{
			//씬에서 사용되지 않고 필수 리소스로 설정되어 있지 않을 경우 지워준다. -> RefCount == 0 이 되므로 알아서 제거
			if (iter->second->GetRefCount() == 1 && !(iter->second->GetEssential()))
			{
				iter = m_mapShader.erase(iter);
				continue;
			}

			SceneMgr->AddSceneResource(iter->second);
			++iter;
		}
	}

	{
		auto iter = m_mapCBuffer.begin();
		auto iterEnd = m_mapCBuffer.end();

		CSceneManager* SceneMgr = CSceneManager::GetInst();

		while (iter != iterEnd)
		{
			//씬에서 사용되지 않고 필수 리소스로 설정되어 있지 않을 경우 지워준다. -> RefCount == 0 이 되므로 알아서 제거
			if (iter->second->GetRefCount() == 1 && !(iter->second->GetEssential()))
			{
				iter = m_mapCBuffer.erase(iter);
				continue;
			}

			SceneMgr->AddSceneResource(iter->second);
			++iter;
		}
	}
}


#include "SceneResource.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/GameResource.h"
#include "../CDO.h"

CSceneResource::CSceneResource() :
	m_Owner()
{
}

CSceneResource::~CSceneResource()
{
	//클래스가 제거됨 -> 씬이 더이상 쓰이지 않음 -> 자신이 쓰던 리소스의 참조 카운트를 내린 다음 사용하지 않는 자원을 제거하도록 요청.
	m_vecResource.clear();
	m_vecCDO.clear();

	CResourceManager::GetInst()->DeleteUnused();
	CCDO::DeleteUnusedCDO();
}

bool CSceneResource::Init()
{
	return true;
}



void CSceneResource::AddGameResource(CGameResource* ResPtr)
{
	m_vecResource.push_back(ResPtr);
}

void CSceneResource::AddCDO(CCDO* CDOPtr)
{
	m_vecCDO.push_back(CDOPtr);
}


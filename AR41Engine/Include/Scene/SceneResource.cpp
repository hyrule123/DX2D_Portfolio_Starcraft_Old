
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
	//Ŭ������ ���ŵ� -> ���� ���̻� ������ ���� -> �ڽ��� ���� ���ҽ��� ���� ī��Ʈ�� ���� ���� ������� �ʴ� �ڿ��� �����ϵ��� ��û.
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


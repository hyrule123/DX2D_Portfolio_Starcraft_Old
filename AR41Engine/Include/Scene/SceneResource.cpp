
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

	CResourceManager::GetInst()->DeleteUnused();
}

bool CSceneResource::Init()
{
	return true;
}



void CSceneResource::AddGameResource(CGameResource* ResPtr)
{
	m_vecResource.push_back(ResPtr);
}


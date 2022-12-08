
#include "SceneManager.h"

#include "../CDO.h"
#include "SceneInfo.h"
#include "../GameObject/GameObject.h"
#include "../Component/SceneComponent.h"
#include "../Animation/Animation2D.h"
#include "../UI/UIWindow.h"
#include "../UI/UIWidget.h"

#include "../Resource/ResourceManager.h"
#include "../Resource/GameResource.h"

DEFINITION_SINGLE(CSceneManager)

CSceneManager::CSceneManager()	:
	m_Scene(nullptr),
	m_NextScene(nullptr),
	m_PrevScene(nullptr)
{
}

CSceneManager::~CSceneManager()
{
	CCDO::ClearAll();

	SAFE_DELETE(m_NextScene);
	SAFE_DELETE(m_Scene);
	SAFE_DELETE(m_PrevScene);
}

bool CSceneManager::Init()
{
	m_Scene = new CScene;

	return true;
}

bool CSceneManager::Update(float DeltaTime)
{
	if (false == m_Scene->m_Start)
	{
		m_Scene->Start();
	}

	m_Scene->Update(DeltaTime);

	return ChangeScene();
}

bool CSceneManager::PostUpdate(float DeltaTime)
{
	if (!m_Scene->m_Start)
	{
		m_Scene->Start();
	}

	m_Scene->PostUpdate(DeltaTime);

	return ChangeScene();
}

bool CSceneManager::Collision(float DeltaTime)
{
	m_Scene->Collision(DeltaTime);

	return ChangeScene();
}

bool CSceneManager::ChangeScene()
{
	if (m_NextScene)
	{
		if (m_NextScene->m_Change)
		{
			//���� ���� �ϴ� ������ ���� ��ü
			m_PrevScene = m_Scene;
			m_Scene = m_NextScene;
			m_NextScene = nullptr;

			//�� �ε� ����. ���� �߰� �ε� ���� ������̶�� ������� ��Ƽ������� �ε��� ������.
			m_Scene->GetSceneInfo()->SceneChangeComplete();

			//�� �ε尡 �Ϸ�Ǹ� ���� ���� �ν��Ͻ��� �����ؼ� ���ҽ� ����
			//���� ��Ƽ�����带 ������� ��쿡�� �̹� ���ŵǾ� ���� ����. �׷��� ���� ��� ���⼭ ����
			SAFE_DELETE(m_PrevScene);
			CResourceManager::GetInst()->DeleteUnused();

			//PreLoaded Object�� ����
			CCDO::DeleteUnusedPLO();

			return true;
		}
	}

	return false;
}



void CSceneManager::CreateNextScene(bool AutoChange)
{
	SAFE_DELETE(m_NextScene);

	m_NextScene = new CScene;

	m_NextScene->m_Change = AutoChange;
}

void CSceneManager::ChangeNextScene()
{
	m_NextScene->m_Change = true;
}

void CSceneManager::DeletePrevScene()
{
	SAFE_DELETE(m_PrevScene);
}

void CSceneManager::AddSceneResource(CGameResource* ResPtr)
{
	if (m_NextScene)
	{
		ResPtr->SetScene(m_NextScene);
		m_NextScene->GetSceneResource()->AddGameResource(ResPtr);
	}
		

	else if (m_Scene)
	{
		ResPtr->SetScene(m_Scene);
		m_Scene->GetSceneResource()->AddGameResource(ResPtr);
	}
		
}

void CSceneManager::AddPLO(CCDO* PLO)
{
	if (m_NextScene)
	{
		m_NextScene->AddPLO(PLO);
	}
	else if (m_Scene)
	{
		m_Scene->AddPLO(PLO);
	}

	
}







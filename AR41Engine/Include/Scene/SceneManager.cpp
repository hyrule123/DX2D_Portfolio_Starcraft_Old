
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
			//이전 씬을 일단 빼놓고 씬을 교체
			m_PrevScene = m_Scene;
			m_Scene = m_NextScene;
			m_NextScene = nullptr;

			//씬 로드 시작. 만약 중간 로딩 씬을 사용중이라면 여기부터 멀티스레드로 로딩을 시작함.
			m_Scene->GetSceneInfo()->SceneChangeComplete();

			//씬 로드가 완료되면 이전 씬의 인스턴스를 제거해서 리소스 관리
			//만약 멀티스레드를 사용중일 경우에는 이미 제거되어 있을 것임. 그렇지 않을 경우 여기서 제거
			SAFE_DELETE(m_PrevScene);
			CResourceManager::GetInst()->DeleteUnused();

			//PreLoaded Object도 제거
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







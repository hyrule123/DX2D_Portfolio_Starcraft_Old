
#include "SceneManager.h"
#include "SceneInfo.h"
#include "../GameObject/GameObject.h"
#include "../Component/SceneComponent.h"
#include "../Animation/Animation2D.h"
#include "../UI/UIWindow.h"
#include "../UI/UIWidget.h"

DEFINITION_SINGLE(CSceneManager)

CSceneManager::CSceneManager()	:
	m_Scene(nullptr),
	m_NextScene(nullptr)
{
}

CSceneManager::~CSceneManager()
{
	SAFE_DELETE(m_NextScene);
	SAFE_DELETE(m_Scene);
}

bool CSceneManager::Init()
{
	m_Scene = new CScene;

	return true;
}

bool CSceneManager::Update(float DeltaTime)
{
	if (!m_Scene->m_Start)
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
			SAFE_DELETE(m_Scene);
			m_Scene = m_NextScene;
			m_NextScene = nullptr;

			m_Scene->GetSceneInfo()->SceneChangeComplete();

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

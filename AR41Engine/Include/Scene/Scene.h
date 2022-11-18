#pragma once

#include "SceneInfo.h"
#include "SceneResource.h"
#include "CameraManager.h"
#include "SceneCollision.h"
#include "SceneViewport.h"
#include "NavigationManager.h"

class CScene
{
	friend class CSceneManager;

private:
	CScene();
	~CScene();

public:
	void Start();
	bool Init();
	void Update(float DeltaTime);
	void PostUpdate(float DeltaTime);
	void Collision(float DeltaTime);
	void Save(const char* FullPath);
	void Load(const char* FullPath);
	void GetAllGameObjectHierarchyName(std::vector<HierarchyObjectName>& vecName);


private:
	bool		m_Change;
	bool		m_Start;
	CSceneInfo* m_SceneInfo;
	CSceneResource* m_SceneResource;
	CCameraManager* m_CameraManager;
	CSceneCollision* m_CollisionManager;
	CSceneViewport* m_Viewport;
	CNavigationManager* m_NavManager;
	std::list<CSharedPtr<class CGameObject>>	m_ObjList;
	std::string	m_Name;
	std::function<void(float)>	m_LoadingCallback;

	//씬에서 사용하기로 한 PLO 목록을 등록한다(참조카운트용)
	std::list<CSharedPtr<class CCDO>> m_listUsingPLO;

public:
	inline void SetName(const std::string& Name);
	inline const std::string& GetName()	const;
	inline CSceneInfo* GetSceneInfo()	const;
	inline CSceneResource* GetSceneResource()	const;
	inline CCameraManager* GetCameraManager()	const;
	inline CSceneCollision* GetCollisionManager()	const;
	inline CSceneViewport* GetViewport()	const;
	inline CNavigationManager* GetNavigationManager()	const;

public:
	class CGameObject* FindObject(const std::string& Name);
	void AddPLO(class CCDO* CDO);


public:
	template <typename T>
	bool CreateSceneInfo();

	template <typename T>
	T* CreateObject(const std::string& Name);

	template <typename T>
	void SetLoadingCallback(T* Obj, void(T::* Func)(float));
};

inline void CScene::SetName(const std::string& Name)
{
	m_Name = Name;
}

inline const std::string& CScene::GetName()	const
{
	return m_Name;
}

inline CSceneInfo* CScene::GetSceneInfo()	const
{
	return m_SceneInfo;
}

inline CSceneResource* CScene::GetSceneResource()	const
{
	return m_SceneResource;
}

inline CCameraManager* CScene::GetCameraManager()	const
{
	return m_CameraManager;
}

inline CSceneCollision* CScene::GetCollisionManager()	const
{
	return m_CollisionManager;
}

inline CSceneViewport* CScene::GetViewport()	const
{
	return m_Viewport;
}

inline CNavigationManager* CScene::GetNavigationManager()	const
{
	return m_NavManager;
}



template <typename T>
bool CScene::CreateSceneInfo()
{
	SAFE_DELETE(m_SceneInfo);

	m_SceneInfo = CCDO::CloneCDO<T>();

	m_SceneInfo->m_Owner = this;

	if (!m_SceneInfo->Init())
	{
		SAFE_DELETE(m_SceneInfo);
		return false;
	}

	return true;
}

template <typename T>
T* CScene::CreateObject(const std::string& Name)
{	
	T* Obj = CCDO::ClonePLO<T>();


	Obj->SetName(Name);
	Obj->SetScene(this);

	if (!Obj->Init())
	{
		SAFE_RELEASE(Obj);
		return nullptr;
	}

	m_ObjList.push_back(Obj);

	if (m_Start)
		Obj->Start();

	return Obj;
}


template <typename T>
void CScene::SetLoadingCallback(T* Obj, void(T::* Func)(float))
{
	m_LoadingCallback = std::bind(Func, Obj, std::placeholders::_1);
}


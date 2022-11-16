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

public:
	void SetName(const std::string& Name)
	{
		m_Name = Name;
	}

	const std::string& GetName()	const
	{
		return m_Name;
	}

	CSceneInfo* GetSceneInfo()	const
	{
		return m_SceneInfo;
	}

	CSceneResource* GetSceneResource()	const
	{
		return m_SceneResource;
	}

	CCameraManager* GetCameraManager()	const
	{
		return m_CameraManager;
	}

	CSceneCollision* GetCollisionManager()	const
	{
		return m_CollisionManager;
	}

	CSceneViewport* GetViewport()	const
	{
		return m_Viewport;
	}

	CNavigationManager* GetNavigationManager()	const
	{
		return m_NavManager;
	}


public:
	void Start();
	bool Init();
	void Update(float DeltaTime);
	void PostUpdate(float DeltaTime);
	void Collision(float DeltaTime);
	void Save(const char* FullPath);
	void Load(const char* FullPath);
	void GetAllGameObjectHierarchyName(std::vector<HierarchyObjectName>& vecName);
	

public:
	class CGameObject* FindObject(const std::string& Name);

public:
	template <typename T>
	bool CreateSceneInfo()
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
	T* CreateObject(const std::string& Name)
	{
		T* Obj = CCDO::CloneCDO<T>();

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
	void SetLoadingCallback(T* Obj, void(T::* Func)(float))
	{
		m_LoadingCallback = std::bind(Func, Obj, std::placeholders::_1);
	}
};


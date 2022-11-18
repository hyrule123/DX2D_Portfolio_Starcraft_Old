#pragma once

#include "Scene.h"

class CSceneManager
{
public:
	bool Init();
	bool Update(float DeltaTime);
	bool PostUpdate(float DeltaTime);
	bool Collision(float DeltaTime);

private:
	CScene* m_Scene;
	CScene* m_NextScene;
	CScene* m_PrevScene;


public:
	inline CScene* GetScene()	const;
	inline CScene* GetNextScene()	const;


private:
	bool ChangeScene();

public:
	void CreateNextScene(bool AutoChange = true);
	void ChangeNextScene();
	void DeletePrevScene();
	void AddSceneResource(class CGameResource* ResPtr);
	void AddPLO(class CCDO* PLO);
	

public:
	template <typename T>
	bool CreateSceneInfo(bool Current = true);

	DECLARE_SINGLE(CSceneManager)
};


template <typename T>
bool CSceneManager::CreateSceneInfo(bool Current)
{
	if (Current)
		return m_Scene->CreateSceneInfo<T>();

	return m_NextScene->CreateSceneInfo<T>();
}

inline CScene* CSceneManager::GetScene()	const
{
	return m_Scene;
}

inline CScene* CSceneManager::GetNextScene()	const
{
	return m_NextScene;
}



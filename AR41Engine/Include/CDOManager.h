#pragma once

#include "EngineInfo.h"

class CCDOManager
{
	friend class CResourceManager;

private:
	CCDOManager();
	~CCDOManager();

	void Init();
	void CreateObjectCDO();
	void CreateSceneInfoCDO();
	void CreateComponentCDO();
	void Create();

private:
	static std::unordered_map<size_t, void*>	m_mapCDO;


public:
	static void AddCDO()


public:
	static void AddComponentCDO(const std::string& Name, CComponent* CDO)
	{
		m_mapComponentCDO.insert(std::make_pair(Name, CDO));
	}

	static CComponent* FindCDO(const std::string& Name)
	{
		auto	iter = m_mapComponentCDO.find(Name);

		if (iter == m_mapComponentCDO.end())
			return nullptr;

		return iter->second;
	}

public:
	inline static void AddObjectCDO(const std::string& Name, class CGameObject* CDO);
	inline static CGameObject* FindObjectCDO(const std::string& Name);


private:
	static void CreateCDO();

public:

	template <typename T>
	static T* CreateObjectCDO()
	{
		T* CDO = new T;

		

		CGameObject::AddObjectCDO(Name, (CGameObject*)CDO);

		return CDO;
	}



	static void AddSceneInfoCDO(const std::string& Name, CSceneInfo* Info)
	{
		m_mapSceneInfoCDO.insert(std::make_pair(Name, Info));
	}

	static CSceneInfo* FindSceneInfoCDO(const std::string& Name)
	{
		auto	iter = m_mapSceneInfoCDO.find(Name);

		if (iter == m_mapSceneInfoCDO.end())
			return nullptr;

		return iter->second;
	}

	template <typename T>
	static T* CreateUIWindowCDO(const std::string& Name)
	{
		T* CDO = new T;

		CDO->Init();

		CUIWindow::AddUIWindowCDO(Name, (CUIWindow*)CDO);

		return CDO;
	}

	template <typename T>
	static T* CreateUIWidgetCDO(const std::string& Name)
	{
		T* CDO = new T;

		CDO->Init();

		CUIWidget::AddUIWidgetCDO(Name, (CUIWidget*)CDO);

		return CDO;
	}

};


inline void CCDOManager::AddObjectCDO(const std::string& Name, CGameObject* CDO)
{
	m_mapObjectCDO.insert(std::make_pair(Name, CDO));
}

inline CGameObject* CCDOManager::FindObjectCDO(const std::string& Name)
{
	auto	iter = m_mapObjectCDO.find(Name);

	if (iter == m_mapObjectCDO.end())
		return nullptr;

	return iter->second;
}
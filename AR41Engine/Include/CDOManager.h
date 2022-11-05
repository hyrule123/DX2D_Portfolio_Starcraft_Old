#pragma once

#include "EngineInfo.h"
#include "CDO.h"

//Object, SceneInfo, UIWindow, UIWidget, 

class CCDOManager
{

public:
	bool Init();

private:
	static std::unordered_map<size_t, CCDO*>	m_mapCDO;

	template <typename T>
	static T* FindCDO();

	static CCDO* FindCDO(size_t hash_code);

	static CCDO* FindCDO(const std::string& Name);

public:
	template <typename T>
	static bool CreateCDO(const std::string& Name = "");

	template <typename T>
	static T* CloneCDO();

	static class CCDO* CloneCDO(const std::string& Name);

	static class CCDO* CloneCDO(size_t hash_code);


	DECLARE_SINGLE(CCDOManager);
};



template<typename T>
inline T* CCDOManager::CloneCDO()
{
	auto iter = m_mapCDO.find(typeid(T).hash_code());

	if (iter == m_mapCDO.end())
		return nullptr;

	return static_cast<T*>(iter->second->Clone());
}



template<typename T>
inline bool CCDOManager::CreateCDO(const std::string& Name)
{

	size_t hashcode = typeid(T).hash_code();

	//이미 만들어져 있으면 return
	if (m_mapCDO.find(hashcode) != m_mapCDO.end())
		return true;

	T* CDO = new T;

	CDO->SetTypeID<T>();
	if (!Name.empty())
		CDO->SetName(Name);

	if (!CDO->Init())
	{
		delete CDO;
		return false;
	}


	m_mapCDO.insert(std::make_pair(hashcode, static_cast<CCDO*>(CDO)));

	return true;
}



template<typename T>
inline T* CCDOManager::FindCDO()
{
	auto iter = m_mapCDO.find(typeid(T).hash_code());
	
	if (iter == m_mapCDO.end())
		return nullptr;

	return static_cast<T*>(iter->second());
}

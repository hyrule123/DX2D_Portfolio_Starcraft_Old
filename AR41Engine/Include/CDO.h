#pragma once

#include "EngineInfo.h"

class CCDO
{
protected:
	CCDO();
	CCDO(const CCDO& CDO);
	virtual ~CCDO() = 0;

	virtual bool Init();
	virtual CCDO* Clone();

	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

	std::string	m_Name;
	std::string	m_TypeName;
	size_t	m_TypeID;


public:
	inline size_t GetTypeID()	const;
	inline const std::string& GetTypeName()	const;
	inline const std::string& GetName()	const;
	inline void SetName(const std::string& Name);

//내부 변수 저장 및 탐색 용도
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

	template <typename T>
	inline void SetTypeID();
};



size_t CCDO::GetTypeID()	const
{
	return m_TypeID;
}

const std::string& CCDO::GetTypeName()	const
{
	return m_TypeName;
}

const std::string& CCDO::GetName()	const
{
	return m_Name;
}

void CCDO::SetName(const std::string& Name)
{
	m_Name = Name;
}


template <typename T>
void CCDO::SetTypeID()
{
	// 타입 이름을 문자열로 얻어온다.
	m_TypeName = typeid(T).name();

	// 타입의 고유한 번호를 얻어온다.
	m_TypeID = typeid(T).hash_code();
}


template<typename T>
inline T* CCDO::CloneCDO()
{
	auto iter = m_mapCDO.find(typeid(T).hash_code());

	if (iter == m_mapCDO.end())
		return nullptr;

	return static_cast<T*>(iter->second->Clone());
}



template<typename T>
inline bool CCDO::CreateCDO(const std::string& Name)
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
inline T* CCDO::FindCDO()
{
	auto iter = m_mapCDO.find(typeid(T).hash_code());

	if (iter == m_mapCDO.end())
		return nullptr;

	return static_cast<T*>(iter->second());
}
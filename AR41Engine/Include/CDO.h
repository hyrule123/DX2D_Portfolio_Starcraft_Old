#pragma once

#include "Ref.h"

class CCDO : public CRef
{
	friend class CSceneManager;

protected:
	CCDO();
	CCDO(const CCDO& CDO);
	virtual ~CCDO();

	//사전 로드해놔도 문제없는 요소들은 이 메소드를 재정의해서 사용
	//cf)Init()에서는 다른 인스턴스를 참조하는 작업을 진행한다.
	virtual bool CDOPreload();
	virtual CCDO* Clone();

	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

	bool m_Essential;	//씬 전환시에도 유지할 인스턴스

	inline void SetEssential(const bool& b);
	inline const bool& GetEssential() const;

//내부 변수 저장 및 탐색 용도
private:
	static std::unordered_map<size_t, CSharedPtr<CCDO>>	m_mapCDO;

	template <typename T>
	static T* FindCDO();

	static CCDO* FindCDO(size_t hash_code);

	static CCDO* FindCDO(const std::string& Name);

	static void AddCDO(CCDO* CDO);

public:
	template <typename T>
	static bool CreateCDO(const std::string& Name = "", const bool& Essential = false);

	template <typename T>
	static T* CloneCDO();

	static class CCDO* CloneCDO(const std::string& Name);

	static class CCDO* CloneCDO(size_t hash_code);
};



inline void CCDO::SetEssential(const bool& b)
{
	m_Essential = b;
}

inline const bool& CCDO::GetEssential() const
{
	return m_Essential;
}



template<typename T>
inline T* CCDO::CloneCDO()
{
	auto iter = m_mapCDO.find(typeid(T).hash_code());

	if (iter == m_mapCDO.end())
	{
		if(!CreateCDO<T>())
			return nullptr;

		return static_cast<T*>(FindCDO<T>()->Clone());
	}
	
	return static_cast<T*>(iter->second->Clone());
}


template<typename T>
inline bool CCDO::CreateCDO(const std::string& Name, const bool& Essential)
{

	size_t hashcode = typeid(T).hash_code();

	//이미 만들어져 있으면 return
	if (m_mapCDO.find(hashcode) != m_mapCDO.end())
		return true;

	CSharedPtr<T> CDO = new T;

	CDO->SetTypeID<T>();
	
	CDO->SetEssential(Essential);

	if (!Name.empty())
		CDO->SetName(Name);

	if (!CDO->CDOPreload())
	{
		delete CDO;
		return false;
	}

	CCDO* Cast = static_cast<CCDO*>(CDO);

	m_mapCDO.insert(std::make_pair(hashcode, static_cast<CCDO*>(Cast)));
	AddCDO(Cast);

	return true;
}




template<typename T>
inline T* CCDO::FindCDO()
{
	auto iter = m_mapCDO.find(typeid(T).hash_code());

	if (iter == m_mapCDO.end())
		return nullptr;

	return static_cast<T*>(iter->second.Get());
}
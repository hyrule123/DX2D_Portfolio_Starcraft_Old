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
	virtual bool Init();


	//사전 로드해야 할 리소스가 있을 경우 여기서 m_vecRequiredResource를 동적 할당하고 값을 채워나감
	virtual bool CDOPreload();
	virtual CCDO* Clone() const = 0;

	virtual void Save(FILE* File);
	virtual void Load(FILE* File);


protected:
	//하나만 생성해서 공유해서 사용하면 되므로 포인터로 선언
	std::vector<RequiredResource>* m_vecRequiredResource;



//내부 변수 저장 및 탐색 용도
private:
	static std::unordered_map<std::string, CSharedPtr<CCDO>>	m_mapCDO;

	template <typename T>
	static T* FindCDO();

	static CCDO* FindCDO(const size_t& hash_code);
	static CCDO* FindCDO(const std::string& Name);
	static CCDO* FindCDOByFileName(const std::string& FileName);


public:
	template <typename T>
	static bool CreateCDO();

	template <typename T>
	static T* CloneCDO();
	static class CCDO* CloneCDO(const std::string& ClassName);
	static class CCDO* CloneCDOByFileName(const std::string& FileName);
	static class CCDO* CloneCDO(const size_t& hash_code);
};




template<typename T>
inline T* CCDO::CloneCDO()
{
	auto iter = m_mapCDO.find(typeid(T).name());

	if (iter == m_mapCDO.end())
	{
		if(!CreateCDO<T>())
			return nullptr;

		return static_cast<T*>(FindCDO<T>()->Clone());
	}
	
	return static_cast<T*>(iter->second->Clone());
}


template<typename T>
inline bool CCDO::CreateCDO()
{
	std::string N = typeid(T).name();

	//이미 만들어져 있으면 return true
	if (m_mapCDO.find(N) != m_mapCDO.end())
		return true;

	CSharedPtr<T> CDO = new T;

	CDO->SetTypeID<T>();


	if (!CDO->CDOPreload())
	{
		delete CDO;
		return false;
	}

	CCDO* Cast = static_cast<CCDO*>(CDO);

	m_mapCDO.insert(std::make_pair(N, static_cast<CCDO*>(Cast)));

	return true;
}


template<typename T>
inline T* CCDO::FindCDO()
{
	auto iter = m_mapCDO.find(typeid(T).name());

	if (iter == m_mapCDO.end())
		return nullptr;

	return static_cast<T*>(iter->second.Get());
}
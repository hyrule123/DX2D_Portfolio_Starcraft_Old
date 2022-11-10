#pragma once

#include "Ref.h"





class CCDO : public CRef
{
	friend class CSceneManager;

protected:
	CCDO();
	CCDO(const CCDO& CDO);
	virtual ~CCDO();

	//���� �ε��س��� �������� ��ҵ��� �� �޼ҵ带 �������ؼ� ���
	//cf)Init()������ �ٸ� �ν��Ͻ��� �����ϴ� �۾��� �����Ѵ�.
	virtual bool Init();


	//���� �ε��ؾ� �� ���ҽ��� ���� ��� ���⼭ m_vecRequiredResource�� ���� �Ҵ��ϰ� ���� ä������
	virtual bool CDOPreload();
	virtual CCDO* Clone() const = 0;

	virtual void Save(FILE* File);
	virtual void Load(FILE* File);


protected:
	//�ϳ��� �����ؼ� �����ؼ� ����ϸ� �ǹǷ� �����ͷ� ����
	std::vector<RequiredResource>* m_vecRequiredResource;



//���� ���� ���� �� Ž�� �뵵
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

	//�̹� ������� ������ return true
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
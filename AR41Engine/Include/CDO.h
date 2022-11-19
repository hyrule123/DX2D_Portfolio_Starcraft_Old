#pragma once

#include "Ref.h"


class CCDO : public CRef
{
	friend class CSceneManager;

protected:
	CCDO();
	CCDO(const CCDO& CDO);
	virtual ~CCDO();

	bool LoadMetaData();

public:
	//���� �ε��ؾ� �� ���ҽ��� ���� ��� ���⼭ m_vecRequiredResource�� ���� �Ҵ��ϰ� ���� ä������
	virtual bool CDOPreload();

	//���� �ε��س��� �������� ��ҵ��� �� �޼ҵ带 �������ؼ� ���
	//cf)Init()������ �ٸ� �ν��Ͻ��� �����ϴ� �۾��� �����Ѵ�.
	virtual bool Init();

	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

	virtual CCDO* Clone() const = 0;

protected:
	//�ϳ��� �����ؼ� �����ؼ� ����ϸ� �ǹǷ� ���������ͷ� ����
	std::vector<RequiredResource> m_vecRequiredResource;
	//std::vector<RequiredComponent>* m_vecRequiredComponent;


//���� ���� ���� �� Ž�� �뵵
private:
	static std::unordered_map<std::string, CSharedPtr<CCDO>>	m_mapCDO;

	//���ڿ� ���� ���ҽ��� ����ü ��ȣ�� ����. �ε��� �� ����.
	//EngineSetting���� �ʱ�ȭ
	static std::unordered_map<std::string, enum class EResourceType> m_mapResType;

	template <typename T>
	static T* FindCDO();

	static CCDO* FindCDO(const size_t& hash_code);
	static CCDO* FindCDO(const std::string& Name);
	static CCDO* FindCDOByFileName(const std::string& FileName);

public:
	static void AddResourceType(const std::string& Name, EResourceType&& ResType);
	static EResourceType FindResourceType(const std::string& Name);

	template <typename T>
	static bool CreateCDO();

	template <typename T>
	static T* CloneCDO();
	static class CCDO* CloneCDO(const std::string& ClassName);
	static class CCDO* CloneCDOByFileName(const std::string& FileName);
	static class CCDO* CloneCDO(const size_t& hash_code);
	static void ClearAll();



//Preloaded Objects
protected:
	//����غ� �Ϸ�� ������Ʈ�� ����(Clone() -> Init() ���ָ� ��� ����)
	static std::unordered_map<std::string, CSharedPtr<class CCDO>> m_mapPreLoadObject;

public:
	static class CCDO* FindPLO(const std::string& ClassName);

	template <typename T>
	static T* ClonePLO();
	static class CCDO* ClonePLO(const std::string& ClassName);


	template <typename T>
	static T* CreatePLO();
	static class CCDO* CreatePLO(const std::string& ClassName);

	static void DeleteUnusedPLO();

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

	CDO->LoadMetaData();

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


template<typename T>
inline T* CCDO::ClonePLO()
{
	return static_cast<T*>(ClonePLO(typeid(T).name()));
}

template<typename T>
inline T* CCDO::CreatePLO()
{
	return static_cast<T*>(CreatePLO(typeid(T).name()));
}
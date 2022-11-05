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
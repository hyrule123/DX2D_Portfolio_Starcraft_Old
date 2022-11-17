#pragma once

#include "EngineInfo.h"

class CRef
{
public:
	CRef();
	CRef(const CRef& ref);
	virtual ~CRef();

protected:
	std::string	m_Name;
	std::string	m_TypeName;
	std::string m_FileName;
	size_t	m_TypeID;
	int		m_RefCount;
	bool	m_Enable;	// 활성, 비활성
	bool	m_Active;	// 살아 있는지 죽었는지


public:
	void AddRef();
	int Release();
	inline void SetName(const std::string& Name);
	inline const std::string& GetName()	const;


public:
	template <typename T>
	inline void SetTypeID();

	inline size_t GetTypeID()	const;

	template <typename T>
	bool CheckTypeID()	const;

	inline const std::string& GetTypeName()	const;
	inline int GetRefCount()	const;
	inline bool GetEnable()	const;
	inline bool GetActive()	const;
	inline void SetEnable(bool Enable);
	inline virtual void Destroy();




public:
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
};


template <typename T>
void CRef::SetTypeID()
{
	// 타입 이름을 문자열로 얻어온다.
	m_TypeName = typeid(T).name();

	m_FileName.assign(m_TypeName.begin() + 7, m_TypeName.end());

	// 타입의 고유한 번호를 얻어온다.
	m_TypeID = typeid(T).hash_code();
}

size_t CRef::GetTypeID()	const
{
	return m_TypeID;
}

template<typename T>
inline bool CRef::CheckTypeID() const
{
	return m_TypeID == typeid(T).hash_code();
}


const std::string& CRef::GetTypeName()	const
{
	return m_TypeName;
}

inline int CRef::GetRefCount() const
{
	return m_RefCount;
}

inline bool CRef::GetEnable() const
{
	return m_Enable;
}

inline bool CRef::GetActive() const
{
	return m_Active;
}

inline void CRef::SetEnable(bool Enable)
{
	m_Enable = Enable;
}

inline void CRef::Destroy()
{
	m_Active = false;
}


inline void CRef::SetName(const std::string& Name)
{
	m_Name = Name;
}

inline const std::string& CRef::GetName()	const
{
	return m_Name;
}
#pragma once

#include "CDO.h"

class CRef: public CCDO
{
public:
	CRef();
	CRef(const CRef& ref);
	virtual ~CRef();

protected:
	int		m_RefCount;
	bool	m_Enable;	// Ȱ��, ��Ȱ��
	bool	m_Active;	// ��� �ִ��� �׾�����

public:
	void AddRef();
	int Release();


public:
	int GetRefCount()	const
	{
		return m_RefCount;
	}

	bool GetEnable()	const
	{
		return m_Enable;
	}

	bool GetActive()	const
	{
		return m_Active;
	}


	void SetEnable(bool Enable)
	{
		m_Enable = Enable;
	}
	template <typename T>
	bool CheckTypeID()	const
	{
		return m_TypeID == typeid(T).hash_code();
	}

	virtual void Destroy()
	{
		m_Active = false;
	}



public:
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
};


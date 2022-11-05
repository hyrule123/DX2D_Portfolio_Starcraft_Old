#include "Ref.h"

CRef::CRef() :
	m_RefCount(0),
	m_Enable(true),
	m_Active(true)
{
}

CRef::CRef(const CRef& ref) :
	CCDO(ref),
	m_RefCount(0),
	m_Enable(ref.m_Enable),
	m_Active(ref.m_Active)
{
}

CRef::~CRef()
{
}

void CRef::AddRef()
{
	++m_RefCount;
}

int CRef::Release()
{
	--m_RefCount;

	if (m_RefCount <= 0)
	{
		delete this;
		return 0;
	}

	return m_RefCount;
}

void CRef::Save(FILE* File)
{
	CCDO::Save(File);
}

void CRef::Load(FILE* File)
{
	CCDO::Load(File);
}


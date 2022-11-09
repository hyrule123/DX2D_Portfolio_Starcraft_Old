#include "Ref.h"

CRef::CRef() :
	m_RefCount(0),
	m_Enable(true),
	m_Active(true),
	m_TypeID()
{
}

CRef::CRef(const CRef& ref):
	m_RefCount(0),
	m_Enable(ref.m_Enable),
	m_Active(ref.m_Active),
	m_Name(ref.m_Name),
	m_TypeID(ref.m_TypeID),
	m_TypeName(ref.m_TypeName)
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

}

void CRef::Load(FILE* File)
{
}


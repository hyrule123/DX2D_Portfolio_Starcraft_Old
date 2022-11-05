#include "ThreadManager.h"

DEFINITION_SINGLE(CThreadManager)

CThreadManager::CThreadManager()
{
}

CThreadManager::~CThreadManager()
{
	{
		auto	iter = m_mapThread.begin();
		auto	iterEnd = m_mapThread.end();

		for (; iter != iterEnd; ++iter)
		{
			SAFE_DELETE(iter->second);
		}

		m_mapThread.clear();
	}

	{
		auto	iter = m_mapCriticalSection.begin();
		auto	iterEnd = m_mapCriticalSection.end();

		for (; iter != iterEnd; ++iter)
		{
			// 전역함수를 호출하겠다는 의미이다.
			::DeleteCriticalSection(iter->second);

			SAFE_DELETE(iter->second);
		}

		m_mapCriticalSection.clear();
	}
}

bool CThreadManager::Init()
{
	// 로딩용 크리티컬 섹션 생성
	CreateCriticalSection("Loading");

	return true;
}

bool CThreadManager::Suspend(const std::string& Name)
{
	CThread* Thread = FindThread(Name);

	if (!Thread)
		return false;

	Thread->Suspend();

	return true;
}

bool CThreadManager::Resume(const std::string& Name)
{
	CThread* Thread = FindThread(Name);

	if (!Thread)
		return false;

	Thread->Resume();

	return true;
}

bool CThreadManager::ReStart(const std::string& Name)
{
	CThread* Thread = FindThread(Name);

	if (!Thread)
		return false;

	Thread->ReStart();

	return true;
}

bool CThreadManager::Delete(const std::string& Name)
{
	auto	iter = m_mapThread.find(Name);

	if (iter == m_mapThread.end())
		return false;

	SAFE_DELETE(iter->second);

	m_mapThread.erase(iter);

	return true;
}

bool CThreadManager::Start(const std::string& Name)
{
	CThread* Thread = FindThread(Name);

	if (!Thread)
		return false;

	Thread->Start();

	return true;
}

CThread* CThreadManager::FindThread(const std::string& Name)
{
	auto	iter = m_mapThread.find(Name);

	if (iter == m_mapThread.end())
		return nullptr;

	return iter->second;
}

bool CThreadManager::CreateCriticalSection(const std::string& Name)
{
	CRITICAL_SECTION* Crt = FindCriticalSection(Name);

	if (Crt)
		return false;

	Crt = new CRITICAL_SECTION;

	InitializeCriticalSection(Crt);

	m_mapCriticalSection.insert(std::make_pair(Name, Crt));

	return true;
}

bool CThreadManager::DeleteCriticalSection(const std::string& Name)
{
	auto	iter = m_mapCriticalSection.find(Name);

	if (iter == m_mapCriticalSection.end())
		return false;

	::DeleteCriticalSection(iter->second);

	SAFE_DELETE(iter->second);

	m_mapCriticalSection.erase(iter);

	return true;
}

CRITICAL_SECTION* CThreadManager::FindCriticalSection(const std::string& Name)
{
	auto	iter = m_mapCriticalSection.find(Name);

	if(iter == m_mapCriticalSection.end())
		return nullptr;

	return iter->second;
}

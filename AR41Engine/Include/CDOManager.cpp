#include "CDOManager.h"



DEFINITION_SINGLE(CCDOManager)
std::unordered_map<size_t, CCDO*>	CCDOManager::m_mapCDO;

CCDOManager::CCDOManager()
{
}

CCDOManager::~CCDOManager()
{
}

bool CCDOManager::Init()
{

	return true;
}

CCDO* CCDOManager::FindCDO(size_t hash_code)
{
	auto iter = m_mapCDO.find(hash_code);

	if (iter == m_mapCDO.end())
		return nullptr;

	return static_cast<CCDO*>(iter->second);
}

CCDO* CCDOManager::FindCDO(const std::string& Name)
{
	auto iter = m_mapCDO.begin();
	auto iterEnd = m_mapCDO.end();

	while (iter != iterEnd)
	{
		if (iter->second->GetName() == Name)
			return iter->second;

		++iter;
	}

	return nullptr;
}

CCDO* CCDOManager::CloneCDO(const std::string& Name)
{
	CCDO* CDO = FindCDO(Name);

	if (!CDO)
		return nullptr;

	return CDO;
}

CCDO* CCDOManager::CloneCDO(size_t hash_code)
{
	CCDO* CDO = FindCDO(hash_code);

	if (!CDO)
		return nullptr;
	
	return CDO;
}
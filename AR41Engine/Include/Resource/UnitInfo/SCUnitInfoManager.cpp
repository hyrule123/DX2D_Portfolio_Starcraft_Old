#include "SCUnitInfoManager.h"
#include "SCUnitInfoData.h"

CSCUnitInfoManager::CSCUnitInfoManager()
{
}

CSCUnitInfoManager::~CSCUnitInfoManager()
{
	for (auto& iter : m_mapSCUnitInfo)
	{
		SAFE_DELETE(iter.second);
	}
}
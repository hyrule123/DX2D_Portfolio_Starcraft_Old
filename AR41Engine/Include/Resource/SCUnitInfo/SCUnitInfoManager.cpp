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


void CSCUnitInfoManager::AddUnitInfo(const std::string& Name, const SCUnitInfo& UnitInfo)
{
	if (FindSCUnitInfo(Name))
		return;

	CSCUnitInfoData* Data = new CSCUnitInfoData(UnitInfo);

	m_mapSCUnitInfo.insert(std::make_pair(Name, Data));
}

SCUnitInfo* CSCUnitInfoManager::GetCloneSCUnitInfo(const std::string& Name)
{
	CSCUnitInfoData* Data = FindSCUnitInfo(Name);

	if (!Data)
		return nullptr;

	SCUnitInfo* Info = Data->CloneData();

	return Info;
}

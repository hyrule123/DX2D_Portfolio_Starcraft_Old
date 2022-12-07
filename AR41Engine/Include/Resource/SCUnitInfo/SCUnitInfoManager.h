#pragma once

#include "../../SCUnitInfo.h"

#include "SCUnitInfoData.h"

class CSCUnitInfoManager
{
public:
	CSCUnitInfoManager();
	~CSCUnitInfoManager();

private:
	std::unordered_map<std::string, CSCUnitInfoData*> m_mapSCUnitInfo;
	inline CSCUnitInfoData* FindSCUnitInfo(const std::string& Name);

public:
	//유닛정보 등록
	void AddUnitInfo(const std::string& Name, const SCUnitInfo& UnitInfo);

	//유닛정보의 '복사본'을 리턴(직접 할당 해제 필요)
	SCUnitInfo* GetCloneSCUnitInfo(const std::string& Name);
};


inline CSCUnitInfoData* CSCUnitInfoManager::FindSCUnitInfo(const std::string& Name)
{
	auto iter = m_mapSCUnitInfo.find(Name);

	if (iter == m_mapSCUnitInfo.end())
		return nullptr;

	return iter->second;
}
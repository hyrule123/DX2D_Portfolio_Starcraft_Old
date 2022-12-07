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
	//�������� ���
	void AddUnitInfo(const std::string& Name, const SCUnitInfo& UnitInfo);

	//���������� '���纻'�� ����(���� �Ҵ� ���� �ʿ�)
	SCUnitInfo* GetCloneSCUnitInfo(const std::string& Name);
};


inline CSCUnitInfoData* CSCUnitInfoManager::FindSCUnitInfo(const std::string& Name)
{
	auto iter = m_mapSCUnitInfo.find(Name);

	if (iter == m_mapSCUnitInfo.end())
		return nullptr;

	return iter->second;
}
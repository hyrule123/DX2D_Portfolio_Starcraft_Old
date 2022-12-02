#pragma once

#include "../../SCUnitInfo.h"

class CSCUnitInfoManager
{
public:
	CSCUnitInfoManager();
	~CSCUnitInfoManager();

private:
	std::unordered_map<std::string, class CSCUnitInfoData*> m_mapSCUnitInfo;

public:
};


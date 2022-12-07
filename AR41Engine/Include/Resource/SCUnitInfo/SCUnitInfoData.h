#pragma once
#include "../GameResource.h"
#include "../../SCUnitInfo.h"

class CSCUnitInfoData :
    public CGameResource
{
private:
    CSCUnitInfoData();

public:
    CSCUnitInfoData(const SCUnitInfo& UnitInfo);
    virtual ~CSCUnitInfoData();

private:
    SCUnitInfo m_SCUnitInfoOriginal;

public:
    void RegisterSCUnitInfo(const SCUnitInfo& UnitInfo);
    SCUnitInfo* CloneData() const;
};


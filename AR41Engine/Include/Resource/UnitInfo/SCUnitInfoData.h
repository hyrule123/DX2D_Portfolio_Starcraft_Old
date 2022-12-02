#pragma once
#include "../GameResource.h"
#include "../../SCUnitInfo.h"

class CSCUnitInfoData :
    public CGameResource
{
public:
    CSCUnitInfoData();
    virtual ~CSCUnitInfoData();

private:
    SCUnitInfo m_SCUnitInfo;

public:
    void RegisterSCUnitInfo(SCUnitInfo&& UnitInfo);
    SCUnitInfo* CloneData() const;
};


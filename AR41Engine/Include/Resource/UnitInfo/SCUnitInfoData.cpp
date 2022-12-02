#include "SCUnitInfoData.h"

CSCUnitInfoData::CSCUnitInfoData():
	m_SCUnitInfo{}
{
}

CSCUnitInfoData::~CSCUnitInfoData()
{
}

void CSCUnitInfoData::RegisterSCUnitInfo(SCUnitInfo&& UnitInfo)
{
	m_SCUnitInfo = UnitInfo;
}

SCUnitInfo* CSCUnitInfoData::CloneData() const
{
	SCUnitInfo* UnitInfo = new SCUnitInfo;

	*UnitInfo = m_SCUnitInfo;

	return UnitInfo;
}

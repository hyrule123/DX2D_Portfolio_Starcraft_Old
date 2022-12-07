#include "SCUnitInfoData.h"

CSCUnitInfoData::CSCUnitInfoData():
	m_SCUnitInfoOriginal()
{
}



CSCUnitInfoData::CSCUnitInfoData(const SCUnitInfo& UnitInfo):
	m_SCUnitInfoOriginal(UnitInfo)
{
}

CSCUnitInfoData::~CSCUnitInfoData()
{
}

void CSCUnitInfoData::RegisterSCUnitInfo(const SCUnitInfo& UnitInfo)
{
	size_t size = sizeof(SCUnitInfo);
	memcpy_s(&m_SCUnitInfoOriginal, size, &UnitInfo, size);
}

SCUnitInfo* CSCUnitInfoData::CloneData() const
{
	SCUnitInfo* UnitInfo = new SCUnitInfo;

	size_t size = sizeof(SCUnitInfo);
	memcpy_s(UnitInfo, size, &m_SCUnitInfoOriginal, size);

	return UnitInfo;
}

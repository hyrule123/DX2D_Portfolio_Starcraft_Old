#include "CDO.h"

CCDO::CCDO():
	m_TypeID()
{
}

CCDO::CCDO(const CCDO& CDO):
	m_Name(CDO.m_Name),
	m_TypeID(CDO.m_TypeID),
	m_TypeName(CDO.m_TypeName)
{
}


bool CCDO::Init()
{
	return true;
}

CCDO* CCDO::Clone()
{
	return nullptr;
}

void CCDO::Save(FILE* File)
{
	int	Length = (int)m_Name.length();

	fwrite(&Length, sizeof(int), 1, File);
	fwrite(m_Name.c_str(), sizeof(char), Length, File);

	Length = (int)m_TypeName.length();

	fwrite(&Length, sizeof(int), 1, File);
	fwrite(m_TypeName.c_str(), sizeof(char), Length, File);

	fwrite(&m_TypeID, sizeof(size_t), 1, File);

}

void CCDO::Load(FILE* File)
{
	int	Length = 0;
	char	Text[256] = {};

	fread(&Length, sizeof(int), 1, File);
	fread(Text, sizeof(char), Length, File);
	m_Name = Text;

	memset(Text, 0, 256);

	fread(&Length, sizeof(int), 1, File);
	fread(Text, sizeof(char), Length, File);
	m_TypeName = Text;

	fread(&m_TypeID, sizeof(size_t), 1, File);
}



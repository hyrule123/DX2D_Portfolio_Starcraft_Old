#include "CDO.h"
#include "Scene/SceneManager.h"

std::unordered_map<size_t, CSharedPtr<CCDO>>	CCDO::m_mapCDO;

CCDO::CCDO():
	m_Essential()
{
}

CCDO::CCDO(const CCDO& CDO):
	CRef(CDO),
	m_Essential(CDO.m_Essential)
{
}

CCDO::~CCDO()
{
}


bool CCDO::CDOPreload()
{
	return true;
}

CCDO* CCDO::Clone()
{
	return new CCDO(*this);
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


CCDO* CCDO::FindCDO(size_t hash_code)
{
	auto iter = m_mapCDO.find(hash_code);

	if (iter == m_mapCDO.end())
		return nullptr;

	return static_cast<CCDO*>(iter->second);
}

CCDO* CCDO::FindCDO(const std::string& Name)
{
	auto iter = m_mapCDO.begin();
	auto iterEnd = m_mapCDO.end();

	while (iter != iterEnd)
	{
		if (iter->second->GetName() == Name)
			return iter->second;

		++iter;
	}

	return nullptr;
}

void CCDO::AddCDO(CCDO* CDO)
{
	CSceneManager::GetInst()->AddCDO(CDO);
}

CCDO* CCDO::CloneCDO(const std::string& Name)
{
	CCDO* CDO = FindCDO(Name);

	if (!CDO)
		return nullptr;

	return CDO;
}

CCDO* CCDO::CloneCDO(size_t hash_code)
{
	CCDO* CDO = FindCDO(hash_code);

	if (!CDO)
		return nullptr;

	return CDO;
}


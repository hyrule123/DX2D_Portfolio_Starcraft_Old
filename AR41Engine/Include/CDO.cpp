#include "CDO.h"

#include "Resource/ResourceManager.h"

std::unordered_map<std::string, CSharedPtr<CCDO>>	CCDO::m_mapCDO;

CCDO::CCDO():
	m_vecRequiredResource()
{
}

CCDO::CCDO(const CCDO& CDO):
	CRef(CDO),
	m_vecRequiredResource(CDO.m_vecRequiredResource)
{
}

CCDO::~CCDO()
{
	if (m_vecRequiredResource)
		SAFE_DELETE(m_vecRequiredResource);
}



bool CCDO::Init()
{
	if (m_vecRequiredResource)
	{
		if (!(*m_vecRequiredResource).empty())
		{
			size_t size = (*m_vecRequiredResource).size();

			for (size_t i = 0; i < size; ++i)
			{
				switch ((*m_vecRequiredResource)[i].ResType)
				{
				case EResourceType::Mesh:
					break;
				case EResourceType::Shader:
					break;
				case EResourceType::CBuffer:
					break;
				case EResourceType::Texture:
				{
					CResourceManager::GetInst()->LoadTexture((*m_vecRequiredResource)[i].Name, (*m_vecRequiredResource)[i].FileName.c_str(), (*m_vecRequiredResource)[i].PathName);
					break;
				}
				case EResourceType::Material:
					break;
				case EResourceType::Animation:
					break;
				case EResourceType::Sound:
					break;
				case EResourceType::Font:
					break;
				case EResourceType::FontCollection:
					break;
				case EResourceType::Map:
					break;
				case EResourceType::End:
					break;
				default:
					break;
				}
			}
		}
	}



	return true;
}

bool CCDO::CDOPreload()
{
	return true;
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


CCDO* CCDO::FindCDO(const size_t& hash_code)
{
	auto iter = m_mapCDO.begin();
	auto iterEnd = m_mapCDO.end();

	while (iter != iterEnd)
	{
		if (iter->second->GetTypeID() == hash_code)
			return iter->second;

		++iter;
	}

	return nullptr;
}

CCDO* CCDO::FindCDO(const std::string& Name)
{
	auto iter = m_mapCDO.find(Name);

	if (iter == m_mapCDO.end())
		return nullptr;

	return iter->second;
}

CCDO* CCDO::FindCDOByFileName(const std::string& FileName)
{
	std::string N = "class C";
	N += FileName;

	auto iter = m_mapCDO.begin();
	auto iterEnd = m_mapCDO.end();

	while (iter != iterEnd)
	{
		if (iter->second->GetName() == N)
			return iter->second;

		++iter;
	}

	return nullptr;
}

CCDO* CCDO::CloneCDO(const std::string& ClassName)
{
	CCDO* CDO = FindCDO(ClassName);

	if (!CDO)
		return nullptr;

	return CDO->Clone();
}

CCDO* CCDO::CloneCDOByFileName(const std::string& FileName)
{
	std::string N = "class C";
	N += FileName;
	
	CCDO* CDO = FindCDO(FileName);

	if (!CDO)
		return nullptr;

	return CDO->Clone();
}

CCDO* CCDO::CloneCDO(const size_t& hash_code)
{
	CCDO* CDO = FindCDO(hash_code);

	if (!CDO)
		return nullptr;

	return CDO->Clone();
}


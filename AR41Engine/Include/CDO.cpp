#include "CDO.h"

#include "Resource/ResourceManager.h"
#include "PathManager.h"
#include "CSVEditor.h"

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

bool CCDO::LoadMetaData()
{
	const PathInfo* Path = CPathManager::GetInst()->FindPath(CDO_PATH);

	tstring FilePath = Path->Path;
	FilePath.append(m_FileName.begin(), m_FileName.end());
	FilePath += TEXT(".csv");

	CCSVEditor* CSV = new CCSVEditor;

	//파일을 읽었다는것은 CDO에서 요구하는 리소스가 있다는 뜻이므로
	if (CSV->ReadFile(FilePath))
	{
		m_vecRequiredResource = new std::vector<RequiredResource>;

		std::string CellVal;
		EResourceType Type;

		size_t sizeR = CSV->GetRowCount();
		for (size_t i = 0; i < sizeR; ++i)
		{
			size_t sizeC = CSV->GetRow(i).size();
			for (size_t j = 0; j < sizeC; ++j)
			{
				CellVal = CSV->GetCell(i, j);
				
				//스킵할지 말지를 확인
				if (j == 0)
				{
					Type = FindResourceType(CellVal);

					//첫번째 칸의 타입이 이상하면 None 반환
					//또는 End일경우 컴포넌트 탐색을 시작
					if (Type == EResourceType::None)
						continue;
					else if (Type == EResourceType::End)
					{
						FoundComponentCol = true;
						continue;
					}
				}
				else
				{
					
				}


					

				//초기화
				Type = EResourceType::None;
				CellVal.clear();
			}





		}
		

		



		return true;
	}


	return false;
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

void CCDO::AddResourceType(const std::string& Name, EResourceType&& ResType)
{
	if (m_mapResType.find(Name) != m_mapResType.end())
		return;

	m_mapResType.insert(std::make_pair(Name, ResType));
}


EResourceType CCDO::FindResourceType(const std::string& Name)
{
	auto iter = m_mapResType.find(Name);

	if (iter == m_mapResType.end())
		return EResourceType::None;

	return iter->second;
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


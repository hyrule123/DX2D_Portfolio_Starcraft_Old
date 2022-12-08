#include "CDO.h"

#include "Resource/ResourceManager.h"
#include "PathManager.h"
#include "CSVEditor.h"

std::unordered_map<std::string, CCDO*>	CCDO::m_mapCDO;
std::unordered_map<std::string, EResourceType> CCDO::m_mapResType;
std::unordered_map<std::string, CSharedPtr<CCDO>> CCDO::m_mapPreLoadObject;

CCDO::CCDO():
	m_vecRequiredResource(),
	m_ObjStatus(EObjStatus::CDO)
{
}

CCDO::CCDO(const CCDO& CDO):
	CRef(CDO)
{
	switch (CDO.m_ObjStatus)
	{
	case EObjStatus::CDO:
		//CDO���� �����ؿ��� ���(PLO ����)���� �䱸 ���ҽ� ����� �����ؿ´�.
		m_vecRequiredResource = std::vector<RequiredResource>(CDO.m_vecRequiredResource);
		break;
	case EObjStatus::PLO:
		//PLO���� �����ؿ��� ��� = ������ ������ ������Ʈ
		//�� ������ �䱸 ���ҽ� ����� �������� �ʴ´�.
		m_ObjStatus = EObjStatus::RealObject;
		break;
	default:
		break;
	}
}

CCDO::~CCDO()
{
}



bool CCDO::Init()
{

	return true;
}

bool CCDO::LoadMetaData()
{
	const PathInfo* Path = CPathManager::GetInst()->FindPath(CDO_PATH);

	tstring FilePath = Path->Path;
	FilePath.append(m_FileName.begin(), m_FileName.end());
	FilePath += TEXT(".csv");

	CCSVEditor* CSV = new CCSVEditor;
	bool ret = false;

	//������ �о��ٴ°��� CDO���� �䱸�ϴ� ���ҽ��� �ִٴ� ���̹Ƿ�
	if (CSV->ReadFile(FilePath))
	{
		std::string CellVal;
		EResourceType Type;

		size_t sizeR = CSV->GetRowCount();
		for (size_t i = 0; i < sizeR; ++i)
		{
			RequiredResource Res;
			bool FoundVal = false;

			size_t sizeC = CSV->GetRow(i).size();
			for (size_t j = 0; j < sizeC; ++j)
			{
				CellVal = CSV->GetCell(static_cast<int>(i), static_cast<int>(j));
				

				//��ŵ���� ������ Ȯ��
				if (j == 0)
				{
					//�빮�ڷ� ��ȯ�ϰ�
					std::transform(CellVal.begin(), CellVal.end(), CellVal.begin(), toupper);

					//�ش� ������ ���ҽ� Ÿ���� Ȯ��
					Type = FindResourceType(CellVal);

					//ù��° ĭ�� Ÿ���� �̻��ϸ� None ��ȯ
					//�Ǵ� End�ϰ�� ������Ʈ Ž���� ����
					if (Type == EResourceType::None || Type == EResourceType::End)
						break;
					else
					{
						FoundVal = true;
						//���ҽ� Ÿ���� �ϴ� ���
						Res.ResType = Type;
					}
				}
				else
				{
					//EResourceType ResType;
					//std::string Name;
					//tstring FileName;
					//std::string PathName;

					switch (static_cast<ERequiredResource>(j))
					{
					case ERequiredResource::Name:
						Res.Name = CellVal;
						break;
					case ERequiredResource::FileName:
					{
#ifdef UNICODE
						
						CPathManager::ConvertMultibyteToUnicode(
							Res.FileName,
							CellVal.c_str(),
							CellVal.size()
						);
#else

						Res.FileName = CSV->GetCell(i, j);
#endif
	
						break;
					}

					default:
						break;
					}
				}

				//�ʱ�ȭ
				Type = EResourceType::None;
				CellVal.clear();
			}

			//������� ���ҽ� ������ ���� �����̳ʿ� ����
			if(FoundVal)
				m_vecRequiredResource.push_back(Res);
		}
		

		ret = true;
	}

	SAFE_DELETE(CSV);
	return ret;
}


bool CCDO::CDOPreload()
{
	std::unordered_map<std::string, std::vector<const TCHAR*>> mapTexArray;

	if (!m_vecRequiredResource.empty())
	{
		size_t size = m_vecRequiredResource.size();

		for (size_t i = 0; i < size; ++i)
		{
			switch (m_vecRequiredResource[i].ResType)
			{
			case EResourceType::Mesh:
				break;
			case EResourceType::Shader:
				break;
			case EResourceType::CBuffer:
				break;
			case EResourceType::Texture:
			{
				CResourceManager::GetInst()->LoadTexture(m_vecRequiredResource[i].Name, m_vecRequiredResource[i].FileName.c_str(), TEXTURE_PATH);
				break;
			}

			//�ϴ� �����͸� ���Ը� �ϰ� ��ȸ�� ������ �ϰ������� ����
			case EResourceType::TextureArray:
			{
				const std::string& Name = m_vecRequiredResource[i].Name;

				auto iter = mapTexArray.find(Name);

				//������ �� ������ ���� ����
				if (iter == mapTexArray.end())
				{
					std::vector<const TCHAR*> vecNames;
					vecNames.push_back(m_vecRequiredResource[i].FileName.c_str());
					mapTexArray.insert(std::make_pair(Name, vecNames));
				}
				else
					iter->second.push_back(m_vecRequiredResource[i].FileName.c_str());

				

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
			case EResourceType::SCUnitInfo:
				break;
			case EResourceType::End:
				break;
			default:
				break;
			}
		}


		
		if (false == mapTexArray.empty())
		{
			for (auto& iter : mapTexArray)
			{
				CResourceManager::GetInst()->LoadTextureArray(iter.first, iter.second, TEXTURE_PATH);
			}
		}
	}


	m_ObjStatus = EObjStatus::PLO;

	return true;
}

void CCDO::Save(FILE* File)
{
	//������ �� ����
}

void CCDO::Load(FILE* File)
{

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

void CCDO::ClearAll()
{
	for (auto& iter : m_mapCDO)
	{
		SAFE_DELETE(iter.second);
	}
	m_mapResType.clear();
	m_mapPreLoadObject.clear();
}


CCDO* CCDO::FindPLO(const std::string& ClassName)
{
	auto iter = m_mapPreLoadObject.find(ClassName);
	if (iter != m_mapPreLoadObject.end())
		return iter->second.Get();

	return nullptr;
}

CCDO* CCDO::ClonePLO(const std::string& ClassName)
{
	CCDO* PLO = FindPLO(ClassName);

	if (!PLO)
	{
		PLO = CreatePLO(ClassName);
	}

	return PLO->Clone();
}

CCDO* CCDO::CreatePLO(const std::string& ClassName)
{
	CSharedPtr<CCDO> PLO = CCDO::CloneCDO(ClassName);

	if (!PLO)
	{
		assert(0);
		return nullptr;
	}

	PLO->CDOPreload();
	m_mapPreLoadObject.insert(std::make_pair(ClassName, PLO));

	CSceneManager::GetInst()->AddPLO(PLO);

	return PLO;
}

void CCDO::DeleteUnusedPLO()
{
	auto iter = m_mapPreLoadObject.begin();
	auto iterEnd = m_mapPreLoadObject.end();

	while (iter != iterEnd)
	{
		if (1 == iter->second->GetRefCount())
		{
			iter = m_mapPreLoadObject.erase(iter);
			continue;
		}

		++iter;
	}
}

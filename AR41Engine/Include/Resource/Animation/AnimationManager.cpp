
#include "AnimationManager.h"
#include "../Shader/Animation2DConstantBuffer.h"
#include "../../Scene/SceneManager.h"

CAnimationManager::CAnimationManager()	:
	m_Anim2DBuffer(nullptr)
{
}

CAnimationManager::~CAnimationManager()
{
	SAFE_DELETE(m_Anim2DBuffer);
}

bool CAnimationManager::Init()
{
	m_Anim2DBuffer = new CAnimation2DConstantBuffer;

	m_Anim2DBuffer->Init();

	return true;
}

CAnimationSequence2D* CAnimationManager::CreateAnimationSequence2D(const std::string& Name,
	const std::string& TextureName, const TCHAR* FileName, const std::string& PathName)
{
	CAnimationSequence2D* Sequence = FindAnimationSequence2D(Name);

	if (Sequence)
		return Sequence;

	Sequence = new CAnimationSequence2D; 
	
	Sequence->SetResourceType(EResourceType::Animation);
	Sequence->SetName(Name);

	if (!Sequence->Init(TextureName, FileName, PathName))
	{
		SAFE_DELETE(Sequence);
		return nullptr;
	}

	m_mapSequence2D.insert(std::make_pair(Name, Sequence));

	return Sequence;
}

CAnimationSequence2D* CAnimationManager::CreateAnimationSequence2D(const std::string& Name,
	CTexture* Texture)
{
	CAnimationSequence2D* Sequence = FindAnimationSequence2D(Name);

	if (Sequence)
		return Sequence;

	Sequence = new CAnimationSequence2D; 
	
	Sequence->SetResourceType(EResourceType::Animation);
	Sequence->SetName(Name);

	if (!Sequence->Init(Texture))
	{
		SAFE_DELETE(Sequence);
		return nullptr;
	}

	m_mapSequence2D.insert(std::make_pair(Name, Sequence));

	return Sequence;
}

CAnimationSequence2D* CAnimationManager::CreateAnimationSequence2DFullPath(
	const std::string& Name, const std::string& TextureName,
	const TCHAR* FullPath)
{
	CAnimationSequence2D* Sequence = FindAnimationSequence2D(Name);

	if (Sequence)
		return Sequence;

	Sequence = new CAnimationSequence2D; 
	
	Sequence->SetResourceType(EResourceType::Animation);
	Sequence->SetName(Name);

	if (!Sequence->InitFullPath(TextureName, FullPath))
	{
		SAFE_DELETE(Sequence);
		return nullptr;
	}

	m_mapSequence2D.insert(std::make_pair(Name, Sequence));

	return Sequence;
}

CAnimationSequence2D* CAnimationManager::CreateAnimationSequence2D(
	const std::string& Name, const std::string& TextureName,
	const std::vector<const TCHAR*>& vecFileName, 
	const std::string& PathName)
{
	CAnimationSequence2D* Sequence = FindAnimationSequence2D(Name);

	if (Sequence)
		return Sequence;

	Sequence = new CAnimationSequence2D; 
	
	Sequence->SetResourceType(EResourceType::Animation);
	Sequence->SetName(Name);

	if (!Sequence->Init(TextureName, vecFileName, PathName))
	{
		SAFE_DELETE(Sequence);
		return nullptr;
	}

	m_mapSequence2D.insert(std::make_pair(Name, Sequence));

	return Sequence;
}

CAnimationSequence2D* CAnimationManager::CreateAnimationSequence2DFullPath(
	const std::string& Name, const std::string& TextureName,
	const std::vector<const TCHAR*>& vecFullPath)
{
	CAnimationSequence2D* Sequence = FindAnimationSequence2D(Name);

	if (Sequence)
		return Sequence;

	Sequence = new CAnimationSequence2D; 
	
	Sequence->SetResourceType(EResourceType::Animation);
	Sequence->SetName(Name);

	if (!Sequence->InitFullPath(TextureName, vecFullPath))
	{
		SAFE_DELETE(Sequence);
		return nullptr;
	}

	m_mapSequence2D.insert(std::make_pair(Name, Sequence));

	return Sequence;
}

bool CAnimationManager::AddSequence2DFrame(const std::string& Name, const Vector2& Start,
	const Vector2& End)
{
	CAnimationSequence2D* Sequence = FindAnimationSequence2D(Name);

	if (!Sequence)
		return false;

	Sequence->AddFrame(Start, End);

	return true;
}

bool CAnimationManager::AddSequence2DFrame(const std::string& Name, float StartX,
	float StartY, float EndX, float EndY)
{
	CAnimationSequence2D* Sequence = FindAnimationSequence2D(Name);

	if (!Sequence)
		return false;

	Sequence->AddFrame(StartX, StartY, EndX, EndY);

	return true;
}

bool CAnimationManager::AddAnimationSequence2DFrameByTileNumber(const std::string& Name, EAnimation2DType Type,
	int TileRowNum, int TileColNum,
	int ColStart, int ColSize,
	int RowStart, int RowSize
)
{
	CAnimationSequence2D* Anim = FindAnimationSequence2D(Name);
	if (!Anim)
		return false;

	Anim->AddFrameByTileNumber(Type, TileRowNum, TileColNum, ColStart, ColSize, RowStart, RowSize);

	return true;
}

//bool CAnimationManager::SetAnimationSequence2DFrameAllByTileNumber(const std::string& Name, int TileRowNum, int TileColNum)
//{
//	CAnimationSequence2D* Sequence = FindAnimationSequence2D(Name);
//
//	if (!Sequence)
//		return false;
//
//	Sequence->SetFrameAllByTileNumber(TileRowNum, TileColNum);
//
//	return true;
//}

bool CAnimationManager::AddSequence2DFrameAll(const std::string& Name, int Count, const Vector2& Start, const Vector2& End)
{
	CAnimationSequence2D* Sequence = FindAnimationSequence2D(Name);

	if (!Sequence)
		return false;

	Sequence->AddFrameAll(Count, Start, End);

	return true;
}

bool CAnimationManager::AddSequence2DFrameAll(const std::string& Name, int Count, float StartX, float StartY, float EndX, float EndY)
{
	CAnimationSequence2D* Sequence = FindAnimationSequence2D(Name);

	if (!Sequence)
		return false;

	Sequence->AddFrameAll(Count, StartX, StartY, EndX, EndY);

	return true;
}

bool CAnimationManager::SaveSequence2D(const std::string& Name, const char* FullPath)
{
	CAnimationSequence2D* Sequence = FindAnimationSequence2D(Name);

	if (!Sequence)
		return false;

	return Sequence->Save(FullPath);
}

bool CAnimationManager::LoadSequence2D(const std::string& Name, const char* FullPath)
{
	CAnimationSequence2D* Sequence = FindAnimationSequence2D(Name);

	bool	Find = true;

	if (!Sequence)
	{
		Sequence = new CAnimationSequence2D; 
		Sequence->SetResourceType(EResourceType::Animation);

		Sequence->SetName(Name);

		Find = false;
	}

	if (!Sequence->Load(FullPath))
		return false;

	if(!Find)
		m_mapSequence2D.insert(std::make_pair(Name, Sequence));

	return true;
}

bool CAnimationManager::SaveSequence2D(const std::string& Name, const char* FileName,
	const std::string& PathName)
{
	CAnimationSequence2D* Sequence = FindAnimationSequence2D(Name);

	if (!Sequence)
		return false;

	return Sequence->Save(FileName, PathName);
}

bool CAnimationManager::LoadSequence2D(const std::string& Name, const char* FileName,
	const std::string& PathName)
{
	CAnimationSequence2D* Sequence = FindAnimationSequence2D(Name);

	bool	Find = true;

	if (!Sequence)
	{
		Sequence = new CAnimationSequence2D; 
		Sequence->SetResourceType(EResourceType::Animation);

		Sequence->SetName(Name);

		Find = false;
	}

	if (!Sequence->Load(FileName, PathName))
		return false;

	if (!Find)
		m_mapSequence2D.insert(std::make_pair(Name, Sequence));

	return true;
}

CAnimationSequence2D* CAnimationManager::FindAnimationSequence2D(
	const std::string& Name)
{
	auto	iter = m_mapSequence2D.find(Name);
	
	if (iter == m_mapSequence2D.end())
		return nullptr;

	return iter->second;
}

void CAnimationManager::ReleaseAnimationSequence2D(const std::string& Name)
{
	auto	iter = m_mapSequence2D.find(Name);

	if (iter != m_mapSequence2D.end())
	{
		if (iter->second->GetRefCount() == 1)
			m_mapSequence2D.erase(iter);
	}
}

void CAnimationManager::DeleteUnused()
{
	auto iter = m_mapSequence2D.begin();
	auto iterEnd = m_mapSequence2D.end();

	CSceneManager* SceneMgr = CSceneManager::GetInst();

	while (iter != iterEnd)
	{
		//씬에서 사용되지 않고 필수 리소스로 설정되어 있지 않을 경우 지워준다. -> RefCount == 0 이 되므로 알아서 제거
		if (iter->second->GetRefCount() == 1 && !(iter->second->GetEssential()))
		{
			iter = m_mapSequence2D.erase(iter);
			continue;
		}

		SceneMgr->AddSceneResource(iter->second);
		++iter;
	}
}

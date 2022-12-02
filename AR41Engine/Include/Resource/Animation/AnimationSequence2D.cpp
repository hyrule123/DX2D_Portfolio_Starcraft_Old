
#include "AnimationSequence2D.h"
#include "../../PathManager.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneResource.h"
#include "../ResourceManager.h"



CAnimationSequence2D::CAnimationSequence2D()	:
	m_Anim2DType(EAnimation2DType::Atlas),
	m_RowStart(-1),
	m_ColStart(-1),
	m_RowSize(-1),
	m_ColSize(-1)
{
	SetTypeID<CAnimationSequence2D>();
}


CAnimationSequence2D::~CAnimationSequence2D()
{
}


bool CAnimationSequence2D::Init(CTexture* Texture)
{
	m_Texture = Texture;

	if (!m_Texture)
		return false;

	m_Anim2DType = (EAnimation2DType)m_Texture->GetImageType();

	return true;
}

bool CAnimationSequence2D::Init(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{

	if (!CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName))
		return false;

	m_Texture = CResourceManager::GetInst()->FindTexture(Name);
	if (!m_Texture)
		return false;

	m_Anim2DType = (EAnimation2DType)m_Texture->GetImageType();
	return true;
}

bool CAnimationSequence2D::InitFullPath(const std::string& Name, 
	const TCHAR* FullPath)
{
	if (!CResourceManager::GetInst()->LoadTextureFullPath(Name, FullPath))
		return false;

	m_Texture = CResourceManager::GetInst()->FindTexture(Name);

	if (!m_Texture)
		return false;

	m_Anim2DType = (EAnimation2DType)m_Texture->GetImageType();

	return true;
}

bool CAnimationSequence2D::Init(const std::string& Name, 
	const std::vector<const TCHAR*>& vecFileName,
	const std::string& PathName)
{
	if (!CResourceManager::GetInst()->LoadTexture(Name, vecFileName, PathName))
		return false;

	m_Texture = CResourceManager::GetInst()->FindTexture(Name);

	if (!m_Texture)
		return false;

	m_Anim2DType = (EAnimation2DType)m_Texture->GetImageType();

	return true;
}

bool CAnimationSequence2D::InitFullPath(const std::string& Name, 
	const std::vector<const TCHAR*>& vecFullPath)
{
	if (!CResourceManager::GetInst()->LoadTextureFullPath(Name, vecFullPath))
		return false;

	m_Texture = CResourceManager::GetInst()->FindTexture(Name);

	if (!m_Texture)
		return false;

	m_Anim2DType = (EAnimation2DType)m_Texture->GetImageType();

	return true;
}


void CAnimationSequence2D::AddFrame(const Vector2& Start, const Vector2& End, int TexIndex)
{
	Animation2DFrameData	Data;

	Data.Start = Start;
	Data.End = End;

	m_vecFrameData.push_back(Data);
}

void CAnimationSequence2D::AddFrame(float StartX, float StartY, float EndX, float EndY, int TexIndex)
{
	Animation2DFrameData	Data;

	Data.Start = Vector2(StartX, StartY);
	Data.End = Vector2(EndX, EndY);

	m_vecFrameData.push_back(Data);
}



void CAnimationSequence2D::AddFrameByTileNumber
(
	int TileRowNum, int TileColNum,
	int ColStart, int ColSize,
	int RowStart, int RowSize,
	EAnimSeqArrayIndexMainAxis MainAxis
)
{

	if (!m_Texture)
	{
		assert(0);
		return;
	}
		
	m_MainAxis = MainAxis;


	//시작 행렬 번호를 지정했을 경우 여기서 설정
	int RStart = 0;
	int RSize = TileRowNum;
	int CStart = 0;
	int CSize = TileColNum;

	if (RowStart != -1)
	{
		if (RowStart < 0 || RowStart > TileRowNum)
		{
			assert(0);
			return;
		}
		RStart = RowStart;
	}
	if (RowSize != -1)
	{
		if (RowSize > TileRowNum)
		{
			assert(0);
			return;
		}

		RSize = RowSize;
	}
	if (ColStart != -1)
	{
		if (ColStart < 0 || ColStart > TileColNum)
		{
			assert(0);
			return;
		}

		CStart = ColStart;
	}
	if (ColSize != -1)
	{
		if (ColSize > TileColNum)
		{
			assert(0);
			return;
		}

		CSize = ColSize;
	}


	m_vecFrameData.clear();
	
	int TileWidth = m_Texture->GetWidth() / TileColNum;
	int TileHeight = m_Texture->GetHeight() / TileRowNum;

	int REnd = RStart + RSize; 
	int CEnd = CStart + CSize;

	for (int i = RStart; i < REnd; ++i)
	{
		for (int j = CStart; j < CEnd; ++j)
		{
			Animation2DFrameData Data;
			
			Data.Start.x = (float)(j * TileWidth);
			Data.Start.y = (float)(i * TileHeight);
			

			Data.End.x = Data.Start.x + (float)TileWidth;
			Data.End.y = Data.Start.y + (float)TileHeight;

			m_vecFrameData.push_back(Data);
		}
	}

	m_RowStart = RStart;
	m_ColStart = CStart;

	m_RowSize = RSize;
	m_ColSize = CSize;

	//기본 프레임 데이터를 초기화
	{
		m_vecCustomFrameIndexX.resize(m_ColSize);
		size_t size = m_vecCustomFrameIndexX.size();
		for (size_t i = 0; i < size; ++i)
		{
			m_vecCustomFrameIndexX[i] = (int)i;
		}
	}

	{
		m_vecCustomFrameIndexY.resize(m_RowSize);
		size_t size = m_vecCustomFrameIndexY.size();
		for (size_t i = 0; i < size; ++i)
		{
			m_vecCustomFrameIndexY[i] = (int)i;
		}
	}


	m_Anim2DType = EAnimation2DType::AtlasIndexed;
}

//void CAnimationSequence2D::SetFrameAllByTileNumber(int TileRowNum, int TileColNum)
//{
//	//예외처리
//	if (!m_Texture)
//	{
//		assert(0);
//		return;
//	}
//	else if (m_Anim2DType == EAnimation2DType::Frame)
//	{
//		assert(0);
//		return;
//	}
//
//	m_Anim2DType = EAnimation2DType::AtlasIndexed;
//
//	//시작 행렬 번호를 지정했을 경우 여기서 설정
//	int RStart = 0;
//	int RSize = TileRowNum;
//	int CStart = 0;
//	int CSize = TileColNum;
//
//
//	m_vecFrameData.clear();
//
//	int TileWidth = m_Texture->GetWidth() / TileRowNum;
//	int TileHeight = m_Texture->GetHeight() / TileColNum;
//
//	int CEnd = CStart + CSize;
//	int REnd = RStart + RSize;
//
//	for (int i = CStart; i < CEnd; ++i)
//	{
//		for (int j = RStart; j < REnd; ++j)
//		{
//			Animation2DFrameData Data;
//
//			Data.Start.x = (float)(j * TileWidth);
//			Data.Start.y = (float)(i * TileHeight);
//
//
//			Data.End.x = Data.Start.x + (float)TileWidth;
//			Data.End.y = Data.Start.y + (float)TileHeight;
//
//			m_vecFrameData.push_back(Data);
//		}
//	}
//
//
//	m_RowSize = RSize;
// 	m_ColSize = CSize;
//}

void CAnimationSequence2D::AddFrameAll(int Count, const Vector2& Start, const Vector2& End, int TexIndex)
{
	for (int i = 0; i < Count; ++i)
	{
		Animation2DFrameData	Data;

		Data.Start = Start;
		Data.End = End;

		m_vecFrameData.push_back(Data);
	}
}

void CAnimationSequence2D::AddFrameAll(int Count, float StartX, float StartY, float EndX, float EndY, int TexIndex)
{
	for (int i = 0; i < Count; ++i)
	{
		Animation2DFrameData	Data;

		Data.Start = Vector2(StartX, StartY);
		Data.End = Vector2(EndX, EndY);

		m_vecFrameData.push_back(Data);
	}
}

void CAnimationSequence2D::DeleteFrame(int Index, int TexIndex)
{
	auto	iter = m_vecFrameData.begin() + Index;

	m_vecFrameData.erase(iter);
}

void CAnimationSequence2D::ClearFrame()
{
	m_vecFrameData.clear();

}


bool CAnimationSequence2D::Save(const char* FullPath)
{
	FILE* File = nullptr;

	fopen_s(&File, FullPath, "wb");

	if (!File)
		return false;

	int	Length = (int)m_Name.length();
	fwrite(&Length, 4, 1, File);
	fwrite(m_Name.c_str(), 1, Length, File);

	fwrite(&m_Anim2DType, sizeof(EAnimation2DType), 1, File);

	if (m_Anim2DType == EAnimation2DType::AtlasIndexed)
	{
		fwrite(&m_RowSize, sizeof(int), 1, File);
		fwrite(&m_ColSize, sizeof(int), 1, File);
	}


	bool	TexEnable = false;

	//if (m_Texture)
	//	TexEnable = true;

	//fwrite(&TexEnable, 1, 1, File);

	//if (m_Texture)
	//{
	//	std::string TexName = m_Texture->GetName();

	//	Length = (int)TexName.length();
	//	fwrite(&Length, 4, 1, File);
	//	fwrite(TexName.c_str(), 1, Length, File);

	//	m_Texture->Save(File);
	//}

	int	FrameCount = (int)m_vecFrameData.size();

	fwrite(&FrameCount, 4, 1, File);
	fwrite(&m_vecFrameData[0], sizeof(Animation2DFrameData), FrameCount, File);

	fclose(File);

	return true;
}

bool CAnimationSequence2D::Load(const char* FullPath)
{
	FILE* File = nullptr;

	fopen_s(&File, FullPath, "rb");

	if (!File)
		return false;

	int	Length = 0;
	char	Name[256] = {};
	fread(&Length, 4, 1, File);
	fread(Name, 1, Length, File);

	m_Name = Name;

	fread(&m_Anim2DType, sizeof(EAnimation2DType), 1, File);

	if (m_Anim2DType == EAnimation2DType::AtlasIndexed)
	{
		fread(&m_RowSize, sizeof(int), 1, File);
		fread(&m_ColSize, sizeof(int), 1, File);
	}


	bool	TexEnable = false;

	fread(&TexEnable, 1, 1, File);

	if (TexEnable)
	{
		char TexName[256] = {};

		Length = 0;
		fread(&Length, 4, 1, File);
		fread(TexName, 1, Length, File);

		EImageType	ImageType;
		fread(&ImageType, sizeof(EImageType), 1, File);

		int	TextureCount = 0;

		fread(&TextureCount, sizeof(int), 1, File);

		TCHAR	FolderName[8] = {};
		lstrcpy(FolderName, TEXT("Texture"));

		if (TextureCount == 1)
		{
			TCHAR	FileName[MAX_PATH] = {};
			char	PathName[MAX_PATH] = {};

			fread(FileName, sizeof(TCHAR), MAX_PATH, File);
			fread(PathName, sizeof(char), MAX_PATH, File);

			CResourceManager::GetInst()->LoadTexture(TexName, FileName, PathName);

			//m_Texture = CResourceManager::GetInst()->FindTexture(TexName);

		}

		else
		{
			if (ImageType == EImageType::Frame)
			{
				std::vector<const TCHAR*>	vecFileName;
				std::string	ResultPathName;

				for (int i = 0; i < TextureCount; ++i)
				{
					TCHAR* FileName = new TCHAR[MAX_PATH];
					char	PathName[MAX_PATH] = {};

					fread(FileName, sizeof(TCHAR), MAX_PATH, File);
					fread(PathName, sizeof(char), MAX_PATH, File);

					ResultPathName = PathName;

					vecFileName.push_back(FileName);
				}


				CResourceManager::GetInst()->LoadTexture(TexName, vecFileName, ResultPathName);

				//m_Texture = CResourceManager::GetInst()->FindTexture(TexName);


				for (int i = 0; i < TextureCount; ++i)
				{
					SAFE_DELETE_ARRAY(vecFileName[i]);
				}
			}

			else
			{
				std::vector<const TCHAR*>	vecFileName;
				std::string	ResultPathName;

				for (int i = 0; i < TextureCount; ++i)
				{
					TCHAR* FileName = new TCHAR[MAX_PATH];
					char	PathName[MAX_PATH] = {};

					fread(FileName, sizeof(TCHAR), MAX_PATH, File);
					fread(PathName, sizeof(char), MAX_PATH, File);

					ResultPathName = PathName;

					vecFileName.push_back(FileName);
				}


				CResourceManager::GetInst()->LoadTextureArray(TexName, vecFileName, ResultPathName);

				//m_Texture = CResourceManager::GetInst()->FindTexture(TexName);

				for (int i = 0; i < TextureCount; ++i)
				{
					SAFE_DELETE_ARRAY(vecFileName[i]);
				}
			}
		}
	}

	int	FrameCount = 0;

	fread(&FrameCount, 4, 1, File);

	m_vecFrameData.resize(FrameCount);

	fread(&m_vecFrameData[0], sizeof(Animation2DFrameData), FrameCount, File);


	fclose(File);

	return true;
}

bool CAnimationSequence2D::Save(const char* FileName, const std::string& PathName)
{
	char	FullPath[MAX_PATH] = {};

	const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

	if (Path)
		strcpy_s(FullPath, Path->PathMultibyte);

	strcat_s(FullPath, FileName);

	return Save(FullPath);
}

bool CAnimationSequence2D::Load(const char* FileName, const std::string& PathName)
{
	char	FullPath[MAX_PATH] = {};

	const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

	if (Path)
		strcpy_s(FullPath, Path->PathMultibyte);

	strcat_s(FullPath, FileName);

	return Load(FullPath);
}

const Animation2DFrameData* CAnimationSequence2D::GetFrameDataArrayIndexed(int IndexX, int IndexY) const
{
	int Idx = m_vecCustomFrameIndexY[IndexY] * m_ColSize + m_vecCustomFrameIndexX[IndexX];

	if (Idx < 0 || Idx > m_vecFrameData.size())
		return nullptr;

	return &m_vecFrameData[Idx];
}
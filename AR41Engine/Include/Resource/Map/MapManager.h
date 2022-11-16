#pragma once

#include "../../EngineInfo.h"



class CMapManager
{
	friend class CResourceManager;

private:
	CMapManager();
	~CMapManager();

	//맵은 하나를 로딩하는데 오래걸리므로 로드 시 여기에 임시로 등록해 놓고,
	//나중에 삭제하는 방식으로 구현
	std::unordered_map <tstring, class CMap*> m_mapMap;
	class CMap* FindMap(const tstring& Name);

	//로드 완료 비교용
	unsigned int m_LoadRef;

public:
	bool Init();
	DirectX::ScratchImage* GetMapImage(tstring MapName);

	bool LoadMapDataFromFile(const TCHAR* FileName, const char* PathName = MAP_PATH);
	//-----------------------------------------------------------------------------
	// Extracts an archived file and saves it to the disk.
	//
	// Parameters :
	//
	//   char * szArchiveName  - Archive file name - 아카이브의 파일명(풀경로)
	//   char * szArchivedFile - Name/number of archived file. 
	//							- 아카이브 내부의 파일명(맵만 뽑아낼것이므로 "staredit\\scenario.chk"로 고정
	//   char * szFileName     - Name of the target disk file.
};


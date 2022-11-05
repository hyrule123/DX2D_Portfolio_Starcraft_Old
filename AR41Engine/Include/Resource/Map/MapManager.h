#pragma once

#include "../../EngineInfo.h"


typedef struct Chunk
{
	char TypeName[5];
	unsigned long length;
	unsigned char* Data;

	Chunk() :
		TypeName{},
		length(),
		Data()
	{
		SAFE_DELETE_ARRAY(Data);
	}

} Chunk;

enum EMapDataType
{
	Terrain,
	MapSize,
	TileMap,
	EMapDataTypeEnd
};


class CMapManager
{
	friend class CResourceManager;

private:
	CMapManager();
	~CMapManager();

	
	std::wstring m_MapName;

	//로드된 맵 데이터를 보관하는 변수
	Chunk m_MapDataChunk[EMapDataTypeEnd];

	//로드 완료 비교용
	unsigned int m_LoadRef;

	//로드 완료 기록용
	unsigned int m_LoadCheck;

private:
	void ReadMapData(char* Data, DWORD Size);
	void ResetMapData();

	//로드된 맵 데이터를 읽어서 타일맵을 생성.
	bool LoadMap();

public:
	bool Init();
	inline bool LoadComplete();



	//-----------------------------------------------------------------------------
	// Extracts an archived file and saves it to the disk.
	//
	// Parameters :
	//
	//   char * szArchiveName  - Archive file name - 아카이브의 파일명(풀경로)
	//   char * szArchivedFile - Name/number of archived file. 
	//							- 아카이브 내부의 파일명(맵만 뽑아낼것이므로 "staredit\\scenario.chk"로 고정
	//   char * szFileName     - Name of the target disk file.

	int LoadMapDataFromFile(const TCHAR* FileName, const char* PathName = MAP_PATH);

};



inline bool CMapManager::LoadComplete()
{
	return (m_LoadCheck == m_LoadRef);
}
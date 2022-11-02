#pragma once

#include "../../EngineInfo.h"
#include "../ResourceManager.h"


//타일셋 종류
enum class TileSets
{
	Badlands,
	SpacePlatform,
	Installation,
	AshWorld,
	JungleWorld,
	Desert,
	Ice,
	Twilight
};

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

	Chunk m_MapDataChunk[EMapDataTypeEnd];
	UINT8 m_LoadCheck;

private:
	void ReadMapData(char* Data, DWORD Size);
	void ResetMapData();


public:
	bool Init();
	bool LoadAll();




	//-----------------------------------------------------------------------------
	// Extracts an archived file and saves it to the disk.
	//
	// Parameters :
	//
	//   char * szArchiveName  - Archive file name - 아카이브의 파일명(풀경로)
	//   char * szArchivedFile - Name/number of archived file. 
	//							- 아카이브 내부의 파일명(맵만 뽑아낼것이므로 "staredit\\scenario.chk"로 고정
	//   char * szFileName     - Name of the target disk file.

	inline char* GetCurrentMapBuffer() const;
	int LoadMapDataFromFile(const TCHAR* FileName, const char* PathName = MAP_PATH);

};

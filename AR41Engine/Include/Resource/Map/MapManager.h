#pragma once

#include "../../EngineInfo.h"
#include "../ResourceManager.h"


//Ÿ�ϼ� ����
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
	//   char * szArchiveName  - Archive file name - ��ī�̺��� ���ϸ�(Ǯ���)
	//   char * szArchivedFile - Name/number of archived file. 
	//							- ��ī�̺� ������ ���ϸ�(�ʸ� �̾Ƴ����̹Ƿ� "staredit\\scenario.chk"�� ����
	//   char * szFileName     - Name of the target disk file.

	inline char* GetCurrentMapBuffer() const;
	int LoadMapDataFromFile(const TCHAR* FileName, const char* PathName = MAP_PATH);

};

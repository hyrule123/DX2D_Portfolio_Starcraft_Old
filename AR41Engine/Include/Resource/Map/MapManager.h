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

	//�ε�� �� �����͸� �����ϴ� ����
	Chunk m_MapDataChunk[EMapDataTypeEnd];

	//�ε� �Ϸ� �񱳿�
	unsigned int m_LoadRef;

	//�ε� �Ϸ� ��Ͽ�
	unsigned int m_LoadCheck;

private:
	void ReadMapData(char* Data, DWORD Size);
	void ResetMapData();

	//�ε�� �� �����͸� �о Ÿ�ϸ��� ����.
	bool LoadMap();

public:
	bool Init();
	inline bool LoadComplete();



	//-----------------------------------------------------------------------------
	// Extracts an archived file and saves it to the disk.
	//
	// Parameters :
	//
	//   char * szArchiveName  - Archive file name - ��ī�̺��� ���ϸ�(Ǯ���)
	//   char * szArchivedFile - Name/number of archived file. 
	//							- ��ī�̺� ������ ���ϸ�(�ʸ� �̾Ƴ����̹Ƿ� "staredit\\scenario.chk"�� ����
	//   char * szFileName     - Name of the target disk file.

	int LoadMapDataFromFile(const TCHAR* FileName, const char* PathName = MAP_PATH);

};



inline bool CMapManager::LoadComplete()
{
	return (m_LoadCheck == m_LoadRef);
}
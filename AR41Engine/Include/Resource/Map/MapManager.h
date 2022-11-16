#pragma once

#include "../../EngineInfo.h"



class CMapManager
{
	friend class CResourceManager;

private:
	CMapManager();
	~CMapManager();

	//���� �ϳ��� �ε��ϴµ� �����ɸ��Ƿ� �ε� �� ���⿡ �ӽ÷� ����� ����,
	//���߿� �����ϴ� ������� ����
	std::unordered_map <tstring, class CMap*> m_mapMap;
	class CMap* FindMap(const tstring& Name);

	//�ε� �Ϸ� �񱳿�
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
	//   char * szArchiveName  - Archive file name - ��ī�̺��� ���ϸ�(Ǯ���)
	//   char * szArchivedFile - Name/number of archived file. 
	//							- ��ī�̺� ������ ���ϸ�(�ʸ� �̾Ƴ����̹Ƿ� "staredit\\scenario.chk"�� ����
	//   char * szFileName     - Name of the target disk file.
};


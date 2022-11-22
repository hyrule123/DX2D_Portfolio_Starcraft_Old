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

	bool LoadMapDataFromFile(const TCHAR* FileName, const char* PathName = MAP_PATH);
	class CTileMapComponent* LoadTileMap(const TCHAR* FileName);

};


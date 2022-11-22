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

	bool LoadMapDataFromFile(const TCHAR* FileName, const char* PathName = MAP_PATH);
	class CTileMapComponent* LoadTileMap(const TCHAR* FileName);

};


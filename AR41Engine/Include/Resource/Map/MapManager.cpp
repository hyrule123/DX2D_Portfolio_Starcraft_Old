#include "MapManager.h"
#include "Map.h"

//타일맵 등록용 게임오브젝트 및 컴포넌트
#include "../../GameObject/GameObject.h"
#include "../../Component/TileMapComponent.h"


//경로 매니저
#include "../../PathManager.h"



CMapManager::CMapManager():
    m_LoadRef()
{
}

CMapManager::~CMapManager()
{
    auto iter = m_mapMap.begin();
    auto iterEnd = m_mapMap.end();

    while (iter != iterEnd)
    {
        SAFE_DELETE(iter->second);

        ++iter;
    }

}

CMap* CMapManager::FindMap(const tstring& Name)
{
    auto iter = m_mapMap.find(Name);

    if (iter == m_mapMap.end())
        return nullptr;

    return iter->second;
}

bool CMapManager::Init()
{
    for (unsigned int i = 0; i < EMapDataTypeEnd; ++i)
    {
        m_LoadRef += 1 << i;
    }

    LoadMapDataFromFile(TEXT("(2)BlueStorm1.2.scx"));

    CMap* map = FindMap(TEXT("(2)BlueStorm1.2.scx"));
    map->LoadMap();


    return true;
}

DirectX::ScratchImage* CMapManager::GetMapImage(tstring MapName)
{
    CMap* map = FindMap(MapName);

    if (!map)
        return nullptr;

    return map->GetMapImage();
}



bool CMapManager::LoadMapDataFromFile(const TCHAR* FileName, const char* PathName)
{

    //이미 생성되어 있을 경우 true를 반환한다.
    if (m_mapMap.find(FileName) != m_mapMap.end())
    {
        return true;
    }

    //경로 생성
    const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

    TCHAR FullPath[MAX_PATH] = {};
    lstrcpy(FullPath, Path->Path);

    lstrcat(FullPath, FileName);

    //맵 클래스 동적할당
    CMap* Map = new CMap;
    Map->m_LoadRef = m_LoadRef;
    Map->m_FileName = FileName;

    //파일로부터 맵 로드 시도
    if (Map->LoadMapDataFromFile(FullPath) != 0)
    {
        SAFE_DELETE(Map);

        return false;
    }

    m_mapMap.insert(std::make_pair(FileName, Map));

    return true;
}





//int main()
//{
//    TCHAR ArchiveName[MAX_PATH] = TEXT("C:\\Users\\ekdrn\\source\\repos\\chkFileParser\\chkFileParser\\mpqe-FS.scx");
//    char ArchivedFile[MAX_PATH] = "staredit\\scenario.chk";
//    TCHAR FileName[MAX_PATH] = TEXT("C:\\Users\\ekdrn\\source\\repos\\chkFileParser\\chkFileParser\\ExtractedChunk.chk");
//
//    WorkSpace* Space = new WorkSpace;
//
//    int ret = WorkSpace::ExtractFile(ArchiveName, ArchivedFile, FileName);
//
//    printf("%d", ret);
//
//    return 0;
//
//}



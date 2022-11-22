#pragma once
#include "../GameResource.h"



typedef struct Chunk
{
    char TypeName[5];
    unsigned long length;
    unsigned char* Data;

    Chunk() :
        TypeName{},
        length(), Data()
    {
    }
    ~Chunk()
    {
        SAFE_DELETE_ARRAY(Data);
    }


} Chunk;


//"ERA" 안의 정보 
enum class TerrainInfo : unsigned char
{
    Badlands = 0x00,
    SpacePlatform = 0x01,
    Installation = 0x02,
    AshWorld = 0x03,
    Jungle = 0x04,
    Desert = 0x05,
    Ice = 0x06,
    Twilight = 0x07
};


class CMap :
    public CGameResource
{
    friend class CMapManager;
protected:
    CMap();
    virtual ~CMap();

    void Save(FILE* File);
    void Load(FILE* File);
    
    inline bool LoadComplete();


protected:
    // ============맵 데이터===========
    tstring m_FileName;
    std::string m_FileNameMultiByte;

    //로드된 맵 데이터를 보관하는 변수
    Chunk m_MapDataChunk[EMapDataTypeEnd];

    //로드 완료 기록용
    unsigned int m_LoadRef;
    unsigned int m_LoadCheck;

    void ReadMapData(char* Data, DWORD Size);
    void ResetMapData();

    //============맵 정보=============
    int m_MapSizeX;
    int m_MapSizeY;
    TerrainInfo m_Terrain;

    //타일맵
    CSharedPtr<class CTileMapComponent> m_TileMap;



public:

    //-----------------------------------------------------------------------------
// Extracts an archived file and saves it to the disk.
//
// Parameters :
//
//   char * szArchiveName  - Archive file name - 아카이브의 파일명(풀경로)
//   char * szArchivedFile - Name/number of archived file. 
//							- 아카이브 내부의 파일명(맵만 뽑아낼것이므로 "staredit\\scenario.chk"로 고정
//   char * szFileName     - Name of the target disk file.
    int LoadMapDataFromFile(const TCHAR* FullPath);

    //로드된 맵 데이터를 읽어서 타일맵을 생성.
    bool LoadTileMap();
    class CTileMapComponent* GetTileMap() const;
    


};



inline bool CMap::LoadComplete()
{
    return (m_LoadCheck == m_LoadRef);
}


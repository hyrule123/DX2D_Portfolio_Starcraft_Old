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
    std::wstring m_FileName;

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
    class CTileMapComponent* m_TileMap;
    //로드된 맵 데이터를 읽어서 타일맵을 생성.
    bool LoadMap();

    //========== 이미지 데이터 보관 ==========



public:
    int LoadMapDataFromFile(const TCHAR* FullPath);
    inline DirectX::ScratchImage* GetMapImage() const;

    


};



inline bool CMap::LoadComplete()
{
    return (m_LoadCheck == m_LoadRef);
}

inline DirectX::ScratchImage* CMap::GetMapImage() const
{
    return m_MapImage;
}
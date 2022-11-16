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
        SAFE_DELETE_ARRAY(Data);
    }

} Chunk;



class CMap :
    public CGameResource
{
    friend class CMapManager;
protected:
    CMap();
    virtual ~CMap();
    

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

    //로드된 맵 데이터를 읽어서 타일맵을 생성.
    bool LoadMap();

    //========== 이미지 데이터 보관 ==========
    DirectX::ScratchImage* m_MapImage;

public:
    int LoadMapDataFromFile(const TCHAR* FullPath);
    inline DirectX::ScratchImage* GetMapImage() const;
    void Save(FILE* File);
    void Load(FILE* File);
};



inline bool CMap::LoadComplete()
{
    return (m_LoadCheck == m_LoadRef);
}

inline DirectX::ScratchImage* CMap::GetMapImage() const
{
    return m_MapImage;
}
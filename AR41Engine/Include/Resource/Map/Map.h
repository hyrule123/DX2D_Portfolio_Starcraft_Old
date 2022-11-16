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
    // ============�� ������===========
    std::wstring m_FileName;

    //�ε�� �� �����͸� �����ϴ� ����
    Chunk m_MapDataChunk[EMapDataTypeEnd];

    //�ε� �Ϸ� ��Ͽ�
    unsigned int m_LoadRef;
    unsigned int m_LoadCheck;

    void ReadMapData(char* Data, DWORD Size);
    void ResetMapData();

    //�ε�� �� �����͸� �о Ÿ�ϸ��� ����.
    bool LoadMap();

    //========== �̹��� ������ ���� ==========
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
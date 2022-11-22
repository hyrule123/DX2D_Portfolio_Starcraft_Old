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


//"ERA" ���� ���� 
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
    // ============�� ������===========
    tstring m_FileName;
    std::string m_FileNameMultiByte;

    //�ε�� �� �����͸� �����ϴ� ����
    Chunk m_MapDataChunk[EMapDataTypeEnd];

    //�ε� �Ϸ� ��Ͽ�
    unsigned int m_LoadRef;
    unsigned int m_LoadCheck;

    void ReadMapData(char* Data, DWORD Size);
    void ResetMapData();

    //============�� ����=============
    int m_MapSizeX;
    int m_MapSizeY;
    TerrainInfo m_Terrain;

    //Ÿ�ϸ�
    CSharedPtr<class CTileMapComponent> m_TileMap;



public:

    //-----------------------------------------------------------------------------
// Extracts an archived file and saves it to the disk.
//
// Parameters :
//
//   char * szArchiveName  - Archive file name - ��ī�̺��� ���ϸ�(Ǯ���)
//   char * szArchivedFile - Name/number of archived file. 
//							- ��ī�̺� ������ ���ϸ�(�ʸ� �̾Ƴ����̹Ƿ� "staredit\\scenario.chk"�� ����
//   char * szFileName     - Name of the target disk file.
    int LoadMapDataFromFile(const TCHAR* FullPath);

    //�ε�� �� �����͸� �о Ÿ�ϸ��� ����.
    bool LoadTileMap();
    class CTileMapComponent* GetTileMap() const;
    


};



inline bool CMap::LoadComplete()
{
    return (m_LoadCheck == m_LoadRef);
}


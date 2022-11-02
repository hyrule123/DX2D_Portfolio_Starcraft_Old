#include "MapManager.h"

//stormlib include
#define STORMLIB_NO_AUTO_LINK
#pragma comment(lib, "../Bin/StormLib.lib")
#include "StormLib.h"

//�� ��� Ž����
#include "../../PathManager.h"

//�� ���� �б��
#define TerrainChunk    "ERA"
#define MapSizeChunk    "DIM"
#define TileMapChunk    "MTXM"



CMapManager::CMapManager():
    m_MapDataChunk{}
{
}

CMapManager::~CMapManager()
{
}

bool CMapManager::Init()
{
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


int CMapManager::LoadMapDataFromFile(const TCHAR* FileName, const char* PathName)
{

    const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

    TCHAR FullPath[MAX_PATH] = {};
    lstrcpy(FullPath, Path->Path);

    lstrcat(FullPath, FileName);

    


    HANDLE hMpq = NULL;          // Open archive handle
    HANDLE hFile = NULL;          // Archived file handle
    //HANDLE handle = NULL;          // Disk file handle
    int    nError = ERROR_SUCCESS; // Result value

    // Open an archive, e.g. "d2music.mpq"
    if (nError == ERROR_SUCCESS)
    {
        if (!SFileOpenArchive(FullPath, 0, 0, &hMpq))
            nError = GetLastError();
    }

    // Open a file in the archive, e.g. "data\global\music\Act1\tristram.wav"
    if (nError == ERROR_SUCCESS)
    {
        //�� ������ scenario.chk �����͸� ã�´�.
        if (!SFileOpenFileEx(hMpq, "staredit\\scenario.chk", 0, &hFile))
            nError = GetLastError();
    }

    // Create the target file
    //if (nError == ERROR_SUCCESS)
    //{
    //    handle = CreateFile(szFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
    //    if (handle == INVALID_HANDLE_VALUE)
    //        nError = GetLastError();
    //}

    if (nError != ERROR_SUCCESS)
        return nError;


    // Read the file from the archive
    char* szBuffer;
    DWORD dwBytes = 1;

    //ã�� scenario.chk ���Ϸκ��� ������ �о�´�.
    DWORD FileSize = SFileGetFileSize(hFile, NULL);

    szBuffer = new char[FileSize];

    SFileReadFile(hFile, szBuffer, FileSize, &dwBytes, NULL);
    //if (dwBytes > 0)
    //    WriteFile(handle, szBuffer, dwBytes, &dwBytes, NULL);



    // Cleanup and exit
    //if (handle != NULL)
    //    CloseHandle(handle);
    if (hFile != NULL)
        SFileCloseFile(hFile);
    if (hMpq != NULL)
        SFileCloseArchive(hMpq);


    //szbuffer�� ���� �����Ͱ� �������� ����.
    //���⼭ ���� �׷����µ� �ʿ��� ������ �����;� ��
    ReadMapData(szBuffer, FileSize);


    SAFE_DELETE_ARRAY(szBuffer);
    return nError;
}



void CMapManager::ReadMapData(char* Data, DWORD Size)
{
    ResetMapData();

    std::string DataStr(Data, Data + Size);

    size_t pos = 0;
    pos = DataStr.find(TerrainChunk);

    if (pos != std::string::npos)
    {
        Chunk* chk = &m_MapDataChunk[Terrain];

        char* adress = Data + pos;

        //�̸� ����
        memcpy(chk->TypeName, adress, 4);

        //������ ����� ������ ����
        int* len = (int*)(adress + 4);
        chk->length = *len;

        chk->Data = new unsigned char[chk->length + 1];
        memcpy(chk->Data, adress + 8, chk->length);
        

        m_LoadCheck |= 1 << Terrain;
    }

    pos = 0;
    pos = DataStr.find(MapSizeChunk);

    if (pos != std::string::npos)
    {
        Chunk* chk = &m_MapDataChunk[MapSize];

        char* adress = Data + pos;

        //�̸� ����
        memcpy(chk->TypeName, adress, 4);

        //������ ����� ������ ����
        int* len = (int*)(adress + 4);
        chk->length = *len;

        chk->Data = new unsigned char[chk->length + 1];
        memcpy(chk->Data, adress + 8, chk->length);

        m_LoadCheck |= 1 << MapSize;
    }


    pos = 0;
    pos = DataStr.find(TileMapChunk);

    if (pos != std::string::npos)
    {
        Chunk* chk = &m_MapDataChunk[TileMap];

        char* adress = Data + pos;

        //�̸� ����
        memcpy(chk->TypeName, adress, 4);

        //������ ����� ������ ����
        int* len = (int*)(adress + 4);
        chk->length = *len;

        chk->Data = new unsigned char[chk->length + 1];
        memcpy(chk->Data, adress + 8, chk->length);

        m_LoadCheck |= 1 << TileMap;
    }

}


void CMapManager::ResetMapData()
{
    m_LoadCheck = 0;

    int Size = EMapDataTypeEnd;

    for (int i = 0; i < Size; ++i)
    {
        delete[] m_MapDataChunk[i].Data;
    }

    memset(m_MapDataChunk, 0, sizeof(Chunk) * Size);
}

bool CMapManager::isAllLoaded()
{
    if((m_LoadCheck & 1 << Terrain & 1 << MapSize & 1 << TileMap)

    return false;
}
#include "MapManager.h"

//stormlib include
#define STORMLIB_NO_AUTO_LINK
#pragma comment(lib, "../Bin/StormLib.lib")
#include "StormLib.h"

//맵 경로 탐색용
#include "../../PathManager.h"

//맵 정보 읽기용
#define TerrainChunk    "ERA"
#define MapSizeChunk    "DIM"
#define TileMapChunk    "MTXM"




//"ERA" 안의 정보 
enum class TerrainInfo : unsigned char
{
    Badlands        =0x00,
    SpacePlatform   =0x01,
    Installation    =0x02,
    AshWorld        =0x03,
    Jungle          =0x04,
    Desert          =0x05,
    Ice             =0x06,
    Twilight        =0x07
};



//동적 2d array를 쉽게 다루기 위한 클래스.
template <typename T>
class array2d {
public:

    array2d(T* ptr, int pitch) :
        _ptr(ptr),
        _pitch(pitch)
    {}

    T* operator[](int idx)
    {
        return _ptr + (_pitch * idx);
    }

private:
    T* const _ptr;
    const int _pitch;
};


typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;


#include <packon.h>

//단위크기: 52byte, 메가타일 인덱스
typedef struct {
    struct CV5Data {
        //필요없는 데이터
        uint8 Something[20];
        //필요한 데이터. 16개의 Mega Tile Index가 저장되어 있음
        uint16 MegatileIndex[16];
    } group[4096];
}CV5;


//단위크기: 32byte, 통과가능여부, 높이
typedef struct {
    struct VF4Data {
        uint16 MiniTileFlags[16];
    } data[65536];
}VF4;

//단위크기: 32bytes, megatile이 어떤 minitile로 이루어졌는지 표시
typedef struct {
    struct VX4Data {
        uint16 VR4Index[16];
    } data[65536];
}VX4;

//단위크기: 64byte - mini tile각각의 픽셀이 WPE 팔레트의 몇 번 색에 해당하는지를 저장
typedef struct {
    struct VR4Data {
        uint8 color[64];
    } image[32768];
}VR4;

//단위크기: 4byte
typedef struct {
    struct WPEData {
        uint8 r;
        uint8 g;
        uint8 b;
        uint8 padding;
    } data[256];
}WPE;

typedef struct RGBAbyte{
    uint8 r;
    uint8 g;
    uint8 b;
    uint8 a;

    RGBAbyte()
    {}
    RGBAbyte(uint8 _r, uint8 _g, uint8 _b, uint8 _a):
        r(_r),
        g(_g),
        b(_b),
        a(_a)
    {
    }
    
}RGBAbyte;

#include <packoff.h>



CMapManager::CMapManager():
    m_MapDataChunk{},
    m_LoadRef(),
    m_LoadCheck()
{
}

CMapManager::~CMapManager()
{
}

bool CMapManager::Init()
{
    for (unsigned int i = 0; i < EMapDataTypeEnd; ++i)
    {
        m_LoadRef += 1 << i;
    }

    LoadMapDataFromFile(TEXT("(2)BlueStorm1.2.scx"));
    LoadMap();

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
        //맵 파일의 scenario.chk 데이터를 찾는다.
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

    //찾은 scenario.chk 파일로부터 정보를 읽어온다.
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


    //szbuffer에 맵의 데이터가 들어와있을 것임.
    //여기서 맵을 그려내는데 필요한 정보를 가져와야 함
    ReadMapData(szBuffer, FileSize);

    //로드 완료된거 확인되면 맵 이름 저장
    if (m_LoadRef == m_LoadCheck)
        m_MapName = FileName;

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

        //이름 복사
        memcpy(chk->TypeName, adress, 4);

        //데이터 덩어리의 사이즈 복사
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

        //이름 복사
        memcpy(chk->TypeName, adress, 4);

        //데이터 덩어리의 사이즈 복사
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

        //이름 복사
        memcpy(chk->TypeName, adress, 4);

        //데이터 덩어리의 사이즈 복사
        int* len = (int*)(adress + 4);
        chk->length = *len;

        chk->Data = new unsigned char[chk->length + 1];
        memcpy(chk->Data, adress + 8, chk->length);

        m_LoadCheck |= 1 << TileMap;
    }


    if (m_LoadCheck != m_LoadRef)
        ResetMapData();
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


bool CMapManager::LoadMap()
{
    if (!LoadComplete())
        return false;


    FILE* CV5fp, * VX4fp, * VR4fp, * WPEfp, * VF4fp;
    CV5* cv5;
    VX4* vx4;
    VR4* vr4;
    WPE* wpe;
    VF4* vf4;


    //맵 사이즈 읽기
    if (m_MapDataChunk[MapSize].length != 4)
        return false;

    int w = (int)*(unsigned short*)(m_MapDataChunk[MapSize].Data);
    int h = (int)*(unsigned short*)(m_MapDataChunk[MapSize].Data + 2);



    int x, y, subx, suby, i, j;



    //지형 파일 읽기
    if (m_MapDataChunk[Terrain].length != 2)
        return false;
    
    //첫 1바이트에 지형 정보가 들어있다.
    unsigned char Info = m_MapDataChunk[Terrain].Data[0];

    //앞의 4비트는 0으로 변환
    unsigned char bitshift = 0b11110000;
    Info &= ~bitshift;
    
    //혹시나 비트시프트 이후 8을 초과하는 값이 나올 경우 8을 빼준다.
    if (Info >= (unsigned char)8)
        Info -= (unsigned char)8;


    const PathInfo* Path = CPathManager::GetInst()->FindPath(MAP_PATH);

    tstring FullPath = Path->Path;

    FullPath += TEXT("Tilesets/");

    //TCHAR FullPath[MAX_PATH] = {};
    //lstrcpy(FullPath, Path->Path);
    //lstrcat(FullPath, TEXT("Tilesets/"));

    switch ((TerrainInfo)Info)
    {
    case TerrainInfo::Badlands:
        FullPath += TEXT("badlands");
        //lstrcat(FullPath, TEXT("badlands"));
        break;
    case TerrainInfo::SpacePlatform:
        FullPath += TEXT("platform");
        //lstrcat(FullPath, TEXT("platform"));
        break;
    case TerrainInfo::Installation:
        FullPath += TEXT("install");
        //lstrcat(FullPath, TEXT("install"));
        break;
    case TerrainInfo::AshWorld:
        FullPath += TEXT("ashworld");
        //lstrcat(FullPath, TEXT("ashworld"));
        break;
    case TerrainInfo::Jungle:
        FullPath += TEXT("jungle");
        //lstrcat(FullPath, TEXT("jungle"));
        break;
    case TerrainInfo::Desert:
        FullPath += TEXT("Desert");
        //lstrcat(FullPath, TEXT("Desert"));
        break;
    case TerrainInfo::Ice:
        FullPath += TEXT("Ice");
        //lstrcat(FullPath, TEXT("Ice"));
        break;
    case TerrainInfo::Twilight:
        FullPath += TEXT("Twilight");
        //lstrcat(FullPath, TEXT("Twilight"));
        break;
    default:
        break;
    }


    lfopen_s(&CV5fp, (FullPath + TEXT(".CV5")).c_str(), TEXT("rb"));
    lfopen_s(&VX4fp, (FullPath + TEXT(".VX4")).c_str(), TEXT("rb"));
    lfopen_s(&VR4fp, (FullPath + TEXT(".VR4")).c_str(), TEXT("rb"));
    lfopen_s(&WPEfp, (FullPath + TEXT(".WPE")).c_str(), TEXT("rb"));
    lfopen_s(&VF4fp, (FullPath + TEXT(".VF4")).c_str(), TEXT("rb"));

    if (CV5fp == NULL || VX4fp == NULL || VR4fp == NULL || WPEfp == NULL) {
        _fcloseall();
        return 0;
    }

    cv5 = new CV5;
    vx4 = new VX4;
    vr4 = new VR4;
    wpe = new WPE;
    vf4 = new VF4;
    memset(cv5, 0, sizeof(CV5));
    memset(vx4, 0, sizeof(VX4));
    memset(vr4, 0, sizeof(VR4));
    memset(wpe, 0, sizeof(WPE));
    memset(vf4, 0, sizeof(VF4));

    fread(cv5, 1, sizeof(CV5), CV5fp);
    fread(vx4, 1, sizeof(VX4), VX4fp);
    fread(vr4, 1, sizeof(VR4), VR4fp);
    fread(wpe, 1, sizeof(WPE), WPEfp);
    fread(vf4, 1, sizeof(VF4), VF4fp);

    _fcloseall();




    uint16* mtxmdata = new uint16[w * h];
    memcpy(mtxmdata, m_MapDataChunk[TileMap].Data, w * h);


    array2d<uint16> mtxm(mtxmdata, w);


    //이미지 파일의 데이터를 준비.
    const int imgw = 32 * w, imgh = 32 * h;
    RGBAbyte* imgdata = new RGBAbyte[imgw * imgh];
    RGBAbyte* walkabilityData = new RGBAbyte[imgw * imgh];
    array2d<RGBAbyte> img(imgdata, imgw);
    array2d<RGBAbyte> Walkabilityimg(walkabilityData, imgw);

    //이미지 기록
    //여기에서 타일을 생성하고 기록해주면 된다.

    //메가타일
    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            int group = mtxm[y][x] >> 4;
            int index = mtxm[y][x] & 0xf;
            //megatile을 얻고 안의 4*4개의 minitile들을 얻어서
            int megatile = cv5->group[group].MegatileIndex[index];

            //메가타일 안의 미니타일 4*4 순회
            for (suby = 0; suby < 4; suby++) {
                for (subx = 0; subx < 4; subx++) {
                    //렌더링할 이미지를 찾고 렌더링한다.
                    //여기서 vf파일도 읽어다가 pass/not pass 여부를 불러온다.
                    int minitileindex = vx4->data[megatile].VR4Index[suby * 4 + subx] >> 1; //상위 15비트는 이미지
                    bool flipped = (vx4->data[megatile].VR4Index[suby * 4 + subx] & 1) == 1; //하위 1비트는 상하반전

                    const VR4::VR4Data& vr4data = vr4->image[minitileindex];
                    const VF4::VF4Data& vf4data = vf4->data[megatile];

                    const int draw_offsetx = x * 32 + subx * 8;
                    const int draw_offsety = y * 32 + suby * 8;


                    //해당 미니타일의 이동가능여부를 계산.
                    //나중에 타일맵에 실제 연결할 때 추가 작업 해줘야 함
                    bool Walkable = false;
                    if (vf4data.MiniTileFlags[suby * 4 + subx] & 0x0001)
                        Walkable = true;

                    //미니타일의 8*8을 순회하면서 렌더링.
                    for (j = 0; j < 8; j++) {
                        for (i = 0; i < 8; i++) {
                            int drawx = draw_offsetx + (flipped ? 7 - i : i);
                            int drawy = draw_offsety + j;
                            const WPE::WPEData& wpedata = wpe->data[vr4data.color[j * 8 + i]];
                            RGBAbyte color = { wpedata.r, wpedata.g, wpedata.b, 255 };
                            img[drawy][drawx] = color; //setpixel(drawx, drawy, color); 와 비슷한 역할.
                            

                        }
                    }
                }
            }
        }
    }

    //PNG 파일로 출력!
    //lodepng_encode32_file("Rendered.png", (unsigned char*)imgdata, imgw, imgh);

    delete[] imgdata;
    delete[] mtxmdata;

    delete cv5;
    delete vx4;
    delete vr4;
    delete wpe;


    return true;
}


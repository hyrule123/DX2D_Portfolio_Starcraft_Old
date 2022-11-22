#include "Map.h"
#include "../../PathManager.h"

//맵 파일 안의 데이터 추출 라이브러리 헤더
#include "StormLib.h"

//CCDO를 통해서 TileMapComponent를 복사해오기 위함
#include "../../CDO.h"
#include "../../Component/TileMapComponent.h"
#include "../Texture/Texture.h"
#include "../ResourceManager.h"


//맵 정보 읽기용
#define TerrainChunk    "ERA"
#define MapSizeChunk    "DIM"
#define TileMapChunk    "MTXM"







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

struct WPEFloatData
{
    float r;
    float g;
    float b;
    float padding;

    WPEFloatData(const WPE::WPEData& Data) :
        r(Data.r / 255.f),
        g(Data.g / 255.f),
        b(Data.b / 255.f),
        padding(1.f)
    {
    }
};


typedef struct RGBAbyte {
    uint8 r;
    uint8 g;
    uint8 b;
    uint8 a;

    RGBAbyte():
        r(),
        g(),
        b(),
        a()
    {}
    RGBAbyte(uint8 _r, uint8 _g, uint8 _b, uint8 _a) :
        r(_r),
        g(_g),
        b(_b),
        a(_a)
    {
    }

}RGBAbyte;

#include <packoff.h>


CMap::CMap() :
    m_LoadCheck(),
    m_LoadRef(),
    m_TileMap(),
    m_MapSizeX(),
    m_MapSizeY(),
    m_Terrain()
{
    SetTypeID<CMap>();
}

CMap::~CMap()
{
    SAFE_DELETE(m_TileMap);
}


int CMap::LoadMapDataFromFile(const TCHAR* FullPath)
{

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


    SAFE_DELETE_ARRAY(szBuffer);
    return nError;
}

CTileMapComponent* CMap::GetTileMap() const
{
    return m_TileMap;
}





void CMap::Save(FILE* File)
{
}

void CMap::Load(FILE* File)
{
}


void CMap::ReadMapData(char* Data, DWORD Size)
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


void CMap::ResetMapData()
{
    m_LoadCheck = 0;

    int Size = EMapDataTypeEnd;

    for (int i = 0; i < Size; ++i)
    {
        delete[] m_MapDataChunk[i].Data;
    }

    memset(m_MapDataChunk, 0, sizeof(Chunk) * Size);
}


bool CMap::LoadTileMap()
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
    m_MapSizeX = w;
    m_MapSizeY = h;


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

    m_Terrain = static_cast<TerrainInfo>(Info);

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

    //프로젝트는 UNORM을 사용중이므로 픽셀 색상정보를 전부 UNORM(0.f ~ 1.f로 변환해줘야한다.)
    // *************** 아님 픽셀정보는 0~255값으로 일단 등록함**********
    //std::vector<WPEFloatData> vecWPEFloat;
    //vecWPEFloat.reserve(256);
    //for (int i = 0; i < 256; ++i)
    //{
    //    vecWPEFloat.emplace_back(WPEFloatData(wpe->data[i]));
    //}
    for (int i = 0; i < 256; ++i)
    {
        wpe->data[i].padding = 255;
    }


    //memset(wpefloat, 0, sizeof(WPEFloat));
    //for (int i = 0; i < 256; ++i)
    //{
    //    for (int j = 0; j < 4; ++j)
    //    {
    //        wpefloat[i].data->r = static_cast<float>(wpe[i].data->r) / 255.f;
    //    }
    //    wpe[256]
    //}

    //mxtmdata 데이터는 2바이트 * 맵 사이즈 이다.
    uint16* mtxmdata = new uint16[w * h];
    memcpy(mtxmdata, m_MapDataChunk[TileMap].Data, w * h * 2);

    array2d<uint16> mtxm(mtxmdata, w);


    //이미지 파일의 데이터를 준비.
    //const int imgw = 32 * w, imgh = 32 * h;




    //이미지 기록
    //여기에서 타일을 생성하고 기록해주면 된다.
    m_TileMap = CCDO::ClonePLO<CTileMapComponent>();

    //'맵'에 사용할 타일맵이므로 무조건 고정된 위치에 생성한다.
    m_TileMap->SetWorldPosition(Vector2(0.f, 0.f));

    //메가타일의 크기는 32 * 32 사이즈이다.
    m_TileMap->CreateTile(ETileShape::Rect, w, h, Vector2(32.f, 32.f));

    //이동가능여부 타일생성
    m_TileMap->CreateTileWalkibility(w * 4, h * 4);

    //megatile index를 키값으로 unordered map을 사용해서 이미지의 중복생성을 방지
    std::vector<TextureResourceInfo*> vecInfo;

    //미리 메가타일이 나올수있는 최대 사이즈까지 예약
    vecInfo.reserve(w * h);

    //현재 등록된 메가타일이 중복되어있는지 확인하기 위한 용도
    //만약 등록되어있을 경우 해당 메가타일을 처음으로 등록한 인덱스를 반환(이 메가타일을 출력하도록 등록해야하므로)
    std::unordered_map<int, int> mapMegatile;
    auto iterEnd = mapMegatile.end();


    //메가타일 사이즈의 이미지 데이터를 생성(픽셀 정보는 Img->pixels에 들어갈 예정.)
    DirectX::Image* Img = new DirectX::Image;
    Img->format = DXGI_FORMAT_R8G8B8A8_UNORM;
    Img->width = static_cast<size_t>(32);
    Img->height = static_cast<size_t>(32);
    Img->pixels = new uint8_t[32 * 32 * 4];
    Img->rowPitch = 4 * 32;
    Img->slicePitch = Img->rowPitch * 32;

    //실제 타일맵에 추가될 인덱스 순서
    //타일에서 사용할 프레임 번호(스타크래프트 타일맵과 프레임워크의 타일맵 인덱스는 정 반대)
    int RealTileIndexY = h - 1;
    int MegaTileSavedIndex = 0;

    //size_t UNORMPitch = sizeof(float) * 4;

    //메가타일. 
    // 스타크래프트의 타일맵 시작은 좌상단부터이고
    // 현재 프레임워크의 타일맵 시작은 좌하단 부터이므로 y축은 거꾸로 순회돌아 준다.
    for (y = 0; y < h; ++y) {
        --RealTileIndexY;

        for (x = 0; x < w; ++x) {
            int group = mtxm[y][x] >> 4;
            int index = mtxm[y][x] & 0xf;
            //megatile을 얻고 안의 4*4개의 minitile들을 얻어서
            int megatile = cv5->group[group].MegatileIndex[index];

            auto iter = mapMegatile.find(megatile);

            int MiniTileIndexX = x * 4;
            int MiniTileIndexY = RealTileIndexY * 4;

            // 여기에서 타일맵에 실제로 추가해야 
            //등록되어있지 않을경우 메가타일을 순회
            if (iter == iterEnd)
            {
                //메가타일 안의 미니타일 4*4 순회
                for (suby = 0; suby < 4; suby++)
                {
                    for (subx = 0; subx < 4; subx++)
                    {

                        //렌더링할 이미지를 찾고 렌더링한다.
                        //여기서 vf파일도 읽어다가 pass/not pass 여부를 불러온다.
                        int minitileindex = vx4->data[megatile].VR4Index[suby * 4 + subx] >> 1; //상위 15비트는 이미지
                        bool flipped = (vx4->data[megatile].VR4Index[suby * 4 + subx] & 1) == 1; //하위 1비트는 상하반전

                        const VR4::VR4Data& vr4data = vr4->image[minitileindex];
                        const VF4::VF4Data& vf4data = vf4->data[megatile];


                        //현재 메가타일의 어느부분을 그리고있는지를 계산.
                        const int draw_offsetx = subx * 8;
                        const int draw_offsety = suby * 8;


                        //해당 미니타일의 이동가능여부를 계산.
                        if (vf4data.MiniTileFlags[suby * 4 + subx] & 0x0001)
                        {
                            m_TileMap->SetTileWalkability(MiniTileIndexX + subx , MiniTileIndexY + suby, ETileOption::Wall);
                        }
                            

                        //미니타일의 8*8을 순회하면서 렌더링.
                        for (j = 0; j < 8; j++) 
                        {
                            for (i = 0; i < 8; i++) 
                            {
                                int drawx = draw_offsetx + (flipped ? 7 - i : i);
                                int drawy = draw_offsety + j;
                                //const WPEFloatData& wpefloat = vecWPEFloat[vr4data.color[j * 8 + i]];
                                const WPE::WPEData& wpedata = wpe->data[vr4data.color[j * 8 + i]];

                                //메가타일에 픽셀을 작성한다.
                                int CurIdx = (drawy * 32 + drawx) * 4;
                                //memcpy(&Img->pixels[CurIdx], &wpefloat, UNORMPitch);
                                memcpy(&Img->pixels[CurIdx], &wpedata, 4);


                                //RGBAbyte color = { wpedata.r, wpedata.g, wpedata.b, 255 };
                                //int curIdx = (drawy * 8 + drawx) * 4;

                                //setpixel(drawx, drawy, color); 와 비슷한 역할.
                                //memcpy(&Img->pixels[curIdx], &color, 4); 
                            }
                        }
                    }
                }


                //메가타일에 이미지를 추가
                TextureResourceInfo* TexInfo = new TextureResourceInfo(true);
                if (FAILED(TexInfo->Image->InitializeFromImage(*Img)))
                {
                    assert(0);
                }

                vecInfo.emplace_back(TexInfo);



                //메가타일의 인덱스번호를 사용했다고 저장한다.
                mapMegatile.insert(std::make_pair(megatile, MegaTileSavedIndex));


                //타일에 프레임 번호를 저장
                m_TileMap->ChangeTileFrame(MegaTileSavedIndex, x, RealTileIndexY);
                ++MegaTileSavedIndex;


                
            }

            //메가타일이 이미 등록되어 있을 경우
            else
            {
                //출력해야할 타일 프레임을 unordered map에 저장되어 있는 프레임으로 등록
                int SavedMegaTileFrame = iter->second;
                m_TileMap->ChangeTileFrame(SavedMegaTileFrame, x, RealTileIndexY);

                //walkability map도 같은 부분의 인덱스를 가져와서 등록
                //메가타일 안의 미니타일 4*4 순회
                //기존에 등록되어있는 정보를 예외처리하면서 가져오는 것보다 이게 훨씬 빠르다
                for (suby = 0; suby < 4; suby++)
                {
                    for (subx = 0; subx < 4; subx++)
                    {
                        const VF4::VF4Data& vf4data = vf4->data[megatile];


                        //해당 미니타일의 이동가능여부를 계산.
                        if (vf4data.MiniTileFlags[suby * 4 + subx] & 0x0001)
                        {
                            m_TileMap->SetTileWalkability(MiniTileIndexX + subx, MiniTileIndexY + suby, ETileOption::Wall);
                        }
                    }
                }
            }
        }
    }


    //만들어낸 메가타일 이미지를 이미지파일로 등록
    bool Success = CResourceManager::GetInst()->LoadTextureArrayByvecTextureResourceInfo(m_FileNameMultiByte, vecInfo);

    CTexture* Tex = CResourceManager::GetInst()->FindTexture(m_FileNameMultiByte);
    
    m_TileMap->AddTileTextureArray(m_FileNameMultiByte);
    //Tex->SetName("TileTexture");

    //PNG 파일로 출력!
    //lodepng_encode32_file("Rendered.png", (unsigned char*)imgdata, imgw, imgh);
    

    delete[] Img->pixels;
    delete Img;

    delete[] mtxmdata;

    delete cv5;
    delete vx4;
    delete vr4;
    delete wpe;
    delete vf4;

    return Success;
}




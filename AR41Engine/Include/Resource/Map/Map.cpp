#include "Map.h"
#include "../../PathManager.h"

//�� ���� ���� ������ ���� ���̺귯�� ���
#include "StormLib.h"

//CCDO�� ���ؼ� TileMapComponent�� �����ؿ��� ����
#include "../../CDO.h"
#include "../../Component/TileMapComponent.h"
#include "../Texture/Texture.h"
#include "../ResourceManager.h"


//�� ���� �б��
#define TerrainChunk    "ERA"
#define MapSizeChunk    "DIM"
#define TileMapChunk    "MTXM"







//���� 2d array�� ���� �ٷ�� ���� Ŭ����.
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

//����ũ��: 52byte, �ް�Ÿ�� �ε���
typedef struct {
    struct CV5Data {
        //�ʿ���� ������
        uint8 Something[20];
        //�ʿ��� ������. 16���� Mega Tile Index�� ����Ǿ� ����
        uint16 MegatileIndex[16];
    } group[4096];
}CV5;


//����ũ��: 32byte, ������ɿ���, ����
typedef struct {
    struct VF4Data {
        uint16 MiniTileFlags[16];
    } data[65536];
}VF4;

//����ũ��: 32bytes, megatile�� � minitile�� �̷�������� ǥ��
typedef struct {
    struct VX4Data {
        uint16 VR4Index[16];
    } data[65536];
}VX4;

//����ũ��: 64byte - mini tile������ �ȼ��� WPE �ȷ�Ʈ�� �� �� ���� �ش��ϴ����� ����
typedef struct {
    struct VR4Data {
        uint8 color[64];
    } image[32768];
}VR4;

//����ũ��: 4byte
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


    //�� ������ �б�
    if (m_MapDataChunk[MapSize].length != 4)
        return false;

    int w = (int)*(unsigned short*)(m_MapDataChunk[MapSize].Data);
    int h = (int)*(unsigned short*)(m_MapDataChunk[MapSize].Data + 2);
    m_MapSizeX = w;
    m_MapSizeY = h;


    int x, y, subx, suby, i, j;



    //���� ���� �б�
    if (m_MapDataChunk[Terrain].length != 2)
        return false;

    //ù 1����Ʈ�� ���� ������ ����ִ�.
    unsigned char Info = m_MapDataChunk[Terrain].Data[0];

    //���� 4��Ʈ�� 0���� ��ȯ
    unsigned char bitshift = 0b11110000;
    Info &= ~bitshift;

    //Ȥ�ó� ��Ʈ����Ʈ ���� 8�� �ʰ��ϴ� ���� ���� ��� 8�� ���ش�.
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

    //������Ʈ�� UNORM�� ������̹Ƿ� �ȼ� ���������� ���� UNORM(0.f ~ 1.f�� ��ȯ������Ѵ�.)
    // *************** �ƴ� �ȼ������� 0~255������ �ϴ� �����**********
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

    //mxtmdata �����ʹ� 2����Ʈ * �� ������ �̴�.
    uint16* mtxmdata = new uint16[w * h];
    memcpy(mtxmdata, m_MapDataChunk[TileMap].Data, w * h * 2);

    array2d<uint16> mtxm(mtxmdata, w);


    //�̹��� ������ �����͸� �غ�.
    //const int imgw = 32 * w, imgh = 32 * h;




    //�̹��� ���
    //���⿡�� Ÿ���� �����ϰ� ������ָ� �ȴ�.
    m_TileMap = CCDO::ClonePLO<CTileMapComponent>();

    //'��'�� ����� Ÿ�ϸ��̹Ƿ� ������ ������ ��ġ�� �����Ѵ�.
    m_TileMap->SetWorldPosition(Vector2(0.f, 0.f));

    //�ް�Ÿ���� ũ��� 32 * 32 �������̴�.
    m_TileMap->CreateTile(ETileShape::Rect, w, h, Vector2(32.f, 32.f));

    //�̵����ɿ��� Ÿ�ϻ���
    m_TileMap->CreateTileWalkibility(w * 4, h * 4);

    //megatile index�� Ű������ unordered map�� ����ؼ� �̹����� �ߺ������� ����
    std::vector<TextureResourceInfo*> vecInfo;

    //�̸� �ް�Ÿ���� ���ü��ִ� �ִ� ��������� ����
    vecInfo.reserve(w * h);

    //���� ��ϵ� �ް�Ÿ���� �ߺ��Ǿ��ִ��� Ȯ���ϱ� ���� �뵵
    //���� ��ϵǾ����� ��� �ش� �ް�Ÿ���� ó������ ����� �ε����� ��ȯ(�� �ް�Ÿ���� ����ϵ��� ����ؾ��ϹǷ�)
    std::unordered_map<int, int> mapMegatile;
    auto iterEnd = mapMegatile.end();


    //�ް�Ÿ�� �������� �̹��� �����͸� ����(�ȼ� ������ Img->pixels�� �� ����.)
    DirectX::Image* Img = new DirectX::Image;
    Img->format = DXGI_FORMAT_R8G8B8A8_UNORM;
    Img->width = static_cast<size_t>(32);
    Img->height = static_cast<size_t>(32);
    Img->pixels = new uint8_t[32 * 32 * 4];
    Img->rowPitch = 4 * 32;
    Img->slicePitch = Img->rowPitch * 32;

    //���� Ÿ�ϸʿ� �߰��� �ε��� ����
    //Ÿ�Ͽ��� ����� ������ ��ȣ(��Ÿũ����Ʈ Ÿ�ϸʰ� �����ӿ�ũ�� Ÿ�ϸ� �ε����� �� �ݴ�)
    int RealTileIndexY = h - 1;
    int MegaTileSavedIndex = 0;

    //size_t UNORMPitch = sizeof(float) * 4;

    //�ް�Ÿ��. 
    // ��Ÿũ����Ʈ�� Ÿ�ϸ� ������ �»�ܺ����̰�
    // ���� �����ӿ�ũ�� Ÿ�ϸ� ������ ���ϴ� �����̹Ƿ� y���� �Ųٷ� ��ȸ���� �ش�.
    for (y = 0; y < h; ++y) {
        --RealTileIndexY;

        for (x = 0; x < w; ++x) {
            int group = mtxm[y][x] >> 4;
            int index = mtxm[y][x] & 0xf;
            //megatile�� ��� ���� 4*4���� minitile���� ��
            int megatile = cv5->group[group].MegatileIndex[index];

            auto iter = mapMegatile.find(megatile);

            int MiniTileIndexX = x * 4;
            int MiniTileIndexY = RealTileIndexY * 4;

            // ���⿡�� Ÿ�ϸʿ� ������ �߰��ؾ� 
            //��ϵǾ����� ������� �ް�Ÿ���� ��ȸ
            if (iter == iterEnd)
            {
                //�ް�Ÿ�� ���� �̴�Ÿ�� 4*4 ��ȸ
                for (suby = 0; suby < 4; suby++)
                {
                    for (subx = 0; subx < 4; subx++)
                    {

                        //�������� �̹����� ã�� �������Ѵ�.
                        //���⼭ vf���ϵ� �о�ٰ� pass/not pass ���θ� �ҷ��´�.
                        int minitileindex = vx4->data[megatile].VR4Index[suby * 4 + subx] >> 1; //���� 15��Ʈ�� �̹���
                        bool flipped = (vx4->data[megatile].VR4Index[suby * 4 + subx] & 1) == 1; //���� 1��Ʈ�� ���Ϲ���

                        const VR4::VR4Data& vr4data = vr4->image[minitileindex];
                        const VF4::VF4Data& vf4data = vf4->data[megatile];


                        //���� �ް�Ÿ���� ����κ��� �׸����ִ����� ���.
                        const int draw_offsetx = subx * 8;
                        const int draw_offsety = suby * 8;


                        //�ش� �̴�Ÿ���� �̵����ɿ��θ� ���.
                        if (vf4data.MiniTileFlags[suby * 4 + subx] & 0x0001)
                        {
                            m_TileMap->SetTileWalkability(MiniTileIndexX + subx , MiniTileIndexY + suby, ETileOption::Wall);
                        }
                            

                        //�̴�Ÿ���� 8*8�� ��ȸ�ϸ鼭 ������.
                        for (j = 0; j < 8; j++) 
                        {
                            for (i = 0; i < 8; i++) 
                            {
                                int drawx = draw_offsetx + (flipped ? 7 - i : i);
                                int drawy = draw_offsety + j;
                                //const WPEFloatData& wpefloat = vecWPEFloat[vr4data.color[j * 8 + i]];
                                const WPE::WPEData& wpedata = wpe->data[vr4data.color[j * 8 + i]];

                                //�ް�Ÿ�Ͽ� �ȼ��� �ۼ��Ѵ�.
                                int CurIdx = (drawy * 32 + drawx) * 4;
                                //memcpy(&Img->pixels[CurIdx], &wpefloat, UNORMPitch);
                                memcpy(&Img->pixels[CurIdx], &wpedata, 4);


                                //RGBAbyte color = { wpedata.r, wpedata.g, wpedata.b, 255 };
                                //int curIdx = (drawy * 8 + drawx) * 4;

                                //setpixel(drawx, drawy, color); �� ����� ����.
                                //memcpy(&Img->pixels[curIdx], &color, 4); 
                            }
                        }
                    }
                }


                //�ް�Ÿ�Ͽ� �̹����� �߰�
                TextureResourceInfo* TexInfo = new TextureResourceInfo(true);
                if (FAILED(TexInfo->Image->InitializeFromImage(*Img)))
                {
                    assert(0);
                }

                vecInfo.emplace_back(TexInfo);



                //�ް�Ÿ���� �ε�����ȣ�� ����ߴٰ� �����Ѵ�.
                mapMegatile.insert(std::make_pair(megatile, MegaTileSavedIndex));


                //Ÿ�Ͽ� ������ ��ȣ�� ����
                m_TileMap->ChangeTileFrame(MegaTileSavedIndex, x, RealTileIndexY);
                ++MegaTileSavedIndex;


                
            }

            //�ް�Ÿ���� �̹� ��ϵǾ� ���� ���
            else
            {
                //����ؾ��� Ÿ�� �������� unordered map�� ����Ǿ� �ִ� ���������� ���
                int SavedMegaTileFrame = iter->second;
                m_TileMap->ChangeTileFrame(SavedMegaTileFrame, x, RealTileIndexY);

                //walkability map�� ���� �κ��� �ε����� �����ͼ� ���
                //�ް�Ÿ�� ���� �̴�Ÿ�� 4*4 ��ȸ
                //������ ��ϵǾ��ִ� ������ ����ó���ϸ鼭 �������� �ͺ��� �̰� �ξ� ������
                for (suby = 0; suby < 4; suby++)
                {
                    for (subx = 0; subx < 4; subx++)
                    {
                        const VF4::VF4Data& vf4data = vf4->data[megatile];


                        //�ش� �̴�Ÿ���� �̵����ɿ��θ� ���.
                        if (vf4data.MiniTileFlags[suby * 4 + subx] & 0x0001)
                        {
                            m_TileMap->SetTileWalkability(MiniTileIndexX + subx, MiniTileIndexY + suby, ETileOption::Wall);
                        }
                    }
                }
            }
        }
    }


    //���� �ް�Ÿ�� �̹����� �̹������Ϸ� ���
    bool Success = CResourceManager::GetInst()->LoadTextureArrayByvecTextureResourceInfo(m_FileNameMultiByte, vecInfo);

    CTexture* Tex = CResourceManager::GetInst()->FindTexture(m_FileNameMultiByte);
    
    m_TileMap->AddTileTextureArray(m_FileNameMultiByte);
    //Tex->SetName("TileTexture");

    //PNG ���Ϸ� ���!
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




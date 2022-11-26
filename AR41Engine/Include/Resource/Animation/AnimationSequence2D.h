#pragma once

#include "../GameResource.h"

//여러개의 다른 텍스처를 연결해서도 애니메이션을 재생할 수 있도록 변경함.

class CAnimationSequence2D :
    public CGameResource
{
    friend class CAnimationManager;

protected:
    CAnimationSequence2D();
    CAnimationSequence2D(const CAnimationSequence2D& Anim);
    ~CAnimationSequence2D();

private:
    std::vector<CSharedPtr<class CTexture>>  m_Texture;
    std::vector<Animation2DFrameData>   m_vecFrameData;
    EAnimation2DType    m_Anim2DType;

    //ArrayIndex 모드일 경우 행렬의 사이즈 저장
    int m_RowNum;   //전체 행의 갯수
    int m_ColNum;   //전체 열의 갯수
    int m_RowStart;
    int m_ColStart;
    int m_RowSize;
    int m_ColSize;


public:
    inline int GetRowNum() const;
    inline int GetColNum() const;
    inline int GetRowSize() const;
    inline int GetColSize() const;


    inline EAnimation2DType GetAnim2DType()    const;


public:
    inline class CTexture* GetTexture(int TexIndex = 0)    const;

    inline const Animation2DFrameData& GetFrameData(int Index) const;

    inline int GetFrameCount() const;

public:

    //============== 초기화 ================
    bool Init(class CTexture* Texture);
    bool Init(const std::string& Name, const TCHAR* FileName,
        const std::string& PathName = TEXTURE_PATH);
    bool InitFullPath(const std::string& Name, const TCHAR* FullPath);
    bool Init(const std::string& Name, const std::vector<const TCHAR*>& vecFileName,
        const std::string& PathName = TEXTURE_PATH);
    bool InitFullPath(const std::string& Name, const std::vector<const TCHAR*>& vecFullPath);


    //============= 텍스처 추가 ==============
    void AddTexture(class CTexture* Texture);
    bool AddTexture(const std::string& Name);


    //============= 프레임 추가 ==============
    void AddFrame(const Vector2& Start, const Vector2& End, int TexIndex = 0);
    void AddFrame(float StartX, float StartY, float EndX, float EndY, int TexIndex = 0);
    void AddFrameByNumAtlas(int NumAtlas, int TexIndex = 0);


    //============== ArrayIndexed ================ 타입으로 추가
    void AddFrameByTileNumber
    (
        EAnimation2DType Type,
        int TileRowNum, int TileColNum,
        int ColStart = -1, int ColSize = -1,
        int RowStart = -1, int RowSize = -1,
        int TexIndex = 0
    );

    ////Atlas 텍스처를 Row Col 수만큼 잘라낸다.(특정 행이나 열을 사용하지 않더라도 일단 잘라내서 보관)
    //void SetFrameAllByTileNumber(int TileRowNum, int TileColNum);


    //일반 애니메이션
    void AddFrameAll(int Count, const Vector2& Start, const Vector2& End, int TexIndex = 0);
    void AddFrameAll(int Count, float StartX, float StartY, float EndX,
        float EndY, int TexIndex = 0);
    void DeleteFrame(int Index, int TexIndex = 0);
    void ClearFrame();



    CAnimationSequence2D* Clone();
    bool Save(const char* FullPath);
    bool Load(const char* FullPath);
    bool Save(const char* FileName, const std::string& PathName);
    bool Load(const char* FileName, const std::string& PathName);
};

inline int CAnimationSequence2D::GetRowNum() const
{
    return m_RowNum;
}

inline int CAnimationSequence2D::GetColNum() const
{
    return m_ColNum;
}

inline int CAnimationSequence2D::GetRowSize() const
{
    return m_RowSize;
}

inline int CAnimationSequence2D::GetColSize() const
{
    return m_ColSize;
}

inline EAnimation2DType CAnimationSequence2D::GetAnim2DType()    const
{
    return m_Anim2DType;
}

inline class CTexture* CAnimationSequence2D::GetTexture(int TexIndex)    const
{
    if (m_Texture.size() <= TexIndex)
        return nullptr;

    return m_Texture[TexIndex];
}

inline const Animation2DFrameData& CAnimationSequence2D::GetFrameData(int Index) const
{
    return m_vecFrameData[Index];
}

inline int CAnimationSequence2D::GetFrameCount() const
{
    if (m_Anim2DType == EAnimation2DType::AtlasIndexed)
        return m_RowSize;

    return (int)m_vecFrameData.size();
}
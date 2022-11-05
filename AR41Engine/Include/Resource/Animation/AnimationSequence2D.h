#pragma once

#include "../../Ref.h"


class CAnimationSequence2D :
    public CRef
{
    friend class CAnimationManager;

private:
    CAnimationSequence2D();
    CAnimationSequence2D(const CAnimationSequence2D& Anim);
    ~CAnimationSequence2D();

private:
    class CScene* m_Scene;
    CSharedPtr<class CTexture>  m_Texture;
    std::vector<Animation2DFrameData>   m_vecFrameData;
    EAnimation2DType    m_Anim2DType;

    //Array 모드일 경우 행렬의 갯수 저장.
    int m_ColStart;
    int m_RowStart;
    int m_ColSize;
    int m_RowSize;


public:
    inline int GetRowStart() const;
    inline int GetColStart() const;
    inline int GetRowNum() const;
    inline int GetColNum() const;


    EAnimation2DType GetAnim2DType()    const
    {
        return m_Anim2DType;
    }

    void SetScene(class CScene* Scene)
    {
        m_Scene = Scene;
    }

public:
    class CTexture* GetTexture()    const
    {
        return m_Texture;
    }

    const Animation2DFrameData& GetFrameData(int Index) const
    {
        return m_vecFrameData[Index];
    }

    int GetFrameCount() const
    {
        if (m_Anim2DType == EAnimation2DType::AtlasIndexed)
            return m_ColSize;

        return (int)m_vecFrameData.size();
    }

public:
    bool Init(class CTexture* Texture);
    bool Init(const std::string& Name, const TCHAR* FileName,
        const std::string& PathName = TEXTURE_PATH);
    bool InitFullPath(const std::string& Name, const TCHAR* FullPath);
    bool Init(const std::string& Name, const std::vector<const TCHAR*>& vecFileName,
        const std::string& PathName = TEXTURE_PATH);
    bool InitFullPath(const std::string& Name, const std::vector<const TCHAR*>& vecFullPath);
    void AddFrame(const Vector2& Start, const Vector2& End);
    void AddFrame(float StartX, float StartY, float EndX, float EndY);
    void AddFrameByTileNumber
    (
        EAnimation2DType Type,
        int TileRowNum, int TileColNum,
        int ColStart = -1, int ColSize = -1,
        int RowStart = -1, int RowSize = -1
    );
    void AddFrameAll(int Count, const Vector2& Start, const Vector2& End);
    void AddFrameAll(int Count, float StartX, float StartY, float EndX,
        float EndY);
    void DeleteFrame(int Index);
    void ClearFrame();
    CAnimationSequence2D* Clone();
    bool Save(const char* FullPath);
    bool Load(const char* FullPath);
    bool Save(const char* FileName, const std::string& PathName);
    bool Load(const char* FileName, const std::string& PathName);
};


inline int CAnimationSequence2D::GetRowStart() const
{
    return m_RowStart;
}

inline int CAnimationSequence2D::GetColStart() const
{
    return m_ColStart;
}

inline int CAnimationSequence2D::GetRowNum() const
{
    return m_RowSize;
}

inline int CAnimationSequence2D::GetColNum() const
{
    return m_ColSize;
}

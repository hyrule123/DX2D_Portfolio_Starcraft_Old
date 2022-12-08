#pragma once

#include "../GameResource.h"

//�������� �ٸ� �ؽ�ó�� �����ؼ��� �ִϸ��̼��� ����� �� �ֵ��� ������.

enum class EAnimSeqArrayIndexMainAxis : bool
{
    X,
    Y
};

class CAnimationSequence2D :
    public CGameResource
{
    friend class CAnimationManager;

protected:
    CAnimationSequence2D();
    ~CAnimationSequence2D();

public:
    bool Save(const char* FullPath);
    bool Load(const char* FullPath);
    bool Save(const char* FileName, const std::string& PathName);
    bool Load(const char* FileName, const std::string& PathName);

private:
    //1. �ؽ�ó���°� �ٸ� ���(Frame -> Atlas)���� ��� �����ؾ� �� -> ���
    //�ٵ� �̷��� �ϸ� �ν��Ͻ����� ����� ��� �� ���� �ؽ�ó�� ��� ����س��ƾ� �Ѵ�.
    //���� �ִϸ��̼� �������� �и��س��� �ִϸ��̼��� ������� ��� ������ �س��°� ������.
    //2. Main-Shadow�� ��� Array ���·� ����ؼ� ����ϸ� �ɵ�.
    CSharedPtr<class CTexture>  m_Texture;
    std::vector<Animation2DFrameData>   m_vecFrameData;
    EAnimation2DType    m_Anim2DType;

    //ArrayIndex ����� ��� ����� ������ ����
    int m_RowStart;
    int m_ColStart;
    int m_RowSize;
    int m_ColSize;

    //ArrayIndex ��Ȳ���� ���.
    //ArrayIndex Ÿ������ �ִϸ��̼��� ������ ��� �⺻���� RowSize(Y), ColSize(X)��ŭ �ڵ� ����
    //y�� ������� ����ϴ� ���� �ƴ϶� ������ ����ϰų� Ư�� ������ ����ؾ� �� ��� ������ ����ؼ� ����Ѵ�.
    //������� 123 ������ ��ϵ� ������ ������ 132222... ������ ����ϰ��� �Ҷ� 
    std::vector<int> m_vecCustomFrameIndexX;
    std::vector<int> m_vecCustomFrameIndexY;
    EAnimSeqArrayIndexMainAxis m_MainAxis;
    

    

public:
    inline int GetRowSize() const;
    inline int GetColSize() const;
    const Animation2DFrameData* GetFrameDataArrayIndexed(int IndexX, int IndexY) const;
    inline void AddCustomFrameX(int CustomFrameNum);
    inline void SetCustomFrameX(std::vector<int>& vecCustomFrame);
    inline void SetCustomFrameX(int Frame, int CustomFrameNum);
    inline void AddCustomFrameY(int CustomFrameNum);
    inline void SetCustomFrameY(std::vector<int>& vecCustomFrame);
    inline void SetCustomFrameY(int Frame, int CustomFrameNum);


    inline EAnimation2DType GetAnim2DType()    const;


public:
    inline class CTexture* GetTexture()    const;

    inline const Animation2DFrameData* GetFrameData(int Index) const;


    inline int GetFrameCount() const;

public:

    //============== �ʱ�ȭ ================
    bool Init(class CTexture* Texture);
    bool Init(const std::string& Name, const TCHAR* FileName,
        const std::string& PathName = TEXTURE_PATH);
    bool InitFullPath(const std::string& Name, const TCHAR* FullPath);
    bool Init(const std::string& Name, const std::vector<const TCHAR*>& vecFileName,
        const std::string& PathName = TEXTURE_PATH);
    bool InitFullPath(const std::string& Name, const std::vector<const TCHAR*>& vecFullPath);



    //============= ������ �߰� ==============
    void AddFrame(const Vector2& Start, const Vector2& End, int TexIndex = 0);
    void AddFrame(float StartX, float StartY, float EndX, float EndY, int TexIndex = 0);

    //���߿� �ʿ��ҋ� ���
    //void AddFrameByNumAtlas(int NumAtlas, int TexIndex = 0);


    //============== ArrayIndexed ================ Ÿ������ �߰�
    //�⺻ ArrayIndexed Ÿ������ ������. Ÿ���� �Ϲ� Array ���·� �ٲٸ� ���� ����� ����.
    void AddFrameByTileNumber
    (
        int TileRowNum, int TileColNum,
        int RowStart = -1, int RowSize = -1,
        int ColStart = -1, int ColSize = -1,
        EAnimSeqArrayIndexMainAxis MainAxis = EAnimSeqArrayIndexMainAxis::Y 
    );


    ////Atlas �ؽ�ó�� Row Col ����ŭ �߶󳽴�.(Ư�� ���̳� ���� ������� �ʴ��� �ϴ� �߶󳻼� ����)
    //void SetFrameAllByTileNumber(int TileRowNum, int TileColNum);


    //�Ϲ� �ִϸ��̼�
    void AddFrameAll(int Count, const Vector2& Start, const Vector2& End, int TexIndex = 0);
    void AddFrameAll(int Count, float StartX, float StartY, float EndX,
        float EndY, int TexIndex = 0);
    void DeleteFrame(int Index, int TexIndex = 0);
    void ClearFrame();




};


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

inline class CTexture* CAnimationSequence2D::GetTexture()    const
{
    return m_Texture;
}



inline const Animation2DFrameData* CAnimationSequence2D::GetFrameData(int Index) const
{
    return &m_vecFrameData[Index];
}



inline int CAnimationSequence2D::GetFrameCount() const
{

    if (m_Anim2DType == EAnimation2DType::AtlasIndexed)
    {
        switch (m_MainAxis)
        {
        case EAnimSeqArrayIndexMainAxis::X:
            return (int)m_vecCustomFrameIndexX.size();
        case EAnimSeqArrayIndexMainAxis::Y:
            return (int)m_vecCustomFrameIndexY.size();
        }
        
    }
        

    return (int)m_vecFrameData.size();
}


inline void CAnimationSequence2D::AddCustomFrameX(int CustomFrameNum)
{
    m_vecCustomFrameIndexX.push_back(CustomFrameNum);
}

inline void CAnimationSequence2D::SetCustomFrameX(std::vector<int>& vecCustomFrame)
{
    m_vecCustomFrameIndexX.clear();
    m_vecCustomFrameIndexX = std::move(vecCustomFrame);
}

inline void CAnimationSequence2D::SetCustomFrameX(int Frame, int CustomFrameNum)
{
    if (m_vecCustomFrameIndexX.size() <= Frame)
        return;

    m_vecCustomFrameIndexX[Frame] = CustomFrameNum;
}

inline void CAnimationSequence2D::AddCustomFrameY(int CustomFrameNum)
{
    m_vecCustomFrameIndexY.push_back(CustomFrameNum);
}

inline void CAnimationSequence2D::SetCustomFrameY(std::vector<int>& vecCustomFrame)
{
    m_vecCustomFrameIndexY.clear();
    m_vecCustomFrameIndexY = std::move(vecCustomFrame);
}

inline void CAnimationSequence2D::SetCustomFrameY(int Frame, int CustomFrameNum)
{
    if (m_vecCustomFrameIndexY.size() <= Frame)
        return;

    m_vecCustomFrameIndexY[Frame] = CustomFrameNum;
}
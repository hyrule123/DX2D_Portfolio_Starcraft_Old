#pragma once

#include "../GameResource.h"

//여러개의 다른 텍스처를 연결해서도 애니메이션을 재생할 수 있도록 변경함.

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
    //1. 텍스처형태가 다를 경우(Frame -> Atlas)에도 재생 가능해야 함 -> 취소
    //근데 이렇게 하면 인스턴싱으로 출력할 경우 두 장의 텍스처를 모두 등록해놓아야 한다.
    //차라리 애니메이션 시퀀스를 분리해놓고 애니메이션이 끝날경우 재생 예약을 해놓는게 맞을듯.
    //2. Main-Shadow의 경우 Array 형태로 등록해서 출력하면 될듯.
    CSharedPtr<class CTexture>  m_Texture;
    std::vector<Animation2DFrameData>   m_vecFrameData;
    EAnimation2DType    m_Anim2DType;

    //ArrayIndex 모드일 경우 행렬의 사이즈 저장
    int m_RowStart;
    int m_ColStart;
    int m_RowSize;
    int m_ColSize;

    //ArrayIndex 상황에서 사용.
    //ArrayIndex 타입으로 애니메이션을 생성할 경우 기본으로 RowSize(Y), ColSize(X)만큼 자동 생성
    //y축 순서대로 재생하는 것이 아니라 띄엄띄엄 재생하거나 특정 구간만 재생해야 할 경우 별도로 등록해서 사용한다.
    //예를들어 123 순으로 등록된 프레임 정보를 132222... 등으로 재생하고자 할때 
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

    //============== 초기화 ================
    bool Init(class CTexture* Texture);
    bool Init(const std::string& Name, const TCHAR* FileName,
        const std::string& PathName = TEXTURE_PATH);
    bool InitFullPath(const std::string& Name, const TCHAR* FullPath);
    bool Init(const std::string& Name, const std::vector<const TCHAR*>& vecFileName,
        const std::string& PathName = TEXTURE_PATH);
    bool InitFullPath(const std::string& Name, const std::vector<const TCHAR*>& vecFullPath);



    //============= 프레임 추가 ==============
    void AddFrame(const Vector2& Start, const Vector2& End, int TexIndex = 0);
    void AddFrame(float StartX, float StartY, float EndX, float EndY, int TexIndex = 0);

    //나중에 필요할떄 등록
    //void AddFrameByNumAtlas(int NumAtlas, int TexIndex = 0);


    //============== ArrayIndexed ================ 타입으로 추가
    //기본 ArrayIndexed 타입으로 설정됨. 타입을 일반 Array 형태로 바꾸면 순차 재생도 가능.
    void AddFrameByTileNumber
    (
        int TileRowNum, int TileColNum,
        int RowStart = -1, int RowSize = -1,
        int ColStart = -1, int ColSize = -1,
        EAnimSeqArrayIndexMainAxis MainAxis = EAnimSeqArrayIndexMainAxis::Y 
    );


    ////Atlas 텍스처를 Row Col 수만큼 잘라낸다.(특정 행이나 열을 사용하지 않더라도 일단 잘라내서 보관)
    //void SetFrameAllByTileNumber(int TileRowNum, int TileColNum);


    //일반 애니메이션
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
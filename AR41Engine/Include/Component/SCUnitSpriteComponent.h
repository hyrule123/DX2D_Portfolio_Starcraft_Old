#pragma once
#include "SpriteComponent.h"

#include "../Resource/Shader/SharedStructuredBuffer.h"

#include "../Resource/Shader/ShaderInfo.h"


//실제 레이어는 이렇게 구성되고,
enum class ETextureLayer
{
    UnitShadow,
    UnitMain,
    UnitTop,
    UnitEffect,
    UnitAttack,
    UnitBoost,
    Max
};

//애니메이션에는 이렇게 겹쳐서 들어감.
enum EAnimationLayer
{
    Anim_Shadow_Main,
    Anim_Top,
    Anim_Effect,
    Anim_Attack_Boost,
    Anim_Max
};


enum class EUnitActions
{
    Birth,
    Idle,
    Move,
    SurfaceAttack,
    AirAttack,
    Death,
    Debris,
    Construction,
    ConstructionComplete,
    BuildUnit
};


class CSCUnitSpriteComponent :
    public CSpriteComponent
{
    friend class CCDO;
    friend class CGameObject;
    friend class CScene;

protected:
    CSCUnitSpriteComponent();
    CSCUnitSpriteComponent(const CSCUnitSpriteComponent& component);
    virtual ~CSCUnitSpriteComponent();
public:
    virtual bool CDOPreload();
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual void RenderInstanced();
    virtual CSCUnitSpriteComponent* Clone()    const;
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);

    virtual bool SetTexture(class CTexture* Texture, int Index = 0);


protected:
    CSharedPtr<class CMaterial> m_Material;


    class CAnimation2D* m_UnitAnimLayer[Anim_Max];

    //각자 개인이 사용할 용도
    SCUnitSBuffer m_PrivateSBuffer;


    //딱 하나의 구조화버퍼 주소를 생성(CDOPreload 시점에서)해놓고 모두가 공유해서 사용한다.
    CSharedPtr<class CSCUnitConstantBuffer > m_CBuffer;
    CSharedPtr<CSharedStructuredBuffer<SCUnitSBuffer>> m_SBufferInfo;
    std::shared_ptr<std::unordered_map<EUnitActions, std::string>> m_UnitActions;
    //class CStructuredBuffer* m_SBuffer;
    //std::vector<SCUnitSBuffer> m_vecSBufferInfo;

protected:
    void TurnOnSBufferFlag(ESCUnitSBufferFlag Flag);
    void TurnOffSBufferFlag(ESCUnitSBufferFlag Flag);

    //유닛에 지정된 액션과 애니메이션을 연결
    void MakePairActionAnimationName(EUnitActions ENumAction, const std::string& AnimationName);

public:
    class CAnimation2D* GetUnitAnimLayer(int Index);
    bool AddUnitAnimation(ETextureLayer Layer, const std::string& Name, const std::string& SequenceName,
        float PlayTime = 1.f, float PlayScale = 1.f,
        bool Loop = false, bool Reverse = false);

    bool AddUnitAnimation(ETextureLayer Layer, const std::string& Name, class CAnimationSequence2D* Sequence, float PlayTime = 1.f, float PlayScale = 1.f,
        bool Loop = false, bool Reverse = false);
};


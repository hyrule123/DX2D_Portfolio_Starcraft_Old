#pragma once
#include "SpriteComponent.h"

#include "../Resource/Shader/SharedStructuredBuffer.h"

enum class EUnitAnimLayer
{
    UnitBackMain,
    UnitFront,
    UnitAttackBoostEffect,
    Max
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
    virtual CSCUnitSpriteComponent* Clone()    const;
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);

    virtual bool SetTexture(class CTexture* Texture, int Index = 0);


protected:
    CSharedPtr<class CMaterial> m_Material;
    class CSCUnitConstantBuffer* m_CBuffer;

    std::vector<CSharedPtr<class CAnimation2D>> m_UnitAnimLayer;

    //각자 개인이 사용할 용도
    SCUnitSBuffer m_PrivateSBuffer;


    //딱 하나의 구조화버퍼 시스템을 생성해놓고 모두가 공유해서 사용한다.
    CSharedPtr<CSharedStructuredBuffer<SCUnitSBuffer>> m_SBufferInfo;
    //class CStructuredBuffer* m_SBuffer;
    //std::vector<SCUnitSBuffer> m_vecSBufferInfo;

public:
    class CAnimation2D* GetUnitAnimLayer(int Index);
    bool AddUnitAnimation(EUnitAnimLayer Layer, const std::string& Name, const std::string& SequenceName,
        float PlayTime = 1.f, float PlayScale = 1.f,
        bool Loop = false, bool Reverse = false);

    bool AddUnitAnimation(EUnitAnimLayer Layer, const std::string& Name, class CAnimationSequence2D* Sequence, float PlayTime = 1.f, float PlayScale = 1.f,
        bool Loop = false, bool Reverse = false);
};


#pragma once
#include "SpriteComponent.h"

#include "../Resource/Shader/SharedStructuredBuffer.h"


#include "../Resource/Shader/ShaderInfo.h"
#include "../SCUnitInfo.h"


//이 클래스는 상속받아서 사용하기 위한 용도임. 따라서 CDO를 따로 생성하지 않음.

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
    //이 클래스를 상속받는 각 유닛은 CDOPreload()를 재정의해서 반드시 SCUnitInfo를 등록해줘야 한다.
    //이 함수는 상속받는 클래스에서 반드시 기본 설정을 완료하고 호출할것
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

    //스타크래프트 애니메이션용 SetTexture
    virtual bool SetTexture(class CTexture* Texture, int Index = 0);


protected:

    class CSCUnitRootComponent* m_SCUnitRoot;

    bool m_Selected;
    
    CSharedPtr<class CMaterial> m_Material;
    class CAnimation2D* m_AnimationUnitLayer[AnimLayer_Max];

    //각자 개별 클래스별 상태를 저장할 구조화 버퍼의 구조체.(매 로직마다 m_SBufferInfo에 푸시백)
    SCUnit_SBuffer m_PrivateSBuffer;

    //현재 액션 상태
    ESCUnit_Actions m_PrevAction;
    ESCUnit_Actions m_CurrentAction;

    //딱 하나의 구조화버퍼 주소를 생성(CDOPreload 시점에서)해놓고 모두가 공유해서 사용한다.
    std::shared_ptr<class CSCUnitConstantBuffer> m_CBuffer;
    CSharedPtr<CSharedStructuredBuffer<SCUnit_SBuffer>> m_SBufferInfo;
    std::shared_ptr<std::unordered_map<ESCUnit_Actions, SCUnit_Anim_LayerNameBind>> m_mapUnitActions;
    //class CStructuredBuffer* m_SBuffer;
    //std::vector<SCUnit_SBuffer> m_vecSBufferInfo;

protected:
    //현재 액션을 계산. 각 유닛마다 액션 계산 형태가 다르므로 가상함수를 통해 구현
    virtual void ComputeCurrentAction();

    void TurnOnSBufferFlag(ESCUnit_SBufferFlag Flag);
    void TurnOffSBufferFlag(ESCUnit_SBufferFlag Flag);

    //유닛에 지정된 액션과 애니메이션을 연결
    void MakePairAction_AnimationName(ESCUnit_Actions ENumAction, const std::string& AnimationName);
    void ChangeAnimationByAction(ESCUnit_Actions ENumAction);
    void CreateNewAnimLayer(ESCUnit_AnimLayer Layer);

    //유닛 정보 구조체를 통해서 애니메이션의 재생시간 계산 및 들어가야 할 레이어를 분류하여 삽입한다.
    void RegisterSequence();

public:
    class CAnimation2D* GetUnitAnimLayer(int Index);
    //bool AddUnitAnimation(ESCUnit_TextureLayer Layer, const std::string& Name, const std::string& SequenceName,
    //    float PlayTime = 1.f, float PlayScale = 1.f,
    //    bool Loop = false, bool Reverse = false);

    //bool AddUnitAnimation(ESCUnit_TextureLayer Layer, const std::string& Name, class CAnimationSequence2D* Sequence, float PlayTime = 1.f, float PlayScale = 1.f,
    //    bool Loop = false, bool Reverse = false);
};


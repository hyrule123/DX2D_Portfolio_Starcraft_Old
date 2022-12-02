#pragma once
#include "SpriteComponent.h"

#include "../Resource/Shader/SharedStructuredBuffer.h"


#include "../Resource/Shader/ShaderInfo.h"
#include "../SCUnitInfo.h"


//�� Ŭ������ ��ӹ޾Ƽ� ����ϱ� ���� �뵵��. ���� CDO�� ���� �������� ����.

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
    //�� Ŭ������ ��ӹ޴� �� ������ CDOPreload()�� �������ؼ� �ݵ�� SCUnitInfo�� �������� �Ѵ�.
    //�� �Լ��� ��ӹ޴� Ŭ�������� �ݵ�� �⺻ ������ �Ϸ��ϰ� ȣ���Ұ�
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

    //��Ÿũ����Ʈ �ִϸ��̼ǿ� SetTexture
    virtual bool SetTexture(class CTexture* Texture, int Index = 0);


protected:

    class CSCUnitRootComponent* m_SCUnitRoot;

    bool m_Selected;
    
    CSharedPtr<class CMaterial> m_Material;
    class CAnimation2D* m_AnimationUnitLayer[AnimLayer_Max];

    //���� ���� Ŭ������ ���¸� ������ ����ȭ ������ ����ü.(�� �������� m_SBufferInfo�� Ǫ�ù�)
    SCUnit_SBuffer m_PrivateSBuffer;

    //���� �׼� ����
    ESCUnit_Actions m_PrevAction;
    ESCUnit_Actions m_CurrentAction;

    //�� �ϳ��� ����ȭ���� �ּҸ� ����(CDOPreload ��������)�س��� ��ΰ� �����ؼ� ����Ѵ�.
    std::shared_ptr<class CSCUnitConstantBuffer> m_CBuffer;
    CSharedPtr<CSharedStructuredBuffer<SCUnit_SBuffer>> m_SBufferInfo;
    std::shared_ptr<std::unordered_map<ESCUnit_Actions, SCUnit_Anim_LayerNameBind>> m_mapUnitActions;
    //class CStructuredBuffer* m_SBuffer;
    //std::vector<SCUnit_SBuffer> m_vecSBufferInfo;

protected:
    //���� �׼��� ���. �� ���ָ��� �׼� ��� ���°� �ٸ��Ƿ� �����Լ��� ���� ����
    virtual void ComputeCurrentAction();

    void TurnOnSBufferFlag(ESCUnit_SBufferFlag Flag);
    void TurnOffSBufferFlag(ESCUnit_SBufferFlag Flag);

    //���ֿ� ������ �׼ǰ� �ִϸ��̼��� ����
    void MakePairAction_AnimationName(ESCUnit_Actions ENumAction, const std::string& AnimationName);
    void ChangeAnimationByAction(ESCUnit_Actions ENumAction);
    void CreateNewAnimLayer(ESCUnit_AnimLayer Layer);

    //���� ���� ����ü�� ���ؼ� �ִϸ��̼��� ����ð� ��� �� ���� �� ���̾ �з��Ͽ� �����Ѵ�.
    void RegisterSequence();

public:
    class CAnimation2D* GetUnitAnimLayer(int Index);
    //bool AddUnitAnimation(ESCUnit_TextureLayer Layer, const std::string& Name, const std::string& SequenceName,
    //    float PlayTime = 1.f, float PlayScale = 1.f,
    //    bool Loop = false, bool Reverse = false);

    //bool AddUnitAnimation(ESCUnit_TextureLayer Layer, const std::string& Name, class CAnimationSequence2D* Sequence, float PlayTime = 1.f, float PlayScale = 1.f,
    //    bool Loop = false, bool Reverse = false);
};


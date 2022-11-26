#pragma once

#include "PrimitiveComponent.h"
#include "../Animation/Animation2D.h"

class CSpriteComponent :
    public CPrimitiveComponent
{
    friend class CCDO;
    friend class CGameObject;
    friend class CScene;

protected:
    CSpriteComponent();
    CSpriteComponent(const CSpriteComponent& component);
    virtual ~CSpriteComponent();


public:
    virtual bool CDOPreload();
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CSpriteComponent* Clone()    const;
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);

protected:
    CSharedPtr<CAnimation2D> m_Animation;

public:
    inline std::string GetAnimationName()  const;

    inline CAnimation2D* GetAnimation()    const;

    void GetAnimationNames(std::vector<std::string>& vecNames);

public:
    virtual bool SetTexture(class CTexture* Texture, int Index = 0);
    bool SetTexture(const std::string& Name, const TCHAR* FileName,
        const std::string& PathName = TEXTURE_PATH);
    bool SetTextureFullPath(const std::string& Name, const TCHAR* FullPath);
    bool SetTexture(const std::string& Name, const std::vector<const TCHAR*>& vecFileName,
        const std::string& PathName = TEXTURE_PATH);
    bool SetTextureFullPath(const std::string& Name, const std::vector<const TCHAR*>& vecFullPath);
    void SetTextureFrameIndex(int Index);

    class CTexture* GetTexture(int Index = 0)    const;

    void ClearAnimation();

public:
    template <typename T>
    inline T* SetAnimation(const std::string& Name);

    //Array ����� ����ϴ� �ִϸ��̼ǿ��� Row�� ��ȣ�� ������ �� ����ϴ� �޼ҵ�.
    //���� ���� Ư���� Row ��ȣ�� �����ؾ� �ϴ� ��� �������ؼ� ����Ѵ�.
    virtual inline int GetRowIndex() const;
};

template <typename T>
inline T* CSpriteComponent::SetAnimation(const std::string& Name)
{
    T* Anim = new T;

    Anim->SetName(Name);
    Anim->m_Owner = this;

    if (!Anim->Init())
    {
        SAFE_DELETE(Anim);
        return nullptr;
    }

    m_Animation = Anim;

    return Anim;
}

inline int CSpriteComponent::GetRowIndex() const
{
    return 0;
}



inline std::string CSpriteComponent::GetAnimationName()  const
{
    if (!m_Animation)
        return "None";

    return m_Animation->GetAnimationClassName();
}

inline CAnimation2D* CSpriteComponent::GetAnimation()    const
{
    return m_Animation;
}
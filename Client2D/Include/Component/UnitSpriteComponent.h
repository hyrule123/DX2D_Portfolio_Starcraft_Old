#pragma once

#include "Component\SpriteComponent.h"

class CUnitSpriteComponent :
    public CSpriteComponent
{
    friend class CGameObject;
    friend class CScene;
    friend class CCDO;

protected:
    CUnitSpriteComponent();
    CUnitSpriteComponent(const CUnitSpriteComponent& component);
    virtual ~CUnitSpriteComponent();

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CUnitSpriteComponent* Clone()    const;
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);

    virtual int GetRowIndex() const;

protected:
    class CSCUnitRootComponent* m_UnitRoot;
};



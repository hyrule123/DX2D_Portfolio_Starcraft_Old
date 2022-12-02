#pragma once
#include "Component\SCUnitSpriteComponent.h"
class CUltraliskComponent :
    public CSCUnitSpriteComponent
{
    friend class CGameObject;
    friend class CScene;
    friend class CCDO;

protected:
    CUltraliskComponent();
    CUltraliskComponent(const CUltraliskComponent& component);
    virtual ~CUltraliskComponent();

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CUltraliskComponent* Clone()    const;
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);

    virtual int GetRowIndex() const;

protected:
    class CSCUnitRootComponent* m_UnitRoot;

};


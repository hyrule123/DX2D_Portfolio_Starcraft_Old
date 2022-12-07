#pragma once
#include "Component\SCUnitSpriteComponent.h"


class CSCUltraliskComponent :
    public CSCUnitSpriteComponent
{
    friend class CGameObject;
    friend class CScene;
    friend class CCDO;

protected:
    CSCUltraliskComponent();
    CSCUltraliskComponent(const CSCUltraliskComponent& component);
    virtual ~CSCUltraliskComponent();

public:
    virtual void Start();
    virtual bool CDOPreload();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CSCUltraliskComponent* Clone()    const;
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);

    virtual int GetRowIndex() const;

};


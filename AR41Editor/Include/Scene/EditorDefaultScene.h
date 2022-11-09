#pragma once

#include "Scene/SceneInfo.h"

class CEditorDefaultScene :
    public CSceneInfo
{
public:
    CEditorDefaultScene();
    CEditorDefaultScene(const CEditorDefaultScene& Scene);
    ~CEditorDefaultScene();

public:
    virtual bool Init();
    virtual CEditorDefaultScene* Clone() const;
};


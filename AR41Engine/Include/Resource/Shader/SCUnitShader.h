#pragma once

#include "GraphicShader.h"

class CSCUnitShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CSCUnitShader();
    virtual ~CSCUnitShader();

public:
    virtual bool Init();
};


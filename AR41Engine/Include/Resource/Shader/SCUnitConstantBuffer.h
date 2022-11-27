#pragma once

#include "ConstantBufferData.h"
#include "ShaderInfo.h"

class CSCUnitConstantBuffer :
    public CConstantBufferData
{
public:
	CSCUnitConstantBuffer();
	CSCUnitConstantBuffer(const CSCUnitConstantBuffer& Buffer);
	virtual ~CSCUnitConstantBuffer();

private:
	SCUnitCBuffer	m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CSCUnitConstantBuffer* Clone();


public:
	inline void TurnOnRenderFlags(ERenderFlag RenderFlags);
	inline void TurnOffRenderFlags(ERenderFlag RenderFlags);
};

inline void CSCUnitConstantBuffer::TurnOnRenderFlags(ERenderFlag RenderFlags)
{
	m_BufferData.RenderFlags |= static_cast<unsigned int>(RenderFlags);
}

inline void CSCUnitConstantBuffer::TurnOffRenderFlags(ERenderFlag RenderFlags)
{
	m_BufferData.RenderFlags &= ~(static_cast<unsigned int>(RenderFlags));
}

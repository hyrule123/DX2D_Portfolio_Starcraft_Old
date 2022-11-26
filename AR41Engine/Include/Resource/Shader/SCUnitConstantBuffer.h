#pragma once

#include "ConstantBufferData.h"

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
	inline void SetRenderFlags(unsigned int RenderFlags);
};

inline void CSCUnitConstantBuffer::SetRenderFlags(unsigned int RenderFlags)
{
	m_BufferData.RenderFlags = RenderFlags;
}

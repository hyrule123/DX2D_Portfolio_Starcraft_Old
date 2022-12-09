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
	SCUnit_CBuffer	m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CSCUnitConstantBuffer* Clone();


public:
	inline void TurnOnRenderFlags(ESCUnit_TextureLayerFlag RenderFlags);
	inline void TurnOffRenderFlags(ESCUnit_TextureLayerFlag RenderFlags);
	inline unsigned int GetRenderFlags() const;
	inline void SetTextureSize(ESCUnit_TextureLayer TexLayer, float Width, float Height);
};

inline void CSCUnitConstantBuffer::TurnOnRenderFlags(ESCUnit_TextureLayerFlag RenderFlags)
{
	m_BufferData.RenderFlags |= static_cast<unsigned int>(RenderFlags);
}

inline void CSCUnitConstantBuffer::TurnOffRenderFlags(ESCUnit_TextureLayerFlag RenderFlags)
{
	m_BufferData.RenderFlags &= ~(static_cast<unsigned int>(RenderFlags));
}

inline unsigned int CSCUnitConstantBuffer::GetRenderFlags() const
{
	return m_BufferData.RenderFlags;
}

inline void CSCUnitConstantBuffer::SetTextureSize(ESCUnit_TextureLayer TexLayer, float Width, float Height)
{
	m_BufferData.SCUnit_CBufferTexSizeInfo[(int)TexLayer].Width = Width;
	m_BufferData.SCUnit_CBufferTexSizeInfo[(int)TexLayer].Height = Height;
}

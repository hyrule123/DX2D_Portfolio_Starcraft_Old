#pragma once

#include "../GameResource.h"

class CConstantBuffer	:
	public CGameResource
{
	friend class CShaderManager;

protected:
	CConstantBuffer();
	~CConstantBuffer();

private:
	ID3D11Buffer* m_Buffer;
	int		m_Size;
	int		m_Register;
	int		m_ShaderBufferType;

public:
	bool Init(int Size, int Register, int ShaderBufferType);
	void UpdateBuffer(void* Data);
};


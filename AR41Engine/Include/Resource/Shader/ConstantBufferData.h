#pragma once

#include "ConstantBuffer.h"

#include "ShaderInfo.h"

class CConstantBufferData
{
public:
	CConstantBufferData();
	CConstantBufferData(const CConstantBufferData& Buffer);
	virtual ~CConstantBufferData();

protected:
	CSharedPtr<class CConstantBuffer>	m_Buffer;

protected:
	void SetConstantBuffer(const std::string& Name);

public:
	virtual bool Init() = 0;
	virtual void UpdateBuffer() = 0;
	virtual CConstantBufferData* Clone() = 0;
};


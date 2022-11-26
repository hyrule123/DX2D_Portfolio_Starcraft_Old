#include "SCUnitConstantBuffer.h"


CSCUnitConstantBuffer::CSCUnitConstantBuffer() :
	m_BufferData{}
{
}

CSCUnitConstantBuffer::CSCUnitConstantBuffer(const CSCUnitConstantBuffer& Buffer) :
	CConstantBufferData(Buffer),
	m_BufferData(Buffer.m_BufferData)
{
}

CSCUnitConstantBuffer::~CSCUnitConstantBuffer()
{
}

bool CSCUnitConstantBuffer::Init()
{
	SetConstantBuffer("SCUnit");

	

	return true;
}

void CSCUnitConstantBuffer::UpdateBuffer()
{
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CSCUnitConstantBuffer* CSCUnitConstantBuffer::Clone()
{
	return new CSCUnitConstantBuffer(*this);
}



#pragma once

//인스턴싱을 사용하는 클래스들에서 '공통'으로 들고있을 하나의 클래스
//인스턴싱용 데이터가 들어갈 구조체 T를 들고있도록 해 놓음.


#include "../../Device.h"
#include "../../Ref.h"

template <typename T>
class CSharedStructuredBuffer : public CRef
{
public:
	CSharedStructuredBuffer();
	~CSharedStructuredBuffer();

	bool Init(const std::string& Name, unsigned int Count,
		int Register, bool Dynamic = false,
		int StructuredBufferShaderType = (int)EShaderBufferType::Compute);
	bool Init(const std::string& Name, unsigned int Size,
		int Register, D3D11_USAGE Usage, UINT BindFlag,
		UINT CpuFlag, bool Dynamic = false,
		int StructuredBufferShaderType = (int)EShaderBufferType::Compute);

private:
	D3D11_BUFFER_DESC	m_Desc;
	ID3D11Buffer* m_Buffer;

	ID3D11ShaderResourceView* m_SRV;
	ID3D11UnorderedAccessView* m_UAV;

	unsigned int	m_Size;
	unsigned int	m_Count;
	int				m_StructuredBufferShaderType;
	int				m_Register;
	bool			m_Dynamic;

	//드로우콜 전까지 데이터를 저장할 컨테이너
	std::vector<T> m_InstancingBufferQueue;
	//현재 구조화 버퍼에 예약된 인스턴싱 수
	int m_InstancingBufferCount;

public:
	ID3D11Buffer* GetBuffer()	const;
	void AddBuffer(const T& BufferData);

	void UpdateBuffer(void* Data, int Count);
	void SetShader();
	void ResetShader();
	void SetShader(int Register, int StructuredBufferShaderType);
	void ResetShader(int Register, int StructuredBufferShaderType);
	void CopyData(void* Data);
	void CopyResource(CSharedStructuredBuffer* Buffer);
};


template <typename T>
CSharedStructuredBuffer<T>::CSharedStructuredBuffer() :
	m_Desc{},
	m_Buffer(nullptr),
	m_SRV(nullptr),
	m_UAV(nullptr),
	m_Size(0),
	m_Count(0),
	m_StructuredBufferShaderType(0),
	m_Register(0),
	m_Dynamic(false),
	m_InstancingBufferCount(0)
{
}



template <typename T>
CSharedStructuredBuffer<T>::~CSharedStructuredBuffer()
{
	SAFE_RELEASE(m_SRV);
	SAFE_RELEASE(m_UAV);
	SAFE_RELEASE(m_Buffer);
}

template <typename T>
bool CSharedStructuredBuffer<T>::Init(const std::string& Name, unsigned int Count, int Register,
	bool Dynamic, int StructuredBufferShaderType)
{
	SAFE_RELEASE(m_SRV);
	SAFE_RELEASE(m_UAV);
	SAFE_RELEASE(m_Buffer);

	m_Dynamic = Dynamic;
	m_Name = Name;
	m_Size = sizeof(T);
	m_Count = Count;
	m_Register = Register;
	m_StructuredBufferShaderType = StructuredBufferShaderType;

	m_Desc.ByteWidth = m_Size * m_Count;
	m_Desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	m_Desc.StructureByteStride = m_Size;

	if (m_Dynamic)
	{
		m_Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		m_Desc.Usage = D3D11_USAGE_DYNAMIC;
		m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	else
	{
		m_Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		m_Desc.Usage = D3D11_USAGE_DEFAULT;
	}

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&m_Desc, nullptr,
		&m_Buffer)))
		return false;

	D3D11_SHADER_RESOURCE_VIEW_DESC	SRVDesc = {};

	SRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	SRVDesc.BufferEx.FirstElement = 0;
	SRVDesc.BufferEx.Flags = 0;
	SRVDesc.BufferEx.NumElements = m_Count;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateShaderResourceView(m_Buffer,
		&SRVDesc, &m_SRV)))
		return false;

	if (!m_Dynamic)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC	UAVDesc = {};

		UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
		UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		UAVDesc.Buffer.FirstElement = 0;
		UAVDesc.Buffer.Flags = 0;
		UAVDesc.Buffer.NumElements = m_Count;

		if (FAILED(CDevice::GetInst()->GetDevice()->CreateUnorderedAccessView(
			m_Buffer, &UAVDesc, &m_UAV)))
			return false;
	}

	//구조화 버퍼 컨테이너도 사이즈를 조절해 준다.
	m_InstancingBufferQueue.resize(m_Count);
	return true;
}

template <typename T>
bool CSharedStructuredBuffer<T>::Init(const std::string& Name,
	 unsigned int Count,
	int Register, D3D11_USAGE Usage, UINT BindFlag,
	UINT CpuFlag, bool Dynamic, int StructuredBufferShaderType)
{
	SAFE_RELEASE(m_SRV);
	SAFE_RELEASE(m_UAV);
	SAFE_RELEASE(m_Buffer);

	m_Dynamic = Dynamic;
	m_Name = Name;
	m_Size = sizeof(T);
	m_Count = Count;
	m_Register = Register;
	m_StructuredBufferShaderType = StructuredBufferShaderType;

	m_Desc.ByteWidth = m_Size * m_Count;
	m_Desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	m_Desc.StructureByteStride = m_Size;

	m_Desc.BindFlags = BindFlag;
	m_Desc.Usage = Usage;
	m_Desc.CPUAccessFlags = CpuFlag;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&m_Desc, nullptr,
		&m_Buffer)))
		return false;

	D3D11_SHADER_RESOURCE_VIEW_DESC	SRVDesc = {};

	SRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	SRVDesc.BufferEx.FirstElement = 0;
	SRVDesc.BufferEx.Flags = 0;
	SRVDesc.BufferEx.NumElements = m_Count;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateShaderResourceView(m_Buffer,
		&SRVDesc, &m_SRV)))
		return false;

	D3D11_UNORDERED_ACCESS_VIEW_DESC	UAVDesc = {};

	UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	UAVDesc.Buffer.FirstElement = 0;
	UAVDesc.Buffer.Flags = 0;
	UAVDesc.Buffer.NumElements = m_Count;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateUnorderedAccessView(
		m_Buffer, &UAVDesc, &m_UAV)))
		return false;

	//구조화 버퍼 컨테이너도 사이즈를 조절해 준다.
	m_InstancingBufferQueue.resize(m_Count);

	return true;
}





template <typename T>
inline ID3D11Buffer* CSharedStructuredBuffer<T>::GetBuffer()	const
{
	return m_Buffer;
}

template<typename T>
inline void CSharedStructuredBuffer<T>::AddBuffer(const T& BufferData)
{
	m_InstancingBufferQueue.push_back(T);
}


template <typename T>
void CSharedStructuredBuffer<T>::UpdateBuffer(void* Data, int Count)
{
	if (!m_Dynamic)
		return;

	D3D11_MAPPED_SUBRESOURCE	Map = {};

	CDevice::GetInst()->GetContext()->Map(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD,
		0, &Map);

	memcpy(Map.pData, Data, m_Size * Count);

	CDevice::GetInst()->GetContext()->Unmap(m_Buffer, 0);
}


template <typename T>
void CSharedStructuredBuffer<T>::SetShader()
{
	if (m_Dynamic)
	{
		if (m_StructuredBufferShaderType & (int)EShaderBufferType::Vertex)
			CDevice::GetInst()->GetContext()->VSSetShaderResources(m_Register, 1, &m_SRV);

		if (m_StructuredBufferShaderType & (int)EShaderBufferType::Pixel)
			CDevice::GetInst()->GetContext()->PSSetShaderResources(m_Register, 1, &m_SRV);

		if (m_StructuredBufferShaderType & (int)EShaderBufferType::Hull)
			CDevice::GetInst()->GetContext()->HSSetShaderResources(m_Register, 1, &m_SRV);

		if (m_StructuredBufferShaderType & (int)EShaderBufferType::Domain)
			CDevice::GetInst()->GetContext()->DSSetShaderResources(m_Register, 1, &m_SRV);

		if (m_StructuredBufferShaderType & (int)EShaderBufferType::Geometry)
			CDevice::GetInst()->GetContext()->GSSetShaderResources(m_Register, 1, &m_SRV);

		if (m_StructuredBufferShaderType & (int)EShaderBufferType::Compute)
			CDevice::GetInst()->GetContext()->CSSetShaderResources(m_Register, 1, &m_SRV);
	}

	else
	{
		unsigned int Count = -1;
		CDevice::GetInst()->GetContext()->CSSetUnorderedAccessViews(m_Register, 1, &m_UAV, &Count);
	}
}

template <typename T>
void CSharedStructuredBuffer<T>::ResetShader()
{
	if (m_Dynamic)
	{
		ID3D11ShaderResourceView* SRV = nullptr;

		if (m_StructuredBufferShaderType & (int)EShaderBufferType::Vertex)
			CDevice::GetInst()->GetContext()->VSSetShaderResources(m_Register, 1, &SRV);

		if (m_StructuredBufferShaderType & (int)EShaderBufferType::Pixel)
			CDevice::GetInst()->GetContext()->PSSetShaderResources(m_Register, 1, &SRV);

		if (m_StructuredBufferShaderType & (int)EShaderBufferType::Hull)
			CDevice::GetInst()->GetContext()->HSSetShaderResources(m_Register, 1, &SRV);

		if (m_StructuredBufferShaderType & (int)EShaderBufferType::Domain)
			CDevice::GetInst()->GetContext()->DSSetShaderResources(m_Register, 1, &SRV);

		if (m_StructuredBufferShaderType & (int)EShaderBufferType::Geometry)
			CDevice::GetInst()->GetContext()->GSSetShaderResources(m_Register, 1, &SRV);

		if (m_StructuredBufferShaderType & (int)EShaderBufferType::Compute)
			CDevice::GetInst()->GetContext()->CSSetShaderResources(m_Register, 1, &SRV);
	}

	else
	{
		unsigned int Count = -1;
		ID3D11UnorderedAccessView* UAV = nullptr;
		CDevice::GetInst()->GetContext()->CSSetUnorderedAccessViews(m_Register, 1, &UAV, &Count);
	}


}

template <typename T>
void CSharedStructuredBuffer<T>::SetShader(int Register, int StructuredBufferShaderType)
{
	if (m_Dynamic)
	{
		if (StructuredBufferShaderType & (int)EShaderBufferType::Vertex)
			CDevice::GetInst()->GetContext()->VSSetShaderResources(Register, 1, &m_SRV);

		if (StructuredBufferShaderType & (int)EShaderBufferType::Pixel)
			CDevice::GetInst()->GetContext()->PSSetShaderResources(Register, 1, &m_SRV);

		if (StructuredBufferShaderType & (int)EShaderBufferType::Hull)
			CDevice::GetInst()->GetContext()->HSSetShaderResources(Register, 1, &m_SRV);

		if (StructuredBufferShaderType & (int)EShaderBufferType::Domain)
			CDevice::GetInst()->GetContext()->DSSetShaderResources(Register, 1, &m_SRV);

		if (StructuredBufferShaderType & (int)EShaderBufferType::Geometry)
			CDevice::GetInst()->GetContext()->GSSetShaderResources(Register, 1, &m_SRV);

		if (StructuredBufferShaderType & (int)EShaderBufferType::Compute)
			CDevice::GetInst()->GetContext()->CSSetShaderResources(Register, 1, &m_SRV);
	}

	else
	{
		unsigned int Count = -1;
		CDevice::GetInst()->GetContext()->CSSetUnorderedAccessViews(Register, 1, &m_UAV, &Count);
	}
}

template <typename T>
void CSharedStructuredBuffer<T>::ResetShader(int Register, int StructuredBufferShaderType)
{
	if (m_Dynamic)
	{
		ID3D11ShaderResourceView* SRV = nullptr;

		if (StructuredBufferShaderType & (int)EShaderBufferType::Vertex)
			CDevice::GetInst()->GetContext()->VSSetShaderResources(Register, 1, &SRV);

		if (StructuredBufferShaderType & (int)EShaderBufferType::Pixel)
			CDevice::GetInst()->GetContext()->PSSetShaderResources(Register, 1, &SRV);

		if (StructuredBufferShaderType & (int)EShaderBufferType::Hull)
			CDevice::GetInst()->GetContext()->HSSetShaderResources(Register, 1, &SRV);

		if (StructuredBufferShaderType & (int)EShaderBufferType::Domain)
			CDevice::GetInst()->GetContext()->DSSetShaderResources(Register, 1, &SRV);

		if (StructuredBufferShaderType & (int)EShaderBufferType::Geometry)
			CDevice::GetInst()->GetContext()->GSSetShaderResources(Register, 1, &SRV);

		if (StructuredBufferShaderType & (int)EShaderBufferType::Compute)
			CDevice::GetInst()->GetContext()->CSSetShaderResources(Register, 1, &SRV);
	}

	else
	{
		unsigned int Count = -1;
		ID3D11UnorderedAccessView* UAV = nullptr;
		CDevice::GetInst()->GetContext()->CSSetUnorderedAccessViews(Register, 1, &UAV, &Count);
	}
}

template <typename T>
void CSharedStructuredBuffer<T>::CopyData(void* Data)
{
	D3D11_MAPPED_SUBRESOURCE	Map = {};

	CDevice::GetInst()->GetContext()->Map(m_Buffer, 0, D3D11_MAP_READ,
		0, &Map);

	memcpy(Data, Map.pData, m_Size * m_Count);

	CDevice::GetInst()->GetContext()->Unmap(m_Buffer, 0);
}

template <typename T>
void CSharedStructuredBuffer<T>::CopyResource(CSharedStructuredBuffer* Buffer)
{
	CDevice::GetInst()->GetContext()->CopyResource(Buffer->m_Buffer, m_Buffer);
}

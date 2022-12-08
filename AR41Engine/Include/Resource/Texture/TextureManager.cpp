
#include "TextureManager.h"
#include "Texture.h"
#include "../../Device.h"
#include "../../Scene/SceneManager.h"

CTextureManager::CTextureManager()
{
}

CTextureManager::~CTextureManager()
{
	auto	iter = m_mapSampler.begin();
	auto	iterEnd = m_mapSampler.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_RELEASE(iter->second);
	}
}

bool CTextureManager::Init()
{
	/*	
	Sampler�� 3������ ���͹���� ������.
	�������, Ȯ������, Mip����

	�������� : �����͸�, �������͸�, ���漺 ���͸�
	�����͸� : �ӵ��� ���� ������. ��, ǰ���� ��������. ������ ũ�Ⱑ �� �¾Ƽ�
	Ȯ�볪 ��Ұ� ���Ͼ�ٸ� ���� ���� �������̴�.

	�������͸� : �ӵ� �߰�. ǰ���� �߰��̴�.

	���漺 ���͸� : �ӵ��� ���� ������. ǰ���� ���� ����.
	*/

	float BorderColor[4] = { 1.f, 1.f,1.f, 1.f };

	if (FAILED(CreateSampler("Point", D3D11_FILTER_MIN_MAG_MIP_POINT,
		D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP, BorderColor)))
		return false;

	if (FAILED(CreateSampler("Linear", D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP, BorderColor)))
		return false;

	if (FAILED(CreateSampler("Anisotropic", D3D11_FILTER_ANISOTROPIC,
		D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP, BorderColor)))
		return false;

	SetSampler("Point", 0);
	SetSampler("Linear", 1);
	SetSampler("Anisotropic", 2);

	return true;
}

CTexture* CTextureManager::LoadTexture(const std::string& Name, const TCHAR* FileName, 
	const std::string& PathName)
{
	CTexture* Texture = FindTexture(Name);

	if (Texture)
		return Texture;

	Texture = new CTexture;
	Texture->SetResourceType(EResourceType::Texture);

	if (!Texture->LoadTexture(Name, FileName, PathName))
	{
		SAFE_DELETE(Texture);
		return nullptr;
	}

	m_mapTexture.insert(std::make_pair(Name, Texture));

	return Texture;
}

CTexture* CTextureManager::LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	CTexture* Texture = FindTexture(Name);

	if (Texture)
		return Texture;

	Texture = new CTexture;
	Texture->SetResourceType(EResourceType::Texture);

	if (!Texture->LoadTextureFullPath(Name, FullPath))
	{
		SAFE_DELETE(Texture);
		return nullptr;
	}

	m_mapTexture.insert(std::make_pair(Name, Texture));

	return Texture;
}

CTexture* CTextureManager::LoadTexture(const std::string& Name, 
	const std::vector<const TCHAR*>& vecFileName, const std::string& PathName)
{
	CTexture* Texture = FindTexture(Name);

	if (Texture)
		return Texture;

	Texture = new CTexture;
	Texture->SetResourceType(EResourceType::Texture);

	if (!Texture->LoadTexture(Name, vecFileName, PathName))
	{
		SAFE_DELETE(Texture);
		return nullptr;
	}

	m_mapTexture.insert(std::make_pair(Name, Texture));

	return Texture;
}

CTexture* CTextureManager::LoadTextureFullPath(const std::string& Name,
	const std::vector<const TCHAR*>& vecFullPath)
{
	CTexture* Texture = FindTexture(Name);

	if (Texture)
		return Texture;

	Texture = new CTexture;
	Texture->SetResourceType(EResourceType::Texture);

	if (!Texture->LoadTextureFullPath(Name, vecFullPath))
	{
		SAFE_DELETE(Texture);
		return nullptr;
	}

	m_mapTexture.insert(std::make_pair(Name, Texture));

	return Texture;
}

CTexture* CTextureManager::LoadTextureArray(const std::string& Name,
	const std::vector<const TCHAR*>& vecFileName, const std::string& PathName)
{
	CTexture* Texture = FindTexture(Name);

	if (Texture)
		return Texture;

	Texture = new CTexture;
	Texture->SetResourceType(EResourceType::TextureArray);

	if (!Texture->LoadTextureArray(Name, vecFileName, PathName))
	{
		SAFE_DELETE(Texture);
		return nullptr;
	}

	m_mapTexture.insert(std::make_pair(Name, Texture));

	return Texture;
}

CTexture* CTextureManager::LoadTextureArrayFullPath(const std::string& Name, const std::vector<const TCHAR*>& vecFullPath)
{
	CTexture* Texture = FindTexture(Name);

	if (Texture)
		return Texture;

	Texture = new CTexture;
	Texture->SetResourceType(EResourceType::TextureArray);

	if (!Texture->LoadTextureArrayFullPath(Name, vecFullPath))
	{
		SAFE_DELETE(Texture);
		return nullptr;
	}

	m_mapTexture.insert(std::make_pair(Name, Texture));

	return Texture;
}


CTexture* CTextureManager::LoadTextureArrayByvecTextureResourceInfo(
	const std::string& Name,
	std::vector<TextureResourceInfo*>& vecTexResInfo)
{
	CTexture* Texture = FindTexture(Name);

	if (Texture)
		return Texture;

	Texture = new CTexture;
	Texture->SetResourceType(EResourceType::TextureArray);

	if (!Texture->LoadTextureArrayByvecTextureResourceInfo(Name, vecTexResInfo))
	{
		SAFE_DELETE(Texture);
		return nullptr;
	}

	m_mapTexture.insert(std::make_pair(Name, Texture));

	return Texture;
}

CTexture* CTextureManager::FindTexture(const std::string& Name)
{
	auto	iter = m_mapTexture.find(Name);

	if (iter == m_mapTexture.end())
		return nullptr;

	return iter->second;
}

void CTextureManager::ReleaseTexture(const std::string& Name)
{
	auto	iter = m_mapTexture.find(Name);

	if (iter != m_mapTexture.end())
	{
		if (iter->second->GetRefCount() == 1)
			m_mapTexture.erase(iter);
	}
}

void CTextureManager::DeleteUnused()
{
	auto iter = m_mapTexture.begin();
	auto iterEnd = m_mapTexture.end();

	CSceneManager* SceneMgr = CSceneManager::GetInst();

	while (iter != iterEnd)
	{
		//������ ������ �ʰ� �ʼ� ���ҽ��� �����Ǿ� ���� ���� ��� �����ش�. -> RefCount == 0 �� �ǹǷ� �˾Ƽ� ����
		if (iter->second->GetRefCount() == 1 && !(iter->second->GetEssential()))
		{
			iter = m_mapTexture.erase(iter);
			continue;
		}

		SceneMgr->AddSceneResource(iter->second);
		++iter;
	}
}

bool CTextureManager::CreateSampler(const std::string& Name, 
	D3D11_FILTER Filter, D3D11_TEXTURE_ADDRESS_MODE AddressU, 
	D3D11_TEXTURE_ADDRESS_MODE AddressV, D3D11_TEXTURE_ADDRESS_MODE AddressW,
	float BorderColor[4])
{
	ID3D11SamplerState* Sampler = FindSampler(Name);

	if (Sampler)
		return true;

	D3D11_SAMPLER_DESC	Desc = {};

	Desc.Filter = Filter;
	Desc.AddressU = AddressU;
	Desc.AddressV = AddressV;
	Desc.AddressW = AddressW;
	Desc.MipLODBias = 0.f;
	Desc.MaxAnisotropy = 1;
	Desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	Desc.MinLOD = -FLT_MAX;
	Desc.MaxLOD = FLT_MAX;

	memcpy(Desc.BorderColor, BorderColor, sizeof(float) * 4);

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateSamplerState(&Desc, &Sampler)))
		return false;

	m_mapSampler.insert(std::make_pair(Name, Sampler));

	return true;
}

ID3D11SamplerState* CTextureManager::FindSampler(const std::string& Name)
{
	auto	iter = m_mapSampler.find(Name);

	if (iter == m_mapSampler.end())
		return nullptr;

	return iter->second;
}

void CTextureManager::SetSampler(const std::string& Name, int Register,
	int ShaderBufferType)
{
	ID3D11SamplerState* Sampler = FindSampler(Name);

	if (!Sampler)
		return;

	if (ShaderBufferType & (int)EShaderBufferType::Vertex)
		CDevice::GetInst()->GetContext()->VSSetSamplers(Register, 1, &Sampler);

	if (ShaderBufferType & (int)EShaderBufferType::Pixel)
		CDevice::GetInst()->GetContext()->PSSetSamplers(Register, 1, &Sampler);

	if (ShaderBufferType & (int)EShaderBufferType::Hull)
		CDevice::GetInst()->GetContext()->HSSetSamplers(Register, 1, &Sampler);

	if (ShaderBufferType & (int)EShaderBufferType::Domain)
		CDevice::GetInst()->GetContext()->DSSetSamplers(Register, 1, &Sampler);

	if (ShaderBufferType & (int)EShaderBufferType::Geometry)
		CDevice::GetInst()->GetContext()->GSSetSamplers(Register, 1, &Sampler);

	if (ShaderBufferType & (int)EShaderBufferType::Compute)
		CDevice::GetInst()->GetContext()->CSSetSamplers(Register, 1, &Sampler);
}

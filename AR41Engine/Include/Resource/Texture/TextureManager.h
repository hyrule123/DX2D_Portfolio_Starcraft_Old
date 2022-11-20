#pragma once

#include "../../EngineInfo.h"

class CTextureManager
{
	friend class CResourceManager;

private:
	CTextureManager();
	~CTextureManager();

private:
	std::unordered_map<std::string, CSharedPtr<class CTexture>>	m_mapTexture;
	std::unordered_map<std::string, ID3D11SamplerState*>	m_mapSampler;

public:
	bool Init();
	class CTexture* LoadTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	class CTexture* LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath);
	class CTexture* LoadTexture(const std::string& Name, const std::vector<const TCHAR*>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	class CTexture* LoadTextureFullPath(const std::string& Name, const std::vector<const TCHAR*>& vecFullPath);
	class CTexture* LoadTextureArray(const std::string& Name, const std::vector<const TCHAR*>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	class CTexture* LoadTextureArrayFullPath(const std::string& Name, const std::vector<const TCHAR*>& vecFullPath);

	//MapManager에서 직접 리소스 로딩하는 용도
	class CTexture* LoadTextureArrayByvecTextureResourceInfo(
		const std::string& Name, const std::vector<struct TextureResourceInfo*>& vecTexResInfo);

	class CTexture* FindTexture(const std::string& Name);

	void ReleaseTexture(const std::string& Name);
	void DeleteUnused();

private:
	/*
	D3D11_FILTER Filter;
    D3D11_TEXTURE_ADDRESS_MODE AddressU;
    D3D11_TEXTURE_ADDRESS_MODE AddressV;
    D3D11_TEXTURE_ADDRESS_MODE AddressW;
    FLOAT MipLODBias;
    UINT MaxAnisotropy;
    D3D11_COMPARISON_FUNC ComparisonFunc;
    FLOAT BorderColor[ 4 ];
    FLOAT MinLOD;
    FLOAT MaxLOD;

	D3D11_FILTER_MIN_MAG_MIP_LINEAR
	*/
	bool CreateSampler(const std::string& Name, D3D11_FILTER Filter,
		D3D11_TEXTURE_ADDRESS_MODE AddressU,
		D3D11_TEXTURE_ADDRESS_MODE AddressV,
		D3D11_TEXTURE_ADDRESS_MODE AddressW,
		float BorderColor[4]);
	ID3D11SamplerState* FindSampler(const std::string& Name);
	void SetSampler(const std::string& Name, int Register,
		int ShaderBufferType = (int)EShaderBufferType::All);
};


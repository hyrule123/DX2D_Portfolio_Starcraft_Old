#pragma once

#include "../GameResource.h"

struct TextureResourceInfo
{
	DirectX::ScratchImage* Image;
	ID3D11ShaderResourceView* SRV;
	unsigned int	Width;
	unsigned int	Height;
	TCHAR* FileName;
	char* PathName;

	TextureResourceInfo()	:
		Image(nullptr),
		SRV(nullptr),
		Width(0),
		Height(0)
	{
		FileName = new TCHAR[MAX_PATH];
		PathName = new char[MAX_PATH];

		memset(FileName, 0, sizeof(TCHAR) * MAX_PATH);
		memset(PathName, 0, sizeof(char) * MAX_PATH);
	}

	//경로정보가 없을경우 동적할당하지 않기 위한 용도
	TextureResourceInfo(bool b) :
		SRV(nullptr),
		Width(0),
		Height(0),
		FileName(),
		PathName()
	{
		Image = new DirectX::ScratchImage;
	}

	~TextureResourceInfo()
	{
		SAFE_RELEASE(SRV);
		SAFE_DELETE_ARRAY(FileName);
		SAFE_DELETE_ARRAY(PathName);
		SAFE_DELETE(Image);
	}
};

class CTexture :
	public CGameResource
{
	friend class CTextureManager;

	//맵에서 직접 TextureInfo를 만들어서 배열텍스처를 만들어야함.
	friend class CMap;

protected:
	CTexture();
	virtual ~CTexture();

protected:
	std::vector<TextureResourceInfo*>	m_vecTextureInfo;
	EImageType	m_ImageType;
	ID3D11ShaderResourceView* m_ArraySRV;

public:
	inline EImageType GetImageType()	const;

	inline ID3D11ShaderResourceView* GetResource(int Index = 0)	const;

	inline unsigned int GetWidth(int Index = 0)		const;

	inline unsigned int GetHeight(int Index = 0)	const;
	inline bool GetvecTextureInfoEmpty() const;

	inline int GetImageCount()	const;



public:
	bool LoadTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath);
	bool LoadTexture(const std::string& Name, const std::vector<const TCHAR*>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& Name, const std::vector<const TCHAR*>& vecFullPath);
	bool LoadTextureArray(const std::string& Name, const std::vector<const TCHAR*>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	bool LoadTextureArrayFullPath(const std::string& Name, const std::vector<const TCHAR*>& vecFullPath);

	//MapManager 용
	//MapManager에서 직접 TextureResourceInfo를 생성하고 여기로 전달
	bool LoadTextureArrayByvecTextureResourceInfo(
		const std::string& Name, std::vector<TextureResourceInfo*>& vecTexResInfo);


private:
	bool CreateResource(int Index);
	bool CreateResourceArray();

public:
	void SetShader(int Register, int ShaderBufferType, int Index);
	void ResetShader(int Register, int ShaderBufferType);


public:
	void Save(FILE* File);
	void Load(FILE* File);
};

inline EImageType CTexture::GetImageType()	const
{
	return m_ImageType;
}

inline ID3D11ShaderResourceView* CTexture::GetResource(int Index)	const
{
	return m_vecTextureInfo[Index]->SRV;
}

inline unsigned int CTexture::GetWidth(int Index)	const
{
	return m_vecTextureInfo[Index]->Width;
}

inline unsigned int CTexture::GetHeight(int Index)	const
{
	return m_vecTextureInfo[Index]->Height;
}

inline bool CTexture::GetvecTextureInfoEmpty() const
{
	return m_vecTextureInfo.empty();
}


inline int CTexture::GetImageCount()	const
{
	return (int)m_vecTextureInfo.size();
}
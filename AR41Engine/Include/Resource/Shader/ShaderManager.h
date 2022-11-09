#pragma once

#include "../../EngineInfo.h"

class CShaderManager
{
	friend class CResourceManager;

private:
	CShaderManager();
	~CShaderManager();

private:
	std::unordered_map<std::string, CSharedPtr<class CShader>>	m_mapShader;
	std::unordered_map<std::string, CSharedPtr<class CConstantBuffer>>	m_mapCBuffer;
	class CColliderConstantBuffer* m_ColliderCBuffer;

public:
	inline class CColliderConstantBuffer* GetColliderCBuffer()	const;

public:
	bool Init();
	class CShader* FindShader(const std::string& Name);
	void ReleaseShader(const std::string& Name);

	class CConstantBuffer* CreateConstantBuffer(const std::string& Name, int Size, int Register,
		int ShaderBufferType = (int)EShaderBufferType::All);
	class CConstantBuffer* FindConstantBuffer(const std::string& Name);
	
	void DeleteUnused();

public:
	template <typename T>
	T* CreateShader(const std::string& Name);

};

inline CColliderConstantBuffer* CShaderManager::GetColliderCBuffer() const
{
	return m_ColliderCBuffer;
}


template <typename T>
T* CShaderManager::CreateShader(const std::string& Name)
{
	T* Shader = (T*)FindShader(Name);

	if (Shader)
		return Shader;

	Shader = new T;

	Shader->SetName(Name);
	Shader->SetResourceType(EResourceType::Shader);

	if (!Shader->Init())
	{
		SAFE_DELETE(Shader);
		return nullptr;
	}

	m_mapShader.insert(std::make_pair(Name, Shader));

	return Shader;
}
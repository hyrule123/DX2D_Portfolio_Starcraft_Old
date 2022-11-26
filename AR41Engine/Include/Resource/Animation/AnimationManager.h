#pragma once

#include "AnimationSequence2D.h"

class CAnimationManager
{
	friend class CResourceManager;

private:
	CAnimationManager();
	~CAnimationManager();

private:
	std::unordered_map<std::string, CSharedPtr<CAnimationSequence2D>>	m_mapSequence2D;
	class CAnimation2DConstantBuffer* m_Anim2DBuffer;

public:
	inline class CAnimation2DConstantBuffer* GetAnim2DConstantBuffer()	const;



public:
	bool Init();
	class CAnimationSequence2D* CreateAnimationSequence2D(const std::string& Name, const std::string& TextureName,
		const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
	class CAnimationSequence2D* CreateAnimationSequence2D(const std::string& Name, class CTexture* Texture);
	class CAnimationSequence2D* CreateAnimationSequence2DFullPath(const std::string& Name, const std::string& TextureName, const TCHAR* FullPath);
	class CAnimationSequence2D* CreateAnimationSequence2D(const std::string& Name, const std::string& TextureName, const std::vector<const TCHAR*>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	class CAnimationSequence2D* CreateAnimationSequence2DFullPath(const std::string& Name, const std::string& TextureName, const std::vector<const TCHAR*>& vecFullPath);
	bool AddSequence2DFrame(const std::string& Name, const Vector2& Start, const Vector2& End);
	bool AddSequence2DFrame(const std::string& Name, float StartX, float StartY, float EndX,
		float EndY);

	bool AddAnimationSequence2DFrameByTileNumber(const std::string& Name, EAnimation2DType Type,
		int TileRowNum, int TileColNum,
		int ColStart = -1, int ColSize = -1,
		int RowStart = -1, int RowSize = -1
		);

	//bool SetAnimationSequence2DFrameAllByTileNumber(const std::string& Name,
	//	int TileRowNum, int TileColNum
	//);

	bool AddSequence2DFrameAll(const std::string& Name, int Count, const Vector2& Start, const Vector2& End);
	bool AddSequence2DFrameAll(const std::string& Name, int Count, float StartX, float StartY, float EndX,
		float EndY);
	bool SaveSequence2D(const std::string& Name, const char* FullPath);
	bool LoadSequence2D(const std::string& Name, const char* FullPath);
	bool SaveSequence2D(const std::string& Name, const char* FileName, const std::string& PathName);
	bool LoadSequence2D(const std::string& Name, const char* FileName, const std::string& PathName);

	CAnimationSequence2D* FindAnimationSequence2D(const std::string& Name);
	void ReleaseAnimationSequence2D(const std::string& Name);
	void DeleteUnused();

};

inline class CAnimation2DConstantBuffer* CAnimationManager::GetAnim2DConstantBuffer()	const
{
	return m_Anim2DBuffer;
}
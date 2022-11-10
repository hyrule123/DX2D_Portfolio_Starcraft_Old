#pragma once

#include "../EngineInfo.h"


//공유 포인터를 활용한 리소스 및 CDO 관리자
//단순 공유 포인터에 Reference Count를 더하기 위한 용도 -> List를 사용


class CSceneResource
{
	friend class CScene;
	friend class CResourceManager;

private:
	CSceneResource();
	~CSceneResource();

private:
	class CScene* m_Owner;

private:
	std::vector<CSharedPtr<class CGameResource>> m_vecResource;

public:
	bool Init();
	void AddGameResource(class CGameResource* ResPtr);
};
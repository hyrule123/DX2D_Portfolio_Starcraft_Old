#pragma once

#include "../EngineInfo.h"


//���� �����͸� Ȱ���� ���ҽ� �� CDO ������
//�ܼ� ���� �����Ϳ� Reference Count�� ���ϱ� ���� �뵵 -> List�� ���


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
	std::vector<CSharedPtr<class CCDO>> m_vecCDO;

public:
	bool Init();
	void AddGameResource(class CGameResource* ResPtr);
	void AddCDO(class CCDO* CDOPtr);
};
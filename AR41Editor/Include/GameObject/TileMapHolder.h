#pragma once
#include "GameObject\GameObject.h"
class CTileMapHolder :
    public CGameObject
{
	friend class CScene;
	friend class CSceneManager;
	friend class CCDO;

protected:
	CTileMapHolder();
	CTileMapHolder(const CTileMapHolder& Obj);
	virtual ~CTileMapHolder();

public:
	virtual bool CDOPreload();
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CTileMapHolder* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

protected:
	CSharedPtr<class CTileMapComponent> m_TileMap;
};


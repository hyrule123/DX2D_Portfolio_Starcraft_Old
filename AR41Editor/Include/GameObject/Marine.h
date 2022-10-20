#pragma once
#include "GameObject\GameObject.h"
class CMarine :
    public CGameObject
{
	friend class CScene;

protected:
	CMarine();
	CMarine(const CMarine& Obj);
	virtual ~CMarine();

private:
	CSharedPtr<class CSceneComponent> m_UnitRoot;
	CSharedPtr<class CUnitSpriteComponent>	m_MainSprite;
	CSharedPtr<class CCameraComponent>	m_Camera;
	CSharedPtr<class CTargetArm>		m_Arm;
	CSharedPtr<class CColliderOBB2D>	m_Coll;


public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CMarine* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

public:
	void MoveUp();
	void MoveDown();
	void Rotation();
	void RotationInv();
	void Fire();
};

